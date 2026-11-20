/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_symbols.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldalmass <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/28 17:10:35 by ldalmass          #+#    #+#             */
/*   Updated: 2026/11/20 15:50:34 by ldalmass         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../includes/ft_nm.h"

char	get_letter(t_nm *nm, t_sym *s)
{
    uint8_t bind = ELF64_ST_BIND(s->info);
    uint8_t type = ELF64_ST_TYPE(s->info);

    if (type == STT_GNU_IFUNC)       return 'i';
    if (bind == STB_GNU_UNIQUE)      return 'u';
    if (bind == STB_WEAK)
    {
        if (type == STT_OBJECT)      return s->shndx == SHN_UNDEF ? 'v' : 'V';
        return s->shndx == SHN_UNDEF ? 'w' : 'W';
    }
    if (s->shndx == SHN_UNDEF)       return 'U';

    char c;
    if      (s->shndx == SHN_ABS)    c = 'A';
    else if (s->shndx == SHN_COMMON) c = 'C';
    else if (s->shndx >= nm->elf_hdr.shnum || get_shdr_header_at(nm, s->shndx))
                                     return '?';
    else
    {
        uint64_t f = nm->sh_hdr.flags;
        if      (nm->sh_hdr.type == SHT_NOBITS && (f & SHF_ALLOC) && (f & SHF_WRITE)) c = 'B';
        else if ((f & SHF_ALLOC) && (f & SHF_EXECINSTR))  c = 'T';
        else if ((f & SHF_ALLOC) && (f & SHF_WRITE))      c = 'D';
        else if (f & SHF_ALLOC)                           c = 'R';
        else                                              c = 'N';
    }
    return bind == STB_LOCAL ? c + 32 : c;   // lowercase = local ('A' + 32 = 'a')
}

bool	parse_symbols(t_nm *nm)
{
	AUTO_LOG;

	uint64_t count = nm->sym_tab.size / nm->sym_tab.entsize;
	for (uint64_t i = 0; i < count; i++)
	{
		LOG(H_BLUE("%d/%d"), i, count - 1);
		get_sym_entry_at(nm, i);
		print_symbol_table(&nm->symbol);
	}

	LOG(PASS);
	return false;
	// error:
	// 	LOG(FAIL);
	// 	return false;
}
