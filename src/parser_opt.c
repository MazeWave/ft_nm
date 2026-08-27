/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_opt.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldalmass <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/27 14:22:16 by ldalmass          #+#    #+#             */
/*   Updated: 2026/08/27 16:24:44 by ldalmass         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_nm.h"

bool	opts_parser(int argc unused, char **argv unused, t_nm *nm)
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
				if (!nm->is_bonus) return (help(nm), false);
				else nm->display_debugger_symbols = true;
				break;
			case 'g':
				if (!nm->is_bonus) return (help(nm), false);
				else nm->display_external_symbols = true;
				break;
			case 'u':
				if (!nm->is_bonus) return (help(nm), false);
				else nm->display_undefined_symbols = true;
				break;
			case 'r':
				if (!nm->is_bonus) return (help(nm), false);
				else nm->revert_sorting = true;
				break;
			case 'p':
				if (!nm->is_bonus) return (help(nm), false);
				else nm->no_sorting = true;
				break;

			// MANDATORY
			case 'v':
				return (version(nm), false);
			case '?':
				return (help(nm), false);
			case 'h':
				return (help(nm), false);
			default:
				LOG(BG_CYAN BLACK "%s" RESET, optarg);
				nm->files_count++;
				// nm->files_list[nm->files_count - 1] = optarg; //crashes
				break;
		}
	}

	print_nm(nm);
	return false;
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
			printf(MAGENTA "  -h -?     : Show this help screen" RESET);
			break;
		default:
			printf(GREEN "Usage: %s [option(s)] [file(s)]\nList symbols in [file(s)] (a.out by default).\n" RESET, nm->program_name);
			break;
	}

	return ;
}
