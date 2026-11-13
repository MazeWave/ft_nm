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
			.version = rd16(nm, hdr.e_version),
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
			.shoff = rd64(nm, hdr.e_shoff),
			.shentsize = rd16(nm, hdr.e_shentsize),
			.shnum = rd16(nm, hdr.e_shnum),
			.shstrndx = rd16(nm, hdr.e_shstrndx),
			.machine = rd16(nm, hdr.e_machine),
			.version = rd16(nm, hdr.e_version),
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
	AUTO_LOG;

	if (id >= nm->elf_hdr.shnum)	goto error;
	uint64_t	offset = id * nm->elf_hdr.shentsize + nm->elf_hdr.shoff;

	if (nm->is_64bits)
	{
		if (!is_inbound(nm, offset, sizeof(Elf64_Shdr)))	goto error;
		Elf64_Shdr	hdr;
		memcpy(&hdr, (uint8_t *)nm->file + offset, sizeof(hdr));
		nm->sh_hdr = (t_shdr)
		{
			.name = rd64(nm, hdr.sh_name),
			.type = rd64(nm, hdr.sh_type),
			.flags = rd64(nm, hdr.sh_flags),
			.offset = rd64(nm, hdr.sh_offset),
			.size = rd64(nm, hdr.sh_size),
			.link = rd64(nm, hdr.sh_link),
			.info = rd64(nm, hdr.sh_info),
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
			.name = rd64(nm, hdr.sh_name),
			.type = rd64(nm, hdr.sh_type),
			.flags = rd64(nm, hdr.sh_flags),
			.offset = rd64(nm, hdr.sh_offset),
			.size = rd64(nm, hdr.sh_size),
			.link = rd64(nm, hdr.sh_link),
			.info = rd64(nm, hdr.sh_info),
			.entsize = rd64(nm, hdr.sh_entsize),
		};
	}

	LOG(PASS);
	return false;
	error:
		LOG(FAIL);
		return true;
}

const char *get_sh_name(t_nm *nm)
{
	AUTO_LOG;

	if (!is_inbound(nm, nm->sh_hdr.offset + nm->sh_hdr.name, sizeof(nm->sh_hdr)))	return NULL;

	return ((const char *)nm->sh_hdr.name + nm->sh_hdr.offset);
}

uint16_t	rd16(t_nm *nm, uint16_t bytes) { return nm->is_big_endian ? be16toh(bytes) : le16toh(bytes); }
uint32_t	rd32(t_nm *nm, uint32_t bytes) { return nm->is_big_endian ? be32toh(bytes) : le32toh(bytes); }
uint64_t	rd64(t_nm *nm, uint64_t bytes) { return nm->is_big_endian ? be64toh(bytes) : le64toh(bytes); }
