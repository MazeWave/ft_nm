#include "../includes/ft_nm.h"

bool	is_inbound(t_nm *nm, uint64_t offset, uint64_t size)
{
	if (offset > (uint64_t)(nm->st.st_size))			return (false);
	if (size > ((uint64_t)(nm->st.st_size) - offset))	return (false);
	return (true);
}

bool	fill_elf_header(t_nm *nm)
{
	AUTO_LOG;

	if (nm->is_64bits)
	{
		if (!is_inbound(nm, 0, sizeof(Elf64_Ehdr)))	goto error;
		Elf64_Ehdr	hdr;
	 	memcpy (&hdr, nm->file, sizeof(Elf64_Ehdr));
		nm->elf_hdr = (t_ehdr)
		{
			.type = rd16(nm, hdr.e_type),
			.shoff = rd64(nm, hdr.e_shoff),
			.shentsize = rd16(nm, hdr.e_shentsize),
			.shnum = rd16(nm, hdr.e_shnum),
			.shstrndx = rd16(nm, hdr.e_shstrndx),
			.machine = rd16(nm, hdr.e_machine),
			.version = rd32(nm, hdr.e_version),
		};
	}
	else
	{
		if (!is_inbound(nm, 0, sizeof(Elf32_Ehdr)))	goto error;
		Elf32_Ehdr	hdr;
	 	memcpy (&hdr, nm->file, sizeof(Elf32_Ehdr));
		nm->elf_hdr = (t_ehdr)
		{
			.type = rd16(nm, hdr.e_type),
			.shoff = rd32(nm, hdr.e_shoff),
			.shentsize = rd16(nm, hdr.e_shentsize),
			.shnum = rd16(nm, hdr.e_shnum),
			.shstrndx = rd16(nm, hdr.e_shstrndx),
			.machine = rd16(nm, hdr.e_machine),
			.version = rd32(nm, hdr.e_version),
		};
	}

	LOG(PASS);
	return false;
	error:
		LOG(FAIL);
		return true;
}

bool	get_shdr_header_at(t_nm *nm, uint64_t id)
{
	// AUTO_LOG;

	if (id >= nm->elf_hdr.shnum)	goto error;
	uint64_t	offset = id * nm->elf_hdr.shentsize + nm->elf_hdr.shoff;
	//                   id * size of a section     + offset start of the section header

	if (nm->is_64bits)
	{
		if (!is_inbound(nm, offset, sizeof(Elf64_Shdr)))	goto error;
		Elf64_Shdr	hdr;
		memcpy(&hdr, (uint8_t *)nm->file + offset, sizeof(hdr));
		nm->sh_hdr = (t_shdr)
		{
			.nameoff = rd32(nm, hdr.sh_name),
			.type = rd32(nm, hdr.sh_type),
			.flags = rd64(nm, hdr.sh_flags),
			.offset = rd64(nm, hdr.sh_offset),
			.size = rd64(nm, hdr.sh_size),
			.link = rd32(nm, hdr.sh_link),
			.info = rd32(nm, hdr.sh_info),
			.entsize = rd64(nm, hdr.sh_entsize),
		};
	}
	else
	{
		if (!is_inbound(nm, offset, sizeof(Elf32_Shdr)))	goto error;
		Elf32_Shdr	hdr;
		memcpy(&hdr, (uint8_t *)nm->file + offset, sizeof(hdr));
		nm->sh_hdr = (t_shdr)
		{
			.nameoff = rd32(nm, hdr.sh_name),
			.type = rd32(nm, hdr.sh_type),
			.flags = rd32(nm, hdr.sh_flags),
			.offset = rd32(nm, hdr.sh_offset),
			.size = rd32(nm, hdr.sh_size),
			.link = rd32(nm, hdr.sh_link),
			.info = rd32(nm, hdr.sh_info),
			.entsize = rd32(nm, hdr.sh_entsize),
		};
	}

	// LOG(PASS);
	return false;
	error:
		// LOG(FAIL);
		return true;
}

const char	*get_sym_name(t_nm *nm, uint32_t nameoff)
{
	AUTO_LOG;

    const char *name;

    if (nameoff >= nm->str_tab.size)	return (NULL);
    name = (const char *)nm->file + nm->str_tab.offset + nameoff;
    if (!memchr(name, '\0', nm->str_tab.size - nameoff))	return (NULL);

    LOG(MAGENTA "Read symbol name: %s" RESET, name);
    return (name);
}

bool	get_sym_entry_at(t_nm *nm, uint64_t id)
{
	AUTO_LOG;

	if (id >= nm->sym_tab.size / nm->sym_tab.entsize)	goto error;
	uint64_t	offset = id * nm->sym_tab.entsize + nm->sym_tab.offset;
	//                   id * start of .symtab    + size of one ElfXX_Sym

	if (nm->is_64bits)
	{
		if (!is_inbound(nm, offset, sizeof(Elf64_Sym)))	goto error;
		Elf64_Sym	sym;
		memcpy(&sym, (uint8_t *)nm->file + offset, sizeof(sym));
		nm->symbol = (t_sym)
		{
			.namestr = get_sym_name(nm, sym.st_name),
			.value = rd64(nm, sym.st_value),
			.nameoff = rd32(nm, sym.st_name),
			.size = rd64(nm, sym.st_size),
			.shndx = rd16(nm, sym.st_shndx),
			.info = sym.st_info,
			.other = sym.st_other,
			.letter = '\0',
		};
	}
	else
	{
		if (!is_inbound(nm, offset, sizeof(Elf32_Sym)))	goto error;
		Elf32_Sym	sym;
		memcpy(&sym, (uint8_t *)nm->file + offset, sizeof(sym));
		nm->symbol = (t_sym)
		{
			.namestr = get_sym_name(nm, sym.st_name),
			.value = rd32(nm, sym.st_value),
			.nameoff = rd32(nm, sym.st_name),
			.size = rd32(nm, sym.st_size),
			.shndx = rd16(nm, sym.st_shndx),
			.info = sym.st_info,
			.other = sym.st_other,
			.letter = '\0',
		};
	}

	LOG(PASS);
	return false;
	error:
		LOG(FAIL);
		return true;
}

uint16_t	rd16(t_nm *nm, uint16_t bytes) { return nm->is_big_endian ? be16toh(bytes) : le16toh(bytes); }
uint32_t	rd32(t_nm *nm, uint32_t bytes) { return nm->is_big_endian ? be32toh(bytes) : le32toh(bytes); }
uint64_t	rd64(t_nm *nm, uint64_t bytes) { return nm->is_big_endian ? be64toh(bytes) : le64toh(bytes); }
