/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_symbols.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldalmass <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/28 17:10:35 by ldalmass          #+#    #+#             */
/*   Updated: 2026/11/26 18:37:19 by ldalmass         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../includes/ft_nm.h"

bool	skip_symbol(t_nm *nm, t_sym *s)
{
	uint8_t type = ELF64_ST_TYPE(s->info);
	uint8_t bind = ELF64_ST_BIND(s->info);
	
	// Default: hide debugger-only symbols, unless -a
	if (!nm->display_debugger_symbols
		&& (type == STT_FILE || type == STT_SECTION))
		return true;
	
	// -g: only external (global or weak) symbols
	if (nm->display_external_symbols && bind == STB_LOCAL)
		return true;
	
	// -u: only undefined symbols
	if (nm->display_undefined_symbols && s->shndx != SHN_UNDEF)
		return true;
	
	return false;
}

char	get_letter(t_nm *nm, t_sym *s)
{
	uint8_t	bind = ELF64_ST_BIND(s->info);
	uint8_t	type = ELF64_ST_TYPE(s->info);
	char	c;

	if (type == STT_GNU_IFUNC)			return 'i';
	if (bind == STB_GNU_UNIQUE)			return 'u';
	if (bind == STB_WEAK)
	{
		if (type == STT_OBJECT)			return (s->shndx == SHN_UNDEF) ? 'v' : 'V';
		return (s->shndx == SHN_UNDEF) ? 'w' : 'W';
	}
	if (s->shndx == SHN_UNDEF)			return 'U';

	if		(s->shndx == SHN_ABS)		c = 'A';
	else if	(s->shndx == SHN_COMMON)	c = 'C';
	else if	(s->shndx >= nm->elf_hdr.shnum || get_shdr_header_at(nm, s->shndx))
										return '?';
	else
	{
		uint64_t f = nm->sh_hdr.flags;
		if		(nm->sh_hdr.type == SHT_NOBITS && (f & SHF_ALLOC) && (f & SHF_WRITE))	c = 'B';
		else if	((f & SHF_ALLOC) && (f & SHF_EXECINSTR))								c = 'T';
		else if	((f & SHF_ALLOC) && (f & SHF_WRITE))									c = 'D';
		else if	(f & SHF_ALLOC)															c = 'R';
		else																			c = 'N';
	}
	return bind == STB_LOCAL ? c + 32 : c; // lowercase = local ('A' + 32 = 'a')
}

static int	cmp_sym(const void *p1, const void *p2)
{
	const t_sym	*a = p1;
	const t_sym	*b = p2;
	const char	*na = a->namestr ? a->namestr : "";
	const char	*nb = b->namestr ? b->namestr : "";
	
	int	r = strcoll(na, nb);		// 1. by name
	if (g_is_reverse_sorting)
		r = -r;
	if (r == 0)						// 2. same name → by address
		r = (a->value > b->value) - (a->value < b->value);
	return r;
}

static const char	*letter_color(char c)
{
	if (c == '?')	return H_BLACK("?");			// unknown symbol
	if (c == 'U')	return H("U");					// undefined symbol
	if (c == 'D')	return H_GREEN("D");			// initialized symbol
	if (c == 'u')	return H_MAGENTA("u");			// unique symbol
	if (c == 'w')	return H_BLUE("w");				// weak untagged symbol
	if (c == 'W')	return H_BLUE("W");				// weak untagged symbol
	if (c == 'v')	return H_RED("v");				// symbol is weak object
	if (c == 'i')	return H_BLUE("i");				// DDL symbol (Windows)
	if (c == 'T')	return H_YELLOW("T");			// symbol is in text code section
	if (c == 't')	return H_YELLOW("t");			// symbol is in text code section
	if (c == 'B')	return H_CYAN(BLACK"B"RESET);	// symbol is in .bss data
	if (c == 'b')	return H_CYAN(BLACK"b"RESET);	// symbol is in .bss data
	if (c == 'R')	return H_GREEN("R");			// symbol is in read-only data section
	if (c == 'r')	return H_GREEN("r");			// symbol is in read-only data section
	if (c == 'D')	return H_RED("D");				// symbol is initialized in data section
	if (c == 'd')	return H_RED("d");				// symbol is initialized in data section
	if (c == 'a')	return H_MAGENTA("a");			// debugger symbol
	return (H_RED("FORGOT OMG"));
}

void	print_sorted_symbols(t_nm *nm, uint64_t count)
{
	AUTO_LOG;
	
	if (nm->files_count > 1)
		dprintf(STDOUT_FILENO, "\n%s:\n", nm->current_opened_filename);

	switch (nm->is_prettier)
	{
		case true:
			for (uint64_t i = 1; i < count; i++)
			{
				if (skip_symbol(nm, &nm->symbols[i]))	continue;
				if (nm->symbols[i].shndx == SHN_UNDEF)
					dprintf(STDOUT_FILENO, "%*s", nm->is_64bits ? 16 : 8, "");
				else
					dprintf(STDOUT_FILENO, nm->is_64bits ? BLUE "%016lx" : BLUE "%08lx", nm->symbols[i].value);
				nm->symbols[i].letter = get_letter(nm, &nm->symbols[i]);
				dprintf(STDOUT_FILENO, " %s" GREEN " %s\n" RESET, letter_color(nm->symbols[i].letter), nm->symbols[i].namestr);
			}
			break;
		case false:
			for (uint64_t i = 1; i < count; i++)
			{
				if (skip_symbol(nm, &nm->symbols[i]))	continue;
				if (nm->symbols[i].shndx == SHN_UNDEF)
					dprintf(STDOUT_FILENO, "%*s", nm->is_64bits ? 16 : 8, "");
				else
					dprintf(STDOUT_FILENO, nm->is_64bits ? "%016lx" : "%08lx", nm->symbols[i].value);
				nm->symbols[i].letter = get_letter(nm, &nm->symbols[i]);
				dprintf(STDOUT_FILENO, " %c %s\n", nm->symbols[i].letter, nm->symbols[i].namestr);
			}
			break;
	}

	return ;
}

bool	parse_symbols(t_nm *nm)
{
	AUTO_LOG;

	uint64_t	count = nm->sym_tab.size / nm->sym_tab.entsize;
	nm->symbols = calloc(count, sizeof(t_sym));
	if (!nm->symbols)	goto error;
	defer(free(nm->symbols), nm->symbols = NULL);

	for (uint64_t i = 1; i < count; i++)
	{
		LOG(H_BLUE("%d/%d"), i, count - 1);
		if (get_sym_entry_at(nm, i))	goto error;
		
		print_symbol_table(&nm->symbol);
		nm->symbols[i] = nm->symbol;
	}

	if (!nm->no_sorting)
		qsort(nm->symbols + 1, count - 1, sizeof(t_sym), cmp_sym);
	print_sorted_symbols(nm, count);

	LOG(PASS);
	return false;
	error:
		LOG(FAIL);
		return true;
}
