#include "pe_hdrs_helper.h"

BYTE* get_nt_hrds(const BYTE *pe_buffer)
{
	if (pe_buffer == NULL) return NULL;

	IMAGE_DOS_HEADER *idh = (IMAGE_DOS_HEADER*)pe_buffer;
	if (idh->e_magic != IMAGE_DOS_SIGNATURE) {
		return NULL;
	}
	const LONG kMaxOffset = 1024;
	LONG pe_offset = idh->e_lfanew;
	if (pe_offset > kMaxOffset) return NULL;
	IMAGE_NT_HEADERS32 *inh = (IMAGE_NT_HEADERS32 *)((BYTE*)pe_buffer + pe_offset);
	if (inh->Signature != IMAGE_NT_SIGNATURE) return NULL;
	return (BYTE*)inh;
}

IMAGE_NT_HEADERS32* get_nt_hrds32(BYTE *pe_buffer)
{
	BYTE *ptr = get_nt_hrds(pe_buffer);
	if (ptr == NULL) return NULL;

	IMAGE_NT_HEADERS32 *inh = (IMAGE_NT_HEADERS32*)(ptr);
	if (inh->FileHeader.Machine == IMAGE_FILE_MACHINE_I386) {
		return inh;
	}
	return NULL;
}

IMAGE_NT_HEADERS64* get_nt_hrds64(const BYTE *pe_buffer)
{
	BYTE *ptr = get_nt_hrds(pe_buffer);
	if (ptr == NULL) return NULL;

	IMAGE_NT_HEADERS32 *inh = (IMAGE_NT_HEADERS32*)(ptr);
	if (inh->FileHeader.Machine == IMAGE_FILE_MACHINE_AMD64) {
		return (IMAGE_NT_HEADERS64*)(ptr);
	}
	return NULL;
}

bool is64bit(const BYTE *pe_buffer)
{
	BYTE *ptr = get_nt_hrds(pe_buffer);
	if (ptr == NULL) return false;

	IMAGE_NT_HEADERS32 *inh = (IMAGE_NT_HEADERS32*)(ptr);
	if (inh->FileHeader.Machine == IMAGE_FILE_MACHINE_AMD64) {
		return true;
	}
	return false;
}

IMAGE_DATA_DIRECTORY* get_pe_directory(const BYTE *pe_buffer, DWORD dir_id)
{
	if (dir_id >= IMAGE_NUMBEROF_DIRECTORY_ENTRIES) return NULL;

	BYTE* nt_headers = get_nt_hrds((BYTE*)pe_buffer);
	if (nt_headers == NULL) return NULL;

	IMAGE_DATA_DIRECTORY* peDir = NULL;
	if (is64bit((BYTE*)pe_buffer)) {
		IMAGE_NT_HEADERS64* nt_headers64 = (IMAGE_NT_HEADERS64*)nt_headers;
		peDir = &(nt_headers64->OptionalHeader.DataDirectory[dir_id]);
	}
	else {
		IMAGE_NT_HEADERS32* nt_headers64 = (IMAGE_NT_HEADERS32*)nt_headers;
		peDir = &(nt_headers64->OptionalHeader.DataDirectory[dir_id]);
	}
	if (peDir->VirtualAddress == NULL) {
		return NULL;
	}
	return peDir;
}
