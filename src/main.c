/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldalmass <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/27 14:08:57 by ldalmass          #+#    #+#             */
/*   Updated: 2026/11/12 13:30:3636 by ldalmass         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_nm.h"

void	free_nm(t_nm *nm)
{
	AUTO_LOG;

	free_file_name(nm);
	if (nm->file != NULL)
	{
		munmap(nm->file, nm->st.st_size);
		nm->file = NULL;
	}

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
	nm->file = NULL;
	nm->current_opened_filename = NULL;

	// Flags
	nm->is_bonus = (strstr(argv[0], "_bonus") == NULL) ? false : true;
	nm->is_64bits = false;
	nm->is_big_endian = false;
	nm->is_prettier = false;
	nm->display_debugger_symbols = false;
	nm->display_external_symbols = false;
	nm->display_undefined_symbols = false;
	nm->revert_sorting = false;
	nm->no_sorting = false;
	return ;
}

int	g_exit_status = 0;

int	main(int argc, char **argv)
{
	AUTO_LOG;
	t_nm	namuru = {0};
	t_nm	*nm = &namuru;

	init_nm(nm, argv);
	opts_parser(argc, argv, nm);

	// loop over each files
	t_file_name	*temp = nm->filenames;
	int			counter = 1;
	while (temp && temp->name)
	{
		LOG("Running on " H("%s") " " H("%d/%d") RESET, temp->name, counter, nm->files_count);
		nm->current_opened_filename = temp->name;
		if (parse_elf(temp->name, nm))	g_exit_status = 1;
		if (parse_section_header(nm))	g_exit_status = 1;
		if (parse_sht_symtab(nm))		g_exit_status = 1;
		// parse_symbols(nm);

		close_file(nm);
		temp = temp->next;
		counter++;
		nm->current_opened_filename = NULL;
	}

	free_nm(nm);
	return (g_exit_status);
}
