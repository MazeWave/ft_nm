/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_elf_hdr.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldalmass <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/28 13:13:59 by ldalmass          #+#    #+#             */
/*   Updated: 2026/08/28 18:15:36 by ldalmass         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_nm.h"

bool	fill_elf_hdr_struct(t_nm *nm)
{
	AUTO_LOG;
	uint8_t	*file = nm->file;

	if (nm->st.st_size < EI_NIDENT) return (LOG(FAIL), true);
	nm->elf_hdr.type = file[0x10];
	nm->elf_hdr.machine = file[0x12];
	switch (nm->is_64bits)
	{
		case true:
			nm->elf_hdr.shoff.shoff64 = file[0x28];
			nm->elf_hdr.shentsize = file[0x3A];
			nm->elf_hdr.shnum = file[0x3C];
			nm->elf_hdr.shstrndx = file[0x3E];
			break;
		case false:
			nm->elf_hdr.shoff.shoff32 = file[0x20];
			nm->elf_hdr.shentsize = file[0x2E];
			nm->elf_hdr.shnum = file[0x30];
			nm->elf_hdr.shstrndx = file[0x32];
			break;
	}
	LOG(PASS);
	return false;
}

bool	parse_architecture(t_nm *nm)
{
	AUTO_LOG;
	Elf64_Ehdr	*file = nm->file;

	if (nm->st.st_size < EI_NIDENT) return (LOG(FAIL), true);
	if (file->e_machine == EM_386 || file->e_machine == EM_X86_64) return (LOG(PASS), false);
	else return(LOG(FAIL), true);
}

bool	parse_bits(t_nm *nm)
{
	AUTO_LOG;
	uint8_t	*file = nm->file;

	if (nm->st.st_size < EI_NIDENT)		return (LOG(FAIL), true);
	if (file[EI_CLASS] == ELFCLASSNONE)	return (LOG(FAIL), true);
	if (file[EI_CLASS] > ELFCLASS64)	return (LOG(FAIL), true);

	nm->is_64bits = (file[EI_CLASS] == ELFCLASS64);
	LOG(PASS);
	return false;
}

bool	parse_magic_number(t_nm *nm)
{
	AUTO_LOG;

	if (nm->st.st_size >= SELFMAG && memcmp(nm->file, ELFMAG, SELFMAG) == 0) return (LOG(PASS), false);
	else return (LOG(FAIL), true);
}

bool	parse_elf_hdr(const char *path, t_nm *nm)
{
	AUTO_LOG;

	bool	failed = false;
	failed += open_file(path, nm);
	failed += parse_magic_number(nm);
	failed += parse_bits(nm);

	print_nm(nm);
	return (failed);
}