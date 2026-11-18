/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_elf_sections.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldalmass <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/28 17:27:06 by ldalmass          #+#    #+#             */
/*   Updated: 2026/11/18 14:09:17 by ldalmass         ###   ########.fr       */
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
	if (get_shdr_header_at(nm, nm->elf_hdr.shstrndx))	goto error;
	
	for (uint64_t i = 0; i < nm->elf_hdr.shnum; i++)
	{
		LOG(H_BLUE("%u/%u"), i, nm->elf_hdr.shnum - 1);
		if (get_shdr_header_at(nm, i))								goto error;
		if (nm->sh_hdr.type != SHT_SYMTAB)							continue;
		if (!is_inbound(nm, nm->sh_hdr.nameoff, nm->sh_hdr.size))	goto error;
		if (nm->sh_hdr.link >= nm->elf_hdr.shnum)					goto error;

		// We save our symbol header
		nm->sym_hdr = nm->sh_hdr;
		print_sh_hdr(nm);
		LOG(PASS);
		return false;
	}
		dprintf(STDERR_FILENO, YELLOW "%s: %s: no symbols\n" RESET, nm->program_name, nm->current_opened_filename);
		return false;

	error:
		LOG(FAIL);
		dprintf(STDERR_FILENO, YELLOW "%s: %s: file format not recognized\n" RESET, nm->program_name, nm->current_opened_filename);
		return true;
}
