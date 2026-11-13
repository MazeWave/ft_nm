#include "../includes/ft_nm.h"

void	print_nm(t_nm *nm unused)
{
	AUTO_LOG;

	LOG(GREEN "program_name " RESET H_BLUE("%s"), nm->program_name);
	LOG(MAGENTA "is_bonus" RESET " %s", BOOL_STR(nm->is_bonus));
	LOG(MAGENTA "is_64bits" RESET " %s", BOOL_STR(nm->is_64bits));
	LOG(BLUE "display_debugger_symbols" RESET " %s", BOOL_STR(nm->display_debugger_symbols));
	LOG(BLUE "display_external_symbols" RESET " %s", BOOL_STR(nm->display_external_symbols));
	LOG(BLUE "display_undefined_symbols" RESET " %s", BOOL_STR(nm->display_undefined_symbols));
	LOG(YELLOW "revert_sorting" RESET " %s", BOOL_STR(nm->revert_sorting));
	LOG(YELLOW "no_sorting" RESET " %s", BOOL_STR(nm->no_sorting));
	return ;
}

#define CASE_STR(x) case x: return #x
static const char	*sht_to_str(uint64_t type)
{
	switch (type)
	{
		CASE_STR(SHT_NULL);
		CASE_STR(SHT_PROGBITS);
		CASE_STR(SHT_SYMTAB);
		CASE_STR(SHT_STRTAB);
		CASE_STR(SHT_RELA);
		CASE_STR(SHT_HASH);
		CASE_STR(SHT_DYNAMIC);
		CASE_STR(SHT_NOTE);
		CASE_STR(SHT_NOBITS);
		CASE_STR(SHT_REL);
		CASE_STR(SHT_SHLIB);
		CASE_STR(SHT_DYNSYM);
		CASE_STR(SHT_INIT_ARRAY);
		CASE_STR(SHT_FINI_ARRAY);
		CASE_STR(SHT_PREINIT_ARRAY);
		CASE_STR(SHT_GROUP);
		CASE_STR(SHT_SYMTAB_SHNDX);
		CASE_STR(SHT_GNU_HASH);
		CASE_STR(SHT_GNU_verdef);
		CASE_STR(SHT_GNU_verneed);
		CASE_STR(SHT_GNU_versym);
		default:
			if (type >= SHT_LOPROC && type <= SHT_HIPROC) return "SHT_PROC (cpu-specific)";
			if (type >= SHT_LOUSER && type <= SHT_HIUSER) return "SHT_USER (app-specific)";
			return "SHT_UNKNOWN";
	}
}

#define FLAG_STR(x) { x, #x }
static const char	*shf_to_str(uint64_t flags, char *buf, size_t size)
{
	static const struct
	{
		uint64_t	bit;
		const char	*name; 
	}
	table[] =
	{
		FLAG_STR(SHF_WRITE),
		FLAG_STR(SHF_ALLOC),
		FLAG_STR(SHF_EXECINSTR),
		FLAG_STR(SHF_MERGE),
		FLAG_STR(SHF_STRINGS),
		FLAG_STR(SHF_INFO_LINK),
		FLAG_STR(SHF_LINK_ORDER),
		FLAG_STR(SHF_OS_NONCONFORMING),
		FLAG_STR(SHF_GROUP),
		FLAG_STR(SHF_TLS),
		FLAG_STR(SHF_COMPRESSED),
	};
	size_t	len = 0;

	buf[0] = '\0';
	for (size_t i = 0; i < sizeof(table) / sizeof(table[0]); i++)
	{
		if (!(flags & table[i].bit))	continue;
		len += snprintf(buf + len, len < size ? size - len : 0, "%s%s", len ? " | " : "", table[i].name);
		flags &= ~table[i].bit;
	}
	if (flags & SHF_MASKPROC)
	{
		len += snprintf(buf + len, len < size ? size - len : 0, "%sSHF_MASKPROC", len ? " | " : "");
		flags &= ~(uint64_t)SHF_MASKPROC;
	}
	if (flags)	// bits nobody defined
		len += snprintf(buf + len, len < size ? size - len : 0, "%s0x%lx", len ? " | " : "", flags);
	if (len == 0)
		snprintf(buf, size, "none");
	return buf;
}

const char	*em_to_str(uint16_t machine)
{
	switch (machine)
	{
		CASE_STR(EM_NONE);
		CASE_STR(EM_M32);
		CASE_STR(EM_SPARC);
		CASE_STR(EM_386);
		CASE_STR(EM_68K);
		CASE_STR(EM_88K);
		CASE_STR(EM_860);
		CASE_STR(EM_MIPS);
		CASE_STR(EM_PARISC);
		CASE_STR(EM_SPARC32PLUS);
		CASE_STR(EM_PPC);
		CASE_STR(EM_PPC64);
		CASE_STR(EM_S390);
		CASE_STR(EM_ARM);
		CASE_STR(EM_SH);
		CASE_STR(EM_SPARCV9);
		CASE_STR(EM_IA_64);
		CASE_STR(EM_X86_64);
		CASE_STR(EM_VAX);
		CASE_STR(EM_AARCH64);
		CASE_STR(EM_RISCV);
		default:
			return "EM_UNKNOWN";
	}
}

const char	*ev_to_str(uint32_t version)
{
	switch (version)
	{
		CASE_STR(EV_NONE);
		CASE_STR(EV_CURRENT);
		default:
			return "EV_UNKNOWN";
	}
}

const char	*et_to_str(uint16_t type)
{
	switch (type)
	{
		CASE_STR(ET_REL);
		CASE_STR(ET_EXEC);
		CASE_STR(ET_DYN);
		CASE_STR(ET_CORE);
		default:
			return "ET_NONE";
	}
}

void	print_elf_hdr(t_nm *nm)
{
	AUTO_LOG;

	LOG(H_BLUE("type") " %s", et_to_str(nm->elf_hdr.type));
	LOG(H_BLUE("shoff") " %x", nm->elf_hdr.shoff);
	LOG(H_BLUE("shentsize") " %u", nm->elf_hdr.shentsize);
	LOG(H_BLUE("shnum") " %u", nm->elf_hdr.shnum);
	LOG(H_BLUE("shstrndx") " %x", nm->elf_hdr.shstrndx);
	LOG(H_BLUE("machine") " %s", em_to_str(nm->elf_hdr.machine));
	LOG(H_BLUE("version") " %s", ev_to_str(nm->elf_hdr.version));

	return ;
}

void	print_sh_hdr(t_nm *nm)
{
	AUTO_LOG;

	// LOG(H_BLUE("name") " %x : %s", nm->sh_hdr.name, get_sh_name(nm));
	LOG(H_BLUE("type") " %s", sht_to_str(nm->sh_hdr.type));
	char	flags_buf[256];
	LOG(H_BLUE("flags") " %s", shf_to_str(nm->sh_hdr.flags, flags_buf, sizeof(flags_buf)));
	LOG(H_BLUE("offset") " %x", nm->sh_hdr.offset);
	LOG(H_BLUE("size") " %u", nm->sh_hdr.size);
	LOG(H_BLUE("link") " %x", nm->sh_hdr.link);
	LOG(H_BLUE("info") " %x", nm->sh_hdr.info);
	LOG(H_BLUE("entsize") " %u", nm->sh_hdr.entsize);
	
	return ;
}
