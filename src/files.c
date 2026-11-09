/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   files.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldalmass <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/28 13:17:05 by ldalmass          #+#    #+#             */
/*   Updated: 2026/11/09 13:15:07 by ldalmass         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_nm.h"

bool	open_file(const char *path, t_nm *nm)
{
	AUTO_LOG;

	int			fd = open(path, O_RDONLY);
	void		*new_mapping = NULL;
	struct stat	new_fstats;
	struct stat	new_stats;
	// struct stat	new_stats;

	// Open and map the file
	error:	// Error case
		g_exit_status = 1;
		close(fd);
		return true;
	if (fd < 0)
	{
		dprintf(STDERR_FILENO, YELLOW "%s: No such a file.\n" RESET, nm->program_name);
		LOG(WARNING "%s: No such a file." RESET, nm->program_name);
		goto error;
	}
	if (stat(path, &new_stats) < 0 || fstat(fd, &new_fstats) < 0)
	{
		dprintf(STDERR_FILENO, YELLOW "%s: Failed to get file's details from '%s'\n" RESET, nm->program_name, path);
		LOG(WARNING "%s: Failed to get file's details from '%s'" RESET, nm->program_name, path);
		goto error;
	}
	if (S_ISDIR(new_fstats.st_mode))
	{
		dprintf(STDERR_FILENO, YELLOW "%s: Warning: '%s' is a directory.\n" RESET, nm->program_name, path);
		LOG(WARNING "%s: Warning: '%s' is a directory.", nm->program_name, path);
		goto error;
	}
	if (!S_ISREG(new_fstats.st_mode))
	{
		dprintf(STDERR_FILENO, YELLOW "%s: Warning: '%s' is not a regular file.\n" RESET, nm->program_name, path);
		LOG(WARNING "%s: Warning: '%s' is not a regular file.", nm->program_name, path);
		goto error;
	}
	new_mapping = mmap(NULL, new_fstats.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
	defer(
		print_mmap_errors(new_stats, nm);
		new_mapping = NULL;
	);
	if (new_mapping == MAP_FAILED)
	{
		dprintf(STDERR_FILENO, "%s: " YELLOW "Warning: mmap failed to map %s into memory.\n" RESET, nm->program_name, path);
		dprintf(STDERR_FILENO, "%s" YELLOW "This can happen on an empty file\n" RESET, indent(nm->program_name));
		LOG(WARNING "%s: Warning: mmap failed to map %s into memory.\n" RESET, nm->program_name, path);
		goto error;
	}

	// Check and free the current map if any
	if (nm->file != NULL)
	{
		munmap(nm->file, nm->st.st_size);
		nm->file = NULL;
		LOG(INFO "A file was already opened before and thus been cleared to open the newly requested file.");
	}

	// Copy the newly mapped region to our global one
	nm->file = new_mapping;
	nm->st = new_fstats;
	LOG(INFO "%s successfully mapped", path);
	return false;
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
