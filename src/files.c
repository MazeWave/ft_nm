/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   files.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldalmass <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/28 13:17:05 by ldalmass          #+#    #+#             */
/*   Updated: 2026/11/05 17:00:13 by ldalmass         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_nm.h"

bool	open_file(const char *path, t_nm *nm)
{
	AUTO_LOG;

	int			fd = open(path, O_RDONLY);
	void		*new_mapping = NULL;
	struct stat	new_stats;

	// Open and map the file
	if (fd < 0)
	{
		dprintf(STDERR_FILENO, YELLOW "%s: File descriptor failed to open '%s'.\n" RESET, nm->program_name, path);
		LOG(WARNING "%s: File descriptor failed to open '%s'." RESET, nm->program_name, path);
		return true;
	}
	if (fstat(fd, &new_stats) < 0)
	{
		dprintf(STDERR_FILENO, YELLOW "%s: Failed to get file's details from '%s'\n" RESET, nm->program_name, path);
		LOG(WARNING "%s: Failed to get file's details from '%s'" RESET, nm->program_name, path);
		close(fd);
		return true;
	}
	if (S_ISDIR(new_stats.st_mode))
	{
		g_exit_status = 1;
		dprintf(STDERR_FILENO, YELLOW "%s: Warning: '%s' is a directory.\n" RESET, nm->program_name, path);
		LOG(WARNING "%s: Warning: '%s' is a directory.", path);
		close(fd);
		return true;
	}
	if (!S_ISREG(new_stats.st_mode))
	{
		g_exit_status = 1;
		dprintf(STDERR_FILENO, YELLOW "%s: Warning: '%s' is not a regular file.\n" RESET, nm->program_name, path);
		LOG(WARNING "%s: Warning: '%s' is not a regular file.", path);
		close(fd);
		return true;
	}
	new_mapping = mmap(NULL, new_stats.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
	close(fd);
	if (new_mapping == MAP_FAILED)
	{
		g_exit_status = 1;
		dprintf(STDERR_FILENO, "%s: " YELLOW "Warning: mmap failed to map %s into memory.\n" RESET, nm->program_name, path);
		LOG(WARNING "%s: Warning: mmap failed to map %s into memory.\n" RESET, nm->program_name, path);
		return (print_mmap_errors(new_stats, nm), new_mapping = NULL, true);
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
	nm->st = new_stats;
	LOG(BG_GREEN BLACK " Opened " RESET " %s", path);
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
	printf(RED "%s: mmap: ", nm->program_name);
	switch (st.st_mode & S_IFMT)
	{
    	default:		printf("unknown file type\n");						break;
		case S_IFBLK:	printf("block device\n");							break;
		case S_IFCHR:	printf("character device\n");						break;
		case S_IFDIR:	printf("is directory\n");							break;
		case S_IFIFO:	printf("is FIFO/pipe\n");							break;
		case S_IFLNK:	printf("is symlink\n");								break;
		case S_IFSOCK:	printf("is socket\n");								break;
		case S_IFREG:	printf("%s, regular file\n", strerror(errno));		break;
    }
    printf(RESET);
	return ;
}
