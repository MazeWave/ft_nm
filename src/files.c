/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   files.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldalmass <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/28 13:17:05 by ldalmass          #+#    #+#             */
/*   Updated: 2026/11/09 16:22:09 by ldalmass         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_nm.h"

bool	open_file(const char *path, t_nm *nm)
{
	AUTO_LOG;

	int			fd = -1;
	void		*new_mapping = NULL;
	struct stat	new_stats;

	// Check the file
	if (stat(path, &new_stats) < 0)
	{
		dprintf(STDERR_FILENO, YELLOW "%s: Failed to get file's details from '%s'\n" RESET, nm->program_name, path);
		goto error;
	}
	if (S_ISDIR(new_stats.st_mode))
	{
		dprintf(STDERR_FILENO, YELLOW "%s: Warning: '%s' is a directory.\n" RESET, nm->program_name, path);
		goto error;
	}
	if (!S_ISREG(new_stats.st_mode))
	{
		dprintf(STDERR_FILENO, YELLOW "%s: Warning: '%s' is not a regular file.\n" RESET, nm->program_name, path);
		goto error;
	}

	// Map the file into memory
	fd = open(path, O_RDONLY);
	if (fd < 0)
	{
		dprintf(STDERR_FILENO, YELLOW "%s: Warning: %s: %s\n" RESET, nm->program_name, path, strerror(errno));
		goto error;
	}
	new_mapping = mmap(NULL, new_stats.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
	if (new_mapping == MAP_FAILED)
	{
		dprintf(STDERR_FILENO, "%s: " YELLOW "Warning: mmap failed to map %s into memory.\n" RESET, nm->program_name, path);
		dprintf(STDERR_FILENO, "%s" YELLOW "This can happen on an empty file\n" RESET, indent(nm->program_name));
		LOG(WARNING "%s: Warning: mmap failed to map %s into memory.\n" RESET, nm->program_name, path);
		print_mmap_errors(new_stats, nm);
		new_mapping = NULL;
		goto error;
	}
	close(fd);

	// Check and free the current map if any
	if (nm->file != NULL)
	{
		munmap(nm->file, nm->st.st_size);
		nm->file = NULL;
		LOG(INFO "A file was already opened before and thus been cleared to open the newly requested file.");
	}

	// Copy the newly mapped region to our global one
	nm->file = new_mapping;
	nm->st = new_stats;
	LOG(INFO "%s successfully mapped", path);
	return false;
	error:	// Error case
		close(fd);
		return true;
}

void	close_file(t_nm *nm)
{
	AUTO_LOG;

	munmap(nm->file, nm->st.st_size);
	nm->file = NULL;
	nm->st.st_size = 0;
	return ;
}

void	print_mmap_errors(struct stat st, t_nm *nm)
{
	dprintf(STDERR_FILENO, RED "%s: mmap: ", nm->program_name);
	switch (st.st_mode & S_IFMT)
	{
    	default:		dprintf(STDERR_FILENO, "unknown file type\n");						break;
     case S_IFBLK:	dprintf(STDERR_FILENO, "block device\n");							break;
     case S_IFCHR:	dprintf(STDERR_FILENO, "character device\n");						break;
     case S_IFDIR:	dprintf(STDERR_FILENO, "is directory\n");							break;
     case S_IFIFO:	dprintf(STDERR_FILENO, "is FIFO/pipe\n");							break;
     case S_IFLNK:	dprintf(STDERR_FILENO, "is symlink\n");								break;
     case S_IFSOCK:	dprintf(STDERR_FILENO, "is socket\n");								break;
     case S_IFREG:	dprintf(STDERR_FILENO, "%s, regular file\n", strerror(errno));		break;
    }
    dprintf(STDERR_FILENO, RESET);
	return ;
}
