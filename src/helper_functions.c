#include "../includes/ft_nm.h"

bool	is_inbound(t_nm *nm, uint64_t offset, uint64_t size)
{
	if (offset > (uint64_t)(nm->st.st_size))			return (false);
	if (size > ((uint64_t)(nm->st.st_size) - offset))	return (false);
	return (true);
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
			.name = hdr.sh_name,
			.type = hdr.sh_type,
			.flags = hdr.sh_flags,
			.offset = hdr.sh_offset,
			.size = hdr.sh_size,
			.link = hdr.sh_link,
			.info = hdr.sh_info,
			.entsize = hdr.sh_entsize,
		};
	}
	else
	{
		if (!is_inbound(nm, offset, sizeof(Elf32_Shdr)))	goto error;
		Elf32_Shdr	hdr;
		memcpy(&hdr, (uint8_t *)nm->file + offset, sizeof(hdr));
		nm->sh_hdr = (t_shdr)
		{
			.name = hdr.sh_name,
			.type = hdr.sh_type,
			.flags = hdr.sh_flags,
			.offset = hdr.sh_offset,
			.size = hdr.sh_size,
			.link = hdr.sh_link,
			.info = hdr.sh_info,
			.entsize = hdr.sh_entsize,
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