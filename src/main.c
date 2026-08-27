/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldalmass <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/27 14:08:57 by ldalmass          #+#    #+#             */
/*   Updated: 2026/08/27 19:00:05 by ldalmass         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_nm.h"

static void nm_aout(t_nm *nm unused)
{
	AUTO_LOG;

	LOG(BG_BLUE "test" RESET);
	return ;
}

void	free_nm(t_nm *nm)
{
	AUTO_LOG;

	free_file_name(nm);

	return ;
}

void	init_nm(t_nm *nm, char **argv)
{
	AUTO_LOG;

	// Structs
	init_file_name(nm);
	
	// Others
	nm->program_name = argv[0];
	nm->files_count = 0;
	nm->files_list = NULL;

	// Flags
	nm->is_bonus = (strstr(argv[0], "_bonus") == NULL) ? false : true;
	nm->display_debugger_symbols = false;
	nm->display_external_symbols = false;
	nm->display_undefined_symbols = false;
	nm->revert_sorting = false;
	nm->no_sorting = false;
	return ;
}

void	print_nm(t_nm *nm)
{
	AUTO_LOG;
	LOG(BG_GREEN " program_name " RESET "%s", nm->program_name);
	LOG(BG_MAGENTA " is_bonus " RESET " %d", nm->is_bonus);
	LOG(BG_BLUE " display_debugger_symbols " RESET " %d", nm->display_debugger_symbols);
	LOG(BG_BLUE " display_external_symbols " RESET " %d", nm->display_external_symbols);
	LOG(BG_BLUE " display_undefined_symbols " RESET " %d", nm->display_undefined_symbols);
	LOG(BG_YELLOW BLACK " revert_sorting " RESET " %d", nm->revert_sorting);
	LOG(BG_YELLOW BLACK " no_sorting " RESET " %d", nm->no_sorting);
	return ;
}

int	main(int argc, char **argv)
{
	AUTO_LOG;
	t_nm	namuru;
	t_nm	*nm = &namuru;

	init_nm(nm, argv);
	if (opts_parser(argc, argv, nm) == EXIT_FAILURE) return (EXIT_FAILURE);
	if (argc < 2) nm_aout(nm);
	free_nm(nm);
	return (EXIT_SUCCESS);
}