/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_nm.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldalmass <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/28 17:10:35 by ldalmass          #+#    #+#             */
/*   Updated: 2026/08/27 14:2929:0505 by ldalmass         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <stddef.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <signal.h>
#include <stdint.h>

#include <getopt.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <elf.h>

#include "log.h"
#include "colors.h"

#define unused __attribute__((unused))

#define true 1
#define false 0
#define bool int

#ifndef GIT_COMMIT
# define GIT_COMMIT "unknown"
#endif

// extern volatile bool g_is_running;

typedef struct s_file_name
{
	char				*name;
	struct s_file_name	*next;
} t_file_name;

typedef struct s_nm
{
	// Structs
	struct s_file_name	*filenames;

	// Opened file in memory (whole file)
	struct stat	st;
	void		*file;

	// Flags
	bool	is_bonus;
	bool	is_64bits;
	bool	display_debugger_symbols;
	bool	display_external_symbols;
	bool	display_undefined_symbols;
	bool	revert_sorting;
	bool	no_sorting;

	// Files
	char		**files_list;
	uint16_t	files_count;

	// Others
	char		*program_name;
} t_nm;

// main.c
void	free_nm(t_nm *nm);
void	init_nm(t_nm *nm, char **argv);
void	print_nm(t_nm *nm);

// parset_opt.c
bool	opts_parser(int argc, char **argv, t_nm *nm);
void	version(t_nm *nm);
void	help(t_nm *nm);

// parse_files.c
void	init_file_name(t_nm *nm);
void	add_file_name(char *name, t_nm *nm);
void	list_file_name(t_nm *nm);
void	free_file_name(t_nm *nm);

// parse_elf_hdr.c
bool	parse_elf_hdr(const char *path, t_nm *nm unused);
bool	parse_magic_number(t_nm *nm);

// files.c
bool	open_file(const char *path, t_nm *nm);
void	print_mmap_errors(struct stat st, t_nm *nm);
