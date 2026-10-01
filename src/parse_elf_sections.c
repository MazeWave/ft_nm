/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_elf_sections.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldalmass <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/28 17:27:06 by ldalmass          #+#    #+#             */
/*   Updated: 2026/10/01 16:56:04 by ldalmass         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_nm.h"

bool	parse_section_header(t_nm *nm)
{
	AUTO_LOG;

	switch (nm->is_64bits)
	{
		case true:
			Elf64_Ehdr	*file64 = nm->file;
			if (nm->st.st_size < EI_NIDENT)	return (LOG(FAIL), true);
			if (file64->e_shoff == 0) 		return (LOG(FAIL), true);
			break;
		case false:
			Elf32_Ehdr	*file32 = nm->file;
			if (nm->st.st_size < EI_NIDENT)	return (LOG(FAIL), true);
			if (file32->e_shoff == 0) 		return (LOG(FAIL), true);
			break;
	}
	return (LOG(PASS), false);
}
