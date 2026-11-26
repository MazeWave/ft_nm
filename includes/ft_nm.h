/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_nm.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldalmass <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/28 17:10:35 by ldalmass          #+#    #+#             */
/*   Updated: 2026/11/2626 1313:2727:2424 by ldalmass         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <stddef.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <signal.h>
#include <stdint.h>

#include <getopt.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <elf.h>
#include <endian.h>
#include <locale.h>

#include "log.h"
#include "colors.h"

#define unused __attribute__((unused))

// BEGIN Defer function
#define CAT_(a, b) a##b
#define CAT(a, b) CAT_(a, b)

#define defer(...) \
    void CAT(_defer_fn_, __LINE__)(int *_d) { (void)_d; __VA_ARGS__; } \
    int CAT(_defer_v_, __LINE__) __attribute__((cleanup(CAT(_defer_fn_, __LINE__)))) = 0
// END Defer function

#define true 1
#define false 0
#define bool int

#ifndef GIT_COMMIT
# define GIT_COMMIT "unknown"
#endif

typedef struct s_file_name
{
	char				*name;
	struct s_file_name	*next;
}	t_file_name;

typedef struct s_ehdr	// ELF header
{
	uint16_t	type;
	uint16_t	machine;
	uint16_t	version;
	uint64_t	shoff;
	uint16_t	shentsize;
	uint16_t	shnum;
	uint16_t	shstrndx;
}	t_ehdr;

typedef struct s_shdr	// Section header
{
	uint32_t	nameoff;
	uint32_t	type;
	uint64_t	flags;
	uint32_t	offset;
	uint64_t	size;
	uint32_t	link;
	uint32_t	info;
	uint64_t	entsize;
}	t_shdr;

typedef struct s_sym
{
	const char	*namestr;
	uint32_t	nameoff;
	uint64_t	value;
	uint64_t	size;
	uint16_t	shndx;
	uint8_t		info;
	uint8_t		other;
	char		letter;
}	t_sym;

typedef struct s_nm
{
	// Structs
	// struct s_file_name	*filenames;
	char		**filenames;

	// Currently opened file maped
	void			*file;
	char			*current_opened_filename;
	struct stat		st;
	struct s_ehdr	elf_hdr;
	struct s_shdr	sh_hdr;
	struct s_sym	symbol;
	struct s_sym	*symbols;
	struct s_shdr	sym_tab;
	struct s_shdr	str_tab;
	bool			has_symtab;

	// Flags
	bool	is_bonus;
	bool	is_64bits;
	bool	is_big_endian;
	bool	is_prettier;
	bool	display_debugger_symbols;
	bool	display_external_symbols;
	bool	display_undefined_symbols;
	bool	no_sorting;

	// Files
	char		**files_list;
	uint16_t	files_count;

	// Others
	char		*program_name;
}	t_nm;

extern int32_t	g_exit_status;
extern bool		g_is_reverse_sorting;

// main.c
void	free_nm(t_nm *nm);
void	init_nm(t_nm *nm, char **argv);

// parser_opt.c
void	opts_parser(int argc, char **argv, t_nm *nm);
void	version(t_nm *nm);
void	help(t_nm *nm, int fd);

// parse_files.c
void	init_file_name(t_nm *nm);
void	add_file_name (char *name, t_nm *nm);
void	list_file_name(t_nm *nm);
void	free_file_name(t_nm *nm);

// files.c
bool	open_file(const char *path, t_nm *nm);
void	close_file(t_nm *nm);
void	print_mmap_errors(struct stat st, t_nm *nm);

// parse_elf_hdr.c
bool	parse_elf(const char *path, t_nm *nm);
bool	parse_magic_number(t_nm *nm);
bool	parse_bits(t_nm *nm);

// parse_elf_sections.c
bool	parse_section_header(t_nm *nm);
bool	parse_section_header_entries(t_nm *nm);

// helper_functions.c
bool		is_inbound(t_nm *nm, uint64_t offset, uint64_t size);
bool		fill_elf_header(t_nm *nm);
bool		get_shdr_header_at(t_nm *nm, uint64_t id);
bool		get_sym_entry_at(t_nm *nm, uint64_t id);
const char	*get_sym_name(t_nm *nm, uint32_t nameoff);
uint16_t	rd16(t_nm *nm, uint16_t bytes);
uint32_t	rd32(t_nm *nm, uint32_t bytes);
uint64_t	rd64(t_nm *nm, uint64_t bytes);

// printer.c
void		print_nm(t_nm *nm);
void		print_elf_hdr(t_nm *nm);
void		print_section_header_entry(t_shdr sh_hdr);
void		print_symbol_table(t_sym *sym);
const char	*sht_to_str(uint64_t type);
const char	*shf_to_str(uint64_t flags, char *buf, size_t size);
const char	*em_to_str(uint16_t machine);
const char	*ev_to_str(uint32_t version);
const char	*stt_to_str(uint8_t info);
const char	*stv_to_str(uint8_t other);

// parse_symbols.c
bool	parse_symbols(t_nm *nm);
bool	skip_symbol(t_nm *nm, t_sym *s);
char	get_letter(t_nm *nm, t_sym *s);
char	*get_letter_prettier(t_nm *nm, t_sym *s);
void	print_sorted_symbols(t_nm *nm, uint64_t count);
