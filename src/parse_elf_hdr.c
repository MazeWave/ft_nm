/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_elf_hdr.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldalmass <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/28 13:13:59 by ldalmass          #+#    #+#             */
/*   Updated: 2026/08/28 15:43:12 by ldalmass         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_nm.h"

bool	parse_magic_number(t_nm *nm unused)
{
	AUTO_LOG;

	if (nm->st.st_size >= SELFMAG && memcmp(nm->file, ELFMAG, SELFMAG) == 0) return (LOG(PASS), false);
	else return (LOG(FAIL), true);
	return false;
}

bool	parse_elf_hdr(const char *path, t_nm *nm unused)
{
	AUTO_LOG;

	bool	failed = false;
	failed += open_file(path, nm);
	
	failed += parse_magic_number(nm);
	// failed += parse_architecture(fd);

	return (failed);
}