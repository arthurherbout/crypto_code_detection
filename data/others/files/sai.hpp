/*
libsai - Library for interfacing with SystemMax PaintTool Sai files

LICENSE
	MIT License
	Copyright (c) 2017-2019 Wunkolo
	Permission is hereby granted, free of charge, to any person obtaining a copy
	of this software and associated documentation files (the "Software"), to deal
	in the Software without restriction, including without limitation the rights
	to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
	copies of the Software, and to permit persons to whom the Software is
	furnished to do so, subject to the following conditions:
	The above copyright notice and this permission notice shall be included in all
	copies or substantial portions of the Software.
	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
	IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
	FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
	AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
	LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
	OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
	SOFTWARE.
*/

#pragma once
#include <cstdint>
#include <cstddef>
#include <ctime>
#include <fstream>
#include <array>
#include <memory>
#include <tuple>
#include <functional>

namespace sai
{
template<std::size_t N>
inline constexpr std::uint32_t Tag(const char (&TagString)[N])
{
	static_assert(N == 5, "Tag must be 4 characters");
	return
		  (TagString[3] <<  0)
		| (TagString[2] <<  8)
		| (TagString[1] << 16)
		| (TagString[0] << 24);
}

enum class LayerType
{
	RootLayer = 0x00, // Canvas pseudo-layer
	Layer     = 0x03,
	Unknown4  = 0x04,
	Linework  = 0x05,
	Mask      = 0x06,
	Unknown7  = 0x07,
	Set       = 0x08
};

enum class BlendingModes : std::uint32_t
{
	PassThrough = Tag("pass"),
	Normal      = Tag("norm"),
	Multiply    = Tag("mul "),
	Screen      = Tag("scrn"),
	Overlay     = Tag("over"),
	Luminosity  = Tag("add "),
	Shade       = Tag("sub "),
	LumiShade   = Tag("adsb"),
	Binary      = Tag("cbin")
};

#pragma pack(push, 1)

struct FATEntry
{
	enum class EntryType : std::uint8_t
	{
		Folder = 0x10,
		File = 0x80
	};

	std::uint32_t Flags;
	char Name[32];
	std::uint8_t Pad1;
	std::uint8_t Pad2;
	EntryType Type;
	std::uint8_t Pad4;
	std::uint32_t PageIndex;
	std::uint32_t Size;
	std::uint64_t TimeStamp; // Windows FILETIME
	std::uint64_t UnknownB;
};

union VirtualPage
{
	static constexpr std::size_t PageSize = 0x1000;
	static constexpr std::size_t TableSpan = PageSize / 8;

	static constexpr std::size_t NearestTableIndex(std::size_t PageIndex)
	{
		return (PageIndex / TableSpan) * TableSpan;
	}
	static constexpr bool IsTableIndex(std::size_t PageIndex)
	{
		return (PageIndex % TableSpan) ? false : true;
	}
	static constexpr bool IsDataIndex(std::size_t PageIndex)
	{
		return (PageIndex % TableSpan) ? true : false;
	}

	// Data
	std::uint8_t u8[PageSize];
	std::int8_t i8[PageSize];
	std::uint32_t u32[PageSize / sizeof(std::uint32_t)];
	std::int32_t i32[PageSize / sizeof(std::int32_t)];

	// Page Table entries
	struct PageEntry
	{
		std::uint32_t Checksum;
		std::uint32_t NextPageIndex;
	} PageEntries[PageSize / sizeof(PageEntry)];

	void DecryptTable(std::uint32_t PageIndex);
	void DecryptData(std::uint32_t PageChecksum);

	// FAT Table Entries
	FATEntry FATEntries[64];

	/*
	To checksum a table be sure to do "u32[0] = 0" first
	*/
	std::uint32_t Checksum();
};

static_assert(sizeof(VirtualPage) == VirtualPage::PageSize);

struct ThumbnailHeader
{
	std::uint32_t Width;
	std::uint32_t Height;
	std::uint32_t Magic; // BM32
};

using LayerID = std::uint32_t;

struct LayerReference
{
	std::uint32_t Identifier;
	std::uint16_t LayerType;
	// These all get added and sent as a windows message 0x80CA for some reason
	std::uint16_t Unknown;
};

struct LayerBounds
{
	std::int32_t X; // (X / 32) * 32
	std::int32_t Y; // (Y / 32) * 32
	std::uint32_t Width; // Width - 31
	std::uint32_t Height; // Height - 31
};

struct LayerHeader
{
	std::uint32_t Type; // LayerType enum
	LayerID Identifier;
	LayerBounds Bounds;
	std::uint32_t Unknown;
	std::uint8_t Opacity;
	std::uint8_t Visible;
	std::uint8_t PreserveOpacity;
	std::uint8_t Clipping;
	std::uint8_t Unknown4;
	std::uint32_t Blending;
};

struct LayerTableEntry
{
	LayerID Identifier;
	std::uint16_t Type;     // LayerType enum
	std::uint16_t Unknown6; // Gets sent as windows message 0x80CA for some reason
};

#pragma pack(pop)

/*
Symmetric keys for decrupting and encrypting the virtual file system
*/
namespace Keys
{
extern const std::uint32_t User[256];
extern const std::uint32_t NotRemoveMe[256];
extern const std::uint32_t LocalState[256];
extern const std::uint32_t System[256];
}

// Streambuf to read from an encrypted file
class ifstreambuf : public std::streambuf
{
public:
	explicit ifstreambuf(
		const std::uint32_t* Key = Keys::User
	);

