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

	// Check and set endianness
	if (file[EI_DATA] == ELFDATANONE || file[EI_DATA] > ELFDATA2MSB)	goto error;
	nm->is_big_endian = (file[EI_DATA] == ELFDATA2MSB);

	LOG(PASS);
	return false;

	error:
		LOG(FAIL);
		dprintf(STDERR_FILENO, YELLOW "%s: Warning: %s: file format not recognized." RESET, nm->program_name, nm->current_opened_filename);
		return true;
}

bool	parse_magic_number(t_nm *nm)
{
	AUTO_LOG;

	if (is_inbound(nm, 0, SELFMAG) && memcmp(nm->file, ELFMAG, SELFMAG) == 0) return (LOG(PASS), false);
	else return (LOG(FAIL), true);
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