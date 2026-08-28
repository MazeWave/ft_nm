/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_elf_hdr.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldalmass <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/28 13:13:59 by ldalmass          #+#    #+#             */
/*   Updated: 2026/08/28 17:01:43 by ldalmass         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_nm.h"

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