	// No copy
	ifstreambuf(const ifstreambuf&) = delete;
	ifstreambuf& operator=(const ifstreambuf&) = delete;

	// Adhere similarly to std::basic_filebuf
	ifstreambuf* open(
		const char* Name
	);
	ifstreambuf* open(
		const wchar_t* Name
	);
	ifstreambuf* close();
	bool is_open() const;

	// std::streambuf overrides
	virtual std::streambuf::int_type underflow() override;
	virtual std::streambuf::pos_type seekoff(
		std::streambuf::off_type Offset,
		std::ios_base::seekdir Direction,
		std::ios_base::openmode Mode = std::ios_base::in
	) override;
	virtual std::streambuf::pos_type seekpos(
		std::streambuf::pos_type Position,
		std::ios_base::openmode Mode = std::ios_base::in
	) override;
private:
	std::ifstream FileIn;

	VirtualPage Buffer;

	// Decryption Key
	const std::uint32_t* Key;

	std::uint32_t CurrentPage;

	// Caching

	bool FetchPage(std::uint32_t PageIndex, VirtualPage* Dest);

	std::unique_ptr<VirtualPage> PageCache;
	std::uint32_t PageCacheIndex;

	std::unique_ptr<VirtualPage> TableCache;
	std::uint32_t TableCacheIndex;

	std::uint32_t PageCount;
};

class ifstream : public std::istream
{
public:
	explicit ifstream(
		const std::string& Path
	);

	explicit ifstream(
		const char* Path
	);
	explicit ifstream(
		const std::wstring& Path
	);

	explicit ifstream(
		const wchar_t* Path
	);

	// Similar to ifstream member functions
	void open(const char* FilePath) const;
	void open(const std::string& FilePath) const;
	void open(const wchar_t* FilePath) const;
	void open(const std::wstring& FilePath) const;
	bool is_open() const;

	virtual ~ifstream();

private:
};

// Forward declarations

class VirtualFileEntry;
class VirtualFileSystem;

// Visitors

class VirtualFileVisitor
{
public:
	virtual ~VirtualFileVisitor();

	// Return false to stop iteration

    virtual bool VisitFolderBegin(VirtualFileEntry&);

    virtual bool VisitFolderEnd(VirtualFileEntry&);

    virtual bool VisitFile(VirtualFileEntry&);
};

class VirtualFileSystem
{
public:
	explicit VirtualFileSystem(const char* FileName);
	explicit VirtualFileSystem(const wchar_t* FileName);
	~VirtualFileSystem();

	// No Copy
	VirtualFileSystem(const VirtualFileSystem&) = delete;
	VirtualFileSystem& operator=(const VirtualFileSystem&) = delete;

	bool IsOpen() const;

	bool Exists(const char* Path);

	std::unique_ptr<VirtualFileEntry> GetEntry(const char* Path);

	std::size_t Read(
		std::size_t Offset,
		void* Destination,
		std::size_t Size) const;

	template< typename T >
	inline std::size_t Read(std::size_t Offset, T& Destination)
	{
		return Read(
			Offset,
			&Destination,
			sizeof(T)
		);
	}

	void IterateFileSystem(VirtualFileVisitor& Visitor);

private:

	void IterateFATBlock(
		std::size_t Index,
		VirtualFileVisitor& Visitor
	);

	std::shared_ptr<ifstream> SaiStream;
};

class VirtualFileEntry
{
public:
	~VirtualFileEntry();

	// No Copy
	VirtualFileEntry(const VirtualFileEntry&) = delete;
	VirtualFileEntry& operator=(const VirtualFileEntry&) = delete;

	const char* GetName() const;

	FATEntry::EntryType GetType() const;
	std::time_t GetTimeStamp() const;
	std::size_t GetSize() const;
	std::size_t GetPageIndex() const;

	std::size_t Tell() const;
	void Seek(std::size_t NewOffset);

	std::size_t Read(void* Destination, std::size_t Size);

	template< typename T >
	inline std::size_t Read(T& Destination)
	{
		return Read(&Destination, sizeof(T));
	}

	template< typename T >
	inline T Read()
	{
		T temp;
		Read(&temp, sizeof(T));
		return temp;
	}

	FATEntry FATData;
private:
	friend VirtualFileSystem;

	VirtualFileEntry(
		std::weak_ptr<ifstream> FileSystem,
		const FATEntry& EntryData
	);

	VirtualPage GetTablePage(std::size_t Offset) const;

	std::weak_ptr<ifstream> FileSystem;

	// "Flat" offset within file
	std::size_t Offset;
	// Index of the page we are currently in
	std::size_t PageIndex;
	// Offset within the page
	std::size_t PageOffset;
};

class Document : public VirtualFileSystem
{
public:
	explicit Document(const char* FileName);
	explicit Document(const wchar_t* FileName);
	~Document();

	// No Copy
	Document(const Document&) = delete;
	Document& operator=(const Document&) = delete;

	// Returns (Width, Height)
	// Returns (0,0) if an error has occured
	std::tuple<
		std::uint32_t,
		std::uint32_t
	> GetCanvasSize();

	// Returns (RGBA Pixel Data, Width, Height).
	// Returns (null,0,0) if an error has occured.
	std::tuple<
		std::unique_ptr<std::uint8_t[]>,
		std::uint32_t,
		std::uint32_t
	> GetThumbnail();

	void IterateLayerFiles(
		const std::function<bool(VirtualFileEntry&)>& LayerProc
	);
	void IterateSubLayerFiles(
		const std::function<bool(VirtualFileEntry&)>& SubLayerProc
	);

private:
};
}
