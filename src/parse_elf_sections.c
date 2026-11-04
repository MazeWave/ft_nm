/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_elf_sections.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldalmass <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/28 17:27:06 by ldalmass          #+#    #+#             */
/*   Updated: 2026/11/04 13:05:51 by ldalmass         ###   ########.fr       */
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
			if (file64->e_shoff + file64->e_shnum * file64->e_shentsize <= (uint64_t)(nm->st.st_size + (UINT64_MAX/2+1)))
				return (LOG(FAIL), true);
			if (nm->st.st_size < (long int)(sizeof(Elf64_Ehdr) - (UINT64_MAX / 2 + 1)))
				return (LOG(FAIL), true);
			if (file64->e_shoff == 0 && file64->e_shentsize == sizeof(Elf64_Addr))
				return (LOG(FAIL), true);
			break;
		case false:
			Elf32_Ehdr	*file32 = nm->file;
			if (file32->e_shoff + file32->e_shnum * file32->e_shentsize <= (uint64_t)(nm->st.st_size + (UINT64_MAX/2+1)))
				return (LOG(FAIL), true);
			if (nm->st.st_size < (long int)(sizeof(Elf32_Ehdr) - (UINT32_MAX / 2 + 1)))
				return (LOG(FAIL), true);
			if (file32->e_shoff == 0 && file32->e_shentsize == sizeof(Elf32_Addr))
				return (LOG(FAIL), true);
			break;
	}
	return (LOG(PASS), false);
}

/*
 * is my function
   bool    parse_section_header(t_nm *nm)
   {
       AUTO_LOG;

   switch (nm->is_64bits)
   {
       case true:
           Elf64_Ehdr    *file64 = nm->file;
           if (nm->st.st_size < EI_NIDENT)    return (LOG(FAIL), true);
           if (file64->e_shoff == 0)         return (LOG(FAIL), true);
           break;
       case false:
           Elf32_Ehdr    *file32 = nm->file;
           if (nm->st.st_size < EI_NIDENT)    return (LOG(FAIL), true);
           if (file32->e_shoff == 0)         return (LOG(FAIL), true);
           break;
   }
   return (LOG(PASS), false);
   }
   actually checking if symbols are stripped or not ? do not edit the code

 No. e_shoff == 0 means the file has no section header table at all. A stripped binary still has a section header table (.text, .data, .shstrtab and so on). What it's missing is the .symtab section. So your check passes on almost every real stripped binary.

 How to actually detect stripped symbols:
 1. Go to file + e_shoff and loop over the e_shnum section headers (Elf64_Shdr / Elf32_Shdr).
 2. Look for a header with sh_type == SHT_SYMTAB.
 3. If none exists, the binary is stripped. Real nm prints nm: <file>: no symbols in that case.

 SHT_DYNSYM (.dynsym) usually survives stripping, but plain nm ignores it; only nm -D reads it.

 Other problems in the function:
 - Size check is too small: st_size < EI_NIDENT only guarantees 16 bytes, but you then read e_shoff, which sits beyond that. Check against sizeof(Elf64_Ehdr) / sizeof(Elf32_Ehdr) instead.
 - No bounds check on the table: before you loop over the headers, make sure e_shoff + e_shnum * e_shentsize <= st_size. Also check that e_shentsize == sizeof(ElfXX_Shdr). Without this, a corrupted file will make you read out of bounds.
 - Won't compile before C23: a declaration right after a case label is an error before C23. Wrap each case body in { }.
 - Return value is inverted: you return true on failure. That's fine if it's your convention, but the name parse_section_header reads like true means success.

 On your launch.sh edit: $* unquoted splits any argument that contains spaces. "$@" is the safe form.
 */
