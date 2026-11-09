/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_opt.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldalmass <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/27 14:22:16 by ldalmass          #+#    #+#             */
/*   Updated: 2026/11/09 18:04:13 by ldalmass         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_nm.h"

static void	can_use_bonus_flag(t_nm *nm, int fd)
{
	if (nm->is_bonus)	return ;
	else
	{
		help(nm, fd);
		free_nm(nm);
		exit(EXIT_FAILURE);
	}

	return ;
}

void	opts_parser(int argc, char **argv, t_nm *nm)
{
	AUTO_LOG;
	int	opt = 0;

	LOG(BLUE "optind: %d, argc: %d" RESET, optind, argc);
	while ((opt = getopt(argc, argv, "-h?vagurp")) != -1)
	{
		switch (opt)
		{
			// BONUSES
			case 'a':
				can_use_bonus_flag(nm, STDOUT_FILENO);
				nm->display_debugger_symbols = true;
				break;
			case 'g':
				can_use_bonus_flag(nm, STDOUT_FILENO);
				nm->display_external_symbols = true;
				break;
			case 'u':
				can_use_bonus_flag(nm, STDOUT_FILENO);
				nm->display_undefined_symbols = true;
				break;
			case 'r':
				can_use_bonus_flag(nm, STDOUT_FILENO);
				nm->revert_sorting = true;
				break;
			case 'p':
				can_use_bonus_flag(nm, STDOUT_FILENO);
				nm->no_sorting = true;
				break;
			case 'b':
				can_use_bonus_flag(nm, STDOUT_FILENO);
				nm->is_prettier = true;
				break;

			// MANDATORY
			case 'v':
				version(nm);
				free_nm(nm);
				exit (EXIT_SUCCESS);
			case '?':
				help(nm, optopt == 0 ? STDOUT_FILENO : STDERR_FILENO);
				free_nm(nm);
				exit(optopt == 0 ? EXIT_SUCCESS : EXIT_FAILURE);
			case 'h':
				help(nm, STDOUT_FILENO);
				free_nm(nm);
				exit (EXIT_SUCCESS);
			default:
				nm->files_count++;
				add_file_name(optarg, nm);
				break;
		}
	}

	print_nm(nm);

	// No file specified, defaulting to read a.out
	if (nm->files_count == 0)
	{
		add_file_name("a.out", nm);
		nm->files_count = 1;
	}
	list_file_name(nm);

	return ;
}

void	version(t_nm *nm)
{
	dprintf(1, GREEN "%s on commit " BG_GREEN BLACK " %s " RESET ".\n" RESET, nm->program_name, GIT_COMMIT);
	return ;
}

void	help(t_nm *nm, int fd)
{
	switch (nm->is_bonus)
	{
		case true:
			dprintf(fd, GREEN "Usage: %s [option(s)] [file(s)]\nList symbols in [file(s)] (a.out by default).\n" RESET, nm->program_name);
			dprintf(fd, BLUE "Options:\n" RESET);
			dprintf(fd, MAGENTA "  -b        : Prettier display, beautiful isn't it ?\n" RESET);
			dprintf(fd, MAGENTA "  -a        : Display debugger-only symbols\n" RESET);
			dprintf(fd, MAGENTA "  -g        : Display only external symbols\n" RESET);
			dprintf(fd, MAGENTA "  -u        : Display only undefined symbols\n" RESET);
			dprintf(fd, MAGENTA "  -r        : Reverse the sense of the sort\n" RESET);
			dprintf(fd, MAGENTA "  -p        : Do not sort the symbols\n" RESET);
			dprintf(fd, MAGENTA "  -h -?     : Show this help screen\n" RESET);
			break;
		default:
			dprintf(fd, GREEN "Usage: %s [option(s)] [file(s)]\nList symbols in [file(s)] (a.out by default).\n" RESET, nm->program_name);
			break;
	}

	return ;
}
