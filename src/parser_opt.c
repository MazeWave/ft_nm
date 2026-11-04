/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_opt.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldalmass <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/27 14:22:16 by ldalmass          #+#    #+#             */
/*   Updated: 2026/11/04 16:50:46 by ldalmass         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_nm.h"

static void	can_use_bonus_flag(t_nm *nm)
{
	if (nm->is_bonus)	return ;
	else
	{
		help(nm);
		exit(EXIT_FAILURE);
	}

	return ;
}

void	opts_parser(int argc unused, char **argv unused, t_nm *nm)
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
				can_use_bonus_flag(nm);
				nm->display_debugger_symbols = true;
				break;
			case 'g':
				can_use_bonus_flag(nm);
				nm->display_external_symbols = true;
				break;
			case 'u':
				can_use_bonus_flag(nm);
				nm->display_undefined_symbols = true;
				break;
			case 'r':
				can_use_bonus_flag(nm);
				nm->revert_sorting = true;
				break;
			case 'p':
				can_use_bonus_flag(nm);
				nm->no_sorting = true;
				break;

			// MANDATORY
			case 'v':
				version(nm);
				exit (EXIT_SUCCESS);
			case '?':
				help(nm);
				exit (EXIT_SUCCESS);
			case 'h':
				help(nm);
				exit (EXIT_SUCCESS);
			default:
				nm->files_count++;
				add_file_name(optarg, nm);
				break;
		}
	}

	print_nm(nm);

	// No file specified, defaulting to read a.out
	if (nm->files_count == 0) add_file_name("a.out", nm);
	list_file_name(nm);

	return ;
}

void	version(t_nm *nm)
{
	printf(GREEN "%s on commit " BG_GREEN BLACK " %s " RESET ".\n" RESET, nm->program_name, GIT_COMMIT);
	return ;
}

void	help(t_nm *nm)
{
	switch (nm->is_bonus)
	{
		case true:
			printf(GREEN "Usage: %s [option(s)] [file(s)]\nList symbols in [file(s)] (a.out by default).\n" RESET, nm->program_name);
			printf(BLUE "Options:\n" RESET);
			printf(MAGENTA "  -a        : Display debugger-only symbols\n" RESET);
			printf(MAGENTA "  -g        : Display only external symbols\n" RESET);
			printf(MAGENTA "  -u        : Display only undefined symbols\n" RESET);
			printf(MAGENTA "  -r        : Reverse the sense of the sort\n" RESET);
			printf(MAGENTA "  -p        : Do not sort the symbols\n" RESET);
			printf(MAGENTA "  -h -?     : Show this help screen\n" RESET);
			break;
		default:
			printf(GREEN "Usage: %s [option(s)] [file(s)]\nList symbols in [file(s)] (a.out by default).\n" RESET, nm->program_name);
			break;
	}

	return ;
}
