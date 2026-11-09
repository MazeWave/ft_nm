/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_elf_hdr.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldalmass <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/28 13:13:59 by ldalmass          #+#    #+#             */
/*   Updated: 2026/11/09 15:03:2020 by ldalmass         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_nm.h"

bool	parse_bits(t_nm *nm)
{
	AUTO_LOG;
	uint8_t	*file = nm->file;

	if (!is_inbound(nm, 0, EI_NIDENT))	goto error;
	if (file[EI_CLASS] == ELFCLASSNONE)	goto error;
	if (file[EI_CLASS] > ELFCLASS64)	goto error;

	// Set if we are using 64bits or 32bits
	nm->is_64bits = (file[EI_CLASS] == ELFCLASS64);
	LOG(PASS);
	return false;

	error:
		LOG(FAIL);
		return true;
}

bool	parse_magic_number(t_nm *nm)
{
	AUTO_LOG;

	if (is_inbound(nm, 0, SELFMAG) && memcmp(nm->file, ELFMAG, SELFMAG) == 0) return (LOG(PASS), false);
	else return (LOG(FAIL), true);
}

bool	fill_elf_header(t_nm *nm)
{
	AUTO_LOG;

	if (nm->is_64bits)
	{
		if (!is_inbound(nm, 0, sizeof(Elf64_Ehdr)))	goto error;
		Elf64_Ehdr	*hdr = nm->file;
		nm->elf_hdr = (t_ehdr)
		{
			hdr->e_type,
			hdr->e_shoff,
			hdr->e_shentsize,
			hdr->e_shnum,
			hdr->e_shstrndx,
		};
	}
	else
	{
		if (!is_inbound(nm, 0, sizeof(Elf32_Ehdr)))	goto error;
		Elf32_Ehdr	*hdr = nm->file;
		nm->elf_hdr = (t_ehdr)
		{
			hdr->e_type,
			hdr->e_shoff,
			hdr->e_shentsize,
			hdr->e_shnum,
			hdr->e_shstrndx,
		};
	}

	LOG(PASS);
	return false;
	error:
		LOG(FAIL);
		return true;
}

bool	parse_elf(const char *path, t_nm *nm)
{
	AUTO_LOG;

	if (open_file(path, nm))	return (true);
	if (parse_magic_number(nm))	return (true);
	if (parse_bits(nm))			return (true);

	
	if (fill_elf_header(nm))	return(true);
	print_nm(nm);
	return (false);
}