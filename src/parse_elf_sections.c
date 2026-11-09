/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_elf_sections.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldalmass <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/28 17:27:06 by ldalmass          #+#    #+#             */
/*   Updated: 2026/11/09 17:46:43 by ldalmass         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_nm.h"

bool	parse_section_header(t_nm *nm)
{
	AUTO_LOG;

	uint64_t	shdr_size = nm->is_64bits ? sizeof(Elf64_Shdr) : sizeof(Elf32_Shdr);

	// Check the section table
	if (nm->elf_hdr.shoff == 0)				goto error;
	if (nm->elf_hdr.shnum == 0)				goto error;
	if (nm->elf_hdr.shentsize != shdr_size)	goto error;
	if (!is_inbound(nm, nm->elf_hdr.shoff, (uint64_t)nm->elf_hdr.shnum * nm->elf_hdr.shentsize))
		goto error;
	if (nm->elf_hdr.shstrndx >= nm->elf_hdr.shnum)	goto error;

	return (LOG(PASS), false);
	error:
		dprintf(STDERR_FILENO, YELLOW "%s: Warning: File format not recognized" RESET, nm->program_name);
		LOG(FAIL);
		return true;
}

bool	parse_sht_symtab(t_nm *nm)
{
	AUTO_LOG;

	goto error;	// TODO: implement sht symtab parsing

	LOG(PASS);
	return false;
	error:
		LOG(FAIL);
		dprintf(STDERR_FILENO, YELLOW "%s: %s: No symbols\n" RESET, nm->program_name, nm->current_opened_filename);
		return true;
}
