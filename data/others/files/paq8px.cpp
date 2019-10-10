/*
  PAQ8PX file compressor/archiver
  see README for information
  see DOC for technical details
  see CHANGELOG for version history
*/

//////////////////////// Versioning ////////////////////////////////////////

#define PROGNAME     "paq8px"
#define PROGVERSION  "182fix1"  //update version here before publishing your changes
#define PROGYEAR     "2019"

//////////////////////// Build options /////////////////////////////////////

// Uncomment one or more of the following #define-s to disable compilation of certain models/transformations
// This is useful when
// - you would like to slim the executable by eliminating unnecessary code for benchmarks where exe size matters or
// - you would like to experiment with the model-mixture
// TODO: make more models "optional"
#define USE_ZLIB
#define USE_AUDIOMODEL
#define USE_TEXTMODEL

//////////////////////// Debug options /////////////////////////////////////

#define NDEBUG    // Remove (comment out) this line for debugging (turns on Array bound checks and asserts)
#include <assert.h>

#define NVERBOSE  // Remove (comment out) this line for more on-screen progress information

//
//  User specified options are above
//  Automatic definitions are below = for compiling no need to change anything below this line
//

//////////////////////// Target OS/Compiler ////////////////////////////////

#if defined(_WIN32) || defined(_MSC_VER)
#ifndef WINDOWS
#define WINDOWS  //to compile for Windows
#endif
#endif

#if defined(unix) || defined(__unix__) || defined(__unix) || defined(__APPLE__)
#ifndef UNIX
#define UNIX //to compile for Unix, Linux, Solaris, MacOS / Darwin, etc)
#endif
#endif

#if !defined(WINDOWS ) && !defined(UNIX)
#error Unknown target system
#endif

// Floating point operations need IEEE compliance
// Do not use compiler optimization options such as the following:
// gcc : -ffast-math (and -Ofast, -funsafe-math-optimizations, -fno-rounding-math)
// vc++: /fp:fast
#if defined(__FAST_MATH__) || defined(_M_FP_FAST) // gcc vc++
#error Avoid using aggressive floating-point compiler optimization flags
#endif

#if defined(_MSC_VER)
#define ALWAYS_INLINE  __forceinline
#elif defined(__GNUC__)
#define ALWAYS_INLINE inline __attribute__((always_inline))
#else 
#define ALWAYS_INLINE inline
#endif

//////////////////////// Includes /////////////////////////////////////////

// Determining the proper printf() format specifier for 64 bit unsigned integers:
// - on Windows MSVC and MinGW-w64 use the MSVCRT runtime where it is "%I64u"
// - on Linux it is "%llu"
// The correct value is provided by the PRIu64 macro which is defined here:
#include <cinttypes> //PRIu64

// Platform-specific includes
#ifdef UNIX
  #include <string.h> //strlen(), strcpy(), strcat(), strerror(), memset(), memcpy(), memmove()
  #include <limits.h> //PATH_MAX (for OSX)
  #include <unistd.h> //isatty()
  #include <errno.h>  //errno
#else
  #ifndef NOMINMAX
  #define NOMINMAX
  #endif
  #include <windows.h>   //CreateDirectoryW, CommandLineToArgvW, GetConsoleOutputCP, SetConsoleOutputCP
                         //GetCommandLineW, GetModuleFileNameW, GetStdHandle, GetTempFileName
                         //MultiByteToWideChar, WideCharToMultiByte,
                         //FileType, FILE_TYPE_PIPE, FILE_TYPE_DISK, 
                         //uRetVal, DWORD, UINT, TRUE, MAX_PATH, CP_UTF8, etc.
#endif

// Platform-independent includes
#include <sys/stat.h> //stat(), mkdir()
#include <math.h>     //floor(), sqrt()
#include <stdexcept>  //std::exception
#include <algorithm>  //std::min, std::max
#include <chrono>     //std::chrono::high_resolution_clock

// zlib
#ifdef USE_ZLIB
#include <zlib.h>
#endif

//////////////////// Cross-platform definitions /////////////////////////////////////

#ifdef _MSC_VER
#define fseeko(a,b,c) _fseeki64(a,b,c)
#define ftello(a) _ftelli64(a)
#else
#ifndef UNIX
#ifndef fseeko
#define fseeko(a,b,c) fseeko64(a,b,c)
#endif
#ifndef ftello
#define ftello(a) ftello64(a)
#endif
#endif
#endif

#ifdef WINDOWS
#define strcasecmp _stricmp
#endif

#if defined(__GNUC__) || defined(__clang__)
#define bswap(x)   __builtin_bswap32(x)
#define bswap64(x) __builtin_bswap64(x)
#elif defined(_MSC_VER)
#define bswap(x)   _byteswap_ulong(x)
#define bswap64(x) _byteswap_uint64(x)
#else
#define bswap(x) \
+   ((((x) & 0xff000000) >> 24) | \
+    (((x) & 0x00ff0000) >>  8) | \
+    (((x) & 0x0000ff00) <<  8) | \
+    (((x) & 0x000000ff) << 24))
#define bswap64(x) \
+    ((x)>>56) |
+   (((x)<<40) & 0x00FF000000000000) | \
+   (((x)<<24) & 0x0000FF0000000000) | \
+   (((x)<<8 ) & 0x000000FF00000000) | \
+   (((x)>>8 ) & 0x00000000FF000000) | \
+   (((x)>>24) & 0x0000000000FF0000) | \
+   (((x)>>40) & 0x000000000000FF00) | \
+   ((x) << 56))
#endif

///////////////////////// SIMD Vectorization detection //////////////////////////////////

// Uncomment one or more of the following includes if you plan adding more SIMD dispatching
//#include <mmintrin.h>  //MMX
//#include <xmmintrin.h> //SSE
#include <emmintrin.h>   //SSE2
//#include <pmmintrin.h> //SSE3
//#include <tmmintrin.h> //SSSE3
//#include <smmintrin.h> //SSE4.1
//#include <nmmintrin.h> //SSE4.2
//#include <ammintrin.h> //SSE4A
#include <immintrin.h>   //AVX, AVX2
//#include <zmmintrin.h> //AVX512

//define CPUID
#ifdef _MSC_VER
#include <intrin.h>
#define cpuid(info, x) __cpuidex(info, x, 0)
#elif defined(__GNUC__)
#include <cpuid.h>
#define cpuid(info, x) __cpuid_count(x, 0, info[0], info[1], info[2], info[3])
#else
#error Unknown compiler
#endif

// Define interface to xgetbv instruction
static inline unsigned long long xgetbv (unsigned long ctr) {
#if (defined (_MSC_FULL_VER) && _MSC_FULL_VER >= 160040000) || (defined (__INTEL_COMPILER) && __INTEL_COMPILER >= 1200)
  return _xgetbv(ctr);
#elif defined(__GNUC__)
  uint32_t a, d;
  __asm("xgetbv" : "=a"(a),"=d"(d) : "c"(ctr) : );
  return a | (((uint64_t) d) << 32);
#else
#error Unknown compiler
#endif
}

/* Returns system's highest supported SIMD instruction set as
0: None
1: MMX
2: SSE
3: SSE2
4: SSE3
5: SSSE3
6: SSE4.1
7: SSE4.2
 : SSE4A //SSE4A is not supported on Intel, so we will exclude it
8: AVX
9: AVX2
 : AVX512 //TODO
*/
int simd_detect() {
  int cpuid_result[4] = {0,0,0,0};
  cpuid(cpuid_result, 0); // call cpuid function 0 ("Get vendor ID and highest basic calling parameter")
  if (cpuid_result[0] == 0) return 0; //cpuid is not supported
  cpuid(cpuid_result, 1);// call cpuid function 1 ("Processor Info and Feature Bits")
  if ((cpuid_result[3] & (1<<23)) == 0) return 0; //no MMX
  if ((cpuid_result[3] & (1<<25)) == 0) return 1; //no SSE
  //SSE: OK
  if ((cpuid_result[3] & (1<<26)) == 0) return 2; //no SSE2
  //SSE2: OK
  if ((cpuid_result[2] & (1<< 0)) == 0) return 3; //no SSE3
  //SSE3: OK
  if ((cpuid_result[2] & (1<< 9)) == 0) return 4; //no SSSE3
  //SSSE3: OK
  if ((cpuid_result[2] & (1<<19)) == 0) return 5; //no SSE4.1
  //SSE4.1: OK
  if ((cpuid_result[2] & (1<<20)) == 0) return 6; //no SSE4.2
  //SSE4.2: OK
  if ((cpuid_result[2] & (1<<27)) == 0) return 7; //no OSXSAVE (no XGETBV)
  if ((xgetbv(0) & 6) != 6)             return 7; //AVX is not enabled in OS
  if ((cpuid_result[2] & (1<<28)) == 0) return 7; //no AVX
  //AVX: OK
  cpuid(cpuid_result, 7); // call cpuid function 7 ("Extended Feature Bits")
  if ((cpuid_result[1] & (1<< 5)) == 0) return 8;  //no AVX2
  //AVX2: OK
  return 9;
}

//////////////////////// Type definitions /////////////////////////////////////////

// shortcuts for unsigned 8, 16, 32 bit integer types
typedef uint8_t  U8;
typedef uint16_t U16;
typedef uint32_t U32;
typedef uint64_t U64;

static_assert(sizeof(U8)==1,"sizeof(U8)");
static_assert(sizeof(U16)==2,"sizeof(U16)");
static_assert(sizeof(U32)==4,"sizeof(U32)");
static_assert(sizeof(U64)==8,"sizeof(U64)");
static_assert(sizeof(short)==2,"sizeof(short)");
static_assert(sizeof(int)==4,"sizeof(int)");

//////////////////////// Helper functions /////////////////////////////////////////

// min, max functions
static inline int min(int a, int b) {return a<b?a:b;}
static inline U64 min(U64 a, U64 b) {return a<b?a:b;}
static inline int max(int a, int b) {return a<b?b:a;}

#define ispowerof2(x) ((x&(x-1))==0)

// A basic exception class to let catch() in main() know
// that the exception was thrown intentionally.
class IntentionalException : public std::exception {};

// Error handler: print message if any, and exit
void quit(const char* const message=nullptr) {
  if(message)
    printf("\n%s",message);
  printf("\n");
  throw IntentionalException();
}

#ifndef NDEBUG
  #if defined(UNIX)
    #include <execinfo.h>
    #define BACKTRACE() {\
      void* callstack[128]; \
      int frames = backtrace(callstack, 128); \
      char** strs = backtrace_symbols(callstack, frames); \
      for(int i = 0; i < frames; ++i) { \
        printf("%s\n", strs[i]); \
      } \
      free(strs); \
    }
  #else
    // TODO: How to implement this on Windows?
    #define BACKTRACE() {}
  #endif
#endif

//////////////////////// Program Checker /////////////////////

// Track time and memory used
// Remark: only Array<T> reports its memory usage, we don't know about other types
class ProgramChecker {
private:
  U64 memused;  // Bytes currently in use (all allocated minus all freed)
  U64 maxmem;   // Most bytes allocated ever
  std::chrono::time_point<std::chrono::high_resolution_clock> start_time;
public:
  void alloc(U64 n) {
    memused+=n;
    if (memused>maxmem) maxmem=memused;
  }
  void free(U64 n) {
    assert(memused>=n);
    memused-=n;
  }
  ProgramChecker(): memused(0), maxmem(0) {
    start_time=std::chrono::high_resolution_clock::now();
  }
  double get_runtime() const {
    const std::chrono::time_point<std::chrono::high_resolution_clock> finish_time=std::chrono::high_resolution_clock::now();
    return std::chrono::duration<double>(finish_time - start_time).count();
  }
  void print() const {  // Print elapsed time and used memory
      const double runtime=get_runtime();
      printf("Time %1.2f sec, used %" PRIu64 " MB (%" PRIu64 " bytes) of memory\n",runtime,maxmem>>20,maxmem);
  }
  ~ProgramChecker() {
    assert(memused==0); // We expect that all reserved memory is already properly freed
  }

} programChecker;

//////////////////////////// Array ////////////////////////////

// Array<T,Align> a(n); allocates memory for n elements of T.
// The base address is aligned if the "alignment" parameter is given.
// Constructors for T are not called, the allocated memory is initialized to 0s.
// It's the caller's responsibility to populate the array with elements.
// Parameters are checked and indexing is bounds checked if assertions are on.
// Use of copy and assignment constructors are not supported.
//
// a.size(): returns the number of T elements currently in the array.
// a.resize(newsize): grows or shrinks the array.
// a.append(x): appends x to the end of the array and reserving space for more elements if needed.
// a.pop_back(): removes the last element by reducing the size by one (but does not free memory).

#ifdef NDEBUG
  #define chkindex(index, upper_bound)((void) 0)
#else
  static void chkindex(U64 index, U64 upper_bound)
  {
    if (index>=upper_bound) {
      fprintf(stderr, "%" PRIu64 " out of upper bound %" PRIu64 "\n", index, upper_bound);
      BACKTRACE();
      quit();
    }
  }
#endif

template <class T, const int Align=16> class Array {
private:
  U64 used_size;
  U64 reserved_size;
  char *ptr; // Address of allocated memory (may not be aligned)
  T* data;   // Aligned base address of the elements, (ptr <= T)
  void create(U64 requested_size);
  inline U64 padding() const {return Align-1;}
  inline U64 allocated_bytes() const {return (reserved_size==0)?0:reserved_size*sizeof(T)+padding();}
public:
  explicit Array(U64 requested_size) {create(requested_size);}
  ~Array();
  T& operator[](U64 i) {
    chkindex(i,used_size);
    return data[i];
  }
  const T& operator[](U64 i) const {
    chkindex(i,used_size);
    return data[i];
  }
  U64 size() const {return used_size;}
  void resize(U64 new_size);
  void pop_back() {assert(used_size>0); --used_size; }  // decrement size
  void push_back(const T& x);  // increment size, append x
  Array(const Array&); //prevent copying
  Array& operator=(const Array&); //prevent assignment
};

template<class T, const int Align> void Array<T,Align>::create(U64 requested_size) {
  assert(ispowerof2(Align));
  used_size=reserved_size=requested_size;
  if (requested_size==0) {
    data=nullptr;ptr=nullptr;
    return;
  }
  const U64 bytes_to_allocate=allocated_bytes();
  ptr=(char*)calloc(bytes_to_allocate,1);
  if(ptr==nullptr)quit("Out of memory.");
  U64 pad=padding();
  data=(T*)(((uintptr_t)ptr+pad) & ~(uintptr_t)pad);
  assert(ptr<=(char*)data && (char*)data<=ptr+Align);
  assert(((uintptr_t)data & (Align-1))==0); //aligned as expected?
  programChecker.alloc(bytes_to_allocate);
}

template<class T, const int Align> void Array<T,Align>::resize(U64 new_size) {
  if (new_size<=reserved_size) {
    used_size=new_size;
    return;
  }
  char *old_ptr=ptr;
  T *old_data=data;
  const U64 old_size=used_size;
  programChecker.free(allocated_bytes());
  create(new_size);
  if(old_size>0) {
    assert(old_ptr!=nullptr && old_data!=nullptr);
    memcpy(data, old_data, sizeof(T)*old_size);
  }
  if(old_ptr!=nullptr){free(old_ptr);old_ptr=nullptr;}
}

template<class T, const int Align> void Array<T,Align>::push_back(const T& x) {
  if(used_size==reserved_size) {
    const U64 old_size=used_size;
    const U64 new_size=used_size*2+16;
    resize(new_size);
    used_size=old_size;
  }
  data[used_size++]=x;
}

template<class T, const int Align> Array<T, Align>::~Array() {
  programChecker.free(allocated_bytes());
  free(ptr);
  used_size=reserved_size=0;
  data=nullptr;ptr=nullptr;
}


/////////////////// String and FileName /////////////////////

// A specialized string class
// size() includes NUL terminator.
// strsize() does not include NUL terminator.

class String: public Array<char> {
private:
  void append_int_recursive(U64 x) {
    if(x<=9)
      push_back('0'+char(x));
    else {
      const U64 rem= x % 10;
      x = x / 10;
      if(x!=0)append_int_recursive(x);
      push_back('0'+char(rem));
    }
  }
protected:
  #ifdef NDEBUG
  #define chk_consistency() ((void) 0)
  #else
  void chk_consistency() const {
    for(U32 i=0;i<size()-1;i++)
      if((*this)[i]==0)quit("Internal error - string consistency check failed (1).");
    if(((*this)[size()-1])!=0)quit("Internal error - string consistency check failed (2).");
  }
  #endif
public:
  const char* c_str() const {return &(*this)[0];}
  U64 strsize() const {chk_consistency();assert(size()>0);return size()-1;}
  void operator=(const char* s) {
    resize(strlen(s)+1);
    memcpy(&(*this)[0], s, size());
    chk_consistency();
  }
  void operator+=(const char* s) {
    const U64 pos=size();
    const U64 len=strlen(s);
    resize(pos+len);
    memcpy(&(*this)[pos-1], s, len+1);
    chk_consistency();
  }
  void operator+=(char c) {
    pop_back(); //Remove NUL
    push_back(c);
    push_back(0); //Append NUL
    chk_consistency();
  }
  void operator+=(U64 x) {
    pop_back(); //Remove NUL
    if(x==0)
      push_back('0');
    else
      append_int_recursive(x);
    push_back(0); //Append NUL
    chk_consistency();
  }
  bool endswith(const char * ending) const {
    const U64 endingsize=strlen(ending);
    if(endingsize>strsize())return false;
    const int cmp=memcmp(ending,&(*this)[strsize()-endingsize],endingsize);
    return(cmp==0);
  }
  void stripend(U64 count) {
    assert(strsize()>=count);
    const U64 newsize=strsize()-count;
    resize(newsize);
    push_back(0); //Append NUL
    chk_consistency();
  }
  bool beginswith(const char * beginning) const {
    const U64 beginningsize=strlen(beginning);
    if(beginningsize>strsize())return false;
    const int cmp=memcmp(beginning,&(*this)[0],beginningsize);
    return(cmp==0);
  }
  void stripstart(U64 count) {
    assert(strsize()>=count);
    const U64 newsize=strsize()-count;
    memmove(&(*this)[0],&(*this)[count],newsize);
    resize(newsize);
    push_back(0); //Append NUL
    chk_consistency();
  }
  int findlast (char c) const {
    U64 i=strsize();
    while(i-->0)if((*this)[i]==c)return (int)i;
    return -1; //not found
  }
  String(const char* s=""): Array<char>(strlen(s)+1) {
    memcpy(&(*this)[0], s, size());
    chk_consistency();
  }
};

//The preferred slash for displaying
//We will change the BADSLASH to GOODSLASH before displaying a path string to the user
#ifdef WINDOWS
#define BADSLASH '/'
#define GOODSLASH '\\'
#else
#define BADSLASH '\\'
#define GOODSLASH '/'
#endif

// A class to represent a filename
class FileName: public String {
public:
  FileName(const char* s=""):String(s){};
  int lastslashpos() const {
    int pos=findlast('/');
    if(pos<0)pos=findlast('\\');
    return pos; //has no path when -1
  }
  void keepfilename() {
    const int pos=lastslashpos();
    stripstart(pos+1); //don't keep last slash
  }
  void keeppath() {
    const int pos=lastslashpos();
    stripend(strsize()-pos-1); //keep last slash
  }
  void replaceslashes() { //prepare path string for screen output
    const U64 ssize=strsize();
    for(U64 i=0;i<ssize;i++)
      if((*this)[i]==BADSLASH)
        (*this)[i]=GOODSLASH;
    chk_consistency();
  }
};


//////////////////// IO functions and classes ///////////////////
// These functions/classes are responsible for all the file
// and directory operations.


// Wrappers to utf8 vs. wchar functions
// Linux i/o works with utf8 char* but on windows it's wchar_t*.
// We'll be using utf8 char* in all our functions, so we need to convert to/from 
// wchar_t* when on windows.

//only for Windows: a class encapsulating a wchar string converted from a utf8 string
//purpose: to properly free the allocated string buffer on destruction
#ifdef WINDOWS
class WcharStr {
private:
  //convert a utf8 string to a wchar string
wchar_t* utf8_to_wchar_str(const char *utf8_str) {
  int buffersize = MultiByteToWideChar(CP_UTF8,0,utf8_str,-1,NULL,0);
  wchar_t* wchar_str = new wchar_t[buffersize];
  MultiByteToWideChar(CP_UTF8,0,utf8_str,-1,wchar_str,buffersize);
  return wchar_str;
}
public:
  wchar_t* wchar_str;
  WcharStr(const char *utf8_str) {
    wchar_str=utf8_to_wchar_str(utf8_str);
  }
  ~WcharStr() {
    delete[] wchar_str;
  }
};
#endif

//only for Windows: a class encapsulating a utf8 string converted from a wchar string
//purpose: to properly free the allocated string buffer on destruction
#ifdef WINDOWS
class Utf8Str {
private:
  //convert a wchar string to a utf8 string
  char* wchar_to_utf8_str(const wchar_t *wchar_str) {
    int buffersize = WideCharToMultiByte(CP_UTF8,0,wchar_str,-1,NULL,0,NULL,NULL);
    char* utf8_str = new char[buffersize];
    WideCharToMultiByte(CP_UTF8,0,wchar_str,-1,utf8_str,buffersize,NULL,NULL);
    return utf8_str;
  }
public:
  char* utf8_str;
  Utf8Str(const wchar_t *wchar_str) {
    utf8_str=wchar_to_utf8_str(wchar_str);
  }
  ~Utf8Str() {
    delete[] utf8_str;
  }
};
#endif

static constexpr int READ=0;
static constexpr int WRITE=1;
static constexpr int APPEND=2;

// Wrapper function (Linux vs Windows) to open a file
FILE* openfile(const char *filename, const int mode) {
  FILE* file;
#ifdef WINDOWS
  file = _wfopen(WcharStr(filename).wchar_str, mode==READ ? L"rb" : mode==WRITE ? L"wb+" : L"a");
#else
  file = fopen(filename, mode==READ ? "rb" : mode==WRITE ? "wb+" : "a");
#endif
  return file;
}

#ifdef WINDOWS
#define STAT _stat
#else
#define STAT stat
#endif

// Wrapper function (Linux vs Windows) to examine a path
bool statpath(const char* path, struct STAT &status) {
#ifdef WINDOWS
  return _wstat(WcharStr(path).wchar_str,&status);
#else
  return stat(path,&status);
#endif
}

//////////////////// Folder operations ///////////////////////////

//examines given "path" and returns:
//0: on error
//1: when exists and is a file
//2: when exists and is a directory
//3: when does not exist, but looks like a file       /abcd/efgh
//4: when does not exist, but looks like a directory  /abcd/efgh/
static int examinepath(const char* path) {
  struct STAT status;
  const bool success = statpath(path, status)==0;
  if(!success) {
    if(errno==ENOENT){ //no such file or directory
      const int len=(int)strlen(path);
      if(len==0)return 0; //error: path is an empty string
      const char lastchar=path[len-1];
      if(lastchar!='/' && lastchar!='\\')return 3; //looks like a file
      else return 4; //looks like a directory
    }
    return 0; //error
  }
  if((status.st_mode & S_IFREG)!=0) return 1; //existing file
  if((status.st_mode & S_IFDIR)!=0) return 2; //existing directory
  return 0; //error: "path" may be a socket, symlink, named pipe, etc.
};

//creates a directory if it does not exist
static int makedir(const char* dir) {
  if(examinepath(dir)==2)//existing directory
    return 2; //2: directory already exists, no need to create
  #ifdef WINDOWS
    const bool created = (CreateDirectoryW(WcharStr(dir).wchar_str, nullptr) == TRUE);
  #else
  #ifdef UNIX
    const bool created = (mkdir(dir, 0777) == 0);
  #else
    #error Unknown target system
  #endif
  #endif
  return created?1:0; //0: failed, 1: created successfully
};

//creates directories recusively if they don't exist
static void makedirectories(const char* filename) {
  String path(filename);
  U64 start = 0;
  if(path[1]==':')start=2; //skip drive letter (c:)
  if(path[start] == '/' || path[start] == '\\')start++; //skip leading slashes (root dir)
  for (U64 i = start; path[i]!=0; ++i) {
    if (path[i] == '/' || path[i] == '\\') {
      char savechar = path[i];
      path[i] = 0;
      const char* dirname = path.c_str();
      const int created = makedir(dirname);
      if (created==0) {
        printf("Unable to create directory %s", dirname);
        quit();
      }
      if(created==1)
        printf("Created directory %s\n", dirname);
      path[i] = savechar;
    }
  }
};


/////////////////////////// File /////////////////////////////
// The main purpose of these classes is to keep temporary files in
// RAM as mush as possible. The default behaviour is to simply pass
// function calls to the operating system - except in case of temporary
// files.

// Helper function: create a temporary file
//
// On Windows when using tmpfile() the temporary file may be created
// in the root directory causing access denied error when User Account Control (UAC) is on.
// To avoid this issue with tmpfile() we simply use fopen() instead.
// We create the temporary file in the directory where the executable is launched from.
// Luckily the MS C runtime library provides two (MS specific) fopen() flags: "T"emporary and "D"elete.

FILE* maketmpfile(void) {
#if defined(WINDOWS)
  char szTempFileName[MAX_PATH];
  const UINT uRetVal = GetTempFileName(TEXT("."), TEXT("tmp"), 0, szTempFileName);
  if(uRetVal==0)return nullptr;
  return fopen(szTempFileName, "w+bTD");
#else
  return tmpfile();
#endif
}

//This is the base class.
//This is an abstract class for all the required file operations.

class File {
public:
  virtual ~File() {};
  virtual bool open(const char* filename, bool must_succeed) = 0;
  virtual void create(const char* filename) = 0;
  virtual void close() = 0;
  virtual int getchar() = 0;
  virtual void putchar(U8 c) = 0;
  void append(const char* s) {for (int i = 0; s[i]; i++)putchar((U8)s[i]);}
  virtual U64 blockread(U8 *ptr, U64 count) = 0;
  virtual void blockwrite(U8 *ptr, U64 count) = 0;
  U32 get32(){return (getchar() << 24) | (getchar() << 16) | (getchar() << 8) | (getchar());}
  void put32(U32 x){putchar((x >> 24) & 255); putchar((x >> 16) & 255); putchar((x >> 8) & 255); putchar(x & 255);}
  U64 getVLI(){
    U64 i=0;
    int k=0;
    U8 B=0;
    do {
      B=getchar();
      i|=U64((B&0x7F)<<k);
      k+=7;
    } while ((B>>7)>0);
    return i;
  }
  void putVLI(U64 i) {
    while (i>0x7F) {
      putchar(0x80|(i&0x7F));
      i>>=7;
    }
    putchar(U8(i));
  }
  virtual void setpos(U64 newpos) = 0;
  virtual void setend() = 0;
  virtual U64 curpos() = 0;
  virtual bool eof() = 0;
};

// This class is responsible for files on disk
// It simply passes function calls to stdio

class FileDisk : public File {
protected:
  FILE *file;
public:
  FileDisk() {file=nullptr;}
  ~FileDisk() {close();}
  bool open(const char *filename, bool must_succeed) override {
    assert(file==nullptr);
    file = openfile(filename,READ);
    const bool success=(file!=nullptr);
    if(!success && must_succeed){printf("Unable to open file %s (%s)", filename, strerror(errno));quit();}
    return success;
  }
  void create(const char *filename) override {
    assert(file==nullptr);
    makedirectories(filename);
    file=openfile(filename,WRITE);
    if (!file) {printf("Unable to create file %s (%s)", filename, strerror(errno));quit();}
  }
  void createtmp() {
    assert(file==nullptr);
    file = maketmpfile();
    if (!file) {printf("Unable to create temporary file (%s)", strerror(errno));quit();}
  }
  void close() override { if(file) fclose(file); file=nullptr;}
  int getchar() override { return fgetc(file); }
  void putchar(U8 c) override { fputc(c, file); }
  U64 blockread(U8 *ptr, U64 count) override {return fread(ptr,1,count,file);}
  void blockwrite(U8 *ptr, U64 count) override {fwrite(ptr,1,count,file);}
  void setpos(U64 newpos) override { fseeko(file, newpos, SEEK_SET); }
  void setend() override { fseeko(file, 0, SEEK_END); }
  U64 curpos() override { return ftello(file); }
  bool eof() override { return feof(file)!=0; }
};

// This class is responsible for temporary files in RAM or on disk
// Initially it uses RAM for temporary file content.
// In case of the content size in RAM grows too large, it is written to disk,
// the RAM is freed and all subsequent file operations will use the file on disk.

class FileTmp : public File {
private:
  //file content in ram
  Array<U8> *content_in_ram; //content of file
  U64 filepos;
  U64 filesize;
  void forget_content_in_ram() {
    if (content_in_ram) {
      delete content_in_ram;
      content_in_ram = nullptr;
      filepos = 0;
      filesize = 0;
    }
  }
  //file on disk
  FileDisk *file_on_disk;
  void forget_file_on_disk() {
    if (file_on_disk) {
      file_on_disk->close();
      delete file_on_disk;
      file_on_disk = nullptr;
    }
  }
  //switch: ram->disk
  #ifdef NDEBUG
  static constexpr U32 MAX_RAM_FOR_TMP_CONTENT = 64 * 1024 * 1024; //64 MB (per file)
  #else
  static constexpr U32 MAX_RAM_FOR_TMP_CONTENT = 64; // to trigger switching to disk earlier - for testing
  #endif
  void ram_to_disk() {
    assert(file_on_disk==nullptr);
    file_on_disk = new FileDisk();
    file_on_disk->createtmp();
    if(filesize>0)
      file_on_disk->blockwrite(&((*content_in_ram)[0]), filesize);
    file_on_disk->setpos(filepos);
    forget_content_in_ram();
  }
public:
  FileTmp() {content_in_ram=new Array<U8>(0); filepos=0; filesize=0; file_on_disk = nullptr;}
  ~FileTmp() {close();}
  bool open(const char *, bool) override { assert(false); return false; } //this method is forbidden for temporary files
  void create(const char *) override { assert(false); } //this method is forbidden for temporary files
  void close() override {
    forget_content_in_ram();
    forget_file_on_disk();
  }
  int getchar() override {
    if(content_in_ram) {
      if (filepos >= filesize)
        return EOF;
      else {
        const U8 c = (*content_in_ram)[filepos];
        filepos++;
        return c;
      }
    }
    else return file_on_disk->getchar();
  }
  void putchar(U8 c) override {
    if(content_in_ram) {
      if (filepos < MAX_RAM_FOR_TMP_CONTENT) {
        if (filepos == filesize) { content_in_ram->push_back(c); filesize++; }
        else (*content_in_ram)[filepos] = c;
        filepos++;
        return;
      }
      else ram_to_disk();
    }
    file_on_disk->putchar(c);
  }
  U64 blockread(U8 *ptr, U64 count) override {
    if(content_in_ram) {
      const U64 available = filesize - filepos;
      if (available<count)count = available;
      if(count>0)memcpy(ptr, &((*content_in_ram)[filepos]), count);
      filepos += count;
      return count;
    }
    else return file_on_disk->blockread(ptr,count);
  }
  void blockwrite(U8 *ptr, U64 count) override {
    if(content_in_ram) {
      if (filepos+count <= MAX_RAM_FOR_TMP_CONTENT) {
        content_in_ram->resize((filepos + count));
        if(count>0)memcpy(&((*content_in_ram)[filepos]), ptr, count);
        filesize += count;
        filepos += count;
        return;
      }
      else ram_to_disk();
    }
    file_on_disk->blockwrite(ptr,count);
  }
  void setpos(U64 newpos) override {
    if(content_in_ram) {
      if(newpos>filesize)ram_to_disk(); //panic: we don't support seeking past end of file (but stdio does) - let's switch to disk
      else {filepos = newpos; return;}
    }
    file_on_disk->setpos(newpos);
  }
  void setend() override {
    if(content_in_ram) filepos = filesize;
    else file_on_disk->setend();
  }
  U64 curpos() override {
    if(content_in_ram) return filepos;
    else return file_on_disk->curpos();
  }
  bool eof() override {
    if(content_in_ram)return filepos >= filesize;
    else return file_on_disk->eof();
  }
};

// Helper class: open a file from the executable's folder
//

static char mypatherror[]="Can't determine my path.";
class OpenFromMyFolder {
private:
public:
  //this static method will open the executable itself for reading
  static void myself(FileDisk *f) {
  #ifdef WINDOWS
    int i;
    Array<wchar_t> myfilename(MAX_PATH);
    if((i=GetModuleFileNameW(nullptr, &myfilename[0], MAX_PATH)) && i<MAX_PATH && i!=0) {
      f->open(Utf8Str(&myfilename[0]).utf8_str,true);
    }
    else
      quit(mypatherror);
  #else
    Array<char> myfilename(PATH_MAX+1);
    if(readlink("/proc/self/exe", &myfilename[0], PATH_MAX)!=-1)
      f->open(&myfilename[0],true);
    else
      quit(mypatherror);
  #endif
  }

  //this static method will open a file from the executable's folder
  //only ASCII filenames are supported
  static void anotherfile(FileDisk *f, const char* filename) {
    const U64 flength=strlen(filename)+1;
    #ifdef WINDOWS
      int i;
      Array<char> myfilename(MAX_PATH+flength);
      if((i=GetModuleFileName(nullptr, &myfilename[0], MAX_PATH)) && i<MAX_PATH && i!=0) {
        char *endofpath=strrchr(&myfilename[0], '\\');
    #endif
    #ifdef UNIX
      char myfilename[PATH_MAX+flength];
      if(readlink("/proc/self/exe", myfilename, PATH_MAX)!=-1) {
        char *endofpath=strrchr(&myfilename[0], '/');
    #endif
        if (endofpath==nullptr)quit(mypatherror);
        endofpath++;
        strcpy(endofpath, filename); //append filename to my path
        f->open(&myfilename[0],true);
      }
      else
        quit(mypatherror);
  }
};

// Verify that the specified file exists and is readable, determine file size
static U64 getfilesize(const char * filename) {
  FileDisk f;
  f.open(filename,true);
  f.setend();
  const U64 filesize=f.curpos();
  f.close();
  if((filesize>>31)!=0)quit("Large files not supported.");
  return filesize;
}

static void appendtofile(const char *filename, const char* s) {
  FILE *f=openfile(filename,APPEND);
  if (f==nullptr)
    printf("Warning: could not log compression results to %s\n",filename);
  else {
    fprintf(f,"%s",s);
    fclose(f);
  }
}

//determine if output is redirected
static bool isoutputdirected()
{
#ifdef WINDOWS
  DWORD FileType=GetFileType(GetStdHandle(STD_OUTPUT_HANDLE));
  return (FileType==FILE_TYPE_PIPE) || (FileType==FILE_TYPE_DISK);
#endif
#ifdef UNIX
  return !isatty(fileno(stdout));
#endif
}
static bool to_screen=!isoutputdirected();

//////////////////////////// rnd ///////////////////////////////

// 32-bit pseudo random number generator
class Random{
  Array<U32> table;
  U64 i;
public:
  Random(): table(64) {
    table[0]=123456789;
    table[1]=987654321;
    for (U64 j=0; j<62; j++) table[j+2]=table[j+1]*11+table[j]*23/16;
    i=0;
  }
  U32 operator()() {
    return ++i, table[i&63]=table[(i-24)&63]^table[(i-55)&63];
  }
};


////////////////////////////// RingBuffer /////////////////////////////

// RingBuffer(n) buf; creates an array of n bytes (must be a power of 2).
// buf[i] returns a reference to the i'th byte with wrap (no out of bounds).
// buf(i) returns i'th byte back from pos (i>0) with wrap (no out of bounds)
// buf.size() returns n.

template <class T> class RingBuffer {
private:
  Array<T> b;
  U32 offset; // Number of input bytes in buffer (not wrapped), will be masked when used for indexing
  U32 mask;
public:
  RingBuffer(const U32 size=0): b(size), offset(0), mask(size-1) {
    assert(ispowerof2(size));
  }
  void setsize(U32 newsize) {
    assert(newsize>0 && ispowerof2(newsize));
    b.resize(newsize);
    offset=0;
    mask=newsize-1;
  }
  U32 getpos() const {
    return offset;
  }
  void fill(const T B) {
    const U32 n=(U32)b.size();
    for(U32 i=0;i<n;i++)
      b[i]=B;
  }
  void add(const T B) {
    b[offset&mask] = B;
    offset++;
  }
  T operator[](const U32 i) const {
    return b[i&mask];
  }
  void set(const U32 i,const T B) {
    b[i&mask]=B;
  }
  T operator()(const U32 i) const {
    //assert(i!=0);
    return b[(offset-i)&mask];
  }
  void reset() {
    fill(0);
    offset=0;
  }
  U32 size() {
    return (U32)b.size();
  }
  void copyto(RingBuffer &dst) {
    dst.setsize(size());
    dst.offset=offset;
    U32 n=(U32)b.size();
    for(U32 i=0;i<n;i++)
      dst.b[i]=b[i];
  }
};

typedef RingBuffer<U8> Buf;

/////////////////////// Global context /////////////////////////

typedef enum {DEFAULT=0, FILECONTAINER, JPEG, HDR, IMAGE1, IMAGE4, IMAGE8, IMAGE8GRAY, IMAGE24, IMAGE32, AUDIO, AUDIO_LE, EXE, CD, ZLIB, BASE64, GIF, PNG8, PNG8GRAY, PNG24, PNG32, TEXT, TEXT_EOL, RLE, LZW} Blocktype;

inline bool hasRecursion(Blocktype ft) { return ft==CD || ft==ZLIB || ft==BASE64 || ft==GIF || ft==RLE || ft==LZW || ft== FILECONTAINER; }
inline bool hasInfo(Blocktype ft) { return ft==IMAGE1 || ft==IMAGE4 || ft==IMAGE8 || ft==IMAGE8GRAY || ft==IMAGE24 || ft==IMAGE32 || ft==AUDIO || ft==AUDIO_LE || ft==PNG8 || ft==PNG8GRAY || ft==PNG24 || ft==PNG32; }
inline bool hasTransform(Blocktype ft) { return ft==IMAGE24 || ft==IMAGE32 || ft==AUDIO_LE || ft==EXE || ft==CD || ft==ZLIB || ft==BASE64 || ft==GIF || ft==TEXT_EOL || ft==RLE || ft==LZW; }
inline bool isPNG(Blocktype ft) { return ft==PNG8 || ft==PNG8GRAY || ft==PNG24 || ft==PNG32; }

int level=0; //this value will be overwritten at the beginning of compression/decompression
#define MEM (U64(65536)<<level)

U8 options=0;
#define OPTION_MULTIPLE_FILE_MODE 1
#define OPTION_BRUTE 2
#define OPTION_TRAINEXE 4
#define OPTION_TRAINTXT 8
#define OPTION_ADAPTIVE 16
#define OPTION_SKIPRGB 32

#define INJECT_STATS_blpos   const U32 blpos=stats->blpos;

// Information shared by models
// Order: in appearance -> models may use information 
// from models that appears above them
struct ModelStats {

  //General shared information
  Blocktype blockType{};  //used by wordModel, recordModel, SSE stage
  U32 blpos{};            //relative position in block, used by many models
  U64 Misses{};           //used by SSE stage

  //MatchModel
  struct {
    U32 length3;     //used by SSE stage and RecordModel
    U8 expectedByte; //used by SSE stage
  } Match{};

  //NormalModel
  int order{};

  //image models
  struct {
    struct {
      U8 WW, W, NN, N, Wp1, Np1; 
    } pixels;        //used by SSE stage
    U8 plane;        //used by SSE stage
    U8 ctx;          //used by SSE stage
  } Image{};

  //AudioModel
  U32 Wav{};           //used by recordModel
  U8 Audio{};

  //JpegModel
  //SparseMatchModel
  //SparseModel
  //CharGroupModel
  //WordModel
  //IndirectModel
  //Dmcforest
  //NestModel
  //XMLModel

  //TextModel
  struct {
    U8 chargrp;     //used by RecordModel, TextModel - Quantized partial byte as ASCII group
    U8 firstLetter;  //used by SSE stage
    U8 mask;         //used by SSE stage
  } Text{};

  //RecordModel
  //ExeModel
  
  void reset() {
    memset(this, 0, sizeof(ModelStats));
  }
};

// helper #defines to access shared variables
#define INJECT_SHARED_buf   const Buf &buf=shared->buf;
#define INJECT_SHARED_pos   const U32 pos=shared->buf.getpos();
#define INJECT_SHARED_y     const U8  y=shared->y;
#define INJECT_SHARED_c0    const U8  c0=shared->c0;
#define INJECT_SHARED_c1    const U8  c1=shared->c1;
#define INJECT_SHARED_bpos  const U8  bpos=shared->bpos;
#define INJECT_SHARED_c4    const U32 c4=shared->c4;
#define INJECT_SHARED_c8    const U32 c8=shared->c8;

// Global context available to all models.
struct Shared {
  Buf buf;     // Rotating input queue set by Predictor
  U8 y=0;      // Last bit, 0 or 1
  U8 c0=1;     // Last 0-7 bits of the partial byte with a leading 1 bit (1-255)
  U8 c1=0;     // Last whole byte, also c4&0xff or buf(1)
  U8 bpos=0;   // Bits in c0 (0 to 7), in other words the position of the bit to be predicted (0=MSB)
  U32 c4=0;    // Last 4 whole bytes (buf(4)..buf(1)), packed.  Last byte is bits 0-7.
  U32 c8=0;    // Another 4 bytes (buf(8)..buf(5))
  void update() {
    c0+=c0+y;
    bpos=(bpos+1)&7;
    if (bpos==0) {
      c1=c0;
      buf.add(c1);
      c8=(c8<<8)|(c4>>24);
      c4=(c4<<8)|c0;
      c0=1;
    }
  }
  void reset() {
    buf.reset();
    y=0;
    c0=1;
    c1=bpos=0;
    c4=c8=0;
  }
  void copyto(Shared *dst) {
    this->buf.copyto(dst->buf);
    dst->y=y;
    dst->c0=c0;
    dst->c1=c1;
    dst->bpos=bpos;
    dst->c4=c4;
    dst->c8=c8;
  }
};

// UTF8 validator
// Based on: http://bjoern.hoehrmann.de/utf-8/decoder/dfa/
// Control caracters (0x00-0x1f) and 0x7f are not allowed (except for 0/tab/cr/lf)

#define UTF8_ACCEPT 0
#define UTF8_REJECT 12

static const U8 utf8_state_table[] = {
  // byte -> character class
  // character_class = utf8_state_table[byte]
  1,1,1,1,1,1,1,1,1,0,0,1,1,0,1,1,  1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1, // 00..1f  
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, // 20..3f
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, // 40..5f
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1, // 60..7f
  1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,  9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9, // 80..9f
  7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,  7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7, // a0..bf
  8,8,2,2,2,2,2,2,2,2,2,2,2,2,2,2,  2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2, // c0..df
 10,3,3,3,3,3,3,3,3,3,3,3,3,4,3,3, 11,6,6,6,5,8,8,8,8,8,8,8,8,8,8,8, // e0..ff
  // validating automaton
  // new_state = utf8_state_table[256*old_state + character_class]
   0,12,24,36,60,96,84,12,12,12,48,72, // state  0-11
  12,12,12,12,12,12,12,12,12,12,12,12, // state 12-23
  12, 0,12,12,12,12,12, 0,12, 0,12,12, // state 24-35
  12,24,12,12,12,12,12,24,12,24,12,12, // state 36-47
  12,12,12,12,12,12,12,24,12,12,12,12, // state 48-59
  12,24,12,12,12,12,12,12,12,24,12,12, // state 60-71
  12,12,12,12,12,12,12,36,12,36,12,12, // state 72-83
  12,36,12,12,12,12,12,36,12,36,12,12, // state 84-95
  12,36,12,12,12,12,12,12,12,12,12,12  // state 96-108
};


#define CacheSize 32

#if (CacheSize&(CacheSize-1)) || (CacheSize<8)
  #error Cache size must be a power of 2 bigger than 4
#endif


///////////////////////////// ilog //////////////////////////////

// ilog(x) = round(log2(x) * 16), 0 <= x < 64K
class Ilog {
  Array<U8> t;
public:
  int operator()(U16 x) const {return (int)t[x];}
  Ilog(): t(65536) {
    // Compute lookup table by numerical integration of 1/x
    U32 x=14155776;
    for (U32 i=2; i<65536; ++i) {
      x+=774541002/(i*2-1);  // numerator is 2^29/ln 2
      t[i]=x>>24;
    }
  }
} ilog;

// llog(x) accepts 32 bits
inline int llog(U32 x) {
  if (x>=0x1000000)
    return 256+ilog(U16(x>>16));
  else if (x>=0x10000)
    return 128+ilog(U16(x>>8));
  else
    return ilog(U16(x));
}

inline U32 BitCount(U32 v) {
  v -= ((v >> 1) & 0x55555555);
  v = ((v >> 2) & 0x33333333) + (v & 0x33333333);
  v = ((v >> 4) + v) & 0x0f0f0f0f;
  v = ((v >> 8) + v) & 0x00ff00ff;
  v = ((v >> 16) + v) & 0x0000ffff;
  return v;
}

inline int VLICost(U64 n) {
  int cost = 1;
  while (n>0x7F) {
    n>>=7;
    cost++;
  }
  return cost;
}

// ilog2
// returns floor(log2(x))
// 0/1->0, 2->1, 3->1, 4->2 ..., 30->4,  31->4, 32->5,  33->5
#ifdef _MSC_VER
#include <intrin.h>
inline U32 ilog2(U32 x) {
  DWORD tmp=0;
  if(x!=0)_BitScanReverse(&tmp,x);
  return tmp;
}
#elif __GNUC__
inline U32 ilog2(U32 x) {
  if(x!=0)x=31-__builtin_clz(x);
  return x;
}
#else
inline U32 ilog2(U32 x) {
  //copy the leading "1" bit to its left (0x03000000 -> 0x03ffffff)
  x |= (x >> 1);
  x |= (x >> 2);
  x |= (x >> 4);
  x |= (x >> 8);
  x |= (x >>16);
  //how many trailing bits do we have (except the first)?
  return BitCount(x >> 1);
}
#endif

inline float rsqrt(const float x) {
  float r = _mm_cvtss_f32(_mm_rsqrt_ss(_mm_set_ss(x))); //SSE
  return (0.5f * (r + 1.0f/(x * r)));
}

///////////////////////// state table ////////////////////////

// State table:
//   nex(state, 0) = next state if bit y is 0, 0 <= state < 256
//   nex(state, 1) = next state if bit y is 1
//   nex(state, 2) = number of zeros in bit history represented by state
//   nex(state, 3) = number of ones represented
//
// States represent a bit history within some context.
// State 0 is the starting state (no bits seen).
// States 1-30 represent all possible sequences of 1-4 bits.
// States 31-252 represent a pair of counts, (n0,n1), the number
//   of 0 and 1 bits respectively.  If n0+n1 < 16 then there are
//   two states for each pair, depending on if a 0 or 1 was the last
//   bit seen.
// If n0 and n1 are too large, then there is no state to represent this
// pair, so another state with about the same ratio of n0/n1 is substituted.
// Also, when a bit is observed and the count of the opposite bit is large,
// then part of this count is discarded to favor newer data over old.

class StateTable {
  static constexpr U8 State_table[256][4]={
  {  1,  2, 0, 0},{  3,  5, 1, 0},{  4,  6, 0, 1},{  7, 10, 2, 0}, // 0-3
  {  8, 12, 1, 1},{  9, 13, 1, 1},{ 11, 14, 0, 2},{ 15, 19, 3, 0}, // 4-7
  { 16, 23, 2, 1},{ 17, 24, 2, 1},{ 18, 25, 2, 1},{ 20, 27, 1, 2}, // 8-11
  { 21, 28, 1, 2},{ 22, 29, 1, 2},{ 26, 30, 0, 3},{ 31, 33, 4, 0}, // 12-15
  { 32, 35, 3, 1},{ 32, 35, 3, 1},{ 32, 35, 3, 1},{ 32, 35, 3, 1}, // 16-19
  { 34, 37, 2, 2},{ 34, 37, 2, 2},{ 34, 37, 2, 2},{ 34, 37, 2, 2}, // 20-23
  { 34, 37, 2, 2},{ 34, 37, 2, 2},{ 36, 39, 1, 3},{ 36, 39, 1, 3}, // 24-27
  { 36, 39, 1, 3},{ 36, 39, 1, 3},{ 38, 40, 0, 4},{ 41, 43, 5, 0}, // 28-31
  { 42, 45, 4, 1},{ 42, 45, 4, 1},{ 44, 47, 3, 2},{ 44, 47, 3, 2}, // 32-35
  { 46, 49, 2, 3},{ 46, 49, 2, 3},{ 48, 51, 1, 4},{ 48, 51, 1, 4}, // 36-39
  { 50, 52, 0, 5},{ 53, 43, 6, 0},{ 54, 57, 5, 1},{ 54, 57, 5, 1}, // 40-43
  { 56, 59, 4, 2},{ 56, 59, 4, 2},{ 58, 61, 3, 3},{ 58, 61, 3, 3}, // 44-47
  { 60, 63, 2, 4},{ 60, 63, 2, 4},{ 62, 65, 1, 5},{ 62, 65, 1, 5}, // 48-51
  { 50, 66, 0, 6},{ 67, 55, 7, 0},{ 68, 57, 6, 1},{ 68, 57, 6, 1}, // 52-55
  { 70, 73, 5, 2},{ 70, 73, 5, 2},{ 72, 75, 4, 3},{ 72, 75, 4, 3}, // 56-59
  { 74, 77, 3, 4},{ 74, 77, 3, 4},{ 76, 79, 2, 5},{ 76, 79, 2, 5}, // 60-63
  { 62, 81, 1, 6},{ 62, 81, 1, 6},{ 64, 82, 0, 7},{ 83, 69, 8, 0}, // 64-67
  { 84, 71, 7, 1},{ 84, 71, 7, 1},{ 86, 73, 6, 2},{ 86, 73, 6, 2}, // 68-71
  { 44, 59, 5, 3},{ 44, 59, 5, 3},{ 58, 61, 4, 4},{ 58, 61, 4, 4}, // 72-75
  { 60, 49, 3, 5},{ 60, 49, 3, 5},{ 76, 89, 2, 6},{ 76, 89, 2, 6}, // 76-79
  { 78, 91, 1, 7},{ 78, 91, 1, 7},{ 80, 92, 0, 8},{ 93, 69, 9, 0}, // 80-83
  { 94, 87, 8, 1},{ 94, 87, 8, 1},{ 96, 45, 7, 2},{ 96, 45, 7, 2}, // 84-87
  { 48, 99, 2, 7},{ 48, 99, 2, 7},{ 88,101, 1, 8},{ 88,101, 1, 8}, // 88-91
  { 80,102, 0, 9},{103, 69,10, 0},{104, 87, 9, 1},{104, 87, 9, 1}, // 92-95
  {106, 57, 8, 2},{106, 57, 8, 2},{ 62,109, 2, 8},{ 62,109, 2, 8}, // 96-99
  { 88,111, 1, 9},{ 88,111, 1, 9},{ 80,112, 0,10},{113, 85,11, 0}, // 100-103
  {114, 87,10, 1},{114, 87,10, 1},{116, 57, 9, 2},{116, 57, 9, 2}, // 104-107
  { 62,119, 2, 9},{ 62,119, 2, 9},{ 88,121, 1,10},{ 88,121, 1,10}, // 108-111
  { 90,122, 0,11},{123, 85,12, 0},{124, 97,11, 1},{124, 97,11, 1}, // 112-115
  {126, 57,10, 2},{126, 57,10, 2},{ 62,129, 2,10},{ 62,129, 2,10}, // 116-119
  { 98,131, 1,11},{ 98,131, 1,11},{ 90,132, 0,12},{133, 85,13, 0}, // 120-123
  {134, 97,12, 1},{134, 97,12, 1},{136, 57,11, 2},{136, 57,11, 2}, // 124-127
  { 62,139, 2,11},{ 62,139, 2,11},{ 98,141, 1,12},{ 98,141, 1,12}, // 128-131
  { 90,142, 0,13},{143, 95,14, 0},{144, 97,13, 1},{144, 97,13, 1}, // 132-135
  { 68, 57,12, 2},{ 68, 57,12, 2},{ 62, 81, 2,12},{ 62, 81, 2,12}, // 136-139
  { 98,147, 1,13},{ 98,147, 1,13},{100,148, 0,14},{149, 95,15, 0}, // 140-143
  {150,107,14, 1},{150,107,14, 1},{108,151, 1,14},{108,151, 1,14}, // 144-147
  {100,152, 0,15},{153, 95,16, 0},{154,107,15, 1},{108,155, 1,15}, // 148-151
  {100,156, 0,16},{157, 95,17, 0},{158,107,16, 1},{108,159, 1,16}, // 152-155
  {100,160, 0,17},{161,105,18, 0},{162,107,17, 1},{108,163, 1,17}, // 156-159
  {110,164, 0,18},{165,105,19, 0},{166,117,18, 1},{118,167, 1,18}, // 160-163
  {110,168, 0,19},{169,105,20, 0},{170,117,19, 1},{118,171, 1,19}, // 164-167
  {110,172, 0,20},{173,105,21, 0},{174,117,20, 1},{118,175, 1,20}, // 168-171
  {110,176, 0,21},{177,105,22, 0},{178,117,21, 1},{118,179, 1,21}, // 172-175
  {110,180, 0,22},{181,115,23, 0},{182,117,22, 1},{118,183, 1,22}, // 176-179
  {120,184, 0,23},{185,115,24, 0},{186,127,23, 1},{128,187, 1,23}, // 180-183
  {120,188, 0,24},{189,115,25, 0},{190,127,24, 1},{128,191, 1,24}, // 184-187
  {120,192, 0,25},{193,115,26, 0},{194,127,25, 1},{128,195, 1,25}, // 188-191
  {120,196, 0,26},{197,115,27, 0},{198,127,26, 1},{128,199, 1,26}, // 192-195
  {120,200, 0,27},{201,115,28, 0},{202,127,27, 1},{128,203, 1,27}, // 196-199
  {120,204, 0,28},{205,115,29, 0},{206,127,28, 1},{128,207, 1,28}, // 200-203
  {120,208, 0,29},{209,125,30, 0},{210,127,29, 1},{128,211, 1,29}, // 204-207
  {130,212, 0,30},{213,125,31, 0},{214,137,30, 1},{138,215, 1,30}, // 208-211
  {130,216, 0,31},{217,125,32, 0},{218,137,31, 1},{138,219, 1,31}, // 212-215
  {130,220, 0,32},{221,125,33, 0},{222,137,32, 1},{138,223, 1,32}, // 216-219
  {130,224, 0,33},{225,125,34, 0},{226,137,33, 1},{138,227, 1,33}, // 220-223
  {130,228, 0,34},{229,125,35, 0},{230,137,34, 1},{138,231, 1,34}, // 224-227
  {130,232, 0,35},{233,125,36, 0},{234,137,35, 1},{138,235, 1,35}, // 228-231
  {130,236, 0,36},{237,125,37, 0},{238,137,36, 1},{138,239, 1,36}, // 232-235
  {130,240, 0,37},{241,125,38, 0},{242,137,37, 1},{138,243, 1,37}, // 236-239
  {130,244, 0,38},{245,135,39, 0},{246,137,38, 1},{138,247, 1,38}, // 240-243
  {140,248, 0,39},{249,135,40, 0},{250, 69,39, 1},{ 80,251, 1,39}, // 244-247
  {140,252, 0,40},{249,135,41, 0},{250, 69,40, 1},{ 80,251, 1,40}, // 248-251
  {140,252, 0,41}};  // 252, 253-255 are reserved

  static constexpr U8 State_group[256]=
  { 0,1,2,3,4,5,6,7,8,9,10,11,12,13,14, // 0-14
    18,23,24,24,26,25,27,26,26,25,27,26,28,28,29,15, //15-30
    18,23,25,25,26,26,27,27,29,15, //31-40
    18,23,25,24,25,26,26,27,28,27,29,15, //41-52
    18,23,24,24,24,25,25,27,27,28,28,28,29,15, //53-66
    19,22,24,23,24,25,25,26,26,27,27,28,29,28,30,16, //67-82
    19,22,24,23,24,28,29,28,30,16, //83-92
    19,22,24,23,24,28,29,28,30,16, //93-102
    19,22,24,23,24,28,29,28,30,16, //103-112
    19,22,24,23,24,28,29,28,30,16, //113-122
    19,22,24,22,23,29,30,28,30,16, //123-132
    19,22,23,22,23,29,30,29,30,16, //133-142
    19,22, 0, 0,30,16, //143-148
    19,22,30,16, 19,22,30,16, 19,22,30,16, 19,22,30,16, 19,22,30,16, //149-152 153-156 157-160 161-164 165-168 
    19,21,31,16, 19,21,31,16, 19,21,31,16, 19,21,31,16, 19,21,31,16, //169-172 173-176 177-180 181-184 185-188 
    19,21,31,16, 19,21,31,16, 19,21,31,16, 19,21,31,16, 19,21,31,17, //189-192 193-196 197-200 201-204 205-208 
    20,21,31,17, 20,21,31,17, 20,21,31,17, 20,21,31,17, 20,21,31,17, //209-212 213-216 217-220 221-224 225-228
    20,21,31,17, 20,21,31,17, 20,21,31,17, 20,21,31,17, 20,21,31,17, //229-232 233-236 237-240 241-244 245-248
    20,21,31,17, 0,0,0, //249-252 253-255
  };

public:
  static U8 next(U8 const state, const int y) {
    return State_table[state][y];
  }

  // Probabilistic increment (approximate counting)
  // For states 205..208, 209..212, ... 249..252 a group of 4 states is not represented by
  // the counts indicated in the state table. An exponential scale is used instead.
  // The highest group (249..252) represents the top of this scale, where we can not increment anymore.
  static U8 next(U8 const old_state, const int y, Random &rnd) {
    U8 new_state = State_table[old_state][y];
    if(new_state>=205) { // for all groups of four states higher than idx 205
      if(old_state<249) { // still climbing
        const int shift=(460-new_state)>>3;
        //for each group the probability to advance to a higher state is less and less as we climb higher and higher
        if((rnd()<<shift)!=0) 
          new_state-=4; 
      }
    }
    return new_state;
  }

  static void update(U8 *const state, const int y, Random &rnd) {
    *state=next(*state,y,rnd);
  }

  static U8 group(const U8 state) {
    return State_group[state];
  }
};


///////////////////////////// Squash //////////////////////////////

// return p = 1/(1 + exp(-d)), d scaled by 8 bits, p scaled by 12 bits
int squash(int d) {
  static constexpr int t[33]={
    1,2,3,6,10,16,27,45,73,120,194,310,488,747,1101,
    1546,2047,2549,2994,3348,3607,3785,3901,3975,4022,
    4050,4068,4079,4085,4089,4092,4093,4094};
  if (d>2047) return 4095;
  if (d<-2047) return 0;
  const int w=d&127;
  d=(d>>7)+16;
  return (t[d]*(128-w)+t[(d+1)]*w+64) >> 7;
}

//////////////////////////// Stretch ///////////////////////////////

// Inverse of squash. d = ln(p/(1-p)), d scaled by 8 bits, p by 12 bits.
// d has range -2047 to 2047 representing -8 to 8.  p has range 0 to 4095.

class Stretch {
  Array<short> t;
public:
  Stretch();
  int operator()(int p) const {
    assert(p>=0 && p<4096);
    return t[p];
  }
} stretch;

Stretch::Stretch(): t(4096) {
  int pi=0;
  for (int x=-2047; x<=2047; ++x) {  // invert squash()
    const int i=squash(x);
    for (int j=pi; j<=i; ++j)
      t[j]=(short)x;
    pi=i+1;
  }
  t[4095]=2047;
}

//////////////////////////// IPredictor //////////////////////////

// Common interface for all probability predictors (probability map
// and mixer classes).
//
// A probability predictor predicts the probability that the 
// next bit is 1.
//
// After each bit becomes known, all probability predictors participated 
// in the prediction must update their internal state to improve future 
// predictions (adapting). In order to achieve this, a probability 
// predictor must 
// 1) inherit from the IPredictor intercace, 
// 2) implement the update() interface method,
// 3) subscribe for the broadcast in p() or mix(),
// 4) receive the update event from UpdateBroadcaster 
//    and update its internal state in update().

// Interface class for all predictors (probability maps, mixers, models)
// It declares the update() event handler.
class IPredictor {
public:
  virtual void update() = 0;
  virtual ~IPredictor() {}
};

///////////////////////// UpdateBroadcaster ///////////////////////

// The purpose of this class is to inform probability predictors when
// then next bit is known by calling the update() method of each predictor.
class UpdateBroadcaster {
private:
  int n; // number of subscribed predictors, (number of items in "subscribers" array)
  IPredictor *subscribers[1024];
public:
  UpdateBroadcaster() : n(0) {}
  void subscribe(IPredictor *subscriber) { 
    subscribers[n] = subscriber;
    n++;
    assert(n < 1024);
  }
  void broadcast_update() {
    for (int i=0;i<n;i++) {
      IPredictor *subscriber = subscribers[i];
      subscriber->update();
    }
    n=0;
  }
} updater;


//////////////////////////// Mixer /////////////////////////////

// Mixer m(N, M, S=1, w=0) combines models using M neural networks with
//   N inputs each, of which up to S may be selected.  If S > 1 then
//   the outputs of these neural networks are combined using another
//   neural network (with arguments S, 1, 1).  If S = 1 then the
//   output is direct.  The weights are initially w (+-32K).
//   It is used as follows:
// m.update() trains the network where the expected output is the
//   last bit (in the global variable y).
// m.add(stretch(p)) inputs prediction from one of N models.  The
//   prediction should be positive to predict a 1 bit, negative for 0,
//   nominally +-256 to +-2K.  The maximum allowed value is +-32K but
//   using such large values may cause overflow if N is large.
// m.set(cxt, range) selects cxt as one of 'range' neural networks to
//   use.  0 <= cxt < range.  Should be called up to S times such
//   that the total of the ranges is <= M.
// m.p() returns the output prediction that the next bit is 1 as a
//   12 bit number (0 to 4095).

#ifdef __GNUC__
__attribute__ ((target ("avx2")))
#endif
static int dot_product_simd_avx2 (const short* const t, const short* const w, int n) {
  __m256i sum = _mm256_setzero_si256();

  while ((n-=16)>=0) {
    __m256i tmp = _mm256_madd_epi16(*( __m256i* ) &t[n], *( __m256i* ) &w[n]);
    tmp = _mm256_srai_epi32(tmp, 8);
    sum = _mm256_add_epi32(sum, tmp);
  }

  __m128i lo = _mm256_extractf128_si256(sum, 0);
  __m128i hi = _mm256_extractf128_si256(sum, 1);

  __m128i newsum = _mm_hadd_epi32(lo, hi);
  newsum = _mm_add_epi32(newsum, _mm_srli_si128(newsum, 8));
  newsum = _mm_add_epi32(newsum, _mm_srli_si128(newsum, 4));
  return _mm_cvtsi128_si32(newsum);
}

#ifdef __GNUC__
__attribute__ ((target ("avx2")))
#endif
static void train_simd_avx2 (const short* const t, short* const w, int n, const int e)  {
  const __m256i one = _mm256_set1_epi16(1);
  const __m256i err = _mm256_set1_epi16(short(e));

  while ((n-=16)>=0) {
    __m256i tmp = _mm256_adds_epi16(*( __m256i* ) &t[n], *( __m256i* ) &t[n]);
    tmp = _mm256_mulhi_epi16(tmp, err);
    tmp = _mm256_adds_epi16(tmp, one);
    tmp = _mm256_srai_epi16(tmp, 1);
    tmp = _mm256_adds_epi16(tmp, *( __m256i* ) &w[n]);
    *( __m256i* ) &w[n] = tmp;
  }
}

#ifdef __GNUC__
__attribute__ ((target ("sse2")))
#endif
static int dot_product_simd_sse2(const short* const t, const short* const w, int n) {
  __m128i sum = _mm_setzero_si128();

  while ((n-=8)>=0) {
    __m128i tmp = _mm_madd_epi16(*(__m128i *) &t[n], *(__m128i *) &w[n]);
    tmp = _mm_srai_epi32(tmp, 8);
    sum = _mm_add_epi32(sum, tmp);
  }

  sum = _mm_add_epi32(sum, _mm_srli_si128 (sum, 8));
  sum = _mm_add_epi32(sum, _mm_srli_si128 (sum, 4));
  return _mm_cvtsi128_si32(sum);
}

#ifdef __GNUC__
__attribute__ ((target ("sse2")))
#endif
static void train_simd_sse2(const short* const t, short* const w, int n, const int e) {
  const __m128i one = _mm_set1_epi16(1);
  const __m128i err = _mm_set1_epi16(short(e));

  while ((n-=8)>=0) {
    __m128i tmp = _mm_adds_epi16(*(__m128i *) &t[n], *(__m128i *) &t[n]);
    tmp = _mm_mulhi_epi16(tmp, err);
    tmp = _mm_adds_epi16(tmp, one);
    tmp = _mm_srai_epi16(tmp, 1);
    tmp = _mm_adds_epi16(tmp, *(__m128i *) &w[n]);
    *(__m128i *) &w[n] = tmp;
  }
}

static int dot_product_simd_none(const short* const t, const short* const w, int n) {
  int sum = 0;
  while ((n -= 2) >= 0) {
    sum += (t[n] * w[n] + t[n + 1] * w[n + 1]) >> 8;
  }
  return sum;
}

static void train_simd_none(const short* const t, short* const w, int n, const int err) {
  while ((n -= 1) >= 0) {
    int wt = w[n] + ((((t[n] * err * 2) >> 16) + 1) >> 1);
    if (wt < -32768) wt = -32768;
    else if (wt > 32767) wt = 32767;
    w[n] = (short)wt;
  }
}

struct ErrorInfo {
  U32 Data[2], Sum, Mask, Collected;
  void reset() {
    memset(this,0,sizeof(*this));
  }
};

static inline U32 SQR(U32 x) {
  return x*x;
}

#define DEFAULT_LEARNING_RATE 7

typedef enum {SIMD_NONE, SIMD_SSE2, SIMD_AVX2} SIMD;
class Mixer : protected IPredictor {
protected:
  const Shared * const shared;
  const U32 N, M, S;     // max inputs, max contexts, max context sets
  int scalefactor;       // scale factor for dot product
  Array<short, 32> tx;   // N inputs from add()
  Array<short, 32> wx;   // N*M weights
  Array<U32> cxt;        // S contexts
  Array<ErrorInfo> info; // stats for the adaptive learning rates
  Array<int> rates;      // learning rates
  U32 ncxt;              // number of contexts (0 to S)
  U32 base;              // offset of next context
  U32 nx;                // number of inputs in tx, 0 to N
  Array<int> pr;         // last result (scaled 12 bits)
public:
  Mixer(const Shared * const sh, const int n, const int m, const int s):
    shared(sh), N(n), M(m), S(s), scalefactor(0), tx(N), wx(N*M), cxt(S), info(S), rates(S), pr(S)
  {
    for (U64 i=0; i<S; ++i) {
      pr[i]=2048; //initial p=0.5
      rates[i] = DEFAULT_LEARNING_RATE;
      info[i].reset();
    }
    reset();
  }
  virtual ~Mixer() {};
  virtual int p() = 0;
  virtual void set_scalefactor(const int sf0, const int sf1) = 0;

  // Input x (call up to N times)
  void add(const int x) {
    assert(nx<N);
    assert(x==short(x));
    tx[nx++]=(short)x;
  }

  // Set a context (call S times, sum of ranges <= M)
  void set(const U32 cx, const U32 range, const int rate = DEFAULT_LEARNING_RATE) {
    assert(ncxt<S);
    assert(cx<range);
    assert(base+range<=M);
    if (!(options&OPTION_ADAPTIVE))
      rates[ncxt] = rate;
    cxt[ncxt++]=base+cx;
    base+=range;
    //printf("ncxt: %d base: %d\n",ncxt,range); //for debugging: how many input sets do we have?
  }

  void reset() {
    nx=base=ncxt=0;
  }
};

// for training NormalModel and ExeModel
class DummyMixer : public Mixer {
public:
  DummyMixer(const Shared * const sh, const int n, const int m, const int s):Mixer(sh, n, m, s){}
  void update() override {reset();}
  int p() override {
    updater.subscribe(this);
    return 2048;
  }
  void set_scalefactor(const int, const int) override {};
};

template <SIMD simd> class SIMDMixer: public Mixer {
private:
  //define padding requirements
  constexpr inline int simd_width() const {
    if(simd==SIMD_AVX2)return 32/sizeof(short); //256 bit (32 byte) data size
    if(simd==SIMD_SSE2)return 16/sizeof(short); //128 bit (16 byte) data size
    if(simd==SIMD_NONE)return  4/sizeof(short); //processes 2 shorts at once -> width is 4 bytes
    assert(false);
  }
  SIMDMixer *mp; // points to a Mixer to combine results
public:
  SIMDMixer(const Shared *sh, const int n, const int m, const int s) :
    Mixer(sh, ((n+(simd_width()-1))&-(simd_width())), m, s)
  {
    assert(n>0 && N>0 && (N&(simd_width()-1))==0 && M>0 && S>=1);
    mp = (S>1) ? new SIMDMixer<simd>(sh, S, 1, 1) : nullptr;
  }

  ~SIMDMixer() {
    delete mp;
  }

  void set_scalefactor(const int sf0, const int sf1) override {
    scalefactor=sf0;
    if(mp){mp->set_scalefactor(sf1,0);}
  }

  // Adjust weights to minimize coding cost of last prediction
  void update() override {
    INJECT_SHARED_y
    const int target=y<<12;
    if(nx>0)
    for (U64 i=0; i<ncxt; ++i) {
      const int err=target-pr[i];
      if(simd==SIMD_NONE)
        train_simd_none(&tx[0], &wx[cxt[i]*N], nx, err*rates[i]);
      if(simd==SIMD_SSE2)
        train_simd_sse2(&tx[0], &wx[cxt[i]*N], nx, err*rates[i]);
      if(simd==SIMD_AVX2)
        train_simd_avx2(&tx[0], &wx[cxt[i]*N], nx, err*rates[i]);
      if (options&OPTION_ADAPTIVE){
        const U32 logErr=min(0xF,ilog2(abs(err)));
        info[i].Sum-=SQR(info[i].Data[1]>>28);
        info[i].Data[1]<<=4; info[i].Data[1]|=info[i].Data[0]>>28;
        info[i].Data[0]<<=4; info[i].Data[0]|=logErr;
        info[i].Sum+=SQR(logErr);
        info[i].Collected+=info[i].Collected<4096;
        info[i].Mask<<=1; info[i].Mask|=(logErr<=((info[i].Data[0]>>4)&0xF));
        const U32 count=BitCount(info[i].Mask);
        if (info[i].Collected>=64 && (info[i].Sum>1500+U32(rates[i])*64 || count<9 || (info[i].Mask&0xFF)==0)){
          rates[i]=DEFAULT_LEARNING_RATE;
          memset(&info[i], 0, sizeof(ErrorInfo));
        }
        else if (info[i].Collected==4096 && info[i].Sum>=56 && info[i].Sum<=144 && count>28-U32(rates[i]) && ((info[i].Mask&0xFF)==0xFF)){
          rates[i]-=rates[i]>2;
          info[i].reset();
        }
      }
    }
    reset();
  }

  // predict next bit
  int p() {
    updater.subscribe(this);
    assert(scalefactor>0);
    //if(mp)printf("nx: %d, ncxt: %d, base: %d\n",nx, ncxt, base); //for debugging: how many inputs do we have?
    while (nx&(simd_width()-1)) tx[nx++]=0;  // pad
    if (mp) {  // combine outputs
      for (U64 i=0; i<ncxt; ++i) {
        int dp;
        if(simd==SIMD_NONE)
          dp=dot_product_simd_none(&tx[0], &wx[cxt[i]*N], nx);
        if(simd==SIMD_SSE2)
          dp=dot_product_simd_sse2(&tx[0], &wx[cxt[i]*N], nx);
        if(simd==SIMD_AVX2)
          dp=dot_product_simd_avx2(&tx[0], &wx[cxt[i]*N], nx);
        dp=(dp*scalefactor)>>16;
        if(dp<-2047)dp=-2047;else if(dp>2047)dp=2047;
        mp->add(dp);
        pr[i]=squash(dp);
      }
      mp->set(0, 1);
      return mp->p();
    }
    else {  // S=1 context
      int dp;
      if(simd==SIMD_NONE)
        dp=dot_product_simd_none(&tx[0], &wx[cxt[0]*N], nx);
      if(simd==SIMD_SSE2)
        dp=dot_product_simd_sse2(&tx[0], &wx[cxt[0]*N], nx);
      if(simd==SIMD_AVX2)
        dp=dot_product_simd_avx2(&tx[0], &wx[cxt[0]*N], nx);
      dp=(dp*scalefactor)>>16;
      return pr[0]=squash(dp);
    }
  }
};

static SIMD chosen_simd=SIMD_NONE; //default value, will be overriden by the CPU dispatcher, and may be overriden from the command line
class MixerFactory {
public:
  static void set_simd(SIMD simd) {
    chosen_simd=simd;
  }
  static Mixer* CreateMixer(const Shared * const sh, const int n, const int m, const int s) {
    if(chosen_simd==SIMD_NONE)return new SIMDMixer<SIMD_NONE>(sh, n, m, s);
    if(chosen_simd==SIMD_SSE2)return new SIMDMixer<SIMD_SSE2>(sh, n, m, s);
    if(chosen_simd==SIMD_AVX2)return new SIMDMixer<SIMD_AVX2>(sh, n, m, s);
    assert(false);
    return nullptr;
  }
};

//////////////////////////// APM1 //////////////////////////////

// APM1 maps a probability and a context into a new probability
// that bit y will next be 1.  After each guess it updates
// its state to improve future guesses.  Methods:
//
// APM1 a(N) creates with N contexts, uses 66*N bytes memory.
// a.p(pr, cx, rate=7) returned adjusted probability in context cx (0 to
//   N-1).  rate determines the learning rate (smaller = faster, default 7).
//   Probabilities are scaled 12 bits (0-4095).

class APM1 : IPredictor {
private:
  const Shared * const shared;
  int index;     // last p, context
  const int N;   // number of contexts
  Array<U16> t;  // [N][33]:  p, context -> p
  const int rate;
public:
  
  APM1(const Shared * const sh, const int n, const int r): shared(sh), 
    index(0), N(n), t(n*33), rate(r)
  {
    assert(n>0 && rate>0 && rate<32);
    // maps p, cxt -> p initially
    for (int i=0; i<N; ++i)
      for (int j=0; j<33; ++j)
        t[i*33+j] = i==0 ? squash((j-16)*128)*16 : t[j];
  }
  void update() override {
    INJECT_SHARED_y
    const int g=(y<<16)+(y<<rate)-y-y;
    t[index] += (g-t[index]) >> rate;
    t[index+1] += (g-t[index+1]) >> rate;
  }
  int p(int pr, const int cxt) {
    updater.subscribe(this);
    assert(pr>=0 && pr<4096 && cxt>=0 && cxt<N);
    pr = stretch(pr);
    const int w=pr&127;  // interpolation weight (33 points)
    index=((pr+2048)>>7)+cxt*33;
    return (t[index]*(128-w)+t[index+1]*w) >> 11;
  }
};


//////////////////////////// StateMap, APM //////////////////////////

// A StateMap maps a context to a probability.  Methods:
//
// Statemap sm(n) creates a StateMap with n contexts using 4*n bytes memory.
// sm.p(y, cx, limit) converts state cx (0..n-1) to a probability (0..4095).
//     that the next y=1, updating the previous prediction with y (0..1).
//     limit (1..1023, default 1023) is the maximum count for computing a
//     prediction.  Larger values are better for stationary sources.

// This class provides a static (common) 1024-element lookup table for integer division
// Initialization will run multiple times, but the table is created only once
class DivisionTable {
public:
  static int* getdt() {
    static int dt[1024];  // i -> 16K/(i+i+3)
    for (int i = 0; i<1024; ++i)
      dt[i] = 16384 / (i + i + 3); 
    return dt;
  }
};

// This is the base class for StateMap and APM
// Purpose: common members are here
class AdaptiveMap : protected IPredictor {
protected:
  const Shared * const shared;
  Array<U32> t; // cxt -> prediction in high 22 bits, count in low 10 bits
  int limit;
  int* dt;      // Pointer to division table
  AdaptiveMap(const Shared * const sh, const int n, const int lim) : shared(sh), t(n), limit(lim) {
    dt=DivisionTable::getdt();
  }
  virtual ~AdaptiveMap() {};
  void update(U32 * const p) {
    U32 p0=p[0];
    const int n=p0&1023;  //count
    const int pr=p0>>10;  //prediction (22-bit fractional part)
    if (n<limit) ++p0;
    else p0=(p0&0xfffffc00)|limit;
    INJECT_SHARED_y
    const int target=y<<22; //(22-bit fractional part)
    const int delta=((target-pr)>>3)*dt[n]; //the larger the count (n) the less it should adapt pr+=(target-pr)/(n+1.5)
    p0+=delta&0xfffffc00;
    p[0]=p0;
  }
public:
  void setlimit(const int lim){limit=lim;};
};

class StateMap : public AdaptiveMap {
protected:
  const U32 S;    // Number of context sets
  const U32 N;    // Number of contexts in each context set
  U32 ncxt;       // Number of context indexes present in cxt (0..S-1)
  Array<U32> cxt; // Context index of last prediction per context set
public:
  StateMap(const Shared * const sh, const int s, const int n, const int lim, const bool init): 
    AdaptiveMap(sh, n*s,lim), S(s), N(n), ncxt(0), cxt(s)
  {
    assert(S>0 && N>0);
    assert(limit>0 && limit<1024);
    if (init) { // when the context is a bit history byte, we have a-priory for p
      assert(N==256);
      for (U32 cx=0; cx<N; ++cx) {
        U32 n0=StateTable::next(cx,2);
        U32 n1=StateTable::next(cx,3);
        if (n0==0) n1*=64;
        if (n1==0) n0*=64;
        n0=n0*2+1;
        n1=n1*2+1;
        for (U32 s=0; s<S; ++s)
          t[s*N+cx] = ((n1<<20)/(n0+n1))<<12;
      }
    }
    else {
      for (U32 i=0; i<N*S; ++i)
        t[i]=(1u<<31)+0;  //initial p=0.5, initial count=0
    }
  }
  void Reset(const int Rate) {
    for (U32 i=0; i<N*S; ++i)
      t[i]=(t[i]&0xfffffc00)|min(Rate, t[i]&0x3FF);
  }
  void update() override {
    assert(ncxt<=S);
    while(ncxt>0) {
      ncxt--;
      const U32 idx=cxt[ncxt];
      if(idx+1==0)continue; //skipped context
      assert(ncxt*N<=idx && idx<(ncxt+1)*N);
      AdaptiveMap::update(&t[idx]);
    }
  }
  int p(const U32 s, const U32 cx) {
    if(ncxt==0)updater.subscribe(this);
    assert(s>=0 && s<S);
    assert(cx>=0 && cx<N);
    assert(s==ncxt);
    const U32 idx=ncxt*N+cx;
    cxt[ncxt]=idx;
    ncxt++;
    return t[idx]>>20;
  }
  void skip(const U32 s) {
    if(ncxt==0)updater.subscribe(this);
    assert(s>=0 && s<S);
    assert(s==ncxt);
    cxt[ncxt]=0-1; //mark for skipping
    ncxt++;
  }
};


// An APM maps a probability and a context to a new probability.  Methods:
//
// APM a(N,STEPS) creates with N contexts using 4*STEPS*N bytes memory.
// a.update() updates probability map. y=(0..1) is the last bit
// a.p(pr, cx, limit) returns a new probability (0..4095) like with StateMap. 
//     cx=(0..n-1) is the context.
//     pr=(0..4095) is considered part of the context.
//     The output is computed by interpolating pr into STEPS ranges nonlinearly
//     with smaller ranges near the ends.  The initial output is pr.
//     limit=(0..1023): set a lower limit (like 255) for faster adaptation.

class APM : public AdaptiveMap {
private:
  const int N;  // Number of contexts
  const int steps;
  int cxt;      // Context index of last prediction
public:
  APM(const Shared * const sh, const int n, const int s) :  AdaptiveMap(sh,n*s,1023), N(n*s), steps(s), cxt(0)  {
    assert(s>4); // number of steps - must be a positive integer bigger than 4
    for (int i=0; i<N; ++i) {
      int p = ((i%steps*2+1)*4096)/(steps*2)-2048;
      t[i] = (U32(squash(p))<<20)+6; //initial count: 6
    }
  }
  void update() override {
    assert(cxt>=0 && cxt<N);
    AdaptiveMap::update(&t[cxt]);
  }
  int p(int pr, int cx, const int lim) {
    updater.subscribe(this);
    assert(pr>=0 && pr<4096);
    assert(cx>=0 && cx<N/steps);
    assert(limit>0 && limit<1024);
    AdaptiveMap::setlimit(lim);
    pr = (stretch(pr)+2048)*(steps-1);
    int wt = pr&0xfff;  // interpolation weight (0..4095)
    cx = cx*steps+(pr>>12);
    assert(cx>=0 && cx<N-1);
    cxt = cx+(wt>>11);
    pr = ((t[cx]>>13)*(4096-wt)+(t[cx+1]>>13)*wt)>>19;
    return pr;
  }
};


//////////////////////// Hash functions //////////////////////////
//
// All hash functions are constructed using multiplicative hashes
// - We usually hash small values
// - Multiplicative hashes promote entropy to the higher bits 
// - When combining ( H(x) + H(y) ) entropy is still in higher bits
// - After combining they must be finalized by taking the higher
//   bits only to reduce the range to the desired hash table size

// Multipliers
// - They don't need to be prime, just large odd numbers
// - The golden ratios are usually preferred as multipliers

// Golden ratio of 2^64 (not a prime)
#define PHI64 UINT64_C(0x9E3779B97F4A7C15) // 11400714819323198485

// Some more arbitrary magic (prime) numbers
#define MUL64_1  UINT64_C(0x993DDEFFB1462949)
#define MUL64_2  UINT64_C(0xE9C91DC159AB0D2D)
#define MUL64_3  UINT64_C(0x83D6A14F1B0CED73)
#define MUL64_4  UINT64_C(0xA14F1B0CED5A841F)
#define MUL64_5  UINT64_C(0xC0E51314A614F4EF) 
#define MUL64_6  UINT64_C(0xDA9CC2600AE45A27)
#define MUL64_7  UINT64_C(0x826797AA04A65737)
#define MUL64_8  UINT64_C(0x2375BE54C41A08ED)
#define MUL64_9  UINT64_C(0xD39104E950564B37)
#define MUL64_10 UINT64_C(0x3091697D5E685623)
#define MUL64_11 UINT64_C(0x20EB84EE04A3C7E1)
#define MUL64_12 UINT64_C(0xF501F1D0944B2383)
#define MUL64_13 UINT64_C(0xE3E4E8AA829AB9B5)

// Finalizers (range reduction)
// - Keep the necessary number of bits after performing a
//   (combination of) multiplicative hash(es)

static ALWAYS_INLINE 
U32 finalize64(const U64 hash, const int hashbits) {
  assert(U32(hashbits)<=32); // just a reasonable upper limit
  return U32(hash>>(64-hashbits));
}

// Get the next 8 or 16 bits following "hasbits" for checksum
static ALWAYS_INLINE 
U64 checksum64(const U64 hash, const int hashbits, const int checksumbits) {
  assert(0<checksumbits && U32(checksumbits)<=32); //32 is just a reasonable upper limit
  return (hash>>(64-hashbits-checksumbits))&((1<<checksumbits)-1); 
}

//
// Value hashing
//
// - Hash 1-13 64-bit (usually small) integers

static ALWAYS_INLINE 
U64 hash(const U64 x0) {
  return (x0+1)*PHI64;
}
static ALWAYS_INLINE 
U64 hash(const U64 x0, const U64 x1) {
  return (x0+1)*PHI64   + (x1+1)*MUL64_1;
}
static ALWAYS_INLINE 
U64 hash(const U64 x0, const U64 x1, const U64 x2) {
  return (x0+1)*PHI64   + (x1+1)*MUL64_1 + (x2+1)*MUL64_2;
}
static ALWAYS_INLINE 
U64 hash(const U64 x0, const U64 x1, const U64 x2, const U64 x3) {
  return (x0+1)*PHI64   + (x1+1)*MUL64_1 + (x2+1)*MUL64_2 +
         (x3+1)*MUL64_3;
}
static ALWAYS_INLINE 
U64 hash(const U64 x0, const U64 x1, const U64 x2, const U64 x3, const U64 x4) {
  return (x0+1)*PHI64   + (x1+1)*MUL64_1 + (x2+1)*MUL64_2 +
         (x3+1)*MUL64_3 + (x4+1)*MUL64_4;
}
static ALWAYS_INLINE 
U64 hash(const U64 x0, const U64 x1, const U64 x2, const U64 x3, const U64 x4,
         const U64 x5) {
  return (x0+1)*PHI64   + (x1+1)*MUL64_1 + (x2+1)*MUL64_2 +
         (x3+1)*MUL64_3 + (x4+1)*MUL64_4 + (x5+1)*MUL64_5;
}
static ALWAYS_INLINE 
U64 hash(const U64 x0, const U64 x1, const U64 x2, const U64 x3, const U64 x4,
         const U64 x5, const U64 x6) {
  return (x0+1)*PHI64   + (x1+1)*MUL64_1 + (x2+1)*MUL64_2 +
         (x3+1)*MUL64_3 + (x4+1)*MUL64_4 + (x5+1)*MUL64_5 +
         (x6+1)*MUL64_6;
}
static ALWAYS_INLINE 
U64 hash(const U64 x0, const U64 x1, const U64 x2, const U64 x3, const U64 x4,
         const U64 x5, const U64 x6, const U64 x7) {
  return (x0+1)*PHI64   + (x1+1)*MUL64_1 + (x2+1)*MUL64_2 +
         (x3+1)*MUL64_3 + (x4+1)*MUL64_4 + (x5+1)*MUL64_5 +
         (x6+1)*MUL64_6 + (x7+1)*MUL64_7;
}
static ALWAYS_INLINE 
U64 hash(const U64 x0, const U64 x1, const U64 x2, const U64 x3, const U64 x4,
         const U64 x5, const U64 x6, const U64 x7, const U64 x8) {
  return (x0+1)*PHI64   + (x1+1)*MUL64_1 + (x2+1)*MUL64_2 +
         (x3+1)*MUL64_3 + (x4+1)*MUL64_4 + (x5+1)*MUL64_5 +
         (x6+1)*MUL64_6 + (x7+1)*MUL64_7 + (x8+1)*MUL64_8;
}
static ALWAYS_INLINE 
U64 hash(const U64 x0, const U64 x1, const U64 x2, const U64 x3, const U64 x4,
         const U64 x5, const U64 x6, const U64 x7, const U64 x8, const U64 x9) {
  return (x0+1)*PHI64   + (x1+1)*MUL64_1 + (x2+1)*MUL64_2 +
         (x3+1)*MUL64_3 + (x4+1)*MUL64_4 + (x5+1)*MUL64_5 +
         (x6+1)*MUL64_6 + (x7+1)*MUL64_7 + (x8+1)*MUL64_8 +
         (x9+1)*MUL64_9;
}
static ALWAYS_INLINE 
U64 hash(const U64 x0, const U64 x1, const U64 x2, const U64 x3, const U64 x4,
         const U64 x5, const U64 x6, const U64 x7, const U64 x8, const U64 x9,
         const U64 x10) {
  return (x0+1)*PHI64   + (x1+1)*MUL64_1 + (x2+1)*MUL64_2 +
         (x3+1)*MUL64_3 + (x4+1)*MUL64_4 + (x5+1)*MUL64_5 +
         (x6+1)*MUL64_6 + (x7+1)*MUL64_7 + (x8+1)*MUL64_8 +
         (x9+1)*MUL64_9 + (x10+1)*MUL64_10;
}
static ALWAYS_INLINE 
U64 hash(const U64 x0, const U64 x1, const U64 x2, const U64 x3, const U64 x4,
         const U64 x5, const U64 x6, const U64 x7, const U64 x8, const U64 x9,
         const U64 x10,const U64 x11) {
  return (x0+1)*PHI64   + (x1+1)*MUL64_1 + (x2+1)*MUL64_2 +
         (x3+1)*MUL64_3 + (x4+1)*MUL64_4 + (x5+1)*MUL64_5 +
         (x6+1)*MUL64_6 + (x7+1)*MUL64_7 + (x8+1)*MUL64_8 +
         (x9+1)*MUL64_9 + (x10+1)*MUL64_10 + (x11+1)*MUL64_11;
}
static ALWAYS_INLINE 
U64 hash(const U64 x0, const U64 x1, const U64 x2, const U64 x3, const U64 x4,
         const U64 x5, const U64 x6, const U64 x7, const U64 x8, const U64 x9,
         const U64 x10,const U64 x11,const U64 x12) {
  return (x0+1)*PHI64   + (x1+1)*MUL64_1 + (x2+1)*MUL64_2 +
         (x3+1)*MUL64_3 + (x4+1)*MUL64_4 + (x5+1)*MUL64_5 +
         (x6+1)*MUL64_6 + (x7+1)*MUL64_7 + (x8+1)*MUL64_8 +
         (x9+1)*MUL64_9 + (x10+1)*MUL64_10 + (x11+1)*MUL64_11 + (x12+1)*MUL64_12;
}

//
// String hashing
//

// Call this function repeatedly for string hashing, or to combine 
// a hash value and a (non-hash) value, or two hash values
static ALWAYS_INLINE 
U64 combine64(const U64 seed, const U64 x) {
  return hash(seed+x);
}

///////////////////////////// BH ////////////////////////////////

// A BH maps a 32 bit hash to an array of B bytes (checksum and B-2 values)
//
// BH bh(N); creates N element table with B bytes each.
//   N must be a power of 2.  The first byte of each element is
//   reserved for a checksum to detect collisions.  The remaining
//   B-1 bytes are values, prioritized by the first value.  This
//   byte is 0 to mark an unused element.
//
// bh[i] returns a pointer to the i'th element, such that
//   bh[i][0] is a checksum of i, bh[i][1] is the priority, and
//   bh[i][2..B-1] are other values (0-255).
//
// The index must be a multiplicative hash.
//
// If a collision is detected, up to M nearby locations in the same
// cache line are tested and the first matching checksum or
// empty element is returned.
//
// Collision detection and resolution policy:
// 2 byte checksum with LRU replacement
// If neither a match nor an empty element is found the lowest priority
// element is replaced by the new element (except last 2 by priority).

template <U64 B> class BH {
  enum {M=8};  // search limit
  Array<U8> t; // elements
  const U32 mask; // size-1
  const int hashbits;
public:
  BH(U64 i): t(i*B), mask(U32(i-1)), hashbits(ilog2(mask+1)) {
    assert(B>=2 && i>0 && ispowerof2(i));
  }
  U8* operator[](const U64 i);
};

template <U64 B>
inline U8* BH<B>::operator[](const U64 ctx) {
  const U16 chk=(U16)checksum64(ctx,hashbits,16);
  const U32 i=finalize64(ctx,hashbits)*M & mask;
  U8 *p;
  U16 *cp;
  int j;
  for (j=0; j<M; ++j) {
    p=&t[(i+j)*B];
    cp=(U16*)p;
    if (p[2]==0) {*cp=chk;break;} // empty slot
    if (*cp==chk) break;  // found
  }
  if (j==0) return p+1;  // front
  static U8 tmp[B];  // element to move to front
  if (j==M) {
    --j;
    memset(tmp, 0, B);
    memmove(tmp, &chk, 2);
    if (M>2 && t[(i+j)*B+2]>t[(i+j-1)*B+2]) --j;
  }
  else memcpy(tmp, cp, B);
  memmove(&t[(i+1)*B], &t[i*B], j*B);
  memcpy(&t[i*B], tmp, B);
  return &t[i*B+1];
}

//////////////////////////// HashTable /////////////////////////

// A HashTable is an array of n items representing n contexts.
// Each item is a storage area of B bytes. In every slot the first
// byte is a checksum using the upper 8 bits of the context
// selector. The second byte is a priority (0 = empty) for hash
// replacement. Priorities must be set by the caller (0: lowest,
// 255: highest). Items with lower priorities will be replaced in
// case of a collision. Only 2 additional items are probed for the
// given checksum.
// The caller can store any information about the given context
// in bytes [2..B-1].
// Checksums must not be modified by the caller.

// HashTable<B> h(n) - creates a hashtable with n slots where
//     n and B must be powers of 2 with n >= B*4, and B >= 2.
// h[i] returns a pointer to the storage area starting from
//     position 1 (e.g. without the checksum)
// The index must be a multiplicative hash.

template <int B>
class HashTable {
private:
  Array<U8,64> t;  // storage area for n items (1 item = B bytes): 0:checksum 1:priority 2:data 3:data  ... B-1:data
  const int mask;
  const int hashbits;
public:
  HashTable(U64 n): t(n), mask((int)n-1), hashbits(ilog2(mask+1))  {
    assert(B>=2   && ispowerof2(B));
    assert(n>=B*4 && ispowerof2(n));
    assert(n<(UINT64_C(1)<<31));
  }
  U8* operator[](U64 i);
};

template <int B>
inline U8* HashTable<B>::operator[](U64 i) { //i: context selector
  U8 chk=(U8)checksum64(i,hashbits,8); // 8-bit checksum
  i=finalize64(i,hashbits)*B & mask;    // index: force bounds
  //search for the checksum in t
  U8 *p = &t[0];
  if (p[i]==chk) return p+i+1;
  if (p[i^B]==chk) return p+(i^B)+1;
  if (p[i^(B*2)]==chk) return p+(i^(B*2))+1;
  //not found, let's overwrite the lowest priority element
  if (p[i+1]>p[(i+1)^B] || p[i+1]>p[(i+1)^(B*2)]) i^=B;
  if (p[i+1]>p[(i+1)^B^(B*2)]) i^=B^(B*2);
  memset(p+i, 0, B);
  p[i]=chk;
  return p+i+1;
}

/////////////////////////// ContextMap /////////////////////////
//
// A ContextMap maps contexts to bit histories and makes predictions
// to a Mixer.  Methods common to all classes:
//
// ContextMap cm(M, C); creates using about M bytes of memory (a power
//   of 2) for C contexts.
// cm.set(cx);  sets the next context to cx, called up to C times
//   cx is an arbitrary 32 bit value that identifies the context.
//   It should be called before predicting the first bit of each byte.
// cm.mix(m) updates Mixer m with the next prediction.  Returns 1
//   if context cx is found, else 0.  Then it extends all the contexts with
//   global bit y.  It should be called for every bit:
//
//     if (bpos==0)
//       for (int i=0; i<C; ++i) cm.set(cxt[i]);
//     cm.mix(m);
//
// The different types are as follows:
//
// - RunContextMap.  The bit history is a count of 0-255 consecutive
//     zeros or ones.  Uses 4 bytes per whole byte context.  C=1.
//     The context must be a multiplicative hash.
// - SmallStationaryContextMap.  0 <= cx*256 < M.
//     The state is a 16-bit probability that is adjusted after each
//     prediction.  C=1.
// - ContextMap.  For large contexts, C >= 1.  Context need not be hashed.

// A RunContextMap maps a context into the next byte and a repeat
// count up to M.  Size should be a power of 2.  Memory usage is 3M/4.
class RunContextMap : IPredictor {
public:
  static constexpr int MIXERINPUTS = 1;
private:
  const Shared * const shared;
  BH<4> t;
  U8* cp;
public:
  RunContextMap(const Shared * const sh, U64 m): shared(sh), t(m/4) {assert(ispowerof2(m));cp=t[0]+1;}
  void update() override { 
    INJECT_SHARED_c1
    if (cp[0]==0 || cp[1]!=c1) cp[0]=1, cp[1]=c1;
    else if (cp[0]<255) ++cp[0];
  }
  void set(U64 cx) {
    cp=t[cx]+1;
  }
  void mix(Mixer& m) {
    INJECT_SHARED_bpos
    if(bpos==7)updater.subscribe(this);
    INJECT_SHARED_c0
    if (cp[0]!=0 && (cp[1]+256)>>(8-bpos)==c0) {
      int sign=(cp[1]>>(7-bpos)&1)*2-1;
      m.add(sign*(ilog(cp[0]+1)<<3));
    }
    else
      m.add(0); //p=0.5
  }
};

/*
Map for modelling contexts of (nearly-)stationary data.
The context is looked up directly. For each bit modelled, a 16bit prediction is stored.
The adaptation rate is controlled by the caller, see mix().

- BitsOfContext: How many bits to use for each context. Higher bits are discarded.
- InputBits: How many bits [1..8] of input are to be modelled for each context.
New contexts must be set at those intervals.

Uses (2^(BitsOfContext+1))*((2^InputBits)-1) bytes of memory.
*/

class SmallStationaryContextMap : IPredictor {
public:
  static constexpr int MIXERINPUTS = 2;
private:
  const Shared * const shared;
  Array<U16> Data;
  const U32 mask, stride, bTotal;
  U32 Context, bCount, B;
  U16 *cp;
  const int rate;
  int scale;
public:
  SmallStationaryContextMap(const Shared * const sh, const int BitsOfContext, 
    const int InputBits, const int Rate, const int Scale) : shared(sh), 
    Data((UINT64_C(1)<<BitsOfContext)*((UINT64_C(1)<<InputBits)-1)), 
    mask((1<<BitsOfContext)-1), stride((1<<InputBits)-1), bTotal(InputBits), 
    rate(Rate), scale(Scale)
  {
    assert(InputBits>0 && InputBits<=8);
    Reset();
    set(0);
  }
  void set(U32 ctx) {
    Context = (ctx&mask)*stride;
    bCount=B=0;
  }
  void Reset() {
    for (U32 i=0; i<Data.size(); ++i)
      Data[i]=0x7FFF;
    cp=&Data[0];
  }
  void update() override {
    INJECT_SHARED_y
    *cp+=((y<<16)-(*cp)+(1<<(rate-1)))>>rate;
    B+=(y && B>0);
  }
  void setscale(const int Scale){scale=Scale;}
  void mix(Mixer& m) {
    updater.subscribe(this);
    cp = &Data[Context+B];
    const int Prediction = (*cp)>>4;
    m.add((stretch(Prediction)*scale)>>8);
    m.add(((Prediction-2048)*scale)>>9);
    bCount++; B+=B+1;
    assert(bCount<=bTotal);
  }
};

/*
  Map for modelling contexts of (nearly-)stationary data.
  The context is looked up directly. For each bit modelled, a 32bit element stores
  a 22 bit prediction and a 10 bit adaptation rate offset.

  - BitsOfContext: How many bits to use for each context. Higher bits are discarded.
  - InputBits: How many bits [1..8] of input are to be modelled for each context.
    New contexts must be set at those intervals.
  - Rate: Initial adaptation rate offset [0..1023]. Lower offsets mean faster adaptation.
    Will be increased on every occurrence until the higher bound is reached.

    Uses (2^(BitsOfContext+2))*((2^InputBits)-1) bytes of memory.
*/

class StationaryMap : IPredictor {
public:
  static constexpr int MIXERINPUTS = 2;
private:
  const Shared * const shared;
  Array<U32> Data;
  const U32 mask, maskbits, stride, bTotal;
  U32 Context, bCount,  B;
  U32 *cp;
  int scale;
  const U16 limit;
  int* dt;
public:
  StationaryMap(const Shared * const sh, const int BitsOfContext, const int InputBits, 
                const int Scale, const U16 Limit) : shared(sh), 
  Data((UINT64_C(1)<<BitsOfContext)*((UINT64_C(1)<<InputBits)-1)), 
  mask((1<<BitsOfContext)-1), maskbits(BitsOfContext), stride((1<<InputBits)-1), bTotal(InputBits),
  scale(Scale), limit(Limit)
 {
    assert(InputBits>0 && InputBits<=8);
    assert(BitsOfContext+InputBits<=24);
    dt=DivisionTable::getdt();
    Reset(0);
    set(0);
  }
  void set_direct(U32 ctx) { // ctx must be a direct context (no hash)
    Context = (ctx&mask)*stride;
    bCount=B=0;
  }
  void set(U64 ctx) { // ctx must be a hash
    Context = (finalize64(ctx,maskbits)&mask)*stride;
    bCount=B=0;
  }
  void Reset(const int Rate){
    for (U32 i=0; i<Data.size(); ++i)
      Data[i]=(0x7FF<<20)|min(1023,Rate);
    cp=&Data[0];
  }
  void update() override {
    U32 Count = min(min(limit,0x3FF), ((*cp)&0x3FF)+1);
    INJECT_SHARED_y
    int Prediction = (*cp)>>10, Error = (y<<22)-Prediction;
    Error = ((Error/8)*dt[Count])/1024;
    Prediction = min(0x3FFFFF,max(0,Prediction+Error));
    *cp = (Prediction<<10)|Count;
    B+=(y && B>0);
  }
  void setscale(const int Scale){scale=Scale;}
  void mix(Mixer& m) {
    updater.subscribe(this);
    cp=&Data[Context+B];
    int Prediction = (*cp)>>20;
    m.add((stretch(Prediction)*scale)>>8);
    m.add(((Prediction-2048)*scale)>>9);
    bCount++; B+=B+1;
    assert(bCount<=bTotal);
  }
};


/////////////////////////// IndirectMap /////////////////////////

class IndirectMap : IPredictor {
public:
  static constexpr int MIXERINPUTS = 2;
private:
  const Shared * const shared;
  Random rnd;
  Array<U8> Data;
  StateMap sm;
  const U32 mask, maskbits, stride, bTotal;
  U32 Context, bCount, B;
  U8 *cp;
  int scale;
public:
  IndirectMap(const Shared * const sh, const int BitsOfContext, const int InputBits, const int Scale, const int Limit): 
    shared(sh), Data((UINT64_C(1)<<BitsOfContext)*((UINT64_C(1)<<InputBits)-1)), 
    sm {sh,1,256,1023,true}, /* StateMap : s, n, lim, init */
    mask((1<<BitsOfContext)-1), maskbits(BitsOfContext), stride((1<<InputBits)-1), bTotal(InputBits),
    scale(Scale)
  {
    assert(InputBits>0 && InputBits<=8);
    assert(BitsOfContext+InputBits<=24);
    cp=&Data[0];
    set_direct(0);
    sm.setlimit(Limit);
  }
  void set_direct(const U32 ctx) {
    Context = (ctx&mask)*stride;
    bCount=B=0;
  }
  void set(const U64 ctx) {
    Context = (finalize64(ctx,maskbits))*stride;
    bCount=B=0;
  }
  void update() override {
    INJECT_SHARED_y
    StateTable::update(cp, y, rnd);
    B+=(y && B>0);
  }
  void setscale(const int Scale){scale=Scale;}
  void mix(Mixer& m) {
    updater.subscribe(this);
    cp=&Data[Context+B];
    const U8 state = *cp;
    const int p1 = sm.p(0,state);
    m.add((stretch(p1)*scale)>>8);
    m.add(((p1-2048)*scale)>>9);
    bCount++; B+=B+1;
    assert(bCount<=bTotal);
  }
};


// Context map for large contexts.  Most modeling uses this type of context
// map.  It includes a built in RunContextMap to predict the last byte seen
// in the same context, and also bit-level contexts that map to a bit
// history state.
//
// Bit histories are stored in a hash table.  The table is organized into
// 64-byte buckets aligned on cache page boundaries.  Each bucket contains
// a hash chain of 7 elements, plus a 2 element queue (packed into 1 byte)
// of the last 2 elements accessed for LRU replacement.  Each element has
// a 2 byte checksum for detecting collisions, and an array of 7 bit history
// states indexed by the last 0 to 2 bits of context.  The buckets are indexed
// by a context ending after 0, 2, or 5 bits of the current byte.  Thus, each
// byte modeled results in 3 main memory accesses per context, with all other
// accesses to cache.
//
// On bits 0, 2 and 5, the context is updated and a new bucket is selected.
// The most recently accessed element is tried first, by comparing the
// 16 bit checksum, then the 7 elements are searched linearly.  If no match
// is found, then the element with the lowest priority among the 5 elements
// not in the LRU queue is replaced.  After a replacement, the queue is
// emptied (so that consecutive misses favor a LFU replacement policy).
// In all cases, the found/replaced element is put in the front of the queue.
//
// The priority is the state number of the first element (the one with 0
// additional bits of context).  The states are sorted by increasing n0+n1
// (number of bits seen), implementing a LFU replacement policy.
//
// When the context ends on a byte boundary (bit 0), only 3 of the 7 bit
// history states are used.  The remaining 4 bytes implement a run model
// as follows: <count:7,d:1> <b1> <unused> <unused> where <b1> is the last byte
// seen, possibly repeated.  <count:7,d:1> is a 7 bit count and a 1 bit
// flag (represented by count * 2 + d).  If d=0 then <count> = 1..127 is the
// number of repeats of <b1> and no other bytes have been seen.  If d is 1 then
// other byte values have been seen in this context prior to the last <count>
// copies of <b1>.
//
// As an optimization, the last two hash elements of each byte (representing
// contexts with 2-7 bits) are not updated until a context is seen for
// a second time.  This is indicated by <count,d> = <1,0> (2).  After update,
// <count,d> is updated to <2,0> or <1,1> (4 or 3).

class ContextMap : IPredictor {
public:
  static constexpr int MIXERINPUTS = 5;
private:
  const Shared * const shared;
  Random rnd;
  const int C;  // max number of contexts
  class E {  // hash element, 64 bytes
    U16 chk[7];  // byte context checksums
    U8 last;     // last 2 accesses (0-6) in low, high nibble
  public:
    U8 bh[7][7]; // byte context, 3-bit context -> bit history state
      // bh[][0] = 1st bit, bh[][1,2] = 2nd bit, bh[][3..6] = 3rd bit
      // bh[][0] is also a replacement priority, 0 = empty
    inline U8* get(const U16 chksum) { // Find element (0-6) matching checksum.
      // If not found, insert or replace lowest priority (not last).
      // Find or create hash element matching checksum ch
      if (chk[last&15]==chksum) return &bh[last&15][0];
      int b=0xffff, bi=0;
      for (int i=0; i<7; ++i) {
        if (chk[i]==chksum) {last=last<<4|i; return (U8*)&bh[i][0];}
        int pri=bh[i][0];
        if (pri<b && (last&15)!=i && last>>4!=i) {b=pri; bi=i;}
      }
      return last=0xf0|bi, chk[bi]=chksum, (U8*)memset(&bh[bi][0], 0, 7);
    }
  };
  Array<E,64> t;  // bit histories for bits 0-1, 2-4, 5-7
    // For 0-1, also contains a run count in bh[][4] and value in bh[][5]
    // and pending update count in bh[7]
  Array<U8*> cp;   // C pointers to current bit history
  Array<U8*> cp0;  // First element of 7 element array containing cp[i]
  Array<U32> cxt;  // C whole byte context hashes
  Array<U16> chk;  // C whole byte context checksums
  Array<U8*> runp; // C [0..3] = count, value, unused, unused
  StateMap sm;   // C maps of state -> p
  int cn;        // Next context to set by set()
  const U32 mask;
  const int hashbits;
  U64 validflags;
public:
  // Construct using m bytes of memory for c contexts                                                    
  ContextMap(const Shared * const sh, U64 m, const int c): shared(sh), C(c), t(m>>6), cp(c), cp0(c),
    cxt(c), chk(c), runp(c), sm(sh,c,256,1023,true), cn(0),
    mask(U32(t.size()-1)), hashbits(ilog2(mask+1)), validflags(0) {
    assert(m>=64 && ispowerof2(m));
    assert(sizeof(E)==64);
    assert(C<(int)sizeof(validflags)*8); // validflags is 64 bits
  }

  // set next whole byte context to cx
  inline void set(const U64 cx) {
    assert(cn>=0 && cn<C);
    const U32 ctx = cxt[cn] = finalize64(cx,hashbits);
    const U16 checksum = chk[cn] = (U16)checksum64(cx,hashbits,16);
    U8* base = cp0[cn]=cp[cn]=t[ctx&mask].get(checksum);
    runp[cn]=base+3;
    // Update pending bit histories for bits 2-7
    if (base[3]==2) {
      const int c=base[4]+256;
      U8 *p=t[(ctx+(c>>6))&mask].get(checksum);
      p[0]=1+((c>>5)&1);
      p[1+((c>>5)&1)]=1+((c>>4)&1);
      p[3+((c>>4)&3)]=1+((c>>3)&1);
      p=t[(ctx+(c>>3))&mask].get(checksum);
      p[0]=1+((c>>2)&1);
      p[1+((c>>2)&1)]=1+((c>>1)&1);
      p[3+((c>>1)&3)]=1+(c&1);
      base[6]=0;
    }
    cn++;
    validflags=(validflags<<1)+1;
  }

  inline void skip() {
    assert(cn>=0 && cn<C);
    cn++;
    validflags<<=1;
  }

  void update() override {
    INJECT_SHARED_bpos INJECT_SHARED_c0 INJECT_SHARED_c1 INJECT_SHARED_y
    for (int i=0; i<cn; ++i) {
      if(((validflags>>(cn-1-i))&1)!=0) {
        // Update bit history state byte
        if (cp[i]!=nullptr) {
          assert(cp[i]>=&t[0].bh[0][0] && cp[i]<=&t[t.size()-1].bh[6][6]);
          assert((uintptr_t(cp[i])&63)>=15);
          StateTable::update(cp[i],y,rnd);
        }

        // Update context pointers
        if (bpos>1 && runp[i][0]==0)
          cp[i]=nullptr;
        else {
          switch(bpos) {
            case 1: case 3: case 6: cp[i]=cp0[i]+1+(c0&1); break;
            case 4: case 7: cp[i]=cp0[i]+3+(c0&3); break;
            case 2: case 5: {
              const U16 checksum = chk[i];
              const U32 ctx = cxt[i];
              cp0[i]=cp[i]=t[(ctx+c0)&mask].get(checksum); break;
            }
            case 0:
            {
              // Update run count of previous context
              if (runp[i][0]==0)  // new context
                runp[i][0]=2, runp[i][1]=c1;
              else if (runp[i][1]!=c1)  // different byte in context
                runp[i][0]=1, runp[i][1]=c1;
              else if (runp[i][0]<254)  // same byte in context
                runp[i][0]+=2;
              else if (runp[i][0]==255)
                runp[i][0]=128;
              break;
            }
          }
        }
      }
    }
    if (bpos==0) {cn = 0; validflags = 0;}
  }

  void mix(Mixer& m) {
    updater.subscribe(this);
    INJECT_SHARED_bpos INJECT_SHARED_c0
    for (int i=0; i<cn; ++i) {
      if(((validflags>>(cn-1-i))&1)!=0) {
        // predict from last byte in context
        if ((runp[i][1]+256)>>(8-bpos)==c0) {
          int rc=runp[i][0];  // count*2, +1 if 2 different bytes seen
          int sign=(runp[i][1]>>(7-bpos)&1)*2-1;  // predicted bit + for 1, - for 0
          int c=ilog(rc+1)<<(2+(~rc&1));
          m.add(sign*c);
        }
        else
          m.add(0); //p=0.5

        // predict from bit context
        const int s = cp[i]!=nullptr ?  *cp[i] : 0;
        if (s==0) {  //skip context
          sm.skip(i);
          m.add(0);
          m.add(0);
          m.add(0);
          m.add(0);
        } else {
          const int p1= sm.p(i,s);
          const int st=stretch(p1)>>2;
          m.add(st);
          m.add((p1-2047)>>3);
          const int n0=-!StateTable::next(s,2);
          const int n1=-!StateTable::next(s,3);
          m.add(st*abs(n1-n0));
          const int p0=4095-p1;
          m.add(((p1&n0)-(p0&n1))>>4);
        }
      } else { //skipped context
        sm.skip(i);
        m.add(0);m.add(0);m.add(0);m.add(0);m.add(0);
      }
    }
  }
};


/*
Context map for large contexts (32bits).
Maps to a bit history state, a 3 MRU byte history, and 1 byte RunStats.

Bit and byte histories are stored in a hash table with 64 byte buckets.
The buckets are indexed by a context ending after 0, 2 or 5 bits of the
current byte. Thus, each byte modeled results in 3 main memory accesses
per context, with all other accesses to cache.

On a byte boundary (bit 0), only 3 of the 7 bit history states are used.
Of the remaining 4 bytes, 3 are then used to store the last bytes seen
in this context, 7 bits to store the length of consecutive occurrences of
the previously seen byte, and 1 bit to signal if more than 1 byte as been
seen in this context. The byte history is then combined with the bit history
states to provide additional states that are then mapped to predictions.
*/

class ContextMap2 : IPredictor {
public:
  static constexpr int MIXERINPUTS = 7;
private:
  const Shared * const shared;
  Random rnd;
  const U32 C; // max number of contexts
  class Bucket { // hash bucket, 64 bytes
    U16 Checksums[7]; // byte context checksums
    U8 MRU; // last 2 accesses (0-6) in low, high nibble
  public:
    U8 BitState[7][7]; // byte context, 3-bit context -> bit history state
                       // BitState[][0] = 1st bit, BitState[][1,2] = 2nd bit, BitState[][3..6] = 3rd bit
                       // BitState[][0] is also a replacement priority, 0 = empty
    inline U8* Find(U16 Checksum) { // Find or create hash element matching checksum.
                                    // If not found, insert or replace lowest priority (skipping 2 most recent).
      if (Checksums[MRU&15]==Checksum)
        return &BitState[MRU&15][0];
      int worst=0xFFFF, idx=0;
      for (int i=0; i<7; ++i) {
        if (Checksums[i]==Checksum) {
          MRU=MRU<<4|i;
          return (U8*)&BitState[i][0];
        }
        if (BitState[i][0]<worst && (MRU&15)!=i && MRU>>4!=i) {
          worst = BitState[i][0];
          idx=i;
        }
      }
      MRU = 0xF0|idx;
      Checksums[idx] = Checksum;
      return (U8*)memset(&BitState[idx][0], 0, 7);
    }
  };
  Array<Bucket, 64> Table; // bit histories for bits 0-1, 2-4, 5-7
                           // For 0-1, also contains run stats in BitState[][3] and byte history in BitState[][4..6]
  Array<U8*> BitState;     // C pointers to current bit history states
  Array<U8*> BitState0;    // First element of 7 element array containing BitState[i]
  Array<U8*> ByteHistory;  // C pointers to run stats plus byte history, 4 bytes, [RunStats,1..3]
  Array<U32> Contexts;     // C whole byte context hashes
  Array<U16> Chk;          // C whole byte context checksums
  StateMap Maps7b, Maps8b, Maps12b;
  U32 index; // Next context to set by set()
  const U32 mask;
  const int hashbits;
  int scale;
public:
  int order=0; // is set after mix()
  // Construct using Size bytes of memory for Count contexts
  ContextMap2(const Shared * const sh, const U64 Size, const U32 Count, const int Scale) : shared(sh), 
    C(Count), Table(Size>>6), BitState(Count), BitState0(Count), ByteHistory(Count), 
    Contexts(Count), Chk(Count),
    Maps7b(sh,Count,(1<<7),1023,false), Maps8b(sh,Count,(1<<8),1023,true), Maps12b(sh,Count,(1<<12),1023,false),
    index(0), mask(U32(Table.size()-1)), hashbits(ilog2(mask+1)), scale(Scale) {
    assert(Size>=64 && ispowerof2(Size));
    assert(sizeof(Bucket)==64);
    for (U32 i=0; i<C; i++) {
      BitState[i] = BitState0[i] = &Table[i].BitState[0][0];
      ByteHistory[i] = BitState[i]+3;
    }
  }
  inline void set(const U64 ctx) { // set next whole byte context to ctx
    assert(index>=0 && index<C);
    const U32 ctx0 = Contexts[index] = finalize64(ctx,hashbits);
    const U16 chk0 = Chk[index] = (U16)checksum64(ctx,hashbits,16);
    U8* base = BitState[index] = BitState0[index] = Table[ctx0&mask].Find(chk0);
    ByteHistory[index] = base+3;
    index++;
    // Update pending bit histories for bits 2-7
    if (base[3]==2) {
      const int c = base[4]+256;
      U8 *p = Table[(ctx0+(c>>6))&mask].Find(chk0);
      p[0] = 1+((c>>5)&1);
      p[1+((c>>5)&1)] = 1+((c>>4)&1);
      p[3+((c>>4)&3)] = 1+((c>>3)&1);
      p = Table[(ctx0+(c>>3))&mask].Find(chk0);
      p[0] = 1+((c>>2)&1);
      p[1+((c>>2)&1)] = 1+((c>>1)&1);
      p[3+((c>>1)&3)] = 1+(c&1);
      base[6] = 0;
    }
  }
  void update() override {
    INJECT_SHARED_bpos INJECT_SHARED_c0 INJECT_SHARED_c1 INJECT_SHARED_y
    for (U32 i=0; i<index; i++) {
      if (BitState[i])
        StateTable::update(BitState[i],y,rnd);

      if (bpos>1 && ByteHistory[i][0]==0)
        BitState[i] = nullptr;
      else {
        switch (bpos) {
          case 0: {
            // Update byte history of previous context
              if (ByteHistory[i][0]==0)  // new context
                ByteHistory[i][0]=2;
              else if (ByteHistory[i][1]!=c1)  // different byte in context
                ByteHistory[i][0]=1;
              else if (ByteHistory[i][0]<254)  // same byte in context
                ByteHistory[i][0]+=2;
              else if (ByteHistory[i][0]==255) // more than one byte seen, but long run of current byte, reset to single byte seen
                ByteHistory[i][0] = 128;
              // scroll byte candidates
              if (ByteHistory[i][1]!=c1 || ByteHistory[i][-3]<7 /*less than 3 bytes seen*/) {
                ByteHistory[i][3] = ByteHistory[i][2];
                ByteHistory[i][2] = ByteHistory[i][1];
                ByteHistory[i][1] = c1;
              }
              break;
          }
          case 2: case 5: {
            const U16 chk = Chk[i];
            const U32 ctx = Contexts[i];
            BitState[i] = BitState0[i] = Table[(ctx+c0)&mask].Find(chk);
            break;
          }
          case 1: case 3: case 6: BitState[i] = BitState0[i]+1+y; break;
          case 4: case 7: BitState[i] = BitState0[i]+3+(c0&3); break;
        }
      }
    }
    if (bpos==0) index = 0;
  }

  void setscale(const int Scale){scale=Scale;}
  void mix(Mixer& m) {
    updater.subscribe(this);
    INJECT_SHARED_bpos INJECT_SHARED_c0
    order = 0;
    for (U32 i=0; i<index; i++) {
      // predict from last byte in context
      const int RunStats = ByteHistory[i][0]; // count*2, +1 if 2 different bytes seen
      if(RunStats!=0) {
        if (((ByteHistory[i][1]+256)>>(8-bpos))==c0) { // 1st candidate matches
          int sign=(ByteHistory[i][1]>>(7-bpos)&1)*2-1;  // predicted bit + for 1, - for 0
          int value = ilog(RunStats+1)<<(3-(RunStats&1));
          m.add(sign*value);
        }
        else if ((RunStats&1)!=0) {
          if (((ByteHistory[i][2]+256)>>(8-bpos))==c0) // 2st candidate matches
            m.add((((ByteHistory[i][2]>>(7-bpos))&1)*2-1)*128);
          else if ((ByteHistory[i][-3])>=15 && ((ByteHistory[i][3]+256)>>(8-bpos))==c0) // 3rd candidate matches
            m.add((((ByteHistory[i][3]>>(7-bpos))&1)*2-1)*128);
          else
            m.add(0);
        }
        else 
          m.add(0);
      }
      else
        m.add(0);

      // predict from bit context
      const int state = BitState[i]!=nullptr ? *BitState[i] : 0;
      if (state==0) {
        Maps8b.skip(i);
        m.add(0);
        m.add(0);
        m.add(0);
        m.add(0);
      } else {
        const int p1 = Maps8b.p(i,state);
        const int st=(stretch(p1)*scale)>>8;
        const int n0=-!StateTable::next(state, 2);
        const int n1=-!StateTable::next(state, 3);
        m.add(st);
        m.add(((p1-2047)*scale)>>9);
        m.add(st*abs(n1-n0));
        const int p0=4095-p1;
        m.add((((p1&n0)-(p0&n1))*scale)>>10);
        order++;
      }

      const U8 bh_bits =
          (((ByteHistory[i][1]>>(7-bpos))&1)   )
        | (((ByteHistory[i][2]>>(7-bpos))&1)<<1)
        | (((ByteHistory[i][3]>>(7-bpos))&1)<<2);

      U8 bh_state=ByteHistory[i][-3];
      if     (bh_state>=7) bh_state=8|bh_bits; //we have seen 3 bytes (at least) 
      else if(bh_state>=3) bh_state=4|bh_bits; //we have seen 2 bytes
      else if(bh_state>=1) bh_state=2|bh_bits; //we have seen 1 byte only
                                               //else new context (bh_state=0)
      
      const U8 g=StateTable::group(state); //0..31
      m.add(stretch( Maps12b.p(i,(bh_state<<8)|(bpos<<5)|g) )>>2);
      m.add(stretch( Maps7b.p(i,(bh_state<<3)|bpos) )>>2);
    }
  }
};

///////////////// Ordinary Least Squares predictor /////////////////

template <typename F, typename T, const bool hasZeroMean = true>
class OLS {
  static constexpr F ftol = 1E-8;
  static constexpr F sub = F(int64_t(!hasZeroMean)<<(8*sizeof(T)-1));
private:
  int n, kmax, km, index;
  F lambda, nu;
  F *x, *w, *b;
  F **mCovariance, **mCholesky;
  int Factor() {
    // copy the matrix
    for (int i=0; i<n; i++)
      for (int j=0; j<n; j++)
        mCholesky[i][j] = mCovariance[i][j];

    for (int i=0; i<n; i++)
      mCholesky[i][i] += nu;
    for (int i=0; i<n; i++) {
      for (int j=0; j<i; j++) {
        F sum = mCholesky[i][j];
        for (int k=0; k<j; k++)
          sum -= (mCholesky[i][k] * mCholesky[j][k]);
        mCholesky[i][j] = sum / mCholesky[j][j];
      }
      F sum = mCholesky[i][i];
      for (int k=0; k<i; k++)
        sum -= (mCholesky[i][k] * mCholesky[i][k]);
      if (sum>ftol)
        mCholesky[i][i] = sqrt(sum);
      else
        return 1;
    }
    return 0;
  }
  void Solve() {
    for (int i=0; i<n; i++) {
      F sum = b[i];
      for (int j=0; j<i; j++)
        sum -= (mCholesky[i][j] * w[j]);
      w[i] = sum / mCholesky[i][i];
    }
    for (int i=n-1; i>=0; i--) {
      F sum = w[i];
      for (int j=i+1; j<n; j++)
        sum -= (mCholesky[j][i] * w[j]);
      w[i] = sum / mCholesky[i][i];
    }
  }
public:
  OLS(int n, int kmax=1, F lambda=0.998, F nu=0.001) : n(n), kmax(kmax), lambda(lambda), nu(nu) {
    km = index = 0;
    x = new F[n], w = new F[n], b = new F[n];
    mCovariance = new F*[n], mCholesky = new F*[n];
    for (int i=0; i<n; i++) {
      x[i] = w[i] = b[i] = 0.;
      mCovariance[i] = new F[n], mCholesky[i] = new F[n];
      for (int j=0; j<n; j++)
        mCovariance[i][j] = mCholesky[i][j] = 0.;
    }
  }
  ~OLS() {
    delete x, delete w, delete b;
    for (int i=0; i<n; i++) {
      delete mCovariance[i];
      delete mCholesky[i];
    }
    delete[] mCovariance, delete[] mCholesky;
  }
  void Add(const T val) {
    assert(index<n);
    x[index++] = F(val)-sub;
  }
  void AddFloat(const F val) {
    assert(index<n);
    x[index++] = val-sub;
  }
  F Predict(const T **p) {
    F sum = 0.;
    for (int i=0; i<n; i++)
      sum += w[i] * (x[i] = F(*p[i])-sub);
    return sum+sub;
  }
  F Predict() {
    assert(index==n);
    index = 0;
    F sum = 0.;
    for (int i=0; i<n; i++)
      sum += w[i] * x[i];
    return sum+sub;
  }
  inline void Update(const T val) {
#ifdef __GNUC__
    if (chosen_simd==SIMD_AVX2)
      UpdateAVX2(val);
    else
#endif
      UpdateUnrolled(val);
  }
#ifdef __GNUC__
  __attribute__ ((target ("avx2")))
#endif
  void UpdateAVX2(const T val) {
    F mul = 1.0-lambda;
    for (int j=0; j<n; j++)
      for (int i=0; i<n; i++)
        mCovariance[j][i] = lambda * mCovariance[j][i] + mul * (x[j] * x[i]);
    mul*=(F(val)-sub);
    for (int i=0; i<n; i++)
      b[i] = lambda * b[i] + mul * x[i];
    km++;
    if (km>=kmax) {
      if (!Factor()) Solve();
      km = 0;
    }
  }
  void UpdateUnrolled(const T val) {
    F mul = 1.0-lambda;
    int l = n-(n&3), i=0;
    for (int j=0; j<n; j++) {
    for (i=0; i<l; i+=4) {
      mCovariance[j][i]   = lambda * mCovariance[j][i  ] + mul * (x[j] * x[i  ]);
      mCovariance[j][i+1] = lambda * mCovariance[j][i+1] + mul * (x[j] * x[i+1]);
      mCovariance[j][i+2] = lambda * mCovariance[j][i+2] + mul * (x[j] * x[i+2]);
      mCovariance[j][i+3] = lambda * mCovariance[j][i+3] + mul * (x[j] * x[i+3]);
    }
    for (; i<n; i++)
      mCovariance[j][i] = lambda * mCovariance[j][i] + mul * (x[j] * x[i]);
    }
    mul*=(F(val)-sub);
    for (i=0; i<l; i+=4) {
      b[i  ] = lambda * b[i  ] + mul * x[i  ];
      b[i+1] = lambda * b[i+1] + mul * x[i+1];
      b[i+2] = lambda * b[i+2] + mul * x[i+2];
      b[i+3] = lambda * b[i+3] + mul * x[i+3];
    }
    for (; i<n; i++)
      b[i] = lambda * b[i] + mul * x[i];
    km++;
    if (km>=kmax) {
      if (!Factor()) Solve();
      km = 0;
    }
  }
};

///////////////// Least Mean Squares predictor /////////////////

template <typename F, typename T>
class LMS {
private:
  F *weights, *eg, *buffer;
  F rates[2];
  F rho, complement, eps, prediction;
  int S, D;
public:
  LMS(const int S, const int D, const F lRate, const F rRate, const F rho = (F)0.95, const F eps = (F)1e-3) : rates{ lRate, rRate }, rho(rho), complement(1.0f - rho), eps(eps), prediction(0.0f), S(S), D(D) {
    assert(S>0 && D>0);
    weights = new F[S+D], eg = new F[S+D], buffer = new F[S+D];
    Reset();
  }
  ~LMS() {
    delete weights, delete eg, delete buffer;
  }
  F Predict(const T sample) {
    memmove(&buffer[S+1], &buffer[S], (D-1) * sizeof(F));
    buffer[S] = sample;
    prediction = 0.;
    for (int i=0; i<S+D; i++)
      prediction+= weights[i] * buffer[i];
    return prediction;
  }
  void Update(const T sample) {
    const F error = sample - prediction;
    int i=0;
    for (; i<S; i++) {
      const F gradient = error * buffer[i];
      eg[i] = rho * eg[i] + complement * (gradient * gradient);
      weights[i]+= (rates[0] * gradient * rsqrt(eg[i] + eps));
    }
    for (; i<S+D; i++) {
      const F gradient = error * buffer[i];
      eg[i] = rho * eg[i] + complement * (gradient * gradient);
      weights[i]+= (rates[1] * gradient * rsqrt(eg[i] + eps));
    }
    memmove(&buffer[1], &buffer[0], (S-1) * sizeof(F));
    buffer[0] = sample;
  }
  void Reset() {
    for (int i=0; i<S+D; i++)
      weights[i] = eg[i] = buffer[i] = 0.;
  }
};

////////////////////////////// Indirect Context //////////////////////////////

template <typename T>
class IndirectContext {
private:
  Array<T> data;
  T* ctx;
  const U32 ctxMask, inputMask, inputBits;
public:
  IndirectContext(const int BitsPerContext, const int InputBits) :
    data(UINT64_C(1)<<BitsPerContext),
    ctx(&data[0]),
    ctxMask((UINT32_C(1)<<BitsPerContext)-1),
    inputMask((UINT32_C(1)<<InputBits)-1),
    inputBits(InputBits)
  {
    assert(BitsPerContext>0 && BitsPerContext<=20);
    assert(InputBits>0 && InputBits<=8);
  }
  void operator+=(const U32 i) {
    assert(i<=inputMask);
    (*ctx)<<=inputBits;
    (*ctx)|=i;
  }
  void operator=(const U32 i) {
    ctx = &data[i&ctxMask];
  }
  T& operator()(void) {
    return *ctx;
  }
};

////////////////////////////// Move-to-Front list //////////////////////////////

class MTFList{
private:
  int Root, Index;
  Array<int, 16> Previous;
  Array<int, 16> Next;
public:
  MTFList(const U16 n): Root(0), Index(0), Previous(n), Next(n) {
    assert(n>0);
    for (int i=0; i<n; i++) {
      Previous[i] = i-1;
      Next[i] = i+1;
    }
    Next[n-1] = -1;
  }
  inline int GetFirst() {
    return Index=Root;
  }
  inline int GetNext() {
    if (Index>=0) Index = Next[Index];
    return Index;
  }
  inline void MoveToFront(const int i) {
    assert(U32(i)<Previous.size());
    if ((Index=i)==Root) return;
    const int p=Previous[Index], n=Next[Index];
    if (p>=0) Next[p] = Next[Index];
    if (n>=0) Previous[n] = Previous[Index];
    Previous[Root] = Index;
    Next[Index] = Root;
    Root = Index;
    Previous[Root]=-1;
  }
};

//////////////////////////// Text modelling /////////////////////////

#define TAB 0x09
#define NEW_LINE 0x0A
#define CARRIAGE_RETURN 0x0D
#define SPACE 0x20
#define QUOTE 0x22
#define APOSTROPHE 0x27

#ifdef USE_TEXTMODEL

inline bool CharInArray(const char c, const char a[], const int len) {
  if (a==nullptr)
    return false;
  int i=0;
  for (; i<len && c!=a[i]; i++);
  return i<len;
}

#define MAX_WORD_SIZE 64
#define WORD_EMBEDDING_SIZE 3

class Word {
private:
  U64 CalculateHash() {
    U64 h=0;
    for (int i=Start; i<=End; i++)
      h=combine64(h,Letters[i]);
    return h;
  }
public:
  U8 Letters[MAX_WORD_SIZE];
  U8 Start, End;
  U64 Hash[2];
  U32 Type, Language, Embedding;
  Word() {reset();}
  void reset() {
    memset(&Letters[0], 0, sizeof(U8)*MAX_WORD_SIZE);
    Start=End=0;
    Hash[0]=Hash[1]=0;
    Type=Language=Embedding=0;
  }
  bool operator==(const char *s) const {
    size_t len=strlen(s);
    return ((size_t)(End-Start+(Letters[Start]!=0))==len && memcmp(&Letters[Start], s, len)==0);
  }
  bool operator!=(const char *s) const {
    return !operator==(s);
  }
  void operator+=(const char c) {
    if (End<MAX_WORD_SIZE-1) {
      End+=(Letters[End]>0);
      Letters[End]=tolower(c);
    }
  }
  U32 operator-(const Word W) const {
    U32 res = 0;
    for (int i=0, j=0; i<WORD_EMBEDDING_SIZE; i++, j+=8)
      res = (res<<8)|U8(U8(Embedding>>j)-U8(W.Embedding>>j));
    return res;
  }
  U32 operator+(const Word W) const {
    U32 res = 0;
    for (int i=0, j=0; i<WORD_EMBEDDING_SIZE; i++, j+=8)
      res = (res<<8)|U8(U8(Embedding>>j)+U8(W.Embedding>>j));
    return res;
  }
  U8 operator[](U8 i) const {
    return (End-Start>=i)?Letters[Start+i]:0;
  }
  U8 operator()(U8 i) const {
    return (End-Start>=i)?Letters[End-i]:0;
  }
  U32 Length() const {
    if (Letters[Start]!=0)
      return End-Start+1;
    return 0;
  }
  U32 DistanceTo(const Word W) const {
    U32 res = 0;
    for (int i=0, j=0; i<WORD_EMBEDDING_SIZE; i++, j+=8)
      res+=SQR(abs(int(U8(Embedding>>j)-U8(W.Embedding>>j))));
    return (U32)sqrt(res);
  }
  void CalculateWordHash() {
    Hash[1] = Hash[0] = CalculateHash(); // Hash[1]: placeholder for stem hash, will be overwritten after stemming
  }
  void CalculateStemHash() { // called by a stemmer after stemming
    Hash[1] = CalculateHash();
  }
  bool ChangeSuffix(const char *OldSuffix, const char *NewSuffix) {
    size_t len=strlen(OldSuffix);
    if (Length()>len && memcmp(&Letters[End-len+1], OldSuffix, len)==0) {
      size_t n=strlen(NewSuffix);
      if (n>0) {
        memcpy(&Letters[End-int(len)+1], NewSuffix, min(MAX_WORD_SIZE-1,End+int(n))-End);
        End=min(MAX_WORD_SIZE-1, End-int(len)+int(n));
      }
      else
        End-=U8(len);
      return true;
    }
    return false;
  }
  bool MatchesAny(const char* a[], const int count) {
    int i=0;
    size_t len = (size_t)Length();
    for (; i<count && (len!=strlen(a[i]) || memcmp(&Letters[Start], a[i], len)!=0); i++);
    return i<count;
  }
  bool EndsWith(const char *Suffix) const {
    size_t len=strlen(Suffix);
    return (Length()>len && memcmp(&Letters[End-len+1], Suffix, len)==0);
  }
  bool StartsWith(const char *Prefix) const {
    size_t len=strlen(Prefix);
    return (Length()>len && memcmp(&Letters[Start], Prefix, len)==0);
  }
  void print() const {
    for(int r=Start;r<=End;r++)
      printf("%c",(char)Letters[r]);
    printf("\n");
  }
};

class Segment {
public:
  Word FirstWord; // useful following questions
  U32 WordCount;
  U32 NumCount;
};

class Sentence : public Segment {
public:
  enum Types { // possible sentence types, excluding Imperative
    Declarative,
    Interrogative,
    Exclamative,
    Count
  };
  Types Type;
  U32 SegmentCount;
  U32 VerbIndex; // relative position of last detected verb
  U32 NounIndex; // relative position of last detected noun
  U32 CapitalIndex; // relative position of last capitalized word, excluding the initial word of this sentence
  Word lastVerb, lastNoun, lastCapital;
};

class Paragraph {
public:
  U32 SentenceCount, TypeCount[Sentence::Types::Count], TypeMask;
};

class Language {
public:
  enum Flags {
    Verb                   = (1<<0),
    Noun                   = (1<<1)
  };
  enum Ids {
    Unknown,
    English,
    French,
    German,
    Count
  };
  virtual ~Language() {};
  virtual bool IsAbbreviation(Word *W) = 0;
};

class English: public Language {
private:
  static constexpr int NUM_ABBREV = 6;
  const char *Abbreviations[NUM_ABBREV]={ "mr","mrs","ms","dr","st","jr" };
public:
  enum Flags {
    Adjective              = (1<<2),
    Plural                 = (1<<3),
    Male                   = (1<<4),
    Female                 = (1<<5),
    Negation               = (1<<6),
    PastTense              = (1<<7)|Verb,
    PresentParticiple      = (1<<8)|Verb,
    AdjectiveSuperlative   = (1<<9)|Adjective,
    AdjectiveWithout       = (1<<10)|Adjective,
    AdjectiveFull          = (1<<11)|Adjective,
    AdverbOfManner         = (1<<12),
    SuffixNESS             = (1<<13),
    SuffixITY              = (1<<14)|Noun,
    SuffixCapable          = (1<<15),
    SuffixNCE              = (1<<16),
    SuffixNT               = (1<<17),
    SuffixION              = (1<<18),
    SuffixAL               = (1<<19)|Adjective,
    SuffixIC               = (1<<20)|Adjective,
    SuffixIVE              = (1<<21),
    SuffixOUS              = (1<<22)|Adjective,
    PrefixOver             = (1<<23),
    PrefixUnder            = (1<<24)
  };
  bool IsAbbreviation(Word *W) { return W->MatchesAny(Abbreviations, NUM_ABBREV); };
};

class French: public Language {
private:
  static constexpr int NUM_ABBREV = 2;
  const char *Abbreviations[NUM_ABBREV]={ "m","mm" };
public:
  enum Flags {
    Adjective              = (1<<2),
    Plural                 = (1<<3)
  };
  bool IsAbbreviation(Word *W) { return W->MatchesAny(Abbreviations, NUM_ABBREV); };
};

class German : public Language {
private:
  static constexpr int NUM_ABBREV = 3;
  const char *Abbreviations[NUM_ABBREV]={ "fr","hr","hrn" };
public:
  enum Flags {
    Adjective              = (1<<2),
    Plural                 = (1<<3),
    Female                 = (1<<4)
  };
  bool IsAbbreviation(Word *W) { return W->MatchesAny(Abbreviations, NUM_ABBREV); };
};

//////////////////////////// Stemming routines /////////////////////////

class Stemmer {
protected:
  U32 GetRegion(const Word *W, const U32 From) {
    bool hasVowel = false;
    for (int i=W->Start+From; i<=W->End; i++) {
      if (IsVowel(W->Letters[i])) {
        hasVowel = true;
        continue;
      }
      else if (hasVowel)
        return i-W->Start+1;
    }
    return W->Start+W->Length();
  }
  bool SuffixInRn(const Word *W, const U32 Rn, const char *Suffix) {
    return (W->Start!=W->End && Rn<=W->Length()-strlen(Suffix));
  }
public:
  virtual ~Stemmer() {};
  virtual bool IsVowel(const char c) = 0;
  virtual bool Stem(Word *W) = 0;
};

//  English affix stemmer, based on the Porter2 stemmer.

class EnglishStemmer: public Stemmer {
private:
  static constexpr int NUM_VOWELS = 6;
  static constexpr char Vowels[NUM_VOWELS]={'a','e','i','o','u','y'};
  static constexpr int NUM_DOUBLES = 9;
  static constexpr char Doubles[NUM_DOUBLES]={'b','d','f','g','m','n','p','r','t'};
  static constexpr int NUM_LI_ENDINGS = 10;
  static constexpr char LiEndings[NUM_LI_ENDINGS]={'c','d','e','g','h','k','m','n','r','t'};
  static constexpr int NUM_NON_SHORT_CONSONANTS = 3;
  static constexpr char NonShortConsonants[NUM_NON_SHORT_CONSONANTS]={'w','x','Y'};
  static constexpr int NUM_MALE_WORDS = 9;
  const char *MaleWords[NUM_MALE_WORDS]={"he","him","his","himself","man","men","boy","husband","actor"};
  static constexpr int NUM_FEMALE_WORDS = 8;
  const char *FemaleWords[NUM_FEMALE_WORDS]={"she","her","herself","woman","women","girl","wife","actress"};
  static constexpr int NUM_COMMON_WORDS = 12;
  const char *CommonWords[NUM_COMMON_WORDS]={"the","be","to","of","and","in","that","you","have","with","from","but"};
  static constexpr int NUM_SUFFIXES_STEP0 = 3;
  const char *SuffixesStep0[NUM_SUFFIXES_STEP0]={"'s'","'s","'"};
  static constexpr int NUM_SUFFIXES_STEP1b = 6;
  const char *SuffixesStep1b[NUM_SUFFIXES_STEP1b]={"eedly","eed","ed","edly","ing","ingly"};
  static constexpr U32 TypesStep1b[NUM_SUFFIXES_STEP1b]={English::AdverbOfManner,0,English::PastTense,English::AdverbOfManner|English::PastTense,English::PresentParticiple,English::AdverbOfManner|English::PresentParticiple};
  static constexpr int NUM_SUFFIXES_STEP2 = 22;
  const char *(SuffixesStep2[NUM_SUFFIXES_STEP2])[2]={
    {"ization", "ize"},
    {"ational", "ate"},
    {"ousness", "ous"},
    {"iveness", "ive"},
    {"fulness", "ful"},
    {"tional", "tion"},
    {"lessli", "less"},
    {"biliti", "ble"},
    {"entli", "ent"},
    {"ation", "ate"},
    {"alism", "al"},
    {"aliti", "al"},
    {"fulli", "ful"},
    {"ousli", "ous"},
    {"iviti", "ive"},
    {"enci", "ence"},
    {"anci", "ance"},
    {"abli", "able"},
    {"izer", "ize"},
    {"ator", "ate"},
    {"alli", "al"},
    {"bli", "ble"}
  };
  const U32 TypesStep2[NUM_SUFFIXES_STEP2]={
    English::SuffixION,
    English::SuffixION|English::SuffixAL,
    English::SuffixNESS,
    English::SuffixNESS,
    English::SuffixNESS,
    English::SuffixION|English::SuffixAL,
    English::AdverbOfManner,
    English::AdverbOfManner|English::SuffixITY,
    English::AdverbOfManner,
    English::SuffixION,
    0,
    English::SuffixITY,
    English::AdverbOfManner,
    English::AdverbOfManner,
    English::SuffixITY,
    0,
    0,
    English::AdverbOfManner,
    0,
    0,
    English::AdverbOfManner,
    English::AdverbOfManner
  };
  static constexpr int NUM_SUFFIXES_STEP3 = 8;
  const char *(SuffixesStep3[NUM_SUFFIXES_STEP3])[2]={
    {"ational", "ate"},
    {"tional", "tion"},
    {"alize", "al"},
    {"icate", "ic"},
    {"iciti", "ic"},
    {"ical", "ic"},
    {"ful", ""},
    {"ness", ""}
  };
  static constexpr U32 TypesStep3[NUM_SUFFIXES_STEP3]={English::SuffixION|English::SuffixAL,English::SuffixION|English::SuffixAL,0,0,English::SuffixITY,English::SuffixAL,English::AdjectiveFull,English::SuffixNESS};
  static constexpr int NUM_SUFFIXES_STEP4 = 20;
  const char *SuffixesStep4[NUM_SUFFIXES_STEP4]={"al","ance","ence","er","ic","able","ible","ant","ement","ment","ent","ou","ism","ate","iti","ous","ive","ize","sion","tion"};
  static constexpr U32 TypesStep4[NUM_SUFFIXES_STEP4]={
    English::SuffixAL,
    English::SuffixNCE,
    English::SuffixNCE,
    0,
    English::SuffixIC,
    English::SuffixCapable,
    English::SuffixCapable,
    English::SuffixNT,
    0,
    0,
    English::SuffixNT,
    0,
    0,
    0,
    English::SuffixITY,
    English::SuffixOUS,
    English::SuffixIVE,
    0,
    English::SuffixION,
    English::SuffixION
  };
  static constexpr int NUM_EXCEPTION_REGION1 = 3;
  const char *ExceptionsRegion1[NUM_EXCEPTION_REGION1]={"gener","arsen","commun"};
  static constexpr int NUM_EXCEPTIONS1 = 19;
  const char *(Exceptions1[NUM_EXCEPTIONS1])[2]={
    {"skis", "ski"},
    {"skies", "sky"},
    {"dying", "die"},
    {"lying", "lie"},
    {"tying", "tie"},
    {"idly", "idl"},
    {"gently", "gentl"},
    {"ugly", "ugli"},
    {"early", "earli"},
    {"only", "onli"},
    {"singly", "singl"},
    {"sky", "sky"},
    {"news", "news"},
    {"howe", "howe"},
    {"atlas", "atlas"},
    {"cosmos", "cosmos"},
    {"bias", "bias"},
    {"andes", "andes"},
    {"texas", "texas"}
  };
  static constexpr U32 TypesExceptions1[NUM_EXCEPTIONS1]={
    English::Noun|English::Plural,
    English::Noun|English::Plural|English::Verb,
    English::PresentParticiple,
    English::PresentParticiple,
    English::PresentParticiple,
    English::AdverbOfManner,
    English::AdverbOfManner,
    English::Adjective,
    English::Adjective|English::AdverbOfManner,
    0,
    English::AdverbOfManner,
    English::Noun,
    English::Noun,
    0,
    English::Noun,
    English::Noun,
    English::Noun,
    English::Noun|English::Plural,
    English::Noun
  };
  static constexpr int NUM_EXCEPTIONS2 = 8;
  const char *Exceptions2[NUM_EXCEPTIONS2]={"inning","outing","canning","herring","earring","proceed","exceed","succeed"};
  static constexpr U32 TypesExceptions2[NUM_EXCEPTIONS2]={English::Noun,English::Noun,English::Noun,English::Noun,English::Noun,English::Verb,English::Verb,English::Verb};
  inline bool IsConsonant(const char c) {
    return !IsVowel(c);
  }
  inline bool IsShortConsonant(const char c) {
    return !CharInArray(c, NonShortConsonants, NUM_NON_SHORT_CONSONANTS);
  }
  inline bool IsDouble(const char c) {
    return CharInArray(c, Doubles, NUM_DOUBLES);
  }
  inline bool IsLiEnding(const char c) {
    return CharInArray(c, LiEndings, NUM_LI_ENDINGS);
  }
  U32 GetRegion1(const Word *W) {
    for (int i=0; i<NUM_EXCEPTION_REGION1; i++) {
      if (W->StartsWith(ExceptionsRegion1[i]))
        return U32(strlen(ExceptionsRegion1[i]));
    }
    return GetRegion(W, 0);
  }
  bool EndsInShortSyllable(const Word *W) {
    if (W->End==W->Start)
      return false;
    else if (W->End==W->Start+1)
      return IsVowel((*W)(1)) && IsConsonant((*W)(0));
    else
      return (IsConsonant((*W)(2)) && IsVowel((*W)(1)) && IsConsonant((*W)(0)) && IsShortConsonant((*W)(0)));
  }
  bool IsShortWord(const Word *W) {
    return (EndsInShortSyllable(W) && GetRegion1(W)==W->Length());
  }
  inline bool HasVowels(const Word *W) {
    for (int i=W->Start; i<=W->End; i++) {
      if (IsVowel(W->Letters[i]))
        return true;
    }
    return false;
  }
  bool TrimApostrophes(Word *W) {
    bool result=false;
    //trim all apostrophes from the beginning
    int cnt=0;
    while(W->Start!=W->End && (*W)[0]==APOSTROPHE) {
      result=true;
      W->Start++;
      cnt++;
    }
    //trim the same number of apostrophes from the end (if there are)
    while(W->Start!=W->End && (*W)(0)==APOSTROPHE) {
      if(cnt==0)break;
      W->End--;
      cnt--;
    }
    return result;
  }
  void MarkYsAsConsonants(Word *W) {
    if ((*W)[0]=='y')
      W->Letters[W->Start]='Y';
    for (int i=W->Start+1; i<=W->End; i++) {
      if (IsVowel(W->Letters[i-1]) && W->Letters[i]=='y')
        W->Letters[i]='Y';
    }
  }
  bool ProcessPrefixes(Word *W) {
    if (W->StartsWith("irr") && W->Length()>5 && ((*W)[3]=='a' || (*W)[3]=='e'))
      W->Start+=2, W->Type|=English::Negation;
    else if (W->StartsWith("over") && W->Length()>5)
      W->Start+=4, W->Type|=English::PrefixOver;
    else if (W->StartsWith("under") && W->Length()>6)
      W->Start+=5, W->Type|=English::PrefixUnder;
    else if (W->StartsWith("unn") && W->Length()>5)
      W->Start+=2, W->Type|=English::Negation;
    else if (W->StartsWith("non") && W->Length()>(U32)(5+((*W)[3]=='-')))
      W->Start+=2+((*W)[3]=='-'), W->Type|=English::Negation;
    else
      return false;
    return true;
  }
  bool ProcessSuperlatives(Word *W) {
    if (W->EndsWith("est") && W->Length()>4) {
      U8 i=W->End;
      W->End-=3;
      W->Type|=English::AdjectiveSuperlative;

      if ((*W)(0)==(*W)(1) && (*W)(0)!='r' && !(W->Length()>=4 && memcmp("sugg",&W->Letters[W->End-3],4)==0)) {
        W->End-= ( ((*W)(0)!='f' && (*W)(0)!='l' && (*W)(0)!='s') ||
                   (W->Length()>4 && (*W)(1)=='l' && ((*W)(2)=='u' || (*W)(3)=='u' || (*W)(3)=='v'))) &&
                   (!(W->Length()==3 && (*W)(1)=='d' && (*W)(2)=='o'));
        if (W->Length()==2 && ((*W)[0]!='i' || (*W)[1]!='n'))
          W->End = i, W->Type&=~English::AdjectiveSuperlative;
      }
      else {
        switch((*W)(0)) {
          case 'd': case 'k': case 'm': case 'y': break;
          case 'g': {
            if (!( W->Length()>3 && ((*W)(1)=='n' || (*W)(1)=='r') && memcmp("cong",&W->Letters[W->End-3],4)!=0 ))
              W->End = i, W->Type&=~English::AdjectiveSuperlative;
            else
              W->End+=((*W)(2)=='a');
            break;
          }
          case 'i': { W->Letters[W->End]='y'; break; }
          case 'l': {
            if (W->End==W->Start+1 || memcmp("mo",&W->Letters[W->End-2],2)==0)
              W->End = i, W->Type&=~English::AdjectiveSuperlative;
            else
              W->End+=IsConsonant((*W)(1));
            break;
          }
          case 'n': {
            if (W->Length()<3 || IsConsonant((*W)(1)) || IsConsonant((*W)(2)))
              W->End = i, W->Type&=~English::AdjectiveSuperlative;
            break;
          }
          case 'r': {
            if (W->Length()>3 && IsVowel((*W)(1)) && IsVowel((*W)(2)))
              W->End+=((*W)(2)=='u') && ((*W)(1)=='a' || (*W)(1)=='i');
            else
              W->End = i, W->Type&=~English::AdjectiveSuperlative;
            break;
          }
          case 's': { W->End++; break; }
          case 'w': {
            if (!(W->Length()>2 && IsVowel((*W)(1))))
              W->End = i, W->Type&=~English::AdjectiveSuperlative;
            break;
          }
          case 'h': {
            if (!(W->Length()>2 && IsConsonant((*W)(1))))
              W->End = i, W->Type&=~English::AdjectiveSuperlative;
            break;
          }
          default: {
            W->End+=3;
            W->Type&=~English::AdjectiveSuperlative;
          }
        }
      }
    }
    return (W->Type&English::AdjectiveSuperlative)>0;
  }
  //Search for the longest among the suffixes, 's' or 's or ' and remove if found.
  //Examples: Children's toys / Vice presidents' duties
  bool Step0(Word *W) {
    for (int i=0; i<NUM_SUFFIXES_STEP0; i++) {
      if (W->EndsWith(SuffixesStep0[i])) {
        W->End-=U8(strlen(SuffixesStep0[i]));
        W->Type|=English::Plural;
        return true;
      }
    }
    return false;
  }

  //Search for the longest among the following suffixes, and perform the action indicated.
  bool Step1a(Word *W) {

    //sses -> replace by ss
    if (W->EndsWith("sses")) {
      W->End-=2;
      W->Type|=English::Plural;
      return true;
    }

    //ied+ ies* -> replace by i if preceded by more than one letter, otherwise by ie (so ties -> tie, cries -> cri) 
    if (W->EndsWith("ied") || W->EndsWith("ies")) {
      W->Type |= ((*W)(0)=='d') ? English::PastTense : English::Plural;
      W->End-= W->Length()>4 ? 2 : 1;
      return true;
    }

    //us+ ss -> do nothing
    if (W->EndsWith("us") || W->EndsWith("ss"))
      return false;

    //s -> delete if the preceding word part contains a vowel not immediately before the s (so gas and this retain the s, gaps and kiwis lose it) 
    if ((*W)(0)=='s' && W->Length()>2) {
      for (int i=W->Start;i<=W->End-2;i++) {
        if (IsVowel(W->Letters[i])) {
          W->End--;
          W->Type|=English::Plural;
          return true;
        }
      }
    }

    if (W->EndsWith("n't") && W->Length()>4) {
      switch ((*W)(3)) {
        case 'a': {
          if ((*W)(4)=='c')
            W->End-=2; // can't -> can
          else
            W->ChangeSuffix("n't","ll"); // shan't -> shall
          break;
        }
        case 'i': { W->ChangeSuffix("in't","m"); break; } // ain't -> am
        case 'o': {
          if ((*W)(4)=='w')
            W->ChangeSuffix("on't","ill"); // won't -> will
          else
            W->End-=3; //don't -> do
          break;
        }
        default: W->End-=3; // couldn't -> could, shouldn't -> should, needn't -> need, hasn't -> has
      }
      W->Type|=English::Negation;	
      return true;
    }

    if (W->EndsWith("hood") && W->Length()>7) {
      W->End-=4;  //brotherhood -> brother
      return true;
    }
    return false;
  }
  //Search for the longest among the following suffixes, and perform the action indicated. 
  bool Step1b(Word *W, const U32 R1) {
    for (int i=0; i<NUM_SUFFIXES_STEP1b; i++) {
      if (W->EndsWith(SuffixesStep1b[i])) {
        switch(i) {
          case 0: case 1: {
            if (SuffixInRn(W, R1, SuffixesStep1b[i]))
              W->End-=1+i*2;
            break;
          }
          default: {
            U8 j=W->End;
            W->End-=U8(strlen(SuffixesStep1b[i]));
            if (HasVowels(W)) {
              if (W->EndsWith("at") || W->EndsWith("bl") || W->EndsWith("iz") || IsShortWord(W))
                (*W)+='e';
              else if (W->Length()>2) {
                if ((*W)(0)==(*W)(1) && IsDouble((*W)(0)))
                  W->End--;
                else if (i==2 || i==3) {
                  switch((*W)(0)) {
                    case 'c': case 's': case 'v': { W->End+=!(W->EndsWith("ss") || W->EndsWith("ias")); break; }
                    case 'd': {
                      static constexpr char nAllowed[4] = {'a','e','i','o'};
                      W->End+=IsVowel((*W)(1)) && (!CharInArray((*W)(2), nAllowed, 4)); break;
                    }
                    case 'k': { W->End+=W->EndsWith("uak"); break; }
                    case 'l': {
                      static constexpr char Allowed1[10] = {'b','c','d','f','g','k','p','t','y','z'};
                      static constexpr char Allowed2[4] = {'a','i','o','u'};
                      W->End+= CharInArray((*W)(1), Allowed1, 10) ||
                                (CharInArray((*W)(1), Allowed2, 4) && IsConsonant((*W)(2)));
                      break;
                    }
                  }
                }
                else if (i>=4) {
                  switch((*W)(0)) {
                    case 'd': {
                      if (IsVowel((*W)(1)) && (*W)(2)!='a' && (*W)(2)!='e' && (*W)(2)!='o')
                        (*W)+='e';
                      break;
                    }
                    case 'g': {
                      static constexpr char Allowed[7] = {'a','d','e','i','l','r','u'};
                      if (
                        CharInArray((*W)(1), Allowed, 7) || (
                         (*W)(1)=='n' && (
                          (*W)(2)=='e' ||
                          ((*W)(2)=='u' && (*W)(3)!='b' && (*W)(3)!='d') ||
                          ((*W)(2)=='a' && ((*W)(3)=='r' || ((*W)(3)=='h' && (*W)(4)=='c'))) ||
                          (W->EndsWith("ring") && ((*W)(4)=='c' || (*W)(4)=='f'))
                         )
                        )
                      )
                        (*W)+='e';
                      break;
                    }
                    case 'l': {
                      if (!((*W)(1)=='l' || (*W)(1)=='r' || (*W)(1)=='w' || (IsVowel((*W)(1)) && IsVowel((*W)(2)))))
                        (*W)+='e';
                      if (W->EndsWith("uell") && W->Length()>4 && (*W)(4)!='q')
                        W->End--;
                      break;
                    }
                    case 'r': {
                      if ((
                        ((*W)(1)=='i' && (*W)(2)!='a' && (*W)(2)!='e' && (*W)(2)!='o') ||
                        ((*W)(1)=='a' && (!((*W)(2)=='e' || (*W)(2)=='o' || ((*W)(2)=='l' && (*W)(3)=='l')))) ||
                        ((*W)(1)=='o' && (!((*W)(2)=='o' || ((*W)(2)=='t' && (*W)(3)!='s')))) ||
                        (*W)(1)=='c' || (*W)(1)=='t') && (!W->EndsWith("str"))
                      )
                        (*W)+='e';
                      break;
                    }
                    case 't': {
                      if ((*W)(1)=='o' && (*W)(2)!='g' && (*W)(2)!='l' && (*W)(2)!='i' && (*W)(2)!='o')
                        (*W)+='e';
                      break;
                    }
                    case 'u': {
                      if (!(W->Length()>3 && IsVowel((*W)(1)) && IsVowel((*W)(2))))
                        (*W)+='e';
                      break;
                    }
                    case 'z': {
                      if (W->EndsWith("izz") && W->Length()>3 && ((*W)(3)=='h' || (*W)(3)=='u'))
                        W->End--;
                      else if ((*W)(1)!='t' && (*W)(1)!='z')
                        (*W)+='e';
                      break;
                    }
                    case 'k': {
                      if (W->EndsWith("uak"))
                        (*W)+='e';
                      break;
                    }
                    case 'b': case 'c': case 's': case 'v': {
                      if (!(
                        ((*W)(0)=='b' && ((*W)(1)=='m' || (*W)(1)=='r')) ||
                        W->EndsWith("ss") || W->EndsWith("ias") || (*W)=="zinc"
                      ))
                        (*W)+='e';
                      break;
                    }
                  }
                }
              }
            }
            else {
              W->End=j;
              return false;
            }
          }
        }
        W->Type|=TypesStep1b[i];
        return true;
      }
    }
    return false;
  }
  bool Step1c(Word *W) {
    if (W->Length()>2 && tolower((*W)(0))=='y' && IsConsonant((*W)(1))) {
      W->Letters[W->End]='i';
      return true;
    }
    return false;
  }
  bool Step2(Word *W, const U32 R1) {
    for (int i=0; i<NUM_SUFFIXES_STEP2; i++) {
      if (W->EndsWith(SuffixesStep2[i][0]) && SuffixInRn(W, R1, SuffixesStep2[i][0])) {
        W->ChangeSuffix(SuffixesStep2[i][0], SuffixesStep2[i][1]);
        W->Type|=TypesStep2[i];
        return true;
      }
    }
    if (W->EndsWith("logi") && SuffixInRn(W, R1, "ogi")) {
      W->End--;
      return true;
    }
    else if (W->EndsWith("li")) {
      if (SuffixInRn(W, R1, "li") && IsLiEnding((*W)(2))) {
        W->End-=2;
        W->Type|=English::AdverbOfManner;
        return true;
      }
      else if (W->Length()>3) {
        switch((*W)(2)) {
            case 'b': {
              W->Letters[W->End]='e';
              W->Type|=English::AdverbOfManner;
              return true;
            }
            case 'i': {
              if (W->Length()>4) {
                W->End-=2;
                W->Type|=English::AdverbOfManner;
                return true;
              }
              break;
            }
            case 'l': {
              if (W->Length()>5 && ((*W)(3)=='a' || (*W)(3)=='u')) {
                W->End-=2;
                W->Type|=English::AdverbOfManner;
                return true;
              }
              break;
            }
            case 's': {
              W->End-=2;
              W->Type|=English::AdverbOfManner;
              return true;
            }
            case 'e': case 'g': case 'm': case 'n': case 'r': case 'w': {
              if (W->Length()>(U32)(4+((*W)(2)=='r'))) {
                W->End-=2;
                W->Type|=English::AdverbOfManner;
                return true;
              }
            }
        }
      }
    }
    return false;
  }
  bool Step3(Word *W, const U32 R1, const U32 R2) {
    bool res=false;
    for (int i=0; i<NUM_SUFFIXES_STEP3; i++) {
      if (W->EndsWith(SuffixesStep3[i][0]) && SuffixInRn(W, R1, SuffixesStep3[i][0])) {
        W->ChangeSuffix(SuffixesStep3[i][0], SuffixesStep3[i][1]);
        W->Type|=TypesStep3[i];
        res=true;
        break;
      }
    }
    if (W->EndsWith("ative") && SuffixInRn(W, R2, "ative")) {
      W->End-=5;
      W->Type|=English::SuffixIVE;
      return true;
    }
    if (W->Length()>5 && W->EndsWith("less")) {
      W->End-=4;
      W->Type|=English::AdjectiveWithout;
      return true;
    }
    return res;
  }

  //Search for the longest among the following suffixes, and, if found and in R2, perform the action indicated.
  bool Step4(Word *W, const U32 R2) {
    bool res=false;
    for (int i=0; i<NUM_SUFFIXES_STEP4; i++) {
      if (W->EndsWith(SuffixesStep4[i]) && SuffixInRn(W, R2, SuffixesStep4[i])) {
        W->End-=U8(strlen(SuffixesStep4[i])-(i>17) /*sion, tion*/); // remove: al   ance   ence   er   ic   able   ible   ant   ement   ment   ent   ism   ate   iti   ous   ive   ize ; ion -> delete if preceded by s or t
        if (!(i==10 /* ent */ && (*W)(0)=='m')) //exception: no TypesStep4 should be assigned for 'agreement'
          W->Type|=TypesStep4[i]; 
        if (i==0 && W->EndsWith("nti")) { 
          W->End--; // ntial -> nt
          res=true; // process ant+ial, ent+ial
          continue;
        }
        return true;
      }
    }
    return res;
  }

  //Search for the the following suffixes, and, if found, perform the action indicated. 
  bool Step5(Word *W, const U32 R1, const U32 R2) {
    if ((*W)(0)=='e' && (*W)!="here") {
      if (SuffixInRn(W, R2, "e"))
        W->End--; //e -> delete if in R2, or in R1 and not preceded by a short syllable 
      else if (SuffixInRn(W, R1, "e")) {
        W->End--; //e -> delete if in R1 and not preceded by a short syllable 
        W->End+=EndsInShortSyllable(W);
      }
      else
        return false;
      return true;
    }
    else if (W->Length()>1 && (*W)(0)=='l' && SuffixInRn(W, R2, "l") && (*W)(1)=='l') {
      W->End--; //l -> delete if in R2 and preceded by l 
      return true;
    }
    return false;
  }
public:
  inline bool IsVowel(const char c) final {
    return CharInArray(c, Vowels, NUM_VOWELS);
  }
  bool Stem(Word *W) {
    if (W->Length()<2) {
      W->CalculateStemHash();
      //W->print(); //for debugging
      return false;
    }
    bool res = TrimApostrophes(W);
    res|=ProcessPrefixes(W);
    res|=ProcessSuperlatives(W);
    for (int i=0; i<NUM_EXCEPTIONS1; i++) {
      if ((*W)==Exceptions1[i][0]) {
        if (i<11) {
          size_t len=strlen(Exceptions1[i][1]);
          memcpy(&W->Letters[W->Start], Exceptions1[i][1], len);
          W->End=W->Start+U8(len-1);
        }
        W->CalculateStemHash();
        W->Type|=TypesExceptions1[i];
        W->Language = Language::English;
        //W->print(); //for debugging
        return (i<11);
      }
    }

    // Start of modified Porter2 Stemmer
    MarkYsAsConsonants(W);
    U32 R1=GetRegion1(W), R2=GetRegion(W,R1);
    res|=Step0(W);
    res|=Step1a(W);
    for (int i=0; i<NUM_EXCEPTIONS2; i++) {
      if ((*W)==Exceptions2[i]) {
        W->CalculateStemHash();
        W->Type|=TypesExceptions2[i];
        W->Language = Language::English;
        //W->print(); //for debugging
        return res;
      }
    }
    res|=Step1b(W, R1);
    res|=Step1c(W);
    res|=Step2(W, R1);
    res|=Step3(W, R1, R2);
    res|=Step4(W, R2);
    res|=Step5(W, R1, R2);

    for (U8 i=W->Start; i<=W->End; i++) {
      if (W->Letters[i]=='Y')
        W->Letters[i]='y';
    }
    if (!W->Type || W->Type==English::Plural) {
      if (W->MatchesAny(MaleWords, NUM_MALE_WORDS))
        res = true, W->Type|=English::Male;
      else if (W->MatchesAny(FemaleWords, NUM_FEMALE_WORDS))
        res = true, W->Type|=English::Female;
    }
    if (!res)
      res=W->MatchesAny(CommonWords, NUM_COMMON_WORDS);
    W->CalculateStemHash();
    if (res)
      W->Language = Language::English;
    //W->print(); //for debugging
    return res;
  }
};

// French suffix stemmer, based on the Porter stemmer.

class FrenchStemmer: public Stemmer {
private:
  static constexpr int NUM_VOWELS = 17;
  static constexpr char Vowels[NUM_VOWELS]={'a','e','i','o','u','y','\xE2','\xE0','\xEB','\xE9','\xEA','\xE8','\xEF','\xEE','\xF4','\xFB','\xF9'};
  static constexpr int NUM_COMMON_WORDS = 10;
  const char *CommonWords[NUM_COMMON_WORDS]={"de","la","le","et","en","un","une","du","que","pas"};
  static constexpr int NUM_EXCEPTIONS = 3;
  const char *(Exceptions[NUM_EXCEPTIONS])[2]={
    {"monument", "monument"},
    {"yeux", "oeil"},
    {"travaux", "travail"},
  };
  static constexpr U32 TypesExceptions[NUM_EXCEPTIONS]={
    French::Noun,
    French::Noun|French::Plural,
    French::Noun|French::Plural
  };
  static constexpr int NUM_SUFFIXES_STEP1 = 39;
  const char *SuffixesStep1[NUM_SUFFIXES_STEP1]={
    "ance","iqUe","isme","able","iste","eux","ances","iqUes","ismes","ables","istes", //11
    "atrice","ateur","ation","atrices","ateurs","ations", //6
    "logie","logies", //2
    "usion","ution","usions","utions", //4
    "ence","ences", //2
    "issement","issements", //2
    "ement","ements", //2
    "it\xE9","it\xE9s", //2
    "if","ive","ifs","ives", //4
    "euse","euses", //2
    "ment","ments" //2
  };
  static constexpr int NUM_SUFFIXES_STEP2a = 35;
  const char *SuffixesStep2a[NUM_SUFFIXES_STEP2a]={
    "issaIent", "issantes", "iraIent", "issante",
    "issants", "issions", "irions", "issais",
    "issait", "issant", "issent", "issiez", "issons",
    "irais", "irait", "irent", "iriez", "irons",
    "iront", "isses", "issez", "\xEEmes",
    "\xEEtes", "irai", "iras", "irez", "isse",
    "ies", "ira", "\xEEt", "ie", "ir", "is",
    "it", "i"
  };
  static constexpr int NUM_SUFFIXES_STEP2b = 38;
  const char *SuffixesStep2b[NUM_SUFFIXES_STEP2b]={
    "eraIent", "assions", "erions", "assent",
    "assiez", "\xE8rent", "erais", "erait",
    "eriez", "erons", "eront", "aIent", "antes",
    "asses", "ions", "erai", "eras", "erez",
    "\xE2mes", "\xE2tes", "ante", "ants",
    "asse", "\xE9""es", "era", "iez", "ais",
    "ait", "ant", "\xE9""e", "\xE9s", "er",
    "ez", "\xE2t", "ai", "as", "\xE9", "a"
  };
  static constexpr int NUM_SET_STEP4 = 6;
  static constexpr char SetStep4[NUM_SET_STEP4]={'a','i','o','u','\xE8','s'};
  static constexpr int NUM_SUFFIXES_STEP4 = 7;
  const char *SuffixesStep4[NUM_SUFFIXES_STEP4]={"i\xE8re","I\xE8re","ion","ier","Ier","e","\xEB"};
  static constexpr int NUM_SUFFIXES_STEP5 = 5;
  const char *SuffixesStep5[NUM_SUFFIXES_STEP5]={"enn","onn","ett","ell","eill"};
  inline bool IsConsonant(const char c) {
    return !IsVowel(c);
  }
  void ConvertUTF8(Word *W) {
    for (int i=W->Start; i<W->End; i++) {
      U8 c = W->Letters[i+1]+((W->Letters[i+1]<0xA0)?0x60:0x40);
      if (W->Letters[i]==0xC3 && (IsVowel(c) || (W->Letters[i+1]&0xDF)==0x87)) {
        W->Letters[i] = c;
        if (i+1<W->End)
          memmove(&W->Letters[i+1], &W->Letters[i+2], W->End-i-1);
        W->End--;
      }
    }
  }
  void MarkVowelsAsConsonants(Word *W) {
    for (int i=W->Start; i<=W->End; i++) {
      switch (W->Letters[i]) {
        case 'i': case 'u': {
          if (i>W->Start && i<W->End && (IsVowel(W->Letters[i-1]) || (W->Letters[i-1]=='q' && W->Letters[i]=='u')) && IsVowel(W->Letters[i+1]))
            W->Letters[i] = toupper(W->Letters[i]);
          break;
        }
        case 'y': {
          if ((i>W->Start && IsVowel(W->Letters[i-1])) || (i<W->End && IsVowel(W->Letters[i+1])))
            W->Letters[i] = toupper(W->Letters[i]);
        }
      }
    }
  }
  U32 GetRV(Word *W) {
    U32 len = W->Length(), res = W->Start+len;
    if (len>=3 && ((IsVowel(W->Letters[W->Start]) && IsVowel(W->Letters[W->Start+1])) || W->StartsWith("par") || W->StartsWith("col") || W->StartsWith("tap") ))
      return W->Start+3;
    else {
      for (int i=W->Start+1;i<=W->End;i++) {
        if (IsVowel(W->Letters[i]))
          return i+1;
      }
    }
    return res;
  }
  bool Step1(Word *W, const U32 RV, const U32 R1, const U32 R2, bool *ForceStep2a) {
    int i = 0;
    for (; i<11; i++) {
      if (W->EndsWith(SuffixesStep1[i]) && SuffixInRn(W, R2, SuffixesStep1[i])) {
        W->End-=U8(strlen(SuffixesStep1[i]));
        if (i==3 /*able*/)
          W->Type|=French::Adjective;
        return true;
      }
    }
    for (; i<17; i++) {
      if (W->EndsWith(SuffixesStep1[i]) && SuffixInRn(W, R2, SuffixesStep1[i])) {
        W->End-=U8(strlen(SuffixesStep1[i]));
        if (W->EndsWith("ic"))
          W->ChangeSuffix("c", "qU");
        return true;
      }
    }
    for (; i<25;i++) {
      if (W->EndsWith(SuffixesStep1[i]) && SuffixInRn(W, R2, SuffixesStep1[i])) {
        W->End-=U8(strlen(SuffixesStep1[i]))-1-(i<19)*2;
        if (i>22) {
          W->End+=2;
          W->Letters[W->End]='t';
        }
        return true;
      }
    }
    for (; i<27; i++) {
      if (W->EndsWith(SuffixesStep1[i]) && SuffixInRn(W, R1, SuffixesStep1[i]) && IsConsonant((*W)((U8)strlen(SuffixesStep1[i])))) {
        W->End-=U8(strlen(SuffixesStep1[i]));
        return true;
      }
    }
    for (; i<29; i++) {
      if (W->EndsWith(SuffixesStep1[i]) && SuffixInRn(W, RV, SuffixesStep1[i])) {
        W->End-=U8(strlen(SuffixesStep1[i]));
        if (W->EndsWith("iv") && SuffixInRn(W, R2, "iv")) {
          W->End-=2;
          if (W->EndsWith("at") && SuffixInRn(W, R2, "at"))
            W->End-=2;
        }
        else if (W->EndsWith("eus")) {
          if (SuffixInRn(W, R2, "eus"))
            W->End-=3;
          else if (SuffixInRn(W, R1, "eus"))
            W->Letters[W->End]='x';
        }
        else if ((W->EndsWith("abl") && SuffixInRn(W, R2, "abl")) || (W->EndsWith("iqU") && SuffixInRn(W, R2, "iqU")))
          W->End-=3;
        else if ((W->EndsWith("i\xE8r") && SuffixInRn(W, RV, "i\xE8r")) || (W->EndsWith("I\xE8r") && SuffixInRn(W, RV, "I\xE8r"))) {
          W->End-=2;
          W->Letters[W->End]='i';
        }
        return true;
      }
    }
    for (; i<31; i++) {
      if (W->EndsWith(SuffixesStep1[i]) && SuffixInRn(W, R2, SuffixesStep1[i])) {
        W->End-=U8(strlen(SuffixesStep1[i]));
        if (W->EndsWith("abil")) {
          if (SuffixInRn(W, R2, "abil"))
            W->End-=4;
          else
            W->End--, W->Letters[W->End]='l';
        }
        else if (W->EndsWith("ic")) {
          if (SuffixInRn(W, R2, "ic"))
            W->End-=2;
          else
            W->ChangeSuffix("c", "qU");
        }
        else if (W->EndsWith("iv") && SuffixInRn(W, R2, "iv"))
          W->End-=2;
        return true;
      }
    }
    for (; i<35; i++) {
      if (W->EndsWith(SuffixesStep1[i]) && SuffixInRn(W, R2, SuffixesStep1[i])) {
        W->End-=U8(strlen(SuffixesStep1[i]));
        if (W->EndsWith("at") && SuffixInRn(W, R2, "at")) {
          W->End-=2;
          if (W->EndsWith("ic")) {
            if (SuffixInRn(W, R2, "ic"))
              W->End-=2;
            else
              W->ChangeSuffix("c", "qU");
          }
        }
        return true;
      }
    }
    for (; i<37; i++) {
      if (W->EndsWith(SuffixesStep1[i])) {
        if (SuffixInRn(W, R2, SuffixesStep1[i])) {
          W->End-=U8(strlen(SuffixesStep1[i]));
          return true;
        }
        else if (SuffixInRn(W, R1, SuffixesStep1[i])) {
          W->ChangeSuffix(SuffixesStep1[i], "eux");
          return true;
        }
      }
    }
    for (; i<NUM_SUFFIXES_STEP1; i++) {
      if (W->EndsWith(SuffixesStep1[i]) && SuffixInRn(W, RV+1, SuffixesStep1[i]) && IsVowel((*W)((U8)strlen(SuffixesStep1[i])))) {
        W->End-=U8(strlen(SuffixesStep1[i]));
        (*ForceStep2a) = true;
        return true;
      }
    }
    if (W->EndsWith("eaux") || (*W)=="eaux") {
      W->End--;
      W->Type|=French::Plural;
      return true;
    }
    else if (W->EndsWith("aux") && SuffixInRn(W, R1, "aux")) {
      W->End--, W->Letters[W->End] = 'l';
      W->Type|=French::Plural;
      return true;
    }
    else if (W->EndsWith("amment") && SuffixInRn(W, RV, "amment")) {
      W->ChangeSuffix("amment", "ant");
      (*ForceStep2a) = true;
      return true;
    }
    else if (W->EndsWith("emment") && SuffixInRn(W, RV, "emment")) {
      W->ChangeSuffix("emment", "ent");
      (*ForceStep2a) = true;
      return true;
    }
    return false;
  }
  bool Step2a(Word *W, const U32 RV) {
    for (int i=0; i<NUM_SUFFIXES_STEP2a; i++) {
      if (W->EndsWith(SuffixesStep2a[i]) && SuffixInRn(W, RV+1, SuffixesStep2a[i]) && IsConsonant((*W)((U8)strlen(SuffixesStep2a[i])))) {
        W->End-=U8(strlen(SuffixesStep2a[i]));
        if (i==31 /*ir*/)
          W->Type|=French::Verb;
        return true;
      }
    }
    return false;
  }
  bool Step2b(Word *W, const U32 RV, const U32 R2) {
    for (int i=0; i<NUM_SUFFIXES_STEP2b; i++) {
      if (W->EndsWith(SuffixesStep2b[i]) && SuffixInRn(W, RV, SuffixesStep2b[i])) {
        switch (SuffixesStep2b[i][0]) {
          case 'a': case '\xE2': {
            W->End-=U8(strlen(SuffixesStep2b[i]));
            if (W->EndsWith("e") && SuffixInRn(W, RV, "e"))
              W->End--;
            return true;
          }
          default: {
            if (i!=14 || SuffixInRn(W, R2, SuffixesStep2b[i])) {
              W->End-=U8(strlen(SuffixesStep2b[i]));
              return true;
            }
          }
        }
      }
    }
    return false;
  }
  void Step3(Word *W) {
    char *final = (char *)&W->Letters[W->End];
    if ((*final)=='Y')
      (*final) = 'i';
    else if ((*final)=='\xE7')
      (*final) = 'c';
  }
  bool Step4(Word *W, const U32 RV, const U32 R2) {
    bool res = false;
    if (W->Length()>=2 && W->Letters[W->End]=='s' && !CharInArray((*W)(1), SetStep4, NUM_SET_STEP4)) {
      W->End--;
      res = true;
    }
    for (int i=0; i<NUM_SUFFIXES_STEP4; i++) {
      if (W->EndsWith(SuffixesStep4[i]) && SuffixInRn(W, RV, SuffixesStep4[i])) {
        switch (i) {
          case 2: { //ion
            char prec = (*W)(3);
            if (SuffixInRn(W, R2, SuffixesStep4[i]) && SuffixInRn(W, RV+1, SuffixesStep4[i]) && (prec=='s' || prec=='t')) {
              W->End-=3;
              return true;
            }
            break;
          }
          case 5: { //e
            W->End--;
            return true;
          }
          case 6: { //\xEB
            if (W->EndsWith("gu\xEB")) {
              W->End--;
              return true;
            }
            break;
          }
          default: {
            W->ChangeSuffix(SuffixesStep4[i], "i");
            return true;
          }
        }
      }
    }
    return res;
  }
  bool Step5(Word *W) {
    for (int i=0; i<NUM_SUFFIXES_STEP5; i++) {
      if (W->EndsWith(SuffixesStep5[i])) {
        W->End--;
        return true;
      }
    }
    return false;
  }
  bool Step6(Word *W) {
    for (int i=W->End; i>=W->Start; i--) {
      if (IsVowel(W->Letters[i])) {
        if (i<W->End && (W->Letters[i]&0xFE)==0xE8) {
          W->Letters[i] = 'e';
          return true;
        }
        return false;
      }
    }
    return false;
  }
public:
  inline bool IsVowel(const char c) final {
    return CharInArray(c, Vowels, NUM_VOWELS);
  }
  bool Stem(Word *W) {
    ConvertUTF8(W);
    if (W->Length()<2) {
      W->CalculateStemHash();
      return false;
    }
    for (int i=0; i<NUM_EXCEPTIONS; i++) {
      if ((*W)==Exceptions[i][0]) {
        size_t len=strlen(Exceptions[i][1]);
        memcpy(&W->Letters[W->Start], Exceptions[i][1], len);
        W->End=W->Start+U8(len-1);
        W->CalculateStemHash();
        W->Type|=TypesExceptions[i];
        W->Language = Language::French;
        return true;
      }
    }
    MarkVowelsAsConsonants(W);
    U32 RV=GetRV(W), R1=GetRegion(W, 0), R2=GetRegion(W, R1);
    bool DoNextStep=false, res=Step1(W, RV, R1, R2, &DoNextStep);
    DoNextStep|=!res;
    if (DoNextStep) {
      DoNextStep = !Step2a(W, RV);
      res|=!DoNextStep;
      if (DoNextStep)
        res|=Step2b(W, RV, R2);
    }
    if (res)
      Step3(W);
    else
      res|=Step4(W, RV, R2);
    res|=Step5(W);
    res|=Step6(W);
    for (int i=W->Start; i<=W->End; i++)
      W->Letters[i] = tolower(W->Letters[i]);
    if (!res)
      res=W->MatchesAny(CommonWords, NUM_COMMON_WORDS);
    W->CalculateStemHash();
    if (res)
      W->Language = Language::French;
    return res;
  }
};

// German suffix stemmer, based on the Porter stemmer.

class GermanStemmer : public Stemmer {
private:
  static constexpr int NUM_VOWELS = 9;
  static constexpr char Vowels[NUM_VOWELS]={'a','e','i','o','u','y','\xE4','\xF6','\xFC'};
  static constexpr int NUM_COMMON_WORDS = 10;
  const char *CommonWords[NUM_COMMON_WORDS]={"der","die","das","und","sie","ich","mit","sich","auf","nicht"};
  static constexpr int NUM_ENDINGS = 10;
  static constexpr char Endings[NUM_ENDINGS]={'b','d','f','g','h','k','l','m','n','t'}; //plus 'r' for words ending in 's'
  static constexpr int NUM_SUFFIXES_STEP1 = 6;
  const char *SuffixesStep1[NUM_SUFFIXES_STEP1]={"em","ern","er","e","en","es"};
  static constexpr int NUM_SUFFIXES_STEP2 = 3;
  const char *SuffixesStep2[NUM_SUFFIXES_STEP2]={"en","er","est"};
  static constexpr int NUM_SUFFIXES_STEP3 = 7;
  const char *SuffixesStep3[NUM_SUFFIXES_STEP3]={"end","ung","ik","ig","isch","lich","heit"};
  void ConvertUTF8(Word *W) {
    for (int i=W->Start; i<W->End; i++) {
      U8 c = W->Letters[i+1]+((W->Letters[i+1]<0x9F)?0x60:0x40);
      if (W->Letters[i]==0xC3 && (IsVowel(c) || c==0xDF)) {
        W->Letters[i] = c;
        if (i+1<W->End)
          memmove(&W->Letters[i+1], &W->Letters[i+2], W->End-i-1);
        W->End--;
      }
    }
  }
  void ReplaceSharpS(Word *W) {
    for (int i=W->Start; i<=W->End; i++) {
      if (W->Letters[i]==0xDF) {
        W->Letters[i]='s';
        if (i<MAX_WORD_SIZE-1) {
          memmove(&W->Letters[i+2], &W->Letters[i+1], MAX_WORD_SIZE-i-2);
          W->Letters[i+1]='s';
          W->End+=(W->End<MAX_WORD_SIZE-1);
        }
      }
    }
  }
  void MarkVowelsAsConsonants(Word *W) {
    for (int i=W->Start+1; i<W->End; i++) {
      U8 c = W->Letters[i];
      if ((c=='u' || c=='y') && IsVowel(W->Letters[i-1]) && IsVowel(W->Letters[i+1]))
        W->Letters[i] = toupper(c);
    }
  }
  inline bool IsValidEnding(const char c, const bool IncludeR = false) {
    return CharInArray(c, Endings, NUM_ENDINGS) || (IncludeR && c=='r');
  }
  bool Step1(Word *W, const U32 R1) {
    int i = 0;
    for (; i<3; i++) {
      if (W->EndsWith(SuffixesStep1[i]) && SuffixInRn(W, R1, SuffixesStep1[i])) {
        W->End-=U8(strlen(SuffixesStep1[i]));
        return true;
      }
    }
    for (; i<NUM_SUFFIXES_STEP1; i++) {
      if (W->EndsWith(SuffixesStep1[i]) && SuffixInRn(W, R1, SuffixesStep1[i])) {
        W->End-=U8(strlen(SuffixesStep1[i]));
        W->End-=U8(W->EndsWith("niss"));
        return true;
      }
    }
    if (W->EndsWith("s") && SuffixInRn(W, R1, "s") && IsValidEnding((*W)(1), true)) {
      W->End--;
      return true;
    }
    return false;
  }
  bool Step2(Word *W, const U32 R1) {
    for (int i=0; i<NUM_SUFFIXES_STEP2; i++) {
      if (W->EndsWith(SuffixesStep2[i]) && SuffixInRn(W, R1, SuffixesStep2[i])) {
        W->End-=U8(strlen(SuffixesStep2[i]));
        return true;
      }
    }
    if (W->EndsWith("st") && SuffixInRn(W, R1, "st") && W->Length()>5 && IsValidEnding((*W)(2))) {
      W->End-=2;
      return true;
    }
    return false;
  }
  bool Step3(Word *W, const U32 R1, const U32 R2) {
    int i = 0;
    for (; i<2; i++) {
      if (W->EndsWith(SuffixesStep3[i]) && SuffixInRn(W, R2, SuffixesStep3[i])) {
        W->End-=U8(strlen(SuffixesStep3[i]));
        if (W->EndsWith("ig") && (*W)(2)!='e' && SuffixInRn(W, R2, "ig"))
          W->End-=2;
        if (i)
          W->Type|=German::Noun;
        return true;
      }
    }
    for (; i<5; i++) {
      if (W->EndsWith(SuffixesStep3[i]) && SuffixInRn(W, R2, SuffixesStep3[i]) && (*W)((U8)strlen(SuffixesStep3[i]))!='e') {
        W->End-=U8(strlen(SuffixesStep3[i]));
        if (i>2)
          W->Type|=German::Adjective;
        return true;
      }
    }
    for (; i<NUM_SUFFIXES_STEP3; i++) {
      if (W->EndsWith(SuffixesStep3[i]) && SuffixInRn(W, R2, SuffixesStep3[i])) {
        W->End-=U8(strlen(SuffixesStep3[i]));
        if ((W->EndsWith("er") || W->EndsWith("en")) && SuffixInRn(W, R1, "e?"))
          W->End-=2;
        if (i>5)
          W->Type|=German::Noun|German::Female;
        return true;
      }
    }
    if (W->EndsWith("keit") && SuffixInRn(W, R2, "keit")) {
      W->End-=4;
      if (W->EndsWith("lich") && SuffixInRn(W, R2, "lich"))
        W->End-=4;
      else if (W->EndsWith("ig") && SuffixInRn(W, R2, "ig"))
        W->End-=2;
      W->Type|=German::Noun|German::Female;
      return true;
    }
    return false;
  }
public:
  inline bool IsVowel(const char c) final {
    return CharInArray(c, Vowels, NUM_VOWELS);
  }
  bool Stem(Word *W) {
    ConvertUTF8(W);
    if (W->Length()<2) {
      W->CalculateStemHash();
      return false;
    }
    ReplaceSharpS(W);
    MarkVowelsAsConsonants(W);
    U32 R1=GetRegion(W, 0), R2=GetRegion(W, R1);
    R1 = min(3, R1);
    bool res = Step1(W, R1);
    res|=Step2(W, R1);
    res|=Step3(W, R1, R2);
    for (int i=W->Start; i<=W->End; i++) {
      switch (W->Letters[i]) {
        case 0xE4: { W->Letters[i] = 'a'; break; }
        case 0xF6: case 0xFC: { W->Letters[i]-=0x87; break; }
        default: W->Letters[i] = tolower(W->Letters[i]);
      }
    }
    if (!res)
      res=W->MatchesAny(CommonWords, NUM_COMMON_WORDS);
    W->CalculateStemHash();
    if (res)
      W->Language = Language::German;
    return res;
  }
};

#pragma pack(push,1)
struct Entry {
  short prefix;
  U8 suffix;
  bool termination;
  U32 embedding;
};
#pragma pack(pop)

class WordEmbeddingDictionary{
private:
  static constexpr int HashSize = 81929;
  Array<Entry> entries;
  Array<short> table;
  int index;
#ifndef NVERBOSE
  U32 requests, hits;
#endif
  int findEntry(const short prefix, const U8 suffix){
    int i = hash(prefix, suffix)%HashSize;
    int offset = (i>0)?HashSize-i:1;
    while (true){
      if (table[i]<0) //free slot?
        return -i-1;
      else if (entries[table[i]].prefix==prefix && entries[table[i]].suffix==suffix) //is it the entry we want?
        return table[i];
      i-=offset;
      if (i<0)
        i+=HashSize;
    }
  }
  void addEntry(const short prefix, const U8 suffix, const int offset = -1){
    if (prefix==-1 || prefix>=index || index>0x7FFF || offset>=0)
      return;
    entries[index].prefix = prefix;
    entries[index].suffix = suffix;
    table[-offset-1] = index;
    index+=(index<0x8000);
  }
public:
  WordEmbeddingDictionary(): entries(0x8000), table(HashSize), index(0) { reset(); }
  ~WordEmbeddingDictionary() {
  #ifndef NVERBOSE
    if (requests>0)
      printf("\nHits: %d, Requests: %d, %.2f%%\n", hits, requests, (hits*100.0)/requests);
  #endif
  }
  void reset(){
    for (index=0; index<HashSize; table[index]=-1, index++);
    for (index=0; index<256; index++){
      table[-findEntry(-1, index)-1] = index;
      entries[index].prefix = -1;
      entries[index].suffix = index;
    }
  #ifndef NVERBOSE
    requests = hits = 0;
  #endif
  }
  bool addWord(const Word *W, const U32 embedding){
    bool res = false;
    int parent=-1, code=0, len=W->Length();
    if (len==0) return res;
    for (int i=0; i<len; i++){
      int idx = findEntry(parent, code=(*W)[i]);
      if (idx<0){
        addEntry(parent, code, idx);
        parent = index-1;
        res = true;
      }
      else
        parent = idx;
    }
    assert(parent>=0);
    if (!res)
      res=!entries[parent].termination;
    entries[parent].termination = true;
    entries[parent].embedding = embedding;
    return res;
  }
  void getWordEmbedding(Word *W) {
    int parent = -1;
  #ifndef NVERBOSE
    requests++;
  #endif
    W->Embedding = -1;
    for (U32 i=0; i<W->Length(); i++){
      if ((parent = findEntry(parent, (*W)[i]))<0)
        return;
    }
    if (!entries[parent].termination)
      return;
    W->Embedding = entries[parent].embedding;
  #ifndef NVERBOSE
    hits++;
  #endif
  }
  void loadFromFile(const char* filename) {
    FileDisk f;
#ifndef NVERBOSE
    if (to_screen) printf("\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b");
    printf("Loading word embeddings...");
#endif
    OpenFromMyFolder::anotherfile(&f, filename);
    Word W;
    int byte=0, embedding=0, total=0;
    do {
      if (f.blockread((U8*)(&embedding), WORD_EMBEDDING_SIZE)!=WORD_EMBEDDING_SIZE)
        break;
      W.reset();
      while ((byte=f.getchar())>=0 && byte!=0x0A)
        W+=byte;
      if (addWord(&W, embedding)) total++;
    } while (byte>=0);
  #ifndef NVERBOSE
    printf(" done [%s, %d words]\n", filename, total);
  #endif
    f.close();
  }
};

#endif //USE_TEXTMODEL

//////////////////////////// Models //////////////////////////////

// All of the models below take a Mixer as a parameter and write
// predictions to it.


//////////////////////////// TextModel ///////////////////////////

#ifdef USE_TEXTMODEL

template <class T, const U32 Size> class Cache {
  static_assert(Size>1 && ispowerof2(Size), "Cache size must be a power of 2 bigger than 1");
private:
  Array<T> Data;
  U32 Index;
public:
  static const U32 size = Size;
  explicit Cache() : Data(Size) { Index=0; }
  T& operator()(U32 i) {
    return Data[(Index-i)&(Size-1)];
  }
  void operator++(int) {
    Index++;
  }
  void operator--(int) {
    Index--;
  }
  T& Next() {
    return Index++, *((T*)memset(&Data[Index&(Size-1)], 0, sizeof(T)));
  }
};

static constexpr U8 AsciiGroupC0[2][254] ={
  {
    0, 10,
    0, 1, 10, 10,
    0, 4, 2, 3, 10, 10, 10, 10,
    0, 0, 5, 4, 2, 2, 3, 3, 10, 10, 10, 10, 10, 10, 10, 10,
    0, 0, 0, 0, 5, 5, 9, 4, 2, 2, 2, 2, 3, 3, 3, 3, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10,
    0, 0, 0, 0, 0, 0, 0, 0, 5, 8, 8, 5, 9, 9, 6, 5, 2, 2, 2, 2, 2, 2, 2, 8, 3, 3, 3, 3, 3, 3, 3, 8, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7, 8, 8, 8, 8, 8, 5, 5, 9, 9, 9, 9, 9, 7, 8, 5, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 8, 8, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 8, 8, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10
  },
  {
    0, 6,
    0, 1, 6, 6,
    4, 5, 1, 1, 6, 6, 6, 6,
    4, 0, 3, 2, 1, 1, 1, 1, 6, 6, 6, 6, 6, 6, 6, 6,
    0, 4, 0, 0, 3, 3, 2, 5, 1, 1, 1, 1, 1, 1, 1, 1, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6,
    0, 0, 4, 4, 0, 0, 0, 0, 3, 3, 3, 3, 2, 2, 5, 5, 1, 1, 1, 1, 1, 1, 1, 3, 1, 1, 1, 1, 1, 1, 1, 3, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6,
    0, 0, 0, 0, 4, 4, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 3, 3, 3, 3, 3, 3, 3, 2, 2, 2, 2, 2, 3, 3, 3, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 3, 3, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 3, 3, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6
  }
};
static constexpr U8 AsciiGroup[128] = {
  0,  5,  5,  5,  5,  5,  5,  5,
  5,  5,  4,  5,  5,  4,  5,  5,
  5,  5,  5,  5,  5,  5,  5,  5,
  5,  5,  5,  5,  5,  5,  5,  5,
  6,  7,  8, 17, 17,  9, 17, 10,
  11, 12, 17, 17, 13, 14, 15, 16,
  1,  1,  1,  1,  1,  1,  1,  1,
  1,  1, 18, 19, 20, 23, 21, 22,
  23,  2,  2,  2,  2,  2,  2,  2,
  2,  2,  2,  2,  2,  2,  2,  2,
  2,  2,  2,  2,  2,  2,  2,  2,
  2,  2,  2, 24, 27, 25, 27, 26,
  27,  3,  3,  3,  3,  3,  3,  3,
  3,  3,  3,  3,  3,  3,  3,  3,
  3,  3,  3,  3,  3,  3,  3,  3,
  3,  3,  3, 28, 30, 29, 30, 30
};

class TextModel {
private:
  static constexpr int nCM2 = 28;
public:
  static constexpr int MIXERINPUTS = nCM2*ContextMap2::MIXERINPUTS; // 196
  static constexpr int MIXERCONTEXTS = 2048+2048+4096+4096+2048+2048+4096+8192+2048; //30720
  static constexpr int MIXERCONTEXTSETS = 9;
private:
  const Shared * const shared;
  ModelStats *stats;
  static constexpr U32 MIN_RECOGNIZED_WORDS = 4;
  ContextMap2 cm;
  Array<Stemmer*> Stemmers;
  Array<Language*> Languages;
  Array<WordEmbeddingDictionary*> Dictionaries;
  Cache<Word, 8> Words[Language::Count];
  Cache<Segment, 4> Segments;
  Cache<Sentence, 4> Sentences;
  Cache<Paragraph, 2> Paragraphs;
  Array<U32> WordPos;
  U32 BytePos[256];
  struct WordDistance {
    U32 distance[4];
    U32 closest;
  };
  Cache<WordDistance, 4> WordDistances;
  Word *cWord, *pWord; // current word, previous word
  Segment *cSegment; // current segment
  Sentence *cSentence; // current sentence
  Paragraph *cParagraph; // current paragraph
  enum Parse {
    Unknown,
    ReadingWord,
    PossibleHyphenation,
    WasAbbreviation,
    AfterComma,
    AfterQuote,
    AfterAbbreviation,
    ExpectDigit
  } State, pState;
  struct {
    U32 Count[Language::Count-1]; // number of recognized words of each language in the last 64 words seen
    U64 Mask[Language::Count-1];  // binary mask with the recognition status of the last 64 words for each language
    int Id;  // current detected language
    int pId; // detected language of the previous word
  } Lang;
  struct {
    U64 numbers[2];   // last 2 numbers seen
    U64 numHashes[2]; // hashes of the last 2 numbers seen
    U8  numLength[2]; // digit length of last 2 numbers seen
    U32 numMask;      // binary mask of the results of the arithmetic comparisons between the numbers seen
    U32 numDiff;      // log2 of the consecutive differences between the last 16 numbers seen, clipped to 2 bits per difference
    U32 lastUpper;    // distance to last uppercase letter
    U32 maskUpper;    // binary mask of uppercase letters seen (in the last 32 bytes)
    U32 lastLetter;   // distance to last letter
    U32 lastDigit;    // distance to last digit
    U32 lastPunct;    // distance to last punctuation character
    U32 lastNewLine;  // distance to last new line character
    U32 prevNewLine;  // distance to penultimate new line character
    U32 wordGap;      // distance between the last words
    U32 spaces;       // binary mask of whitespace characters seen (in the last 32 bytes)
    U32 spaceCount;   // count of whitespace characters seen (in the last 32 bytes)
    U32 commas;       // number of commas seen in this line (not this segment/sentence)
    U32 quoteLength;  // length (in words) of current quote
    U32 maskPunct;    // mask of relative position of last comma related to other punctuation
    U32 nestHash;     // hash representing current nesting state
    U32 lastNest;     // distance to last nesting character
    U32 masks[5],
        wordLength[2];
    int UTF8Remaining;// remaining bytes for current UTF8-encoded Unicode code point (-1 if invalid byte found)
    U8 firstLetter;   // first letter of current word
    U8 firstChar;     // first character of current line
    U8 expectedDigit; // next expected digit of detected numerical sequence
    U8 prevPunct;     // most recent punctuation character seen
    Word TopicDescriptor; // last word before ':'
  } Info;
  U64 ParseCtx;       // state of parser + relevant features used as a context (hash) 
  void SetContexts();
public:
  TextModel(const Shared * const sh, ModelStats *st, const U64 Size) : 
    shared(sh), 
    stats(st),
    cm(sh,Size,nCM2,64),
    Stemmers(Language::Count-1),
    Languages(Language::Count-1),
    Dictionaries(Language::Count-1),
    WordPos(0x10000),
    State(Parse::Unknown),
    pState(State),
    Lang{ {0}, {0}, Language::Unknown, Language::Unknown },
    Info{},
    ParseCtx(0)
  {
    Stemmers[Language::English-1] = new EnglishStemmer();
    Stemmers[Language::French-1] = new FrenchStemmer();
    Stemmers[Language::German-1] = new GermanStemmer();
    Languages[Language::English-1] = new English();
    Languages[Language::French-1] = new French();
    Languages[Language::German-1] = new German();
    cWord = &Words[Lang.Id](0);
    pWord = &Words[Lang.Id](1);
    cSegment = &Segments(0);
    cSentence = &Sentences(0);
    cParagraph = &Paragraphs(0);
    memset(&BytePos[0], 0, sizeof(BytePos));
  }
  ~TextModel() {
    for (int i=0; i<Language::Count-1; i++) {
      delete Stemmers[i];
      delete Languages[i];
      delete Dictionaries[i];
    }
  }
  void Update();
  void mix(Mixer& m) {
    INJECT_SHARED_bpos 
    if (bpos==0) {
      Update();
      SetContexts();
    }
    cm.mix(m);

    const U8 chargrp = stats->Text.chargrp;
    INJECT_SHARED_c1 INJECT_SHARED_c0
    m.set(finalize64(hash((Lang.Id!=Language::Unknown)?1+Stemmers[Lang.Id-1]->IsVowel(c1):0, Info.masks[1]&0xFF, c0),11), 2048);
    m.set(finalize64(hash(ilog2(Info.wordLength[0]+1), c0,
      (Info.lastDigit<Info.wordLength[0]+Info.wordGap)|
      ((Info.lastUpper<Info.lastLetter+Info.wordLength[1])<<1)|
      ((Info.lastPunct<Info.wordLength[0]+Info.wordGap)<<2)|
      ((Info.lastUpper<Info.wordLength[0])<<3)
    ),11), 2048);
    m.set(finalize64(hash(Info.masks[1]&0x3FF, chargrp, Info.lastUpper<Info.wordLength[0], Info.lastUpper<Info.lastLetter+Info.wordLength[1]),12), 4096);
    m.set(finalize64(hash(Info.spaces&0x1FF, chargrp,
      (Info.lastUpper<Info.wordLength[0])|
      ((Info.lastUpper<Info.lastLetter+Info.wordLength[1])<<1)|
      ((Info.lastPunct<Info.lastLetter)<<2)|
      ((Info.lastPunct<Info.wordLength[0]+Info.wordGap)<<3)|
      ((Info.lastPunct<Info.lastLetter+Info.wordLength[1]+Info.wordGap)<<4)
    ),12), 4096);
    m.set(finalize64(hash(Info.firstLetter*(Info.wordLength[0]<4), min(6, Info.wordLength[0]), c0),11), 2048);
    m.set(finalize64(hash((*pWord)[0], (*pWord)(0), min(4, Info.wordLength[0]), Info.lastPunct<Info.lastLetter),11), 2048);
    m.set(finalize64(hash(min(4, Info.wordLength[0]), chargrp,
      Info.lastUpper<Info.wordLength[0],
      (Info.nestHash>0)?Info.nestHash&0xFF:0x100|(Info.firstLetter*(Info.wordLength[0]>0 && Info.wordLength[0]<4))
    ),12), 4096);
    m.set(finalize64(hash(chargrp, Info.masks[4]&0x1F, (Info.masks[4]>>5)&0x1F), 13), 8192);
    m.set(finalize64(hash(chargrp, U8(pWord->Embedding), Lang.Id, State), 11), 2048);
  }
};

void TextModel::Update() {
  Info.lastUpper  = min(0xFF, Info.lastUpper+1), Info.maskUpper<<=1;
  Info.lastLetter = min(0x1F, Info.lastLetter+1);
  Info.lastDigit  = min(0xFF, Info.lastDigit+1);
  Info.lastPunct  = min(0x3F, Info.lastPunct+1);
  Info.lastNewLine++; Info.prevNewLine++; Info.lastNest++;
  Info.spaceCount-=(Info.spaces>>31); Info.spaces<<=1;
  Info.masks[0]<<=2; Info.masks[1]<<=2; Info.masks[2]<<=4; Info.masks[3]<<=3;
  pState = State;
  
  INJECT_SHARED_buf  
  U8 c = buf(1), lc = tolower(c), g = (c<0x80)?AsciiGroup[c]:31;
  if (g>4 || g!=(Info.masks[4]&0x1F))
    Info.masks[4]<<=5, Info.masks[4]|=g;
  INJECT_SHARED_pos
  BytePos[c] = pos;
  if (c!=lc) {
    c = lc;
    Info.lastUpper = 0, Info.maskUpper|=1;
  }
  U8 pC = buf(2);
  State = Parse::Unknown;
  ParseCtx = hash(State, pWord->Hash[0], c, (ilog2(Info.lastNewLine)+1)*(Info.lastNewLine*3>Info.prevNewLine), Info.masks[1]&0xFC);

  if ((c>='a' && c<='z') || c==APOSTROPHE || c=='-' || c>0x7F) {
    if (Info.wordLength[0]==0) {
      // check for hyphenation with "+" (book1 from Calgary)
      if (pC==NEW_LINE && ((Info.lastLetter==3 && buf(3)=='+') || (Info.lastLetter==4 && buf(3)==CARRIAGE_RETURN && buf(4)=='+'))) {
        Info.wordLength[0] = Info.wordLength[1];
        for (int i=Language::Unknown; i<Language::Count; i++)
          Words[i]--;
        cWord = pWord, pWord = &Words[Lang.pId](1);
        cWord->reset();
        for (U32 i=0; i<Info.wordLength[0]; i++)
          (*cWord)+=buf(Info.wordLength[0]-i+Info.lastLetter);
        Info.wordLength[1] = (*pWord).Length();
        cSegment->WordCount--;
        cSentence->WordCount--;
      }
      else {
        Info.wordGap = Info.lastLetter;
        Info.firstLetter = c;
      }
    }
    Info.lastLetter = 0;
    Info.wordLength[0]++;
    Info.masks[0]+=(Lang.Id!=Language::Unknown)?1+Stemmers[Lang.Id-1]->IsVowel(c):1, Info.masks[1]++, Info.masks[3]+=Info.masks[0]&3;
    if (c==APOSTROPHE) {
      Info.masks[2]+=12;
      if (Info.wordLength[0]==1) {
        if (Info.quoteLength==0 && pC==SPACE)
          Info.quoteLength = 1;
        else if (Info.quoteLength>0 && Info.lastPunct==1) {
          Info.quoteLength = 0;
          State = Parse::AfterQuote;
          ParseCtx = hash(State, pC);
        }
      }
    }
    (*cWord)+=c;
    cWord->CalculateWordHash();
    State = Parse::ReadingWord;
    ParseCtx = hash(State, cWord->Hash[0]);
  }
  else {
    if (cWord->Length()>0) {
      if (Lang.Id!=Language::Unknown)
        memcpy(&Words[Language::Unknown](0), cWord, sizeof(Word));

      for (int i=Language::Count-1; i>Language::Unknown; i--) {
        Lang.Count[i-1]-=(Lang.Mask[i-1]>>63), Lang.Mask[i-1]<<=1;
        if (i!=Lang.Id)
          memcpy(&Words[i](0), cWord, sizeof(Word));
        if (Stemmers[i-1]->Stem(&Words[i](0)))
          Lang.Count[i-1]++, Lang.Mask[i-1]|=1;
      }
      Lang.Id = Language::Unknown;
      U32 best = MIN_RECOGNIZED_WORDS;
      for (int i=Language::Count-1; i>Language::Unknown; i--) {
        if (Lang.Count[i-1]>=best) {
          best = Lang.Count[i-1] + (i==Lang.pId); //bias to prefer the previously detected language
          Lang.Id = i;
        }
        Words[i]++;
      }
      Words[Language::Unknown]++;

      if (Lang.Id!=Lang.pId) {
        #ifndef NVERBOSE
        if (to_screen) printf("\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b");
        switch (Lang.Id) {
          case Language::Unknown: { printf("[Language: Unknown, blpos: %d]\n",blpos); break; };
          case Language::English: { printf("[Language: English, blpos: %d]\n",blpos); break; };
          case Language::French : { printf("[Language: French,  blpos: %d]\n",blpos); break; };
          case Language::German : { printf("[Language: German,  blpos: %d]\n",blpos); break; };
        }
        #endif
        if (options&OPTION_TRAINTXT && Lang.Id!=Language::Unknown && Dictionaries[Lang.Id-1]==nullptr) {
          switch (Lang.Id) {
            case Language::English: {
              Dictionaries[Lang.Id-1] = new WordEmbeddingDictionary();
              Dictionaries[Lang.Id-1]->loadFromFile("english.emb");
            }
          }
        }
      }

      Lang.pId = Lang.Id;
      pWord = &Words[Lang.Id](1);
      cWord = &Words[Lang.Id](0);
      cWord->reset();
      if (options&OPTION_TRAINTXT) {
        if (Lang.Id!=Language::Unknown && Dictionaries[Lang.Id-1]!=nullptr)
          Dictionaries[Lang.Id-1]->getWordEmbedding(pWord);
        WordDistances++;
        for (U32 i=WordDistances.size-1; i>0; i--)
          memcpy(&WordDistances(i), &WordDistances(i-1), sizeof(WordDistance));
        U32 minDistance = UINT32_MAX;
        for (U32 i=1; i<=4; i++) {
          U32 d = pWord->DistanceTo(Words[Lang.Id](i+1));
          WordDistances(0).distance[i-1] = d;
          if (d<minDistance) {
            minDistance = d;
            WordDistances(0).closest = Words[Lang.Id](i+1).Embedding;
          }
        }
      }
      WordPos[pWord->Hash[0]&(WordPos.size()-1)] = pos;
      if (cSegment->WordCount==0)
        memcpy(&cSegment->FirstWord, pWord, sizeof(Word));
      cSegment->WordCount++;
      if (cSentence->WordCount==0)
        memcpy(&cSentence->FirstWord, pWord, sizeof(Word));
      cSentence->WordCount++;
      Info.wordLength[1] = Info.wordLength[0], Info.wordLength[0] = 0;
      Info.quoteLength+=(Info.quoteLength>0);
      if (Info.quoteLength>0x1F)
        Info.quoteLength = 0;
      cSentence->VerbIndex++, cSentence->NounIndex++, cSentence->CapitalIndex++;
      if ((pWord->Type&Language::Verb)!=0) {
        cSentence->VerbIndex = 0;
        memcpy(&cSentence->lastVerb, pWord, sizeof(Word));
      }
      if ((pWord->Type&Language::Noun)!=0) {
        cSentence->NounIndex = 0;
        memcpy(&cSentence->lastNoun, pWord, sizeof(Word));
      }
      if (cSentence->WordCount>1 && Info.lastUpper<Info.wordLength[1]) {
        cSentence->CapitalIndex = 0;
        memcpy(&cSentence->lastCapital, pWord, sizeof(Word));
      }
    }
    bool skip = false;
    switch (c) {
      case '.': {
        if (Lang.Id!=Language::Unknown && Info.lastUpper==Info.wordLength[1] && Languages[Lang.Id-1]->IsAbbreviation(pWord)) {
          State = Parse::WasAbbreviation;
          ParseCtx = hash(State, pWord->Hash[0]);
          break;
        }
      }
      case '?': case '!': {
        cSentence->Type = (c=='.')?Sentence::Types::Declarative:(c=='?')?Sentence::Types::Interrogative:Sentence::Types::Exclamative;
        cSentence->SegmentCount++;
        cParagraph->SentenceCount++;
        cParagraph->TypeCount[cSentence->Type]++;
        cParagraph->TypeMask<<=2, cParagraph->TypeMask|=cSentence->Type;
        cSentence = &Sentences.Next();
        Info.masks[3]+=3;
        skip = true;
      }
      case ',': case ';': case ':': {
        if (c==',') {
          Info.commas++;
          State = Parse::AfterComma;
          ParseCtx = hash(State, ilog2(Info.quoteLength+1), ilog2(Info.lastNewLine), Info.lastUpper<Info.lastLetter+Info.wordLength[1]);
        }
        else if (c==':')
          memcpy(&Info.TopicDescriptor, pWord, sizeof(Word));
        if (!skip) {
          cSentence->SegmentCount++;
          Info.masks[3]+=4;
        }
        Info.lastPunct = 0, Info.prevPunct = c;
        Info.masks[0]+=3, Info.masks[1]+=2, Info.masks[2]+=15;
        cSegment = &Segments.Next();
        break;
      }
      case NEW_LINE: {
        Info.prevNewLine = Info.lastNewLine, Info.lastNewLine = 0;
        Info.commas = 0;
        if (Info.prevNewLine==1 || (Info.prevNewLine==2 && pC==CARRIAGE_RETURN))
          cParagraph = &Paragraphs.Next();
        else if ((Info.lastLetter==2 && pC=='+') || (Info.lastLetter==3 && pC==CARRIAGE_RETURN && buf(3)=='+')) {
          ParseCtx = hash(Parse::ReadingWord, pWord->Hash[0]);
          State = Parse::PossibleHyphenation;
        }
      }
      case TAB: case CARRIAGE_RETURN: case SPACE: {
        Info.spaceCount++, Info.spaces|=1;
        Info.masks[1]+=3, Info.masks[3]+=5;
        if (c==SPACE && pState==Parse::WasAbbreviation) {
          State = Parse::AfterAbbreviation;
          ParseCtx = hash(State, pWord->Hash[0]);
        }
        break;
      }
      case '(' : Info.masks[2]+=1; Info.masks[3]+=6; Info.nestHash+=31; Info.lastNest=0; break;
      case '[' : Info.masks[2]+=2; Info.nestHash+=11; Info.lastNest=0; break;
      case '{' : Info.masks[2]+=3; Info.nestHash+=17; Info.lastNest=0; break;
      case '<' : Info.masks[2]+=4; Info.nestHash+=23; Info.lastNest=0; break;
      case 0xAB: Info.masks[2]+=5; break;
      case ')' : Info.masks[2]+=6; Info.nestHash-=31; Info.lastNest=0; break;
      case ']' : Info.masks[2]+=7; Info.nestHash-=11; Info.lastNest=0; break;
      case '}' : Info.masks[2]+=8; Info.nestHash-=17; Info.lastNest=0; break;
      case '>' : Info.masks[2]+=9; Info.nestHash-=23; Info.lastNest=0; break;
      case 0xBB: Info.masks[2]+=10; break;
      case QUOTE: {
        Info.masks[2]+=11;
        // start/stop counting
        if (Info.quoteLength==0)
          Info.quoteLength = 1;
        else {
          Info.quoteLength = 0;
          State = Parse::AfterQuote;
          ParseCtx = hash(State, 0x100|pC);
        }
        break;
      }
      case '/' : case '-': case '+': case '*': case '=': case '%': Info.masks[2]+=13; break;
      case '\\': case '|': case '_': case '@': case '&': case '^': Info.masks[2]+=14; break;
    }
    if (c>='0' && c<='9') {
      Info.numbers[0] = Info.numbers[0]*10 + (c&0xF), Info.numLength[0] = min(19, Info.numLength[0]+1);
      Info.numHashes[0] = combine64(Info.numHashes[0], c);
      Info.expectedDigit = -1;
      if (Info.numLength[0]<Info.numLength[1] && (pState==Parse::ExpectDigit || ((Info.numDiff&3)==0 && Info.numLength[0]<=1))) {
        U64 ExpectedNum = Info.numbers[1]+(Info.numMask&3)-2, PlaceDivisor=1;
        for (int i=0; i<Info.numLength[1]-Info.numLength[0]; i++, PlaceDivisor*=10);
        if (ExpectedNum/PlaceDivisor==Info.numbers[0]) {
          PlaceDivisor/=10;
          Info.expectedDigit = (ExpectedNum/PlaceDivisor)%10;
          State = Parse::ExpectDigit;
        }
      }
      else {
        U8 d = buf(Info.numLength[0]+2);
        if (Info.numLength[0]<3 && buf(Info.numLength[0]+1)==',' && d>='0' && d<='9')
          State = Parse::ExpectDigit;
      }
      Info.lastDigit = 0;
      Info.masks[3]+=7;
    }
    else if (Info.numbers[0]>0) {
      Info.numMask<<=2, Info.numMask|=1+(Info.numbers[0]>=Info.numbers[1])+(Info.numbers[0]>Info.numbers[1]);
      Info.numDiff<<=2, Info.numDiff|=min(3,ilog2(abs((int)(Info.numbers[0]-Info.numbers[1]))));
      Info.numbers[1] = Info.numbers[0], Info.numbers[0] = 0;
      Info.numHashes[1] = Info.numHashes[0], Info.numHashes[0] = 0;
      Info.numLength[1] = Info.numLength[0], Info.numLength[0] = 0;
      cSegment->NumCount++, cSentence->NumCount++;
    }
  }
  if (Info.lastNewLine==1)
    Info.firstChar = (Lang.Id!=Language::Unknown)?c:min(c,96);
  if (Info.lastNest>512)
    Info.nestHash = 0;
  int leadingBitsSet = 0;
  while (((c>>(7-leadingBitsSet))&1)!=0) leadingBitsSet++;

  if (Info.UTF8Remaining>0 && leadingBitsSet==1)
    Info.UTF8Remaining--;
  else
    Info.UTF8Remaining = (leadingBitsSet!=1)?(c!=0xC0 && c!=0xC1 && c<0xF5)?(leadingBitsSet-(leadingBitsSet>0)):-1:0;
  Info.maskPunct = (BytePos[',']>BytePos['.'])|((BytePos[',']>BytePos['!'])<<1)|((BytePos[',']>BytePos['?'])<<2)|((BytePos[',']>BytePos[':'])<<3)|((BytePos[',']>BytePos[';'])<<4);

  stats->Text.firstLetter = Info.firstLetter;
  stats->Text.mask = Info.masks[1]&0xFF;
}

void TextModel::SetContexts() {
  INJECT_SHARED_buf
  const U8 c = buf(1), lc = tolower(c), m2 = Info.masks[2]&0xF, column = min(0xFF, Info.lastNewLine);
  const U64 w = State==Parse::ReadingWord ? cWord->Hash[0] : pWord->Hash[0];

  const U64 cWordHash0 = cWord->Hash[0];
  const U64 pWordHash0 = pWord->Hash[0];
  const U64 pWordHash1 = pWord->Hash[1];

  cm.set(ParseCtx);
  U64 i = State*64;
  cm.set(hash(++i, cWordHash0, pWordHash0,
    (Info.lastUpper<Info.wordLength[0])|
    ((Info.lastDigit<Info.wordLength[0]+Info.wordGap)<<1)
  ));
  cm.set(hash(++i, cWordHash0, Words[Lang.pId](2).Hash[0], min(10,ilog2((U32)Info.numbers[0])),
    (Info.lastUpper<Info.lastLetter+Info.wordLength[1])|
    ((Info.lastLetter>3)<<1)|
    ((Info.lastLetter>0 && Info.wordLength[1]<3)<<2)
  ));
  cm.set(hash(++i, cWordHash0, Info.masks[1]&0x3FF, Words[Lang.pId](3).Hash[1],
    (Info.lastDigit<Info.wordLength[0]+Info.wordGap)|
    ((Info.lastUpper<Info.lastLetter+Info.wordLength[1])<<1)|
    ((Info.spaces&0x7F)<<2)
  ));
  cm.set(hash(++i, cWordHash0, pWordHash1));
  cm.set(hash(++i, cWordHash0, pWordHash1, Words[Lang.pId](2).Hash[1]));
  cm.set(hash(++i, w, Words[Lang.pId](2).Hash[0], Words[Lang.pId](3).Hash[0]));
  cm.set(hash(++i, cWordHash0, c, (cSentence->VerbIndex<cSentence->WordCount)?cSentence->lastVerb.Hash[0]:0));
  cm.set(hash(++i, pWordHash1, Info.masks[1]&0xFC, lc, Info.wordGap));
  cm.set(hash(++i, (Info.lastLetter==0)?cWordHash0:pWordHash0, c, cSegment->FirstWord.Hash[1], min(3,ilog2(cSegment->WordCount+1))));
  cm.set(hash(++i, cWordHash0, c, Segments(1).FirstWord.Hash[1]));
  cm.set(hash(++i, max(31,lc), Info.masks[1]&0xFFC, (Info.spaces&0xFE)|(Info.lastPunct<Info.lastLetter), (Info.maskUpper&0xFF)|(((0x100|Info.firstLetter)*(Info.wordLength[0]>1))<<8)));
  cm.set(hash(++i, column, min(7,ilog2(Info.lastUpper+1)), ilog2(Info.lastPunct+1)));
  cm.set(hash(++i,
      (column&0xF8)|(Info.masks[1]&3)|((Info.prevNewLine-Info.lastNewLine>63)<<2)|
      (min(3, Info.lastLetter)<<8)|
      (Info.firstChar<<10)|
      ((Info.commas>4)<<18)|
      ((m2>=1 && m2<=5)<<19)|
      ((m2>=6 && m2<=10)<<20)|
      ((m2==11 || m2==12)<<21)|
      ((Info.lastUpper<column)<<22)|
      ((Info.lastDigit<column)<<23)|
      ((column<Info.prevNewLine-Info.lastNewLine)<<24)
    )
  );
  cm.set(hash(++i,
    (2*column)/3,
    min(13, Info.lastPunct)+(Info.lastPunct>16)+(Info.lastPunct>32)+Info.maskPunct*16,
    ilog2(Info.lastUpper+1),
    ilog2(Info.prevNewLine-Info.lastNewLine),
    ((Info.masks[1]&3)==0)|
    ((m2<6)<<1)|
    ((m2<11)<<2)
  ));
  cm.set(hash(++i, column>>1, Info.spaces&0xF));
  cm.set(hash(++i,
    Info.masks[3]&0x3F,
    min((max(Info.wordLength[0],3)-2)*(Info.wordLength[0]<8),3),
    Info.firstLetter*(Info.wordLength[0]<5),
    w,
    (c==buf(2))|
    ((Info.masks[2]>0)<<1)|
    ((Info.lastPunct<Info.wordLength[0]+Info.wordGap)<<2)|
    ((Info.lastUpper<Info.wordLength[0])<<3)|
    ((Info.lastDigit<Info.wordLength[0]+Info.wordGap)<<4)|
    ((Info.lastPunct<2+Info.wordLength[0]+Info.wordGap+Info.wordLength[1])<<5)
  ));
  cm.set(hash(++i, w, c, Info.numHashes[1]));
  INJECT_SHARED_pos
  cm.set(hash(++i, w, c, llog(pos-WordPos[w&(WordPos.size()-1)])>>1));
  cm.set(hash(++i, w, c, Info.TopicDescriptor.Hash[0]));
  cm.set(hash(++i, Info.numLength[0], c, Info.TopicDescriptor.Hash[0]));
  cm.set(hash(++i, (Info.lastLetter>0)?c:0x100, Info.masks[1]&0xFFC, Info.nestHash&0x7FF));
  cm.set(hash(++i, w, c, Info.masks[3]&0x1FF,
    ((cSentence->VerbIndex==0 && cSentence->lastVerb.Length()>0)<<6)|
    ((Info.wordLength[1]>3)<<5)|
    ((cSegment->WordCount==0)<<4)|
    ((cSentence->SegmentCount==0 && cSentence->WordCount<2)<<3)|
    ((Info.lastPunct>=Info.lastLetter+Info.wordLength[1]+Info.wordGap)<<2)|
    ((Info.lastUpper<Info.lastLetter+Info.wordLength[1])<<1)|
    (Info.lastUpper<Info.wordLength[0]+Info.wordGap+Info.wordLength[1])
  ));
  cm.set(hash(++i, c, pWordHash1, Info.firstLetter*(Info.wordLength[0]<6),
    ((Info.lastPunct<Info.wordLength[0]+Info.wordGap)<<1)|
    (Info.lastPunct>=Info.lastLetter+Info.wordLength[1]+Info.wordGap)
  ));
  cm.set(hash(++i, w, c, Words[Lang.pId](1+(Info.wordLength[0]==0)).Letters[Words[Lang.pId](1+(Info.wordLength[0]==0)).Start], Info.firstLetter*(Info.wordLength[0]<7)));
  cm.set(hash(++i, column, Info.spaces&7, Info.nestHash&0x7FF));
  cm.set(hash(++i, cWordHash0, (Info.lastUpper<column)|((Info.lastUpper<Info.wordLength[0])<<1), min(5, Info.wordLength[0])));
  cm.set(hash(++i, Lang.Id, w,
    U8(Words[Lang.Id](1+(State!=Parse::ReadingWord)).Embedding),
    (Info.lastUpper<Info.wordLength[0])|
    ((cSegment->WordCount==0)<<1)
  ));
  assert(i-State*64+1==nCM2);
}

#endif //USE_TEXTMODEL

//////////////////////////// MatchModel ///////////////////////////

//
// Predict the next bit based on a preceding long matching byte sequence
//
// This model monitors byte sequences and keeps their most recent 
// positions in a hashtable.
// When the current byte sequence matches an older sequence (having 
// the same hash) the model predicts the forthcoming bits.

class MatchModel {
private:
  static constexpr int NumHashes = 3;
  static constexpr int nCM = 2;
  static constexpr int nST = 3;
  static constexpr int nSSM = 2;
  static constexpr int nSM = 2;
public:
  static constexpr int MIXERINPUTS = 2 + nCM*ContextMap::MIXERINPUTS + nST + nSSM*SmallStationaryContextMap::MIXERINPUTS + nSM*StationaryMap::MIXERINPUTS; // 23
  static constexpr int MIXERCONTEXTS = 8;
  static constexpr int MIXERCONTEXTSETS = 1;
private:
  const Shared * const shared;
  ModelStats *stats;
  enum Parameters : U32 {
    MaxExtend = 0,   // longest allowed match expansion // warning: larger value -> slowdown
    MinLen = 5,      // minimum required match length
    StepSize = 2,    // additional minimum length increase per higher order hash
  };
  Array<U32> Table;
  StateMap StateMaps[nST];
  ContextMap cm;
  SmallStationaryContextMap SCM, SCM_pos;
  StationaryMap Maps[nSM];
  IndirectContext<U8> iCtx;
  U32 hashes[NumHashes]{ 0 };
  U32 ctx[nST]{ 0 };
  U32 length=0;      // rebased length of match (length=1 represents the smallest accepted match length), or 0 if no match
  U32 index=0;       // points to next byte of match in buf, 0 when there is no match
  U32 length_bak=0;  // allows match recovery after a 1-byte mismatch 
  U32 index_bak=0;   // 
  U8 expectedByte=0; // prediction is based on this byte (buf[index]), valid only when length>0
  bool delta=false;  // indicates that a match has just failed (delta mode)
  const U32 mask;
  const int hashbits;
public:
  MatchModel(const Shared * const sh, ModelStats *st, const U64 Size) : 
    shared(sh), stats(st), Table(Size/sizeof(U32)), 
    StateMaps{ //StateMap:  s, n, lim, 
      {sh,1,56*256,1023,false}, {sh,1,8*256*256+1,1023,false}, {sh,1,256*256,1023,false}
    },
    cm(sh,MEM/4,nCM),
    SCM{sh,6,1,6,64}, /* SmallStationaryContextMap: BitsOfContext, InputBits, Rate, Scale */
    SCM_pos {sh,8,8,5,64},
    Maps{ /* StationaryMap: BitsOfContext, InputBits, Scale, Limit  */
      {sh,23,1,64,1023}, {sh,15,1,64,1023}
    },
    iCtx{15,1}, // IndirectContext<U8>: BitsPerContext, InputBits
    mask(U32(Size/sizeof(U32)-1)), hashbits(ilog2(mask+1))
  {
    assert(ispowerof2(Size));
  }
  void Update() {
    INJECT_SHARED_bpos INJECT_SHARED_buf 
    if (length!=0) {
      const int expectedBit = (expectedByte>>((8-bpos)&7))&1;
      INJECT_SHARED_y
        if (y!=expectedBit) {
          if(length_bak!=0 && length-length_bak<MinLen) { // mismatch too soon in recovery mode -> give up
            length_bak=0;
            index_bak=0;
          } 
          else { //backup match information: maybe we can recover it just after this mismatch
            length_bak=length;
            index_bak=index;
          }
          delta = true;
          length = 0;
        }
    }

    if(bpos==0) {
      // update hashes
      for (U32 i=0, minLen=MinLen+(NumHashes-1)*StepSize; i<NumHashes; i++, minLen-=StepSize) {
        U64 hash = 0;
        for (U32 j=minLen; j>0; j--)
          hash = combine64(hash, buf(j));
        hashes[i] = finalize64(hash,hashbits);
      }

      // recover match after a 1-byte mismatch
      if(length==0 && !delta && length_bak!=0)  { //match failed (2 bytes ago), no new match found, and we have a backup
        index_bak++;
        if(length_bak<mask)length_bak++;
        INJECT_SHARED_c1
        if(buf[index_bak]==c1) { // match continues -> recover
          length=length_bak;
          index=index_bak;
        }
        else { // still mismatch
          length_bak=index_bak=0; // purge backup
        }
      }
      // extend current match
      if(length!=0) {
        index++;
        if(length<mask)length++;
      }
      delta = false;

      // find a new match, starting with the highest order hash and falling back to lower ones
      if(length==0) {
        U32 minLen = MinLen+(NumHashes-1)*StepSize, bestLen = 0, bestIndex = 0;
        for (U32 i=0; i<NumHashes && length<minLen; i++, minLen-=StepSize) {
          index = Table[hashes[i]];
          if (index>0) {
            length = 0;
            while (length<(minLen+MaxExtend) && buf(length+1)==buf[index-length-1])
              length++;
            if (length>bestLen) {
              bestLen = length;
              bestIndex = index;
            }
          }
        }
        if (bestLen>=MinLen) {
          length = bestLen-(MinLen-1); // rebase, a length of 1 actually means a length of MinLen
          index = bestIndex;
          length_bak=index_bak=0; // purge any backup
        }
        else
          length = index = 0;
      }
      // update position information in hashtable
      INJECT_SHARED_pos
      for (U32 i=0; i<NumHashes; i++)
        Table[hashes[i]] = pos;
      stats->Match.expectedByte = expectedByte = (length!=0 ? buf[index] : 0);
    }
  }

  void mix(Mixer& m) {
    Update();
    
    for (U32 i=0; i<nST; i++) // reset contexts
      ctx[i] = 0;

    INJECT_SHARED_bpos INJECT_SHARED_buf INJECT_SHARED_c1 INJECT_SHARED_c0 
    const int expectedBit = length!=0 ? (expectedByte>>(7-bpos))&1 : 0;
    if (length!=0) {
      if (length<=16)
        ctx[0] = (length-1)*2 + expectedBit; // 0..31
      else
        ctx[0] = 24 + (min(length-1, 63)>>2)*2 + expectedBit; // 32..55
      ctx[0] = ((ctx[0]<<8) | c0);
      ctx[1] = ((expectedByte<<11) | (bpos<<8) | c1) + 1;
      const int sign = 2*expectedBit-1;
      m.add(sign*(min(length, 32)<<5)); // +/- 32..1024
      m.add(sign*(ilog(min(length,65535))<<2));   // +/-  0..1024
    }
    else { // no match at all or delta mode
      m.add(0);
      m.add(0);
    }

    if (delta) // delta mode: helps predicting the remaining bits of a character when a mismatch occurs
      ctx[2] = (expectedByte<<8) | c0;

    for (U32 i=0; i<nST; i++) {
      const U32 c = ctx[i];
      const U32 p = StateMaps[i].p(0,c);
      if (c!=0)
        m.add(stretch(p)>>1);
      else
        m.add(0);
    }

    const U32 length_ilog2  = ilog2(length+1);
    //no match:      length_ilog2=0
    //length=1..2:   length_ilog2=1
    //length=3..6:   length_ilog2=2
    //length=7..14:  length_ilog2=3
    //length=15..30: length_ilog2=4

    const U8 length3 = min(length_ilog2,3); // 2 bits
    const U8 rm = length_bak!=0 && length-length_bak==1; // predicting the first byte in recovery mode is still uncertain
    const U8 length3_rm = length3<<1|rm; // 3 bits

    //bytewise contexts
    if(bpos==0) {
      cm.set(hash(0,expectedByte,length3_rm));
      cm.set(hash(1,expectedByte,length3_rm,c1));
      INJECT_SHARED_pos
      SCM_pos.set(pos&255); //~sparsemodel (pos mod 256)
    }
    cm.mix(m);
    SCM_pos.mix(m);

    //bitwise contexts
    {
      Maps[0].set(hash(expectedByte, c0, c1, buf(2), length3_rm));
      INJECT_SHARED_y
      iCtx+=y; 
      const U8 C = length3_rm<<1 | expectedBit; // 4 bits
      iCtx=(bpos<<11)|(c1<<3)|C;
      Maps[1].set_direct(iCtx());
      SCM.set((bpos<<3)|C);
    }
    Maps[0].mix(m);
    Maps[1].mix(m);
    SCM.mix(m);

    const U32 length_C = length_ilog2 !=0 ? length_ilog2 +1 : delta;
    //no match, no delta mode:   length_C=0
    //failed match, delta mode:  length_C=1
    //length=1..2:   length_C=2
    //length=3..6:   length_C=3
    //length=7..14:  length_C=4
    //length=15..30: length_C=5

    m.set(min(length_C,7),8);
    stats->Match.length3 = min(length_C,3);
  }
};

//////////////////////////// SparseMatchModel ///////////////////////////

class SparseMatchModel {
private:
  static constexpr int NumHashes = 4;
  static constexpr int nSM = 4;
public:
  static constexpr int MIXERINPUTS = 3 + nSM*StationaryMap::MIXERINPUTS; // 11
  static constexpr int MIXERCONTEXTS = NumHashes*(64+2048); // 8448
  static constexpr int MIXERCONTEXTSETS = 2;
private:
  const Shared * const shared;
  enum Parameters : U32 {
    MaxLen    = 0xFFFF, // longest allowed match
    MinLen    = 3,      // default minimum required match length
  };
  struct sparseConfig {
    U32 offset    = 0;      // number of last input bytes to ignore when searching for a match
    U32 stride    = 1;      // look for a match only every stride bytes after the offset
    U32 deletions = 0;      // when a match is found, ignore these many initial post-match bytes, to model deletions
    U32 minLen    = MinLen;
    U32 bitMask   = 0xFF;   // match every byte according to this bit mask
  };
  const sparseConfig sparse[NumHashes] = { {0,1,0,5,0xDF}, {1,1,0,4,0xFF}, {0,2,0,4,0xDF}, {0,1,0,5,0x0F} };
  Array<U32> Table;
  StationaryMap Maps[nSM];
  IndirectContext<U8>  iCtx8{19,1};  // BitsPerContext, InputBits
  IndirectContext<U16> iCtx16{16,8}; // BitsPerContext, InputBits
  MTFList list{NumHashes};
  U32 hashes[NumHashes]{};
  U32 hashIndex=0;   // index of hash used to find current match
  U32 length=0;      // rebased length of match (length=1 represents the smallest accepted match length), or 0 if no match
  U32 index=0;       // points to next byte of match in buf, 0 when there is no match
  U8 expectedByte=0; // prediction is based on this byte (buf[index]), valid only when length>0
  bool valid=false;
  const U32 mask;
  const int hashbits;
public:
  SparseMatchModel(const Shared * const sh, const U64 Size) : shared(sh), 
    Table(Size/sizeof(U32)), 
    Maps{ /* StationaryMap: BitsOfContext, InputBits, Scale, Limit  */
      {sh,22,1,128,1023}, {sh,17,4,128,1023}, {sh,8,1,128,1023}, {sh,19,1,128,1023}
    },
    mask(U32(Size/sizeof(U32)-1)), hashbits(ilog2(mask+1))
  {
    assert(ispowerof2(Size));
  }
  void Update() {
    INJECT_SHARED_buf  
    // update sparse hashes
    for (U32 i=0; i<NumHashes; i++) {
      U64 hash = 0;
      for (U32 j=0, k=sparse[i].offset+1; j<sparse[i].minLen; j++, k+=sparse[i].stride)
        hash = combine64(hash, buf(k)&sparse[i].bitMask);
      hashes[i] = finalize64(hash,hashbits);
    }
    // extend current match, if available
    if (length) {
      index++;
      if (length<MaxLen)
        length++;
    }
    // or find a new match
    else {     
      for (int i=list.GetFirst(); i>=0; i=list.GetNext()) {
        index = Table[hashes[i]];
        if (index>0) {
          U32 offset = sparse[i].offset+1;
          while (length<sparse[i].minLen && ((buf(offset)^buf[index-offset])&sparse[i].bitMask)==0) {
            length++;
            offset+=sparse[i].stride;
          }
          if (length>=sparse[i].minLen) {
            length-=(sparse[i].minLen-1);
            index+=sparse[i].deletions;
            hashIndex = i;
            list.MoveToFront(i);
            break;
          }
        }
        length = index = 0;
      }
    }
    // update position information in hashtable
    INJECT_SHARED_pos
    for (U32 i=0; i<NumHashes; i++)
      Table[hashes[i]] = pos;
    
    expectedByte = length!=0 ? buf[index] : 0;
    INJECT_SHARED_c1
    if (valid) {
      INJECT_SHARED_y
      iCtx8+=y;
      iCtx16+=c1;
    }
    valid = length>1; // only predict after at least one byte following the match
    if (valid) {
      INJECT_SHARED_c0 INJECT_SHARED_c4
      Maps[0].set(hash(expectedByte, c0, c1, (c4>>8)&0xff, ilog2(length+1)*NumHashes+hashIndex));
      const U32 c1_expectedByte=(c1<<8)|expectedByte;
      Maps[1].set_direct(c1_expectedByte);
      iCtx8=c1_expectedByte;
      iCtx16=c1_expectedByte;
      Maps[2].set_direct(iCtx8());
      Maps[3].set_direct(iCtx16());
    }
  }

  void mix(Mixer& m) {
    INJECT_SHARED_bpos INJECT_SHARED_c0  
    const U8 B = c0<<(8-bpos);
    if (bpos==0)
      Update();
    else if (valid) {
      INJECT_SHARED_c1 INJECT_SHARED_c4
      Maps[0].set(hash(expectedByte, c0, c1, (c4>>8)&0xff, ilog2(length+1)*NumHashes+hashIndex));
      if (bpos==4)
        Maps[1].set_direct(0x10000|((expectedByte^U8(c0<<4))<<8)|c1);
      INJECT_SHARED_y
      iCtx8+=y; iCtx8=(bpos<<16)|(c1<<8)|(expectedByte^B);
      Maps[2].set_direct(iCtx8());
      Maps[3].set_direct((bpos<<16)|(iCtx16()^U32(B|(B<<8))));
    }

    // check if next bit matches the prediction, accounting for the required bitmask
    if (length>0 && (((expectedByte^B)&sparse[hashIndex].bitMask)>>(8-bpos))!=0)
      length = 0;

    if (valid) {
      if (length>1 && ((sparse[hashIndex].bitMask>>(7-bpos))&1)>0) {
        const int expectedBit = (expectedByte>>(7-bpos))&1;
        const int sign = 2*expectedBit-1;
        m.add(sign*(min(length-1, 64)<<4)); // +/- 16..1024
        m.add(sign*(1<<min(length-2, 3))*min(length-1, 8)<<4); // +/- 16..1024
        m.add(sign*512);
      }
      else {
        m.add(0); m.add(0); m.add(0);
      }
      for (int i=0;i<nSM;i++) {
        Maps[i].mix(m);
      }
    }
    else
      for (int i=0; i<MIXERINPUTS; i++) m.add(0);

    m.set((hashIndex<<6)|(bpos<<3)|min(7, length), NumHashes*64);
    m.set((hashIndex<<11)|(min(7, ilog2(length+1))<<8)|(c0^(expectedByte>>(8-bpos))), NumHashes*2048);
  }
};

//////////////////////////// CharGroupModel /////////////////////////
//
// modeling ascii character sequences

class CharGroupModel {
private:
  static constexpr int nCM=7;
public:
  static constexpr int MIXERINPUTS = nCM*ContextMap::MIXERINPUTS; // 35
  static constexpr int MIXERCONTEXTS = 0;
  static constexpr int MIXERCONTEXTSETS = 0;
private:
  const Shared * const shared;
  ContextMap cm;
  U32 g_ascii3=0, g_ascii2=0, g_ascii1=0; // group identifiers of the last 12 (4+4+4) characters; the most recent is 'g_ascii1'
public:
  CharGroupModel (const Shared * const sh, const U64 size): shared(sh), cm(sh,size,nCM){}
  void mix(Mixer& m) {
    INJECT_SHARED_bpos 
    if(bpos==0) {
      INJECT_SHARED_c1 
      U32 g=c1; // group identifier
      if('0'<=g && g<='9') g='0'; //all digits are in one group
      else if('A'<=g && g<='Z') g='A'; //all uppercase letters are in one group
      else if('a'<=g && g<='z') g='a'; //all lowercase letters are in one group
      else if(g>=128) g=128;

      const bool to_be_collapsed = (g=='0' || g=='A' || g=='a') && (g == (g_ascii1&0xff));
      if(!to_be_collapsed) {
        g_ascii3 <<= 8;
        g_ascii3  |= g_ascii2>>(32-8);
        g_ascii2 <<= 8;
        g_ascii2  |= g_ascii1>>(32-8);
        g_ascii1 <<= 8;
        g_ascii1  |= g;
      }

      U64 i = to_be_collapsed*8;
      cm.set(hash( (++i), g_ascii3, g_ascii2,          g_ascii1));        // last 12 groups
      cm.set(hash( (++i),           g_ascii2,          g_ascii1));        // last 8 groups
      cm.set(hash( (++i),           g_ascii2&0xffff,   g_ascii1));        // last 6 groups
      cm.set(hash( (++i),                              g_ascii1));        // last 4 groups
      cm.set(hash( (++i),                              g_ascii1&0xffff)); // last 2 groups
      INJECT_SHARED_c4
      cm.set(hash( (++i),           g_ascii2&0xffffff, g_ascii1, c4&0x0000ffff )); // last 7 groups + last 2 chars
      cm.set(hash( (++i),           g_ascii2&0xff,     g_ascii1, c4&0x00ffffff )); // last 5 groups + last 3 chars
    }
    cm.mix(m);
  }
};


//////////////////////////// WordModel /////////////////////////

// Model words, expressions, numbers, paragraphs/lines, etc.
// simple procesing of pdf text
// simple modeling of some binary content

#ifdef USE_TEXTMODEL

class WordModel {
private:
  static constexpr int nCM1=17; // pdf / non_pdf contexts
  static constexpr int nCM2=35; // common contexts
  static constexpr int nCM = nCM1+nCM2; // 52
public:
  static constexpr int MIXERINPUTS = nCM*ContextMap::MIXERINPUTS; // 260
  static constexpr int MIXERCONTEXTS = 0;
  static constexpr int MIXERCONTEXTSETS = 0;
private:
  const Shared * const shared;
  ModelStats const *stats;
  ContextMap cm;
  static constexpr int wposbits=16;
  static constexpr int maxwordlen=45;
  static constexpr int maxlinematch=16;
  static constexpr int maxlastUpper=63;
  static constexpr int maxlastLetter=16;
  class Info {
    const Shared * const shared;
    ModelStats const *stats;
    ContextMap &cm;
    Array<U32> wpos{1<<wposbits};  // last positions of whole words
    Array<U16> wchk{1<<wposbits};  // checksums for whole words
    U32 c4;                      // last 4 processed characters
    U8 c, pC, ppC;           // last char, previous char, char before the previous char (converted to lowearcase)
    bool is_letter, is_letter_pC, is_letter_ppC;
    U8 opened;     // "(", "[", "{", "<", opening QUOTE, opening APOSTROPHE (or 0 when none of them)
    U8 wordlen0, wordlen1;     // length of word0 and word1
    U64 line0, frstword;       // hash of line content, hash of first word on line
    U64 word0, word1, word2, word3, word4;  // wordtoken hashes, word0 is the partally processed ("current") word
    U64 expr0, expr1, expr2, expr3, expr4;  // wordtoken hashes for expressions
    U64 keyword0, gaptoken0, gaptoken1;         // hashes
    U16 w, chk; // finalized hash and checksum of last partially processed word
    int frstchar;  // category of first character of the current line (or -1 when in first column)
    int linematch; // the length of match of the current line vs the previos line
    int nl1, nl2;  // current newline position, previous newline position
    U64 groups; // 8 last character categories
    U64 text0;  // uninterrupted stream of letters
    U32 lastLetter, lastUpper, wordGap;
    U32 mask, word0chars, mask2, f4;
  public:
    Info(const Shared * const sh, ModelStats const *st, ContextMap &contextmap): shared(sh), stats(st), cm(contextmap) {
      reset();
    }
    void reset() {
      //zero contents
      memset(&wpos[0], 0, (1<<wposbits)*sizeof(U32));
      memset(&wchk[0], 0, (1<<wposbits)*sizeof(U16));
      c4=0;c=pC=ppC=0;
      is_letter=is_letter_pC=is_letter_ppC=false;
      opened=wordlen0=wordlen1=0;
      line0=frstword=0;
      word0=word1=word2=word3=word4=0;
      expr0=expr1=expr2=expr3=expr4=0;
      keyword0=gaptoken0=gaptoken1=0;
      w=chk=0;
      frstchar=linematch=nl1=nl2=0;
      groups=text0=0;
      lastLetter=lastUpper=wordGap=0;
      mask=word0chars=mask2=f4=0;
      //---
      lastUpper=maxlastUpper;
      lastLetter=wordGap=maxlastLetter;
      frstchar=linematch=-1;
    }
    void shiftwords() {
      word4=word3;
      word3=word2;
      word2=word1;
      word1=word0;
      wordlen1=wordlen0;
    }
    void killwords() {
      word4=word3=word2=word1=0;
    }
    void process_char(const bool is_extended_char) {
      //note: the pdf model uses this function, therefore only c1 may be referenced, c4, c8, etc. should not

      //shift history
      ppC=pC; pC=c;
      is_letter_ppC=is_letter_pC; is_letter_pC=is_letter;

      INJECT_SHARED_c1 
      c4=c4<<8|c1;
      c=c1;
      if(c>='A' && c<='Z') {c+='a'-'A'; lastUpper=0;}

                 is_letter = (c>='a' && c<='z') || is_extended_char;
      const bool is_number = (c>='0' && c<='9') || (pC>='0' && pC<='9' && c=='.' /* decimal point (english) or thousand separator (misc) */);

      lastUpper=min(lastUpper+1,maxlastUpper);
      lastLetter=min(lastLetter+1,maxlastLetter);
      mask2<<=3;

      if (is_letter || is_number) {
        if (wordlen0==0){ // the beginning of a new word
          if (pC==NEW_LINE && lastLetter==3 && ppC=='+') { 
            // correct hyphenation with "+" (book1 from Calgary)
            // (undo some of the recent context changes)
            word0=word1; word1=word2; word2=word3; 
            word3=word4=0;
            wordlen0=wordlen1; 
          } else {
            wordGap = lastLetter;
          }
          gaptoken1=gaptoken0;
          gaptoken0=0;
          if (pC==QUOTE || (pC==APOSTROPHE && !is_letter_ppC)) opened=pC;
        }
        lastLetter=0;
        word0=combine64(word0, c);
        w=U16(finalize64(word0,wposbits));
        chk=U16(checksum64(word0,wposbits,16));
        word0chars = is_number ? 0 : word0chars<<8|c; // last 4 consecutive letters
        text0=(text0<<8|c)&0xffffffffff; // last 5 alphanumeric chars (other chars are ignored)
        wordlen0=min(wordlen0+1,maxwordlen);
        if ((c=='a' || c=='e' || c=='i' || c=='o' || c=='u') || (c=='y' && (wordlen0>0 && pC!='a' && pC!='e' && pC!='i' && pC!='o' && pC!='u')))
          mask2++;
        else if (c>='b' && c<='z')
          mask2+=c=='q'?2:3;
      }
      else { //it's not a letter/number 
        gaptoken0=combine64(gaptoken0,c1==NEW_LINE?' ':c1);
        if(c1==NEW_LINE && pC=='+' && is_letter_ppC) {} //calgary/book1 hyphenation - don't shift again
        else if(c=='?' || pC=='!' || pC=='.') killwords(); //end of sentence
        else if(c==pC) {} //don't shift when anything repeats
        else if((c==SPACE || c==NEW_LINE) && (pC==SPACE || pC==NEW_LINE)) {} //ignore repeating whitespace
        else shiftwords();
        if (wordlen0!=0) { //beginning of a new non-word token
          INJECT_SHARED_pos
          wpos[w]=pos;
          wchk[w]=chk;
          w=0;
          chk=0;

          if (c1==':'||c1=='=') keyword0 = word0; // enwik, world95.txt, html/xml
          if(frstword==0)frstword=word0;

          word0=0;
          wordlen0=0;
          word0chars=0;
        }

             if (c1=='.' || c1=='!' || c1=='?') mask2+=4;
        else if (c1==',' || c1==';' || c1==':') mask2+=5;
        else if (c1=='(' || c1=='{' || c1=='[' || c1=='<') {mask2+=6;opened=c1;}
        else if (c1==')' || c1=='}' || c1==']' || c1=='>' || c1==QUOTE || c1==APOSTROPHE) {mask2+=7;opened=0;}
      }
      
      //const U8 chargrp = stats->Text.chargrp;
      U8 g=c1;
      if(g>=128){
        //utf8 code points (weak context)
        if((g&0xf8)==0xf0)g=0xf0;
        else if((g&0xf0)==0xe0)g=0xe0;
        else if((g&0xe0)==0xc0)g=0xc0;
        else if((g&0xc0)==0x80)g=0x80;
        else if (g!=0xff)g=0xfe;
      }
      else if(g>='0' && g<='9')g='0';
      else if(g>='a' && g<='z')g='a';
      else if(g>='A' && g<='Z')g='A';
      else if(g<32 && g!=0 && g!=NEW_LINE)g=1;
      groups=groups<<8|g;

      // Expressions (words separated by single spaces)
      //
      // Remarks: (1) formatted text files may contain SPACE+NEW_LINE (dickens) or NEW_LINE+SPACE (world95.txt)
      // or just a NEW_LINE instead of a SPACE. (2) quotes and apostrophes are ignored during processing
      if(is_letter/* || is_number*/) {
        expr0=combine64(expr0,c);
      } else if((c==SPACE || c==NEW_LINE) && (is_letter_pC || pC==APOSTROPHE || pC==QUOTE)) {
        expr4=expr3;
        expr3=expr2;
        expr2=expr1;
        expr1=expr0;
        expr0=0;
      } else if(c==APOSTROPHE || c==QUOTE || (c==NEW_LINE && pC==SPACE) || (c==SPACE && pC==NEW_LINE)) {
        //ignore
      } else {
        expr4=expr3=expr2=expr1=expr0=0;
      }
    }
    void line_model_predict(U8 in_pdf_text_block) {
      U64 i = 2048;

      INJECT_SHARED_c1 
      INJECT_SHARED_pos
      if (c1==NEW_LINE || c1==0) {  // a new line has just started (or: zero in asciiz or in binary data)
        nl2=nl1; nl1=pos; 
        frstchar=-1; frstword=0; line0=0;
      }
      line0=combine64(line0,c1);
      cm.set(hash(++i,line0));

      int col = pos-nl1;
      if (col==1) frstchar = groups&0xff;
      INJECT_SHARED_buf
      const U8  c_above = buf[nl2+col];
      const U8 pC_above = buf[nl2+col-1];

      const bool is_new_line_start = col==0 && nl2>0;
      const bool is_prev_char_match_above = c1==pC_above && col!=0 && nl2!=0;
      const U32 above_ctx = c_above<<1|is_prev_char_match_above;
      if(is_new_line_start) linematch=0; //first char not yet known = nothing to match
      else if(linematch>=0 && is_prev_char_match_above) linematch=min(linematch+1,maxlinematch); //match continues
      else linematch=-1; //match does not continue

      // context: matches with the previous line
      if(linematch>=0) cm.set(hash(++i,c_above,linematch));
      else {cm.skip();i++;}
      cm.set(hash(++i,       above_ctx,c1));
      cm.set(hash(++i,col<<9|above_ctx,c1));
      const int line_length = nl1-nl2;
      cm.set(hash(++i,nl1-nl2, col, above_ctx, groups&0xff)); // english_mc
      cm.set(hash(++i,nl1-nl2, col, above_ctx, c1)); // english_mc

      // modeling line content per column (and NEW_LINE is some extent)
      cm.set(hash(++i,col,c1==SPACE)); // after space vs after other char in this column // world95.txt
      cm.set(hash(++i,col,c1));
      cm.set(hash(++i,col,mask&0x1ff));
      cm.set(hash(++i,col,line_length)); // the lenght of the previous line may foretell the content of columns

      cm.set(hash(++i,col, frstchar, ((int)lastUpper<col)<<8 | (groups&0xff))); // book1 book2 news

      // content of lines, paragraphs
      cm.set(hash(++i,nl1,in_pdf_text_block));    //chars occurring in this paragraph (order 0)
      cm.set(hash(++i,nl1,in_pdf_text_block,c));  //chars occurring in this paragraph (order 1)
      cm.set(hash(++i,frstchar));   //chars occurring in a paragraph that began with frstchar (order 0)
      cm.set(hash(++i,frstchar,c)); //chars occurring in a paragraph that began with frstchar (order 1)
      cm.set(hash(++i,frstword));   //chars occurring in a paragraph that began with frstword (order 0)
      cm.set(hash(++i,frstword,c)); //chars occurring in a paragraph that began with frstword (order 1)
      assert(i==2048+nCM1);
    }
    void line_model_skip(ContextMap &cm) {
      for(int i=0;i<nCM1;i++)
        cm.skip();
    }
    void predict(U8 in_pdf_text_block) {
      U64 i = 1024;
      cm.set(hash(++i,text0));

      // expressions in normal text
      cm.set(hash(++i,expr0,expr1,expr2,expr3,expr4));
      cm.set(hash(++i,expr0,expr1,expr2));

      // sections introduced by keywords (enwik world95.txt html/xml)
      cm.set(hash(++i,gaptoken0,keyword0)); // chars occurring in a section introduced by "keyword:" or "keyword=" (order 0 and variable order for the gap)
      cm.set(hash(++i,word0, c, keyword0)); // tokens occurring in a section introduced by "keyword:" or "keyword=" (order 1 and variable order for a word)

      // Simple word morphology (order 1-3)
      //
      if(is_letter)
        if(is_letter_pC)
          if(is_letter_ppC) cm.set(hash(++i,c,pC,ppC)); //order 3 word token
          else cm.set(hash(++i,c,pC)); //order 2 word token
        else cm.set(hash(++i,c)); //order 1 word token
      else {cm.skip(); i++;} //not applicable

      cm.set(hash(++i,word0));
      cm.set(hash(++i,gaptoken0));

      cm.set(hash(++i,word0, c, gaptoken1)); // 
      cm.set(hash(++i,gaptoken0, c, word1)); // stronger //french texts need that "c"
      cm.set(hash(++i,word0, word1));        // stronger
      cm.set(hash(++i,word0, word1, word2)); // weaker
      cm.set(hash(++i,gaptoken0,gaptoken1,word1,word2)); //stronger
      cm.set(hash(++i,word0    ,gaptoken1,word1,word2)); //weaker

      const U8 c1=c4&0xff;
      cm.set(hash(++i,word0, c1, word2));
      cm.set(hash(++i,word0, c1, word3));
      cm.set(hash(++i,word0, c1, word4));
      cm.set(hash(++i,word0, c1, word1, word3));
      cm.set(hash(++i,word0, c1, word2, word3));

      INJECT_SHARED_pos
      const U32 lastpos = wchk[w]!=chk ? 0 : wpos[w];
      const U32 dist = lastpos==0 ? 0 : min(llog(pos-lastpos+120)>>4,20);

      cm.set(hash(++i,word0, dist));
      cm.set(hash(++i,word1, gaptoken0, dist));

      cm.set(hash(++i,pos>>10, word0)); //word tokens occurring in this 1K block (variable order)

      cm.set(hash(++i,opened,groups&0xff,in_pdf_text_block));
      cm.set(hash(++i,opened,c,dist!=0,in_pdf_text_block));

      cm.set(hash(++i,groups));
      cm.set(hash(++i,groups,c));
      cm.set(hash(++i,groups,c4&0xffff));

      if(c1=='.' || c1=='!' || c1=='?' || c1=='/'|| c1==')'|| c1=='}') f4=(f4&0xfffffff0)+2;
      f4=(f4<<4) | (c1==' ' ? 0 : c1>>4);

      cm.set(hash(++i,f4&0xfff));
      cm.set(hash(++i,f4));

      int fl = 0;
      if (c1 != 0) {
        if (isalpha(c1)) fl = 1;
        else if (ispunct(c1)) fl = 2;
        else if (isspace(c1)) fl = 3;
        else if ((c1) == 0xff) fl = 4;
        else if ((c1) < 16) fl = 5;
        else if ((c1) < 64) fl = 6;
        else fl = 7;
      }
      mask = (mask<<3)|fl;

      cm.set(hash(++i,mask));
      cm.set(hash(++i,mask,c1));
      cm.set(hash(++i,mask,c4&0x00ffff00));
      cm.set(hash(++i,mask&0x1ff,f4&0x00fff0));

      cm.set(hash(++i, word0, c1, llog(wordGap), mask&0x1FF,
        ((wordlen1 > 3)<<6)|
        ((wordlen0 > 0)<<5)|
        ((lastUpper < lastLetter + wordlen1)<<1)|
        (lastUpper < wordlen0 + wordlen1 + wordGap)
      )); //weak
      cm.set(hash(++i, mask2&0x1FF, word0chars&0xffff, min(wordlen0,6)));
      assert(i==1024+nCM2);
    }
  };
  U8 pdf_text_parser_state; // 0,1,2,3
  Info info_normal;
  Info info_pdf;
public:
  WordModel (const Shared * const sh, ModelStats const *st, const U64 size) : 
    shared(sh), stats(st), cm(sh,size,nCM), info_normal(sh,st,cm), info_pdf(sh,st,cm) {}
  void reset() {
    info_normal.reset();
    info_pdf.reset();
  }
  void mix(Mixer &m) {
    INJECT_SHARED_bpos 
    if (bpos==0) {
      //extract text from pdf
      INJECT_SHARED_c4
      const U8 c1=c4;
      if(c4==0x0a42540a /* "\nBT\n" */) pdf_text_parser_state=1; // Begin Text
      else if (c4==0x0a45540a /* "\nET\n" */) {pdf_text_parser_state=0;} // End Text
      bool do_pdf_process=true;
      if(pdf_text_parser_state!=0) {
        const U8 pC=c4>>8;
        if(pC!='\\') {
             if(c1=='[') {pdf_text_parser_state|=2;} //array begins
        else if(c1==']') {pdf_text_parser_state&=(255-2);}
        else if(c1=='(') {pdf_text_parser_state|=4; do_pdf_process=false;} //signal: start text extraction
        else if(c1==')') {pdf_text_parser_state&=(255-4);} //signal: start pdf gap processing
        }
      }

      const bool is_pdftext = (pdf_text_parser_state&4)!=0;
      if(is_pdftext) {
        if(do_pdf_process) {
          //printf("%c",c1); //debug: print the extracted pdf text
          const bool is_extended_char = c1=='\\';
          info_pdf.process_char(is_extended_char);
        }
        info_pdf.predict(pdf_text_parser_state);
        info_pdf.line_model_skip(cm);
      } else {
        const bool is_textblock = stats->blockType==TEXT || stats->blockType==TEXT_EOL;
        const bool is_extended_char = is_textblock && c1>=128;
        info_normal.process_char(is_extended_char);
        info_normal.predict(pdf_text_parser_state);
        info_normal.line_model_predict(pdf_text_parser_state);
      }
      
    }
    cm.mix(m);
  }
};

#endif //USE_TEXTMODEL


//////////////////////////// RecordModel ///////////////////////

// Model 2-D data with fixed record length.  Also order 1-2 models
// that include the distance to the last match.

inline U8 Clip(int const Px){
  if(Px>255)return 255;
  if(Px<0)return 0;
  return Px;
}
inline U8 Clamp4(const int Px, const U8 n1, const U8 n2, const U8 n3, const U8 n4){
  int maximum=n1;if(maximum<n2)maximum=n2;if(maximum<n3)maximum=n3;if(maximum<n4)maximum=n4;
  int minimum=n1;if(minimum>n2)minimum=n2;if(minimum>n3)minimum=n3;if(minimum>n4)minimum=n4;
  if(Px<minimum)return minimum;
  if(Px>maximum)return maximum;
  return Px;
}
inline U8 LogMeanDiffQt(const U8 a, const U8 b, const U8 limit = 7){
  if (a==b) return 0;
  U8 sign=a>b?8:0;
  return sign | min(limit, ilog2((a+b)/max(2,abs(a-b)*2)+1));
}
inline U32 LogQt(const U8 Px, const U8 bits){
  return (U32(0x100|Px))>>max(0,(int)(ilog2(Px)-bits));
}

struct dBASE {
  U8 Version;
  U32 nRecords;
  U16 RecordLength, HeaderLength;
  U32 Start, End;
};

class RecordModel {
private:
  static constexpr int nCM = 3+3+3+16; //cm,cn,co,cp
  static constexpr int nSM = 6;
  static constexpr int nSSM = 3;
  static constexpr int nIM = 3;
  static constexpr int nIndCtxs = 5;
public:
  static constexpr int MIXERINPUTS = nCM*ContextMap::MIXERINPUTS + nSM*StationaryMap::MIXERINPUTS + nSSM*SmallStationaryContextMap::MIXERINPUTS + nIM*IndirectMap::MIXERINPUTS; // 149
  static constexpr int MIXERCONTEXTS = 1024+512+11*32; //1888
  static constexpr int MIXERCONTEXTSETS = 3;
private:
  const Shared * const shared;
  ModelStats *stats;
  ContextMap cm, cn, co; 
  ContextMap cp; 
  StationaryMap Maps[nSM];
  SmallStationaryContextMap sMap[nSSM];
  IndirectMap iMap[nIM];
  IndirectContext<U16> iCtx[nIndCtxs];
  Array<U32> cpos1{256}, cpos2{256}, cpos3{256}, cpos4{256};
  Array<U32> wpos1{256*256}; // buf(1..2) -> last position
  U32 rlen[3] = {2,0,0}; // run length and 2 candidates
  U32 rcount[2] = {0,0}; // candidate counts
  U8 padding = 0; // detected padding byte
  U8 N=0, NN=0, NNN=0, NNNN=0, WxNW=0;
  U32 prevTransition = 0, nTransition = 0; // position of the last padding transition
  U32 col = 0, mxCtx = 0, x = 0;
  bool MayBeImg24b = false;
  dBASE dbase {};
public:
  RecordModel (const Shared * const sh, ModelStats *st, const U64 size): shared(sh), stats(st), 
    cm(sh,32768,3), cn(sh,32768/2,3), co(sh,32768*2,3), // cm,cn,co: memory pressure is advantageous
    cp(sh,size,16),
    Maps{ /* BitsOfContext, InputBits, Scale, Limit  */
      {sh,10,8,86,1023},{sh,10,8,86,1023},{sh,8,8,86,1023},{sh,8,8,86,1023},{sh,8,8,86,1023},{sh,11,1,86,1023}
    },
    sMap{ /* BitsOfContext, InputBits, Rate, Scale */
      {sh,11,1,6,86}, {sh,3,1,6,86}, {sh,19,1,5,128}
    },
    iMap{ /* BitsOfContext, InputBits, Scale, Limit */
      {sh,8,8,86,255}, {sh,8,8,86,255}, {sh,8,8,86,255}
    },
    iCtx{ // BitsPerContext, InputBits
      {16,8}, {16,8}, {16,8}, {20,8}, {11,1}
    }
  {} 
  void mix(Mixer& m) {
    INJECT_SHARED_bpos
    // Find record length
    if (bpos==0) {
      INJECT_SHARED_c4 INJECT_STATS_blpos INJECT_SHARED_buf INJECT_SHARED_pos
      int w=c4&0xffff, c=w&255, d=w>>8;
      if ((stats->Wav)>2 && (stats->Wav) != rlen[0]) {
        rlen[0] = stats->Wav;
        rcount[0]=rcount[1]=0;
      }
      else{
        // detect dBASE tables
        if (blpos==0 || (dbase.Version>0 && blpos>=dbase.End))
          dbase.Version = 0;
        else if (dbase.Version==0 && stats->blockType==DEFAULT && blpos>=31){
          U8 b = buf(32);
          if ( ((b&7)==3 || (b&7)==4 || (b>>4)==3 || b==0xF5) &&
               ((b=buf(30))>0 && b<13) &&
               ((b=buf(29))>0 && b<32) &&
               ((dbase.nRecords = buf(28)|(buf(27)<<8)|(buf(26)<<16)|(buf(25)<<24)) > 0 && dbase.nRecords<0xFFFFF) &&
               ((dbase.HeaderLength = buf(24)|(buf(23)<<8)) > 32 && ( ((dbase.HeaderLength-32-1)%32)==0 || (dbase.HeaderLength>255+8 && (((dbase.HeaderLength-=255+8)-32-1)%32)==0) )) &&
               ((dbase.RecordLength = buf(22)|(buf(21)<<8)) > 8) &&
               (buf(20)==0 && buf(19)==0 && buf(17)<=1 && buf(16)<=1)
          ){
            dbase.Version = (((b=buf(32))>>4)==3)?3:b&7;
            dbase.Start = blpos - 32 + dbase.HeaderLength;
            dbase.End = dbase.Start + dbase.nRecords * dbase.RecordLength;
            if (dbase.Version==3){
              rlen[0] = 32;
              rcount[0]=rcount[1]=0;
            }
          }
        }
        else if (dbase.Version>0 && blpos==dbase.Start){
          rlen[0] = dbase.RecordLength;
          rcount[0]=rcount[1]=0;
        }

        U32 r=pos-cpos1[c];
        if (r>1 && r==cpos1[c]-cpos2[c]
            && r==cpos2[c]-cpos3[c] && (r>32 || r==cpos3[c]-cpos4[c])
            && (r>10 || ((c==buf(r*5+1)) && c==buf(r*6+1)))) {
          if (r==rlen[1]) ++rcount[0];
          else if (r==rlen[2]) ++rcount[1];
          else if (rcount[0]>rcount[1]) rlen[2]=r, rcount[1]=1;
          else rlen[1]=r, rcount[0]=1;
        }

        // check candidate lengths
        for (int i=0; i < 2; i++) {
          if ((int)rcount[i] > max(0,12-(int)ilog2(rlen[i+1]))){
            if (rlen[0] != rlen[i+1]){
              if (MayBeImg24b && rlen[i+1]==3){
                rcount[0]>>=1;
                rcount[1]>>=1;
                continue;
              }
              else if ( (rlen[i+1] > rlen[0]) && (rlen[i+1] % rlen[0] == 0) ){
                // maybe we found a multiple of the real record size..?
                // in that case, it is probably an immediate multiple (2x).
                // that is probably more likely the bigger the length, so
                // check for small lengths too
                if ((rlen[0] > 32) && (rlen[i+1] == rlen[0]*2)){
                  rcount[0]>>=1;
                  rcount[1]>>=1;
                  continue;
                }
              }
              rlen[0] = rlen[i+1];
              //printf("\nrecordModel: detected rlen: %d\n",rlen[0]); // for debugging
              rcount[i] = 0;
              MayBeImg24b = (rlen[0]>30 && (rlen[0]%3)==0);
              nTransition = 0;
            }
            else
              // we found the same length again, that's positive reinforcement that
              // this really is the correct record size, so give it a little boost
              rcount[i]>>=2;

            // if the other candidate record length is orders of
            // magnitude larger, it will probably never have enough time
            // to increase its counter before it's reset again. and if
            // this length is not a multiple of the other, than it might
            // really be worthwhile to investigate it, so we won't set its
            // counter to 0
            if (rlen[i+1]<<4 > rlen[1+(i^1)])
              rcount[i^1] = 0;
          }
        }
      }

      assert(rlen[0]>=2);
      col=pos%rlen[0];
      x = min(0x1F,col/max(1,rlen[0]/32));
      N = buf(rlen[0]), NN = buf(rlen[0]*2), NNN = buf(rlen[0]*3), NNNN = buf(rlen[0]*4);
      for (int i=0; i<nIndCtxs-1; iCtx[i]+=c, i++);
      iCtx[0]=(c<<8)|N;
      iCtx[1]=(buf(rlen[0]-1)<<8)|N;
      iCtx[2]=(c<<8)|buf(rlen[0]-1);
      iCtx[3]=finalize64(hash(c, N, buf(rlen[0]+1)), 20);

      /*
      Consider record structures that include fixed-length strings.
      These usually contain the text followed by either spaces or 0's,
      depending on whether they're to be trimmed or they're null-terminated.
      That means we can guess the length of the string field by looking
      for small repetitions of one of these padding bytes followed by a
      different byte. By storing the last position where this transition
      ocurred, and what was the padding byte, we are able to model these
      runs of padding bytes.
      Special care is taken to skip record structures of less than 9 bytes,
      since those may be little-endian 64 bit integers. If they contain
      relatively low values (<2^40), we may consistently get runs of 3 or
      even more 0's at the end of each record, and so we could assume that
      to be the general case. But with integers, we can't be reasonably sure
      that a number won't have 3 or more 0's just before a final non-zero MSB.
      And with such simple structures, there's probably no need to be fancy
      anyway
      */

      if (!col)
        nTransition = 0;
      if ((((c4>>8) == SPACE*0x010101) && (c != SPACE)) || (!(c4>>8) && c && ((padding != SPACE) || (pos-prevTransition > rlen[0])))){
        prevTransition = pos;
        nTransition+=(nTransition<31);
        padding = (U8)d;
      }

      U64 i=0;

      // Set 2-3 dimensional contexts
      // assuming rlen[0]<1024; col<4096
      cm.set(hash(++i, c<<8 | (min(255, pos-cpos1[c])>>2)));
      cm.set(hash(++i, w<<9 | llog(pos-wpos1[w])>>2));
      cm.set(hash(++i, rlen[0] | N<<10 | NN<<18));

      cn.set(hash(++i, w | rlen[0]<<16));
      cn.set(hash(++i, d | rlen[0]<<8));
      cn.set(hash(++i, c | rlen[0]<<8));

      co.set(hash(++i, c<<8|min(255, pos-cpos1[c])));
      co.set(hash(++i, c<<17 | d<<9|llog(pos-wpos1[w])>>2));
      co.set(hash(++i, c<<8 | N));

      cp.set(hash(++i, rlen[0] | N<<10 | col<<18));
      cp.set(hash(++i, rlen[0] | c<<10 | col<<18));
      cp.set(hash(++i, col | rlen[0]<<12));

      if (rlen[0]>8){
        cp.set(hash(++i, min(min(0xFF,rlen[0]),pos-prevTransition), min(0x3FF,col), (w&0xF0F0)|(w==((padding<<8)|padding)), nTransition ) );
        cp.set(hash(++i, w, (buf(rlen[0]+1)==padding && N==padding), col/max(1,rlen[0]/32) ) );
      }
      else
        cp.set(0), cp.set(0);

      cp.set(hash(++i, N|((NN&0xF0)<<4)|((NNN&0xE0)<<7)|((NNNN&0xE0)<<10)|((col/max(1,rlen[0]/16))<<18)));
      cp.set(hash(++i, (N&0xF8)|((NN&0xF8)<<8)|(col<<16)));
      cp.set(hash(++i, N, NN));

      cp.set(hash(++i, col, iCtx[0]()));
      cp.set(hash(++i, col, iCtx[1]()));
      cp.set(hash(++i, col, iCtx[0]()&0xFF, iCtx[1]()&0xFF));

      cp.set(hash(++i, iCtx[2]()));
      cp.set(hash(++i, iCtx[3]()));
      cp.set(hash(++i, iCtx[1]()&0xFF, iCtx[3]()&0xFF));

      cp.set(hash(++i, N, (WxNW=c^buf(rlen[0]+1))));
      cp.set(hash(++i, (stats->Match.length3!=0)<<8 | stats->Match.expectedByte, U8(iCtx[1]()), N, WxNW));

      int k=0x300;
      if (MayBeImg24b) {
        k = (col%3)<<8;
        Maps[0].set_direct(Clip(((U8)(c4>>16))+c-(c4>>24))|k);
      }
      else
        Maps[0].set_direct(Clip(c*2-d)|k);
      Maps[1].set_direct(Clip(c+N-buf(rlen[0]+1))|k);
      Maps[2].set_direct(Clip(N+NN-NNN));
      Maps[3].set_direct(Clip(N*2-NN));
      Maps[4].set_direct(Clip(N*3-NN*3+NNN));
      iMap[0].set_direct(N+NN-NNN);
      iMap[1].set_direct(N*2-NN);
      iMap[2].set_direct(N*3-NN*3+NNN);

      // update last context positions
      cpos4[c]=cpos3[c];
      cpos3[c]=cpos2[c];
      cpos2[c]=cpos1[c];
      cpos1[c]=pos;
      wpos1[w]=pos;

      mxCtx = (rlen[0]>128)?(min(0x7F,col/max(1,rlen[0]/128))):col;
    }
    INJECT_SHARED_c0 
    U8 B = c0<<(8-bpos);
    U32 ctx = (N^B)|(bpos<<8);
    INJECT_SHARED_y
    iCtx[nIndCtxs-1]+=y;
    iCtx[nIndCtxs-1]=ctx;
    Maps[5].set_direct(ctx);
    sMap[0].set(ctx);
    sMap[1].set(iCtx[nIndCtxs-1]());
    sMap[2].set((ctx<<8)|WxNW);

    cm.mix(m);
    cn.mix(m);
    co.mix(m);
    cp.mix(m);
    for (int i=0;i<nSM;i++)
      Maps[i].mix(m);
    for (int i=0; i<nIM; i++)
      iMap[i].mix(m);
    for (int i=0; i<nSSM; i++)
      sMap[i].mix(m);

    m.set( (rlen[0]>2)*( (bpos<<7)|mxCtx ), 1024 );
    m.set( ((N^B)>>4)|(x<<4), 512 );
    m.set( ((stats->Text.chargrp)<<5)|x, 11*32);

    stats->Wav = min(0xFFFF,rlen[0]);
  }
};


//////////////////////// LinearPredictionModel ///////////////////

class LinearPredictionModel {
private:
  static constexpr int nOLS=3;
  static constexpr int nSSM=nOLS+2;
public:
  static constexpr int MIXERINPUTS = nSSM*SmallStationaryContextMap::MIXERINPUTS; // 10
  static constexpr int MIXERCONTEXTS = 0;
  static constexpr int MIXERCONTEXTSETS = 0;
private:
  const Shared * const shared;
  SmallStationaryContextMap sMap[nSSM];
  OLS<double, U8> ols[nOLS]{ {32, 4, 0.995}, {32, 4, 0.995}, {32, 4, 0.995} };
  U8 prd[nSSM]{ 0 };
public:
  LinearPredictionModel(const Shared * const sh) : shared(sh),
    sMap{  /* SmallStationaryContextMap : BitsOfContext, InputBits, Rate, Scale */
      {sh,11,1,6,128},{sh,11,1,6,128},{sh,11,1,6,128},{sh,11,1,6,128},{sh,11,1,6,128}
    }
  {}
  void mix (Mixer& m) {
    INJECT_SHARED_bpos
    if (bpos==0) {
      INJECT_SHARED_buf
      const U8 W=buf(1), WW=buf(2), WWW=buf(3);
      int i=0;
      for (; i<nOLS; i++)
        ols[i].Update(W);
      for (i=1; i<=32; i++) {
        ols[0].Add(buf(i));
        ols[1].Add(buf(i*2-1));
        ols[2].Add(buf(i*2));
      }
      for (i=0; i<nOLS; i++)
        prd[i]=Clip((int)floor(ols[i].Predict()));
      prd[i++]=Clip(W*2-WW);
      prd[i  ]=Clip(W*3-WW*3+WWW);
    }
    INJECT_SHARED_c0
    const U8 B=c0<<(8-bpos);
    for (int i=0; i<nSSM; i++) {
      sMap[i].set((prd[i]-B)*8+bpos);
      sMap[i].mix(m);
    }
  }
};

//////////////////////////// SparseModel ///////////////////////

// Model order 1-2-3 contexts with gaps.

class SparseModel {
private:
  static constexpr int nCM=38; //17+3*7
public:
  static constexpr int MIXERINPUTS = nCM*ContextMap::MIXERINPUTS; // 190
  static constexpr int MIXERCONTEXTS = 0;
  static constexpr int MIXERCONTEXTSETS = 0;
private:
  const Shared * const shared;
  ContextMap cm;
public:
  SparseModel(const Shared * const sh, const U64 size) : shared(sh), cm(sh,size,nCM){}
  void mix(Mixer& m) {
    INJECT_SHARED_bpos
    if(bpos==0) {
      INJECT_SHARED_buf INJECT_SHARED_c4
      U64 i=0;
      cm.set(hash(++i,buf(1)|buf(5)<<8));
      cm.set(hash(++i,buf(1)|buf(6)<<8));
      cm.set(hash(++i,buf(3)|buf(6)<<8));
      cm.set(hash(++i,buf(4)|buf(8)<<8));
      cm.set(hash(++i,buf(1)|buf(3)<<8|buf(5)<<16));
      cm.set(hash(++i,buf(2)|buf(4)<<8|buf(6)<<16));
      cm.set(hash(++i,c4&0x00f0f0ff));
      cm.set(hash(++i,c4&0x00ff00ff));
      cm.set(hash(++i,c4&0xff0000ff));
      cm.set(hash(++i,c4&0x00f8f8f8));
      cm.set(hash(++i,c4&0xf8f8f8f8));
      cm.set(hash(++i,c4&0x00e0e0e0));
      cm.set(hash(++i,c4&0xe0e0e0e0));
      cm.set(hash(++i,c4&0x810000c1));
      cm.set(hash(++i,c4&0xC3CCC38C));
      cm.set(hash(++i,c4&0x0081CC81));
      cm.set(hash(++i,c4&0x00c10081));
      for (int j=2; j<=8; ++j) {
        cm.set(hash(++i,buf(j)<<8));
        cm.set(hash(++i,(buf(j+1)<<8)|buf(j)));
        cm.set(hash(++i,(buf(j+2)<<8)|buf(j)));
      }
    }
    cm.mix(m);
  }
};


//////////////////////////// Image24bitModel /////////////////////////////////

inline U8 Paeth(U8 const W, U8 const N, U8 const NW){
  int p = W+N-NW;
  int pW=abs(p-(int)W), pN=abs(p-(int)N), pNW=abs(p-(int)NW);
  if (pW<=pN && pW<=pNW) return W;
  else if (pN<=pNW) return N;
  return NW;
}

// Model for filtered (PNG) or unfiltered 24/32-bit image data

class Image24bitModel {
private:
  static constexpr int nSM0 = 18;
  static constexpr int nSM1 = 76;
  static constexpr int nOLS = 6;
  static constexpr int nSM = nSM0+nSM1+nOLS;
  static constexpr int nSSM = 59;
  static constexpr int nCM = 45;
public:
  static constexpr int MIXERINPUTS = nSSM*SmallStationaryContextMap::MIXERINPUTS + nSM*StationaryMap::MIXERINPUTS + nCM*ContextMap::MIXERINPUTS;
  static constexpr int MIXERCONTEXTS = 6+256+512+2048+8*32+6*64+256*2+1024+8192+8192+8192+8192+256; //38022
  static constexpr int MIXERCONTEXTSETS = 13;

  const Shared * const shared;
  ModelStats *stats;
  ContextMap cm;
  SmallStationaryContextMap SCMap[nSSM];
  StationaryMap Map[nSM];
  Buf buffer{0x100000}; // internal rotating buffer for (PNG unfiltered) pixel data
  //pixel neighborhood
  U8 WWWWWW=0, WWWWW=0, WWWW=0, WWW=0, WW=0, W=0;
  U8 NWWWW=0, NWWW=0, NWW=0, NW=0, N=0, NE=0, NEE=0, NEEE=0, NEEEE=0;
  U8 NNWWW=0, NNWW=0, NNW=0, NN=0, NNE=0, NNEE=0, NNEEE=0;
  U8 NNNWW=0, NNNW=0, NNN=0, NNNE=0, NNNEE=0;
  U8 NNNNW=0, NNNN=0, NNNNE=0;
  U8 NNNNN=0;
  U8 NNNNNN=0;
  U8 WWp1=0, Wp1=0, p1=0, NWp1=0, Np1=0, NEp1=0, NNp1=0;
  U8 WWp2=0, Wp2=0, p2=0, NWp2=0, Np2=0, NEp2=0, NNp2=0;
  U8 px = 0; // current PNG filter prediction
  int info=0;
  U32 alpha=0, isPNG=0;
  int color = -1;
  int stride = 3;
  int col=0;
  int ctx[2]{}, padding=0, filter=0, x=0, w=0, line=0, R1=0, R2=0;
  U32 lastPos=0, lastWasPNG=0;
  bool filterOn = false;
  int columns[2] = {1,1}, column[2]{};
  U8 MapCtxs[nSM1] = { 0 }, SCMapCtxs[nSSM] = { 0 }, pOLS[nOLS] = { 0 };
  static constexpr double lambda[nOLS] ={ 0.98, 0.87, 0.9, 0.8, 0.9, 0.7 };
  static constexpr int num[nOLS] ={ 32, 12, 15, 10, 14, 8 };
  OLS<double, U8> ols[nOLS][4] = {
    {{num[0], 1, lambda[0]}, {num[0], 1, lambda[0]}, {num[0], 1, lambda[0]}, {num[0], 1, lambda[0]}},
    {{num[1], 1, lambda[1]}, {num[1], 1, lambda[1]}, {num[1], 1, lambda[1]}, {num[1], 1, lambda[1]}},
    {{num[2], 1, lambda[2]}, {num[2], 1, lambda[2]}, {num[2], 1, lambda[2]}, {num[2], 1, lambda[2]}},
    {{num[3], 1, lambda[3]}, {num[3], 1, lambda[3]}, {num[3], 1, lambda[3]}, {num[3], 1, lambda[3]}},
    {{num[4], 1, lambda[4]}, {num[4], 1, lambda[4]}, {num[4], 1, lambda[4]}, {num[4], 1, lambda[4]}},
    {{num[5], 1, lambda[5]}, {num[5], 1, lambda[5]}, {num[5], 1, lambda[5]}, {num[5], 1, lambda[5]}}
  };
  const U8 *ols_ctx1[32] = { &WWWWWW, &WWWWW, &WWWW, &WWW, &WW, &W, &NWWWW, &NWWW, &NWW, &NW, &N, &NE, &NEE, &NEEE, &NEEEE, &NNWWW, &NNWW, &NNW, &NN, &NNE, &NNEE, &NNEEE, &NNNWW, &NNNW, &NNN, &NNNE, &NNNEE, &NNNNW, &NNNN, &NNNNE, &NNNNN, &NNNNNN };
  const U8 *ols_ctx2[12] = { &WWW, &WW, &W, &NWW, &NW, &N, &NE, &NEE, &NNW, &NN, &NNE, &NNN }; 
  const U8 *ols_ctx3[15] = { &N, &NE, &NEE, &NEEE, &NEEEE, &NN, &NNE, &NNEE, &NNEEE, &NNN, &NNNE, &NNNEE, &NNNN, &NNNNE, &NNNNN };
  const U8 *ols_ctx4[10] = { &N, &NE, &NEE, &NEEE, &NN, &NNE, &NNEE, &NNN, &NNNE, &NNNN };
  const U8 *ols_ctx5[14] = { &WWWW, &WWW, &WW, &W, &NWWW, &NWW, &NW, &N, &NNWW, &NNW, &NN, &NNNW, &NNN, &NNNN };
  const U8 *ols_ctx6[ 8] = { &WWW, &WW, &W, &NNN, &NN, &N, &p1, &p2 };
  const U8 **ols_ctxs[nOLS] = { &ols_ctx1[0], &ols_ctx2[0], &ols_ctx3[0], &ols_ctx4[0], &ols_ctx5[0], &ols_ctx6[0] };
public:
  Image24bitModel(const Shared * const sh, ModelStats *st,const U64 size): shared(sh), stats(st), cm(sh,size,nCM),
    SCMap { /* SmallStationaryContextMap : BitsOfContext, InputBits, Rate, Scale */
      {sh,11,1,9,86}, {sh,11,1,9,86}, {sh,11,1,9,86}, {sh,11,1,9,86}, {sh,11,1,9,86}, {sh,11,1,9,86}, {sh,11,1,9,86}, {sh,11,1,9,86},
      {sh,11,1,9,86}, {sh,11,1,9,86}, {sh,11,1,9,86}, {sh,11,1,9,86}, {sh,11,1,9,86}, {sh,11,1,9,86}, {sh,11,1,9,86}, {sh,11,1,9,86},
      {sh,11,1,9,86}, {sh,11,1,9,86}, {sh,11,1,9,86}, {sh,11,1,9,86}, {sh,11,1,9,86}, {sh,11,1,9,86}, {sh,11,1,9,86}, {sh,11,1,9,86},
      {sh,11,1,9,86}, {sh,11,1,9,86}, {sh,11,1,9,86}, {sh,11,1,9,86}, {sh,11,1,9,86}, {sh,11,1,9,86}, {sh,11,1,9,86}, {sh,11,1,9,86},
      {sh,11,1,9,86}, {sh,11,1,9,86}, {sh,11,1,9,86}, {sh,11,1,9,86}, {sh,11,1,9,86}, {sh,11,1,9,86}, {sh,11,1,9,86}, {sh,11,1,9,86},
      {sh,11,1,9,86}, {sh,11,1,9,86}, {sh,11,1,9,86}, {sh,11,1,9,86}, {sh,11,1,9,86}, {sh,11,1,9,86}, {sh,11,1,9,86}, {sh,11,1,9,86},
      {sh,11,1,9,86}, {sh,11,1,9,86}, {sh,11,1,9,86}, {sh,11,1,9,86}, {sh,11,1,9,86}, {sh,11,1,9,86}, {sh,11,1,9,86}, {sh,11,1,9,86},
      {sh,11,1,9,86}, {sh,11,1,9,86}, {sh, 0,8,9,86}
    },
    Map { /* StationaryMap: BitsOfContext, InputBits, Scale, Limit  */
    /*nSM0: 0- 8*/  {sh,8,8,86,1023},  {sh,8,8,86,1023},  {sh,8,8,86,1023},  {sh,2,8,86,1023},  {sh,0,8,86,1023}, {sh,15,1,86,1023}, {sh,15,1,86,1023}, {sh,15,1,86,1023}, {sh,15,1,86,1023}, 
    /*nSM0: 9-17*/ {sh,15,1,86,1023}, {sh,17,1,86,1023}, {sh,17,1,86,1023}, {sh,17,1,86,1023}, {sh,17,1,86,1023}, {sh,13,1,86,1023}, {sh,13,1,86,1023}, {sh,13,1,86,1023}, {sh,13,1,86,1023}, 
    /*nSM1: 0- 8*/ {sh,11,1,86,1023}, {sh,11,1,86,1023}, {sh,11,1,86,1023}, {sh,11,1,86,1023}, {sh,11,1,86,1023}, {sh,11,1,86,1023}, {sh,11,1,86,1023}, {sh,11,1,86,1023}, {sh,11,1,86,1023},
    /*nSM1: 9-17*/ {sh,11,1,86,1023}, {sh,11,1,86,1023}, {sh,11,1,86,1023}, {sh,11,1,86,1023}, {sh,11,1,86,1023}, {sh,11,1,86,1023}, {sh,11,1,86,1023}, {sh,11,1,86,1023}, {sh,11,1,86,1023},
    /*nSM1:18-26*/ {sh,11,1,86,1023}, {sh,11,1,86,1023}, {sh,11,1,86,1023}, {sh,11,1,86,1023}, {sh,11,1,86,1023}, {sh,11,1,86,1023}, {sh,11,1,86,1023}, {sh,11,1,86,1023}, {sh,11,1,86,1023},
    /*nSM1:27-35*/ {sh,11,1,86,1023}, {sh,11,1,86,1023}, {sh,11,1,86,1023}, {sh,11,1,86,1023}, {sh,11,1,86,1023}, {sh,11,1,86,1023}, {sh,11,1,86,1023}, {sh,11,1,86,1023}, {sh,11,1,86,1023},
    /*nSM1:36-44*/ {sh,11,1,86,1023}, {sh,11,1,86,1023}, {sh,11,1,86,1023}, {sh,11,1,86,1023}, {sh,11,1,86,1023}, {sh,11,1,86,1023}, {sh,11,1,86,1023}, {sh,11,1,86,1023}, {sh,11,1,86,1023}, 
    /*nSM1:45-53*/ {sh,11,1,86,1023}, {sh,11,1,86,1023}, {sh,11,1,86,1023}, {sh,11,1,86,1023}, {sh,11,1,86,1023}, {sh,11,1,86,1023}, {sh,11,1,86,1023}, {sh,11,1,86,1023}, {sh,11,1,86,1023},
    /*nSM1:54-62*/ {sh,11,1,86,1023}, {sh,11,1,86,1023}, {sh,11,1,86,1023}, {sh,11,1,86,1023}, {sh,11,1,86,1023}, {sh,11,1,86,1023}, {sh,11,1,86,1023}, {sh,11,1,86,1023}, {sh,11,1,86,1023},
    /*nSM1:63-71*/ {sh,11,1,86,1023}, {sh,11,1,86,1023}, {sh,11,1,86,1023}, {sh,11,1,86,1023}, {sh,11,1,86,1023}, {sh,11,1,86,1023}, {sh,11,1,86,1023}, {sh,11,1,86,1023}, {sh,11,1,86,1023},
    /*nSM1:72-75*/ {sh,11,1,86,1023}, {sh,11,1,86,1023}, {sh,11,1,86,1023}, {sh,11,1,86,1023},
    /*nOLS:   0- 5*/ {sh,11,1,86,1023}, {sh,11,1,86,1023}, {sh,11,1,86,1023}, {sh,11,1,86,1023}, {sh,11,1,86,1023}, {sh,11,1,86,1023} 
    }
  {}
  void update() { 
    INJECT_SHARED_bpos
    if (bpos==0) {
      INJECT_SHARED_c1 INJECT_SHARED_buf
      if (x==1 && isPNG)
        filter = c1;
      else {
        if (x+padding<w) {
          color++;
          if (color>=stride) color=0;
        }
        else {
          if (padding>0) color=stride;
          else color=0;
        }
        if (isPNG) {
          switch (filter) {
            case 1: {
              buffer.add((U8)(c1+buffer(stride)*(x>stride+1 || !x)));
              filterOn = x>stride;
              px = buffer(stride);
              break;
            }
            case 2: {
              buffer.add((U8)(c1+buffer(w)*(filterOn=(line>0))));
              px = buffer(w);
              break;
            }
            case 3: {
              buffer.add((U8)(c1+(buffer(w)*(line>0)+buffer(stride)*(x>stride+1 || !x))/2));
              filterOn = (x>stride || line>0);
              px = (buffer(stride)*(x>stride)+buffer(w)*(line>0))/2;
              break;
            }
            case 4: {
              buffer.add((U8)(c1+Paeth(buffer(stride)*(x>stride+1 || !x), buffer(w)*(line>0), buffer(w+stride)*(line>0 && (x>stride+1 || !x)))));
              filterOn = (x>stride || line>0);
              px = Paeth(buffer(stride)*(x>stride), buffer(w)*(line>0), buffer(w+stride)*(x>stride && line>0));
              break;
            }
            default: buffer.add(c1);
              filterOn = false;
              px = 0;
          }
          if (!filterOn) px=0;
        }
        else
          buffer.add(c1);
      }

      if (x>0 || !isPNG) {
        column[0]=(x-isPNG)/columns[0];
        column[1]=(x-isPNG)/columns[1];
        WWWWWW=buffer(6*stride), WWWWW=buffer(5*stride), WWWW=buffer(4*stride), WWW=buffer(3*stride), WW=buffer(2*stride), W=buffer(stride);
        NWWWW=buffer(w+4*stride), NWWW=buffer(w+3*stride), NWW=buffer(w+2*stride), NW=buffer(w+stride), N=buffer(w), NE=buffer(w-stride), NEE=buffer(w-2*stride), NEEE=buffer(w-3*stride), NEEEE=buffer(w-4*stride);
        NNWWW=buffer(w*2+stride*3), NNWW=buffer((w+stride)*2), NNW=buffer(w*2+stride), NN=buffer(w*2), NNE=buffer(w*2-stride), NNEE=buffer((w-stride)*2), NNEEE=buffer(w*2-stride*3);
        NNNWW=buffer(w*3+stride*2), NNNW=buffer(w*3+stride), NNN=buffer(w*3), NNNE=buffer(w*3-stride), NNNEE=buffer(w*3-stride*2);
        NNNNW=buffer(w*4+stride), NNNN=buffer(w*4), NNNNE=buffer(w*4-stride);
        NNNNN=buffer(w*5);
        NNNNNN=buffer(w*6);
        WWp1=buffer(stride*2+1), Wp1=buffer(stride+1), p1=buffer(1), NWp1=buffer(w+stride+1), Np1=buffer(w+1), NEp1=buffer(w-stride+1), NNp1=buffer(w*2+1);
        WWp2=buffer(stride*2+2), Wp2=buffer(stride+2), p2=buffer(2), NWp2=buffer(w+stride+2), Np2=buffer(w+2), NEp2=buffer(w-stride+2), NNp2=buffer(w*2+2);

        int j = -1;
        MapCtxs[++j] = Clamp4(N+p1-Np1, W, NW, N, NE);
        MapCtxs[++j] = Clamp4(N+p2-Np2, W, NW, N, NE);
        MapCtxs[++j] = (W+Clamp4(NE*3-NNE*3+NNNE, W, N, NE, NEE))/2;
        MapCtxs[++j] = Clamp4((W+Clip(NE*2-NNE))/2, W, NW, N, NE);
        MapCtxs[++j] = (W+NEE)/2;
        MapCtxs[++j] = Clip((WWW-4*WW+6*W+Clip(NE*4-NNE*6+NNNE*4-NNNNE))/4);
        MapCtxs[++j] = Clip((-WWWW+5*WWW-10*WW+10*W+Clamp4(NE*4-NNE*6+NNNE*4-NNNNE, N, NE, NEE, NEEE))/5);
        MapCtxs[++j] = Clip((-4*WW+15*W+10*Clip(NE*3-NNE*3+NNNE)-Clip(NEEE*3-NNEEE*3+buffer(w*3-3*stride)))/20);
        MapCtxs[++j] = Clip((-3*WW+8*W+Clamp4(NEE*3-NNEE*3+NNNEE, NE, NEE, NEEE, NEEEE))/6);
        MapCtxs[++j] = Clip((W+Clip(NE*2-NNE))/2+p1-(Wp1+Clip(NEp1*2-buffer(w*2-stride+1)))/2);
        MapCtxs[++j] = Clip((W+Clip(NE*2-NNE))/2+p2-(Wp2+Clip(NEp2*2-buffer(w*2-stride+2)))/2);
        MapCtxs[++j] = Clip((-3*WW+8*W+Clip(NEE*2-NNEE))/6+p1-(-3*WWp1+8*Wp1+Clip(buffer(w-stride*2+1)*2-buffer(w*2-stride*2+1)))/6);
        MapCtxs[++j] = Clip((-3*WW+8*W+Clip(NEE*2-NNEE))/6+p2-(-3*WWp2+8*Wp2+Clip(buffer(w-stride*2+2)*2-buffer(w*2-stride*2+2)))/6);
        MapCtxs[++j] = Clip((W+NEE)/2+p1-(Wp1+buffer(w-stride*2+1))/2);
        MapCtxs[++j] = Clip((W+NEE)/2+p2-(Wp2+buffer(w-stride*2+2))/2);
        MapCtxs[++j] = Clip((WW+Clip(NEE*2-NNEE))/2+p1-(WWp1+Clip(buffer(w-stride*2+1)*2-buffer(w*2-stride*2+1)))/2);
        MapCtxs[++j] = Clip((WW+Clip(NEE*2-NNEE))/2+p2-(WWp2+Clip(buffer(w-stride*2+2)*2-buffer(w*2-stride*2+2)))/2);
        MapCtxs[++j] = Clip(WW+NEE-N+p1-Clip(WWp1+buffer(w-stride*2+1)-Np1));
        MapCtxs[++j] = Clip(WW+NEE-N+p2-Clip(WWp2+buffer(w-stride*2+2)-Np2));
        MapCtxs[++j] = Clip(W+N-NW);
        MapCtxs[++j] = Clip(W+N-NW+p1-Clip(Wp1+Np1-NWp1));
        MapCtxs[++j] = Clip(W+N-NW+p2-Clip(Wp2+Np2-NWp2));
        MapCtxs[++j] = Clip(W+NE-N);
        MapCtxs[++j] = Clip(N+NW-NNW);
        MapCtxs[++j] = Clip(N+NW-NNW+p1-Clip(Np1+NWp1-buffer(w*2+stride+1)));
        MapCtxs[++j] = Clip(N+NW-NNW+p2-Clip(Np2+NWp2-buffer(w*2+stride+2)));
        MapCtxs[++j] = Clip(N+NE-NNE);
        MapCtxs[++j] = Clip(N+NE-NNE+p1-Clip(Np1+NEp1-buffer(w*2-stride+1)));
        MapCtxs[++j] = Clip(N+NE-NNE+p2-Clip(Np2+NEp2-buffer(w*2-stride+2)));
        MapCtxs[++j] = Clip(N+NN-NNN);
        MapCtxs[++j] = Clip(N+NN-NNN+p1-Clip(Np1+NNp1-buffer(w*3+1)));
        MapCtxs[++j] = Clip(N+NN-NNN+p2-Clip(Np2+NNp2-buffer(w*3+2)));
        MapCtxs[++j] = Clip(W+WW-WWW);
        MapCtxs[++j] = Clip(W+WW-WWW+p1-Clip(Wp1+WWp1-buffer(stride*3+1)));
        MapCtxs[++j] = Clip(W+WW-WWW+p2-Clip(Wp2+WWp2-buffer(stride*3+2)));
        MapCtxs[++j] = Clip(W+NEE-NE);
        MapCtxs[++j] = Clip(W+NEE-NE+p1-Clip(Wp1+buffer(w-stride*2+1)-NEp1));
        MapCtxs[++j] = Clip(W+NEE-NE+p2-Clip(Wp2+buffer(w-stride*2+2)-NEp2));
        MapCtxs[++j] = Clip(NN+p1-NNp1);
        MapCtxs[++j] = Clip(NN+p2-NNp2);
        MapCtxs[++j] = Clip(NN+W-NNW);
        MapCtxs[++j] = Clip(NN+W-NNW+p1-Clip(NNp1+Wp1-buffer(w*2+stride+1)));
        MapCtxs[++j] = Clip(NN+W-NNW+p2-Clip(NNp2+Wp2-buffer(w*2+stride+2)));
        MapCtxs[++j] = Clip(NN+NW-NNNW);
        MapCtxs[++j] = Clip(NN+NW-NNNW+p1-Clip(NNp1+NWp1-buffer(w*3+stride+1)));
        MapCtxs[++j] = Clip(NN+NW-NNNW+p2-Clip(NNp2+NWp2-buffer(w*3+stride+2)));
        MapCtxs[++j] = Clip(NN+NE-NNNE);
        MapCtxs[++j] = Clip(NN+NE-NNNE+p1-Clip(NNp1+NEp1-buffer(w*3-stride+1)));
        MapCtxs[++j] = Clip(NN+NE-NNNE+p2-Clip(NNp2+NEp2-buffer(w*3-stride+2)));
        MapCtxs[++j] = Clip(NN+NNNN-NNNNNN);
        MapCtxs[++j] = Clip(NN+NNNN-NNNNNN+p1-Clip(NNp1+buffer(w*4+1)-buffer(w*6+1)));
        MapCtxs[++j] = Clip(NN+NNNN-NNNNNN+p2-Clip(NNp2+buffer(w*4+2)-buffer(w*6+2)));
        MapCtxs[++j] = Clip(WW+p1-WWp1);
        MapCtxs[++j] = Clip(WW+p2-WWp2);
        MapCtxs[++j] = Clip(WW+WWWW-WWWWWW);
        MapCtxs[++j] = Clip(WW+WWWW-WWWWWW+p1-Clip(WWp1+buffer(stride*4+1)-buffer(stride*6+1)));
        MapCtxs[++j] = Clip(WW+WWWW-WWWWWW+p2-Clip(WWp2+buffer(stride*4+2)-buffer(stride*6+2)));
        MapCtxs[++j] = Clip(N*2-NN+p1-Clip(Np1*2-NNp1));
        MapCtxs[++j] = Clip(N*2-NN+p2-Clip(Np2*2-NNp2));
        MapCtxs[++j] = Clip(W*2-WW+p1-Clip(Wp1*2-WWp1));
        MapCtxs[++j] = Clip(W*2-WW+p2-Clip(Wp2*2-WWp2));
        MapCtxs[++j] = Clip(N*3-NN*3+NNN);
        MapCtxs[++j] = Clamp4(N*3-NN*3+NNN, W, NW, N, NE);
        MapCtxs[++j] = Clamp4(W*3-WW*3+WWW, W, NW, N, NE);
        MapCtxs[++j] = Clamp4(N*2-NN, W, NW, N, NE);
        MapCtxs[++j] = Clip((NNNNN-6*NNNN+15*NNN-20*NN+15*N+Clamp4(W*4-NWW*6+NNWWW*4-buffer(w*3+4*stride), W, NW, N, NN))/6);
        MapCtxs[++j] = Clip((buffer(w*3-3*stride)-4*NNEE+6*NE+Clip(W*4-NW*6+NNW*4-NNNW))/4);
        MapCtxs[++j] = Clip(((N+3*NW)/4)*3-((NNW+NNWW)/2)*3+(NNNWW*3+buffer(w*3+3*stride))/4);
        MapCtxs[++j] = Clip((W*2+NW)-(WW+2*NWW)+NWWW);
        MapCtxs[++j] = (Clip(W*2-NW)+Clip(W*2-NWW)+N+NE)/4;
        MapCtxs[++j] = NNNNNN;
        MapCtxs[++j] = (NEEEE+buffer(w-6*stride))/2;
        MapCtxs[++j] = (WWWWWW+WWWW)/2;
        MapCtxs[++j] = ((W+N)*3-NW*2)/4;
        MapCtxs[++j] = N;
        MapCtxs[++j] = NN;
        assert(++j==nSM1);
        j = -1;
        SCMapCtxs[++j] = N+p1-Np1;
        SCMapCtxs[++j] = N+p2-Np2;
        SCMapCtxs[++j] = W+p1-Wp1;
        SCMapCtxs[++j] = W+p2-Wp2;
        SCMapCtxs[++j] = NW+p1-NWp1;
        SCMapCtxs[++j] = NW+p2-NWp2;
        SCMapCtxs[++j] = NE+p1-NEp1;
        SCMapCtxs[++j] = NE+p2-NEp2;
        SCMapCtxs[++j] = NN+p1-NNp1;
        SCMapCtxs[++j] = NN+p2-NNp2;
        SCMapCtxs[++j] = WW+p1-WWp1;
        SCMapCtxs[++j] = WW+p2-WWp2;
        SCMapCtxs[++j] = W+N-NW;
        SCMapCtxs[++j] = W+N-NW+p1-Wp1-Np1+NWp1;
        SCMapCtxs[++j] = W+N-NW+p2-Wp2-Np2+NWp2;
        SCMapCtxs[++j] = W+NE-N;
        SCMapCtxs[++j] = W+NE-N+p1-Wp1-NEp1+Np1;
        SCMapCtxs[++j] = W+NE-N+p2-Wp2-NEp2+Np2;
        SCMapCtxs[++j] = W+NEE-NE;
        SCMapCtxs[++j] = W+NEE-NE+p1-Wp1-buffer(w-stride*2+1)+NEp1;
        SCMapCtxs[++j] = W+NEE-NE+p2-Wp2-buffer(w-stride*2+2)+NEp2;
        SCMapCtxs[++j] = N+NN-NNN;
        SCMapCtxs[++j] = N+NN-NNN+p1-Np1-NNp1+buffer(w*3+1);
        SCMapCtxs[++j] = N+NN-NNN+p2-Np2-NNp2+buffer(w*3+2);
        SCMapCtxs[++j] = N+NE-NNE;
        SCMapCtxs[++j] = N+NE-NNE+p1-Np1-NEp1+buffer(w*2-stride+1);
        SCMapCtxs[++j] = N+NE-NNE+p2-Np2-NEp2+buffer(w*2-stride+2);
        SCMapCtxs[++j] = N+NW-NNW;
        SCMapCtxs[++j] = N+NW-NNW+p1-Np1-NWp1+buffer(w*2+stride+1);
        SCMapCtxs[++j] = N+NW-NNW+p2-Np2-NWp2+buffer(w*2+stride+2);
        SCMapCtxs[++j] = NE+NW-NN;
        SCMapCtxs[++j] = NE+NW-NN+p1-NEp1-NWp1+NNp1;
        SCMapCtxs[++j] = NE+NW-NN+p2-NEp2-NWp2+NNp2;
        SCMapCtxs[++j] = NW+W-NWW;
        SCMapCtxs[++j] = NW+W-NWW+p1-NWp1-Wp1+buffer(w+stride*2+1);
        SCMapCtxs[++j] = NW+W-NWW+p2-NWp2-Wp2+buffer(w+stride*2+2);
        SCMapCtxs[++j] = W*2-WW;
        SCMapCtxs[++j] = W*2-WW+p1-Wp1*2+WWp1;
        SCMapCtxs[++j] = W*2-WW+p2-Wp2*2+WWp2;
        SCMapCtxs[++j] = N*2-NN;
        SCMapCtxs[++j] = N*2-NN+p1-Np1*2+NNp1;
        SCMapCtxs[++j] = N*2-NN+p2-Np2*2+NNp2;
        SCMapCtxs[++j] = NW*2-NNWW;
        SCMapCtxs[++j] = NW*2-NNWW+p1-NWp1*2+buffer(w*2+stride*2+1);
        SCMapCtxs[++j] = NW*2-NNWW+p2-NWp2*2+buffer(w*2+stride*2+2);
        SCMapCtxs[++j] = NE*2-NNEE;
        SCMapCtxs[++j] = NE*2-NNEE+p1-NEp1*2+buffer(w*2-stride*2+1);
        SCMapCtxs[++j] = NE*2-NNEE+p2-NEp2*2+buffer(w*2-stride*2+2);
        SCMapCtxs[++j] = N*3-NN*3+NNN+p1-Np1*3+NNp1*3-buffer(w*3+1);
        SCMapCtxs[++j] = N*3-NN*3+NNN+p2-Np2*3+NNp2*3-buffer(w*3+2);
        SCMapCtxs[++j] = N*3-NN*3+NNN;
        SCMapCtxs[++j] = (W+NE*2-NNE)/2;
        SCMapCtxs[++j] = (W+NE*3-NNE*3+NNNE)/2;
        SCMapCtxs[++j] = (W+NE*2-NNE)/2+p1-(Wp1+NEp1*2-buffer(w*2-stride+1))/2;
        SCMapCtxs[++j] = (W+NE*2-NNE)/2+p2-(Wp2+NEp2*2-buffer(w*2-stride+2))/2;
        SCMapCtxs[++j] = NNE+NE-NNNE;
        SCMapCtxs[++j] = NNE+W-NN;
        SCMapCtxs[++j] = NNW+W-NNWW;
        SCMapCtxs[++j] = 0;
        assert(++j==nSSM);
        j = 0;
        for (int k=(color>0)?color-1:stride-1; j<nOLS; j++) {
          pOLS[j] = Clip(int(floor(ols[j][color].Predict(ols_ctxs[j]))));
          ols[j][k].Update(p1);
        }

        if (!isPNG) {
          int mean=W+NW+N+NE;
          const int var=(W*W+NW*NW+N*N+NE*NE-mean*mean/4)>>2;
          mean>>=2;
          const int logvar=ilog(var);

          U64 i=0;
          cm.set(hash(++i,        (N+1)>>1, LogMeanDiffQt(N, Clip(NN*2-NNN))));
          cm.set(hash(++i,        (W+1)>>1, LogMeanDiffQt(W, Clip(WW*2-WWW))));
          cm.set(hash(++i,        Clamp4(W+N-NW, W, NW, N, NE), LogMeanDiffQt(Clip(N+NE-NNE), Clip(N+NW-NNW))));
          cm.set(hash(++i,        (NNN+N+4)/8, Clip(N*3-NN*3+NNN)>>1));
          cm.set(hash(++i,        (WWW+W+4)/8, Clip(W*3-WW*3+WWW)>>1));
          cm.set(hash(++i, color, (W+Clip(NE*3-NNE*3+NNNE))/4, LogMeanDiffQt(N, (NW+NE)/2)));
          cm.set(hash(++i, color, Clip((-WWWW+5*WWW-10*WW+10*W+Clamp4(NE*4-NNE*6+NNNE*4-NNNNE, N, NE, NEE, NEEE))/5)/4));
          cm.set(hash(++i,        Clip(NEE+N-NNEE), LogMeanDiffQt(W, Clip(NW+NE-NNE))));
          cm.set(hash(++i,        Clip(NN+W-NNW), LogMeanDiffQt(W, Clip(NNW+WW-NNWW))));
          cm.set(hash(++i, color, p1));
          cm.set(hash(++i, color, p2));
          cm.set(hash(++i, color, Clip(W+N-NW)/2, Clip(W+p1-Wp1)/2));
          cm.set(hash(++i,        Clip(N*2-NN)/2, LogMeanDiffQt(N, Clip(NN*2-NNN))));
          cm.set(hash(++i,        Clip(W*2-WW)/2, LogMeanDiffQt(W, Clip(WW*2-WWW))));
          cm.set(hash(++i,        Clamp4(N*3-NN*3+NNN, W, NW, N, NE)/2));
          cm.set(hash(++i,        Clamp4(W*3-WW*3+WWW, W, N, NE, NEE)/2));
          cm.set(hash(++i, color, LogMeanDiffQt(W, Wp1), Clamp4((p1*W)/(Wp1<1?1:Wp1), W, N, NE, NEE))); //using max(1,Wp1) results in division by zero in VC2015
          cm.set(hash(++i, color, Clamp4(N+p2-Np2, W, NW, N, NE)));
          cm.set(hash(++i, color, Clip(W+N-NW), column[0]));
          cm.set(hash(++i, color, Clip(N*2-NN), LogMeanDiffQt(W, Clip(NW*2-NNW))));
          cm.set(hash(++i, color, Clip(W*2-WW), LogMeanDiffQt(N, Clip(NW*2-NWW))));
          cm.set(hash(++i,        (W+NEE)/2, LogMeanDiffQt(W, (WW+NE)/2)));
          cm.set(hash(++i,        (Clamp4(Clip(W*2-WW)+Clip(N*2-NN)-Clip(NW*2-NNWW), W, NW, N, NE))));
          cm.set(hash(++i, color, W, p2));
          cm.set(hash(++i,        N, NN&0x1F, NNN&0x1F));
          cm.set(hash(++i,        W, WW&0x1F, WWW&0x1F));
          cm.set(hash(++i, color, N, column[0]));
          cm.set(hash(++i, color, Clip(W+NEE-NE), LogMeanDiffQt(W, Clip(WW+NE-N))));
          cm.set(hash(++i,        NN, NNNN&0x1F, NNNNNN&0x1F, column[1]));
          cm.set(hash(++i,        WW, WWWW&0x1F, WWWWWW&0x1F, column[1]));
          cm.set(hash(++i,        NNN, NNNNNN&0x1F, buffer(w*9)&0x1F, column[1]));
          cm.set(hash(++i, color, column[1]));

          cm.set(hash(++i, color, W, LogMeanDiffQt(W, WW)));
          cm.set(hash(++i, color, W, p1));
          cm.set(hash(++i, color, W/4, LogMeanDiffQt(W, p1), LogMeanDiffQt(W, p2)));
          cm.set(hash(++i, color, N, LogMeanDiffQt(N, NN)));
          cm.set(hash(++i, color, N, p1));
          cm.set(hash(++i, color, N/4, LogMeanDiffQt(N, p1), LogMeanDiffQt(N, p2)));
          cm.set(hash(++i, color, (W+N)>>3, p1>>4, p2>>4));
          cm.set(hash(++i, color, p1/2, p2/2));
          cm.set(hash(++i, color, W, p1-Wp1));
          cm.set(hash(++i, color, W+p1-Wp1));
          cm.set(hash(++i, color, N, p1-Np1));
          cm.set(hash(++i, color, N+p1-Np1));
          cm.set(hash(++i, color, mean, logvar>>4));

          ctx[0] = (min(color, stride-1)<<9)|((abs(W-N)>3)<<8)|((W>N)<<7)|((W>NW)<<6)|((abs(N-NW)>3)<<5)|((N>NW)<<4)|((abs(N-NE)>3)<<3)|((N>NE)<<2)|((W>WW)<<1)|(N>NN);
          ctx[1] = ((LogMeanDiffQt(p1, Clip(Np1+NEp1-buffer(w*2-stride+1)))>>1)<<5)|((LogMeanDiffQt(Clip(N+NE-NNE), Clip(N+NW-NNW))>>1)<<2)|min(color, stride-1);
        }
        else {
          int residuals[5] ={ ((int8_t)buf(stride+(x<=stride)))+128,
                              ((int8_t)buf(1+(x<2)))+128,
                              ((int8_t)buf(stride+1+(x<=stride)))+128,
                              ((int8_t)buf(2+(x<3)))+128,
                              ((int8_t)buf(stride+2+(x<=stride)))+128
          };
          R1 = (residuals[1]*residuals[0])/max(1, residuals[2]);
          R2 = (residuals[3]*residuals[0])/max(1, residuals[4]);

          U64 i=(filterOn ? filter+1 : 0)*1024;
          cm.set(0);
          cm.set(hash(++i, color, Clip(W+N-NW)-px, Clip(W+p1-Wp1)-px, R1));
          cm.set(hash(++i, color, Clip(W+N-NW)-px, LogMeanDiffQt(p1, Clip(Wp1+Np1-NWp1))));
          cm.set(hash(++i, color, Clip(W+N-NW)-px, LogMeanDiffQt(p2, Clip(Wp2+Np2-NWp2)), R2/4));
          cm.set(hash(++i, color, Clip(W+N-NW)-px, Clip(N+NE-NNE)-Clip(N+NW-NNW)));
          cm.set(hash(++i, color, Clip(W+N-NW+p1-(Wp1+Np1-NWp1)), px, R1));
          cm.set(hash(++i, color, Clamp4(W+N-NW, W, NW, N, NE)-px, column[0]));
          cm.set(hash(++i,        Clamp4(W+N-NW, W, NW, N, NE)/8, px));
          cm.set(hash(++i, color, N-px, Clip(N+p1-Np1)-px));
          cm.set(hash(++i, color, Clip(W+p1-Wp1)-px, R1));
          cm.set(hash(++i, color, Clip(N+p1-Np1)-px));
          cm.set(hash(++i, color, Clip(N+p1-Np1)-px, Clip(N+p2-Np2)-px));
          cm.set(hash(++i, color, Clip(W+p1-Wp1)-px, Clip(W+p2-Wp2)-px));
          cm.set(hash(++i, color, Clip(NW+p1-NWp1)-px));
          cm.set(hash(++i, color, Clip(NW+p1-NWp1)-px, column[0]));
          cm.set(hash(++i, color, Clip(NE+p1-NEp1)-px, column[0]));
          cm.set(hash(++i, color, Clip(NE+N-NNE)-px, Clip(NE+p1-NEp1)-px));
          cm.set(hash(++i,        Clip(N+NE-NNE)-px, column[0]));
          cm.set(hash(++i, color, Clip(NW+N-NNW)-px, Clip(NW+p1-NWp1)-px));
          cm.set(hash(++i,        Clip(N+NW-NNW)-px, column[0]));
          cm.set(hash(++i,        Clip(NN+W-NNW)-px, LogMeanDiffQt(N, Clip(NNN+NW-NNNW))));
          cm.set(hash(++i,        Clip(W+NEE-NE)-px, LogMeanDiffQt(W, Clip(WW+NE-N))));
          cm.set(hash(++i, color, Clip(N+NN-NNN+buffer(1+(!color))-Clip(buffer(w+1+(!color))+buffer(w*2+1+(!color))-buffer(w*3+1+(!color))))-px));
          cm.set(hash(++i,        Clip(N+NN-NNN)-px, Clip(5*N-10*NN+10*NNN-5*NNNN+NNNNN)-px));
          cm.set(hash(++i, color, Clip(N*2-NN)-px, LogMeanDiffQt(N, Clip(NN*2-NNN))));
          cm.set(hash(++i, color, Clip(W*2-WW)-px, LogMeanDiffQt(W, Clip(WW*2-WWW))));
          cm.set(hash(++i,        Clip(N*3-NN*3+NNN)-px));
          cm.set(hash(++i, color, Clip(N*3-NN*3+NNN)-px, LogMeanDiffQt(W, Clip(NW*2-NNW))));
          cm.set(hash(++i,        Clip(W*3-WW*3+WWW)-px));
          cm.set(hash(++i, color, Clip(W*3-WW*3+WWW)-px, LogMeanDiffQt(N, Clip(NW*2-NWW))));
          cm.set(hash(++i,        Clip((35*N-35*NNN+21*NNNNN-5*buffer(w*7))/16)-px));
          cm.set(hash(++i, color, (W+Clip(NE*3-NNE*3+NNNE))/2-px, R2));
          cm.set(hash(++i, color, (W+Clamp4(NE*3-NNE*3+NNNE, W, N, NE, NEE))/2-px, LogMeanDiffQt(N, (NW+NE)/2)));
          cm.set(hash(++i, color, (W+NEE)/2-px, R1/2));
          cm.set(hash(++i, color, Clamp4(Clip(W*2-WW)+Clip(N*2-NN)-Clip(NW*2-NNWW), W, NW, N, NE)-px));
          cm.set(hash(++i, color, buf(stride+(x<=stride)), buf(1+(x<2)), buf(2+(x<3))));
          cm.set(hash(++i, color, buf(1+(x<2)), px));
          cm.set(hash(++i,        buf(w+1), buf((w+1)*2), buf((w+1)*3), px));
          ctx[0] = (min(color, stride-1)<<9)|((abs(W-N)>3)<<8)|((W>N)<<7)|((W>NW)<<6)|((abs(N-NW)>3)<<5)|((N>NW)<<4)|((N>NE)<<3)|min(5, filterOn?filter+1:0);
          ctx[1] = ((LogMeanDiffQt(p1, Clip(Np1+NEp1-buffer(w*2-stride+1)))>>1)<<5)|((LogMeanDiffQt(Clip(N+NE-NNE), Clip(N+NW-NNW))>>1)<<2)|min(color, stride-1);
        }

        int i=-1;
        Map[++i].set_direct((W&0xC0)|((N&0xC0)>>2)|((WW&0xC0)>>4)|(NN>>6));
        Map[++i].set_direct((N&0xC0)|((NN&0xC0)>>2)|((NE&0xC0)>>4)|(NEE>>6));
        Map[++i].set_direct(buf(1+(isPNG && x<2)));
        Map[++i].set_direct(min(color, stride-1));
        Map[++i].set_direct(0);
        stats->Image.plane = std::min<int>(color, stride-1);
        stats->Image.pixels.W = W;
        stats->Image.pixels.N = N;
        stats->Image.pixels.NN = NN;
        stats->Image.pixels.WW = WW;
        stats->Image.pixels.Wp1 = Wp1;
        stats->Image.pixels.Np1 = Np1;
        stats->Image.ctx = ctx[0]>>3;
      }
    }
    if (x>0 || !isPNG) {
      INJECT_SHARED_c0
      U8 B=(c0<<(8-bpos));
      int i=4;

      Map[++i].set_direct((((U8)(Clip(W+N-NW)-px-B))*8+bpos)|(LogMeanDiffQt(Clip(N+NE-NNE), Clip(N+NW-NNW))<<11));
      Map[++i].set_direct((((U8)(Clip(N*2-NN)-px-B))*8+bpos)|(LogMeanDiffQt(W, Clip(NW*2-NNW))<<11));
      Map[++i].set_direct((((U8)(Clip(W*2-WW)-px-B))*8+bpos)|(LogMeanDiffQt(N, Clip(NW*2-NWW))<<11));
      Map[++i].set_direct((((U8)(Clip(W+N-NW)-px-B))*8+bpos)|(LogMeanDiffQt(p1, Clip(Wp1+Np1-NWp1))<<11));
      Map[++i].set_direct((((U8)(Clip(W+N-NW)-px-B))*8+bpos)|(LogMeanDiffQt(p2, Clip(Wp2+Np2-NWp2))<<11));
      Map[++i].set(hash(W-px-B, N-px-B)*8+bpos);
      Map[++i].set(hash(W-px-B, WW-px-B)*8+bpos);
      Map[++i].set(hash(N-px-B, NN-px-B)*8+bpos);
      Map[++i].set(hash(Clip(N+NE-NNE)-px-B, Clip(N+NW-NNW)-px-B)*8+bpos);
      Map[++i].set_direct((min(color, stride-1)<<11)|(((U8)(Clip(N+p1-Np1)-px-B))*8+bpos));
      Map[++i].set_direct((min(color, stride-1)<<11)|(((U8)(Clip(N+p2-Np2)-px-B))*8+bpos));
      Map[++i].set_direct((min(color, stride-1)<<11)|(((U8)(Clip(W+p1-Wp1)-px-B))*8+bpos));
      Map[++i].set_direct((min(color, stride-1)<<11)|(((U8)(Clip(W+p2-Wp2)-px-B))*8+bpos));
      ++i;
      assert(i==nSM0);

      for (int j=0; j<nSM1; i++, j++)
        Map[i].set_direct((MapCtxs[j]-px-B)*8+bpos);

      for (int j=0; i<nSM; i++, j++)
        Map[i].set_direct((pOLS[j]-px-B)*8+bpos);

      for (int i=0; i<nSSM; i++)
        SCMap[i].set((SCMapCtxs[i]-px-B)*8+bpos);
    }   
  }
  void init() { //new image
    stride = 3+alpha;
    w = info&0xFFFFFF;
    padding = w%stride;
    x = color = line = px = 0;
    filterOn = false;
    columns[0] = max(1, w/max(1, ilog2(w)*3));
    columns[1] = max(1, columns[0]/max(1, ilog2(columns[0])));
    if (lastPos>0 && lastWasPNG!=isPNG) {
      for (int i=0; i<nSM; i++)
        Map[i].Reset(0);
    }
    lastWasPNG = isPNG;
    buffer.fill(0x7F);
  }
  void setparam(int info0, U32 alpha0, U32 isPNG0) {
    info=info0;
    alpha=alpha0;
    isPNG=isPNG0;
  }
  void mix(Mixer& m) {
    INJECT_SHARED_bpos INJECT_SHARED_pos
    if (bpos==0) {
      if ((color<0) || (pos-lastPos!=1))
        init();
      else {
        x++;
        if (x>=w+(int)isPNG) { x=0; line++; }
      }
      lastPos = pos;
    }
    
    update();
    
    // Predict next bit
    if (x>0 || !isPNG){
      cm.mix(m);
      
      for (int i=0;i<nSM;i++)
        Map[i].mix(m);
        
      for (int i=0;i<nSSM;i++)
        SCMap[i].mix(m);
        
      if (++col>=stride*8) col=0;
      m.set(5, 6);
      m.set(min(63,column[0])+((ctx[0]>>3)&0xC0), 256);
      m.set(min(127,column[1])+((ctx[0]>>2)&0x180), 512);
      INJECT_SHARED_c0 
      m.set((ctx[0]&0x7FC)|(bpos>>1), 2048);
      m.set(col+(isPNG?(ctx[0]&7)+1:(c0==((0x100|((N+W)/2))>>(8-bpos))))*32, 8*32);
      m.set(((isPNG?p1:0)>>4)*stride+(x%stride) + min(5,filterOn?filter+1:0)*64, 6*64);
      m.set(c0+256*(isPNG && abs(R1-128)>8), 256*2);
      m.set((ctx[1]<<2)|(bpos>>1), 1024);
      m.set(finalize64(hash(LogMeanDiffQt(W,WW,5), LogMeanDiffQt(N,NN,5), LogMeanDiffQt(W,N,5), ilog2(W), color),13), 8192);
      m.set(finalize64(hash(ctx[0], column[0]/8),13), 8192);
      m.set(finalize64(hash(LogQt(N,5), LogMeanDiffQt(N,NN,3), c0),13), 8192);
      m.set(finalize64(hash(LogQt(W,5), LogMeanDiffQt(W,WW,3), c0),13), 8192);
      m.set(min(255,(x+line)/32), 256);
    }
    else {
      m.add(-2048+((filter>>(7-bpos))&1)*4096);
      m.set(min(4,filter), MIXERCONTEXTSETS);
    }
  }
};

//////////////////////////// Image8bitModel /////////////////////////////////

// Model for 8-bit image data
class Image8bitModel {
private:
  static constexpr int nSM0 = 2;
  static constexpr int nSM1 = 55;
  static constexpr int nOLS = 5;
  static constexpr int nSM = nSM0 + nSM1 + nOLS;
  static constexpr int nPltMaps = 4;
  static constexpr int nCM = nPltMaps+49;
public:
  static constexpr int MIXERINPUTS = nSM*StationaryMap::MIXERINPUTS + nCM*ContextMap::MIXERINPUTS;
  static constexpr int MIXERCONTEXTS =(2048+5)+6*16+6*32+256+1024+64+128+256; // 4069
  static constexpr int MIXERCONTEXTSETS = 8;

  const Shared * const shared;
  ModelStats *stats;
  ContextMap cm;
  StationaryMap Map[nSM];
  SmallStationaryContextMap pltMap[nPltMaps];
  IndirectMap sceneMap[5];
  IndirectContext<U8> iCtx[nPltMaps];
  Buf buffer{0x100000}; // internal rotating buffer for (PNG unfiltered) pixel data
  Array<short> jumps{0x8000};
  //pixel neighborhood
  U8 WWWWWW=0, WWWWW=0, WWWW=0, WWW=0, WW=0, W=0;
  U8 NWWWW=0, NWWW=0, NWW=0, NW=0, N=0, NE=0, NEE=0, NEEE=0, NEEEE=0;
  U8 NNWWW=0, NNWW=0, NNW=0, NN=0, NNE=0, NNEE=0, NNEEE=0;
  U8 NNNWW=0, NNNW=0, NNN=0, NNNE=0, NNNEE=0;
  U8 NNNNW=0, NNNN=0, NNNNE=0;
  U8 NNNNN=0;
  U8 NNNNNN=0;
  U8 px = 0, res = 0, prvFrmPx = 0, prvFrmPred = 0; // current PNG filter prediction, expected residual, corresponding pixel in previous frame
  U32 lastPos=0, lastWasPNG=0;
  U32 gray=0, isPNG=0;
  int w=0, ctx=0, col=0, line=0, x=0, filter=0, jump=0;
  int framePos=0, prevFramePos=0, frameWidth=0, prevFrameWidth=0;
  bool filterOn = false;
  int columns[2] = {1,1}, column[2]{};
  U8 MapCtxs[nSM1] = { 0 }, pOLS[nOLS] = { 0 };
  static constexpr double lambda[nOLS] ={ 0.996, 0.87, 0.93, 0.8, 0.9 };
  static constexpr int num[nOLS] ={ 32, 12, 15, 10, 14 };
  OLS<double, U8> ols[nOLS] = { 
    {num[0], 1, lambda[0]},
    {num[1], 1, lambda[1]},
    {num[2], 1, lambda[2]},
    {num[3], 1, lambda[3]},
    {num[4], 1, lambda[4]}
  };
  OLS<double, U8> sceneOls{13, 1, 0.994};
  const U8 *ols_ctx1[32] = { &WWWWWW, &WWWWW, &WWWW, &WWW, &WW, &W, &NWWWW, &NWWW, &NWW, &NW, &N, &NE, &NEE, &NEEE, &NEEEE, &NNWWW, &NNWW, &NNW, &NN, &NNE, &NNEE, &NNEEE, &NNNWW, &NNNW, &NNN, &NNNE, &NNNEE, &NNNNW, &NNNN, &NNNNE, &NNNNN, &NNNNNN };
  const U8 *ols_ctx2[12] = { &WWW, &WW, &W, &NWW, &NW, &N, &NE, &NEE, &NNW, &NN, &NNE, &NNN }; 
  const U8 *ols_ctx3[15] = { &N, &NE, &NEE, &NEEE, &NEEEE, &NN, &NNE, &NNEE, &NNEEE, &NNN, &NNNE, &NNNEE, &NNNN, &NNNNE, &NNNNN };
  const U8 *ols_ctx4[10] = { &N, &NE, &NEE, &NEEE, &NN, &NNE, &NNEE, &NNN, &NNNE, &NNNN };
  const U8 *ols_ctx5[14] = { &WWWW, &WWW, &WW, &W, &NWWW, &NWW, &NW, &N, &NNWW, &NNW, &NN, &NNNW, &NNN, &NNNN };
  const U8 **ols_ctxs[nOLS] = { &ols_ctx1[0], &ols_ctx2[0], &ols_ctx3[0], &ols_ctx4[0], &ols_ctx5[0] };
public:
  Image8bitModel(const Shared * const sh, ModelStats *st, const U64 size): 
    shared(sh), stats(st), cm(sh,size,nCM),
    Map{ /* StationaryMap: BitsOfContext, InputBits, Scale, Limit  */
    /*nSM0: 0- 1*/ {sh, 0,8,64,1023}, {sh,15,1,64,1023},
    /*nSM1: 0- 4*/ {sh,11,1,64,1023}, {sh,11,1,64,1023}, {sh,11,1,64,1023}, {sh,11,1,64,1023}, {sh,11,1,64,1023}, 
    /*nSM1: 5- 9*/ {sh,11,1,64,1023}, {sh,11,1,64,1023}, {sh,11,1,64,1023}, {sh,11,1,64,1023}, {sh,11,1,64,1023}, 
    /*nSM1:10-14*/ {sh,11,1,64,1023}, {sh,11,1,64,1023}, {sh,11,1,64,1023}, {sh,11,1,64,1023}, {sh,11,1,64,1023}, 
    /*nSM1:15-19*/ {sh,11,1,64,1023}, {sh,11,1,64,1023}, {sh,11,1,64,1023}, {sh,11,1,64,1023}, {sh,11,1,64,1023},
    /*nSM1:20-24*/ {sh,11,1,64,1023}, {sh,11,1,64,1023}, {sh,11,1,64,1023}, {sh,11,1,64,1023}, {sh,11,1,64,1023},
    /*nSM1:25-29*/ {sh,11,1,64,1023}, {sh,11,1,64,1023}, {sh,11,1,64,1023}, {sh,11,1,64,1023}, {sh,11,1,64,1023}, 
    /*nSM1:30-34*/ {sh,11,1,64,1023}, {sh,11,1,64,1023}, {sh,11,1,64,1023}, {sh,11,1,64,1023}, {sh,11,1,64,1023},
    /*nSM1:35-39*/ {sh,11,1,64,1023}, {sh,11,1,64,1023}, {sh,11,1,64,1023}, {sh,11,1,64,1023}, {sh,11,1,64,1023},
    /*nSM1:40-44*/ {sh,11,1,64,1023}, {sh,11,1,64,1023}, {sh,11,1,64,1023}, {sh,11,1,64,1023}, {sh,11,1,64,1023},
    /*nSM1:45-49*/ {sh,11,1,64,1023}, {sh,11,1,64,1023}, {sh,11,1,64,1023}, {sh,11,1,64,1023}, {sh,11,1,64,1023},
    /*nSM1:50-54*/ {sh,11,1,64,1023}, {sh,11,1,64,1023}, {sh,11,1,64,1023}, {sh,11,1,64,1023}, {sh,11,1,64,1023},
    /*nOLS:   0- 4*/ {sh,11,1,64,1023}, {sh,11,1,64,1023}, {sh,11,1,64,1023}, {sh,11,1,64,1023}, {sh,11,1,64,1023} 
  },
  pltMap{   /* SmallStationaryContextMap: BitsOfContext, InputBits, Rate, Scale */
    {sh,11,1,7,64}, {sh,11,1,7,64}, {sh,11,1,7,64}, {sh,11,1,7,64}
  },
  sceneMap{ /* IndirectMap: BitsOfContext, InputBits, Scale, Limit */
    {sh,8,8,64,255}, {sh,8,8,64,255}, {sh,22,1,64,255}, {sh,11,1,64,255}, {sh,11,1,64,255} 
  },
  iCtx{     /* IndirectContext<U8>: BitsPerContext, InputBits */
    {16,8}, {16,8}, {16,8}, {16,8}
  }
  {}
  void setparam(int info0, U32 gray0, U32 isPNG0) {
    w=info0;
    gray=gray0;
    isPNG=isPNG0;
  }
  void mix(Mixer& m) {
    // Select nearby pixels as context
    INJECT_SHARED_bpos 
    if (bpos==0) {
      INJECT_SHARED_c1 INJECT_SHARED_buf INJECT_SHARED_pos
      if (pos!=lastPos+1){
        x = line = px = jump = 0;
        filterOn = false;
        columns[0] = max(1,w/max(1,ilog2(w)*2));
        columns[1] = max(1,columns[0]/max(1,ilog2(columns[0])));
        if (gray){
          if (lastPos && lastWasPNG!=isPNG){
            for (int i=0;i<nSM;i++)
              Map[i].Reset(0);
          }
          lastWasPNG = isPNG;
        }
        buffer.fill(0x7F);
        prevFramePos = framePos;
        framePos = pos;
        prevFrameWidth = frameWidth;
        frameWidth = w;
      }
      else{
        x++;
        if (x>=w+(int)isPNG){x=0; line++;}
      }
      lastPos = pos;

      if (isPNG){
        if (x==1)
          filter = (U8)c1;
        else{
          switch (filter){
            case 1: {
              buffer.add((U8)( c1 + buffer(1)*(x>2 || !x) ) );
              filterOn = x>1;
              px = buffer(1);
              break;
            }
            case 2: {
              buffer.add((U8)( c1 + buffer(w)*(filterOn=(line>0)) ) );
              px = buffer(w);
              break;
            }
            case 3: {
              buffer.add((U8)( c1 + (buffer(w)*(line>0) + buffer(1)*(x>2 || !x))/2 ) );
              filterOn = (x>1 || line>0);
              px = (buffer(1)*(x>1)+buffer(w)*(line>0))/2;
              break;
            }
            case 4: {
              buffer.add((U8)( c1 + Paeth(buffer(1)*(x>2 || !x), buffer(w)*(line>0), buffer(w+1)*(line>0 && (x>2 || !x))) ) );
              filterOn = (x>1 || line>0);
              px = Paeth(buffer(1)*(x>1),buffer(w)*(line>0),buffer(w+1)*(x>1 && line>0));
              break;
            }
            default: buffer.add(c1);
              filterOn = false;
              px = 0;
          }
          if (!filterOn) px=0;
        }
      }
      else { // non-png
        buffer.add(c1);
        if (x==0) {
          memset(&jumps[0], 0, sizeof(short)*jumps.size());
          if (line>0 && w>8) {
            U8 bMask = 0xFF-((1<<gray)-1);
            U32 pMask = bMask*0x01010101u;
            U32 left=0, right=0;
            int l=min(w, (int)jumps.size()), end=l-4;
            do {
              left = ((buffer(l-x)<<24)|(buffer(l-x-1)<<16)|(buffer(l-x-2)<<8)|buffer(l-x-3))&pMask;
              int i = end;
              while (i>=x+4) {
                right = ((buffer(l-i-3)<<24)|(buffer(l-i-2)<<16)|(buffer(l-i-1)<<8)|buffer(l-i))&pMask;
                if (left==right) {
                  int j=(i+3-x-1)/2, k=0;
                  for (; k<=j; k++) {
                    if (k<4 || (buffer(l-x-k)&bMask)==(buffer(l-i-3+k)&bMask)) {
                      jumps[x+k] = -(x+(l-i-3)+2*k);
                      jumps[i+3-k] = i+3-x-2*k;
                    }
                    else
                      break;
                  }
                  x+=k;
                  end-=k;
                  break;
                }
                i--;
              }
              x++;
              if (x>end)
                break;
            } while (x+4<l);
            x = 0;
          }
        }
      }

      if (x || !isPNG){
        column[0]=(x-isPNG)/columns[0];
        column[1]=(x-isPNG)/columns[1];
        WWWWW=buffer(5), WWWW=buffer(4), WWW=buffer(3), WW=buffer(2), W=buffer(1);
        NWWWW=buffer(w+4), NWWW=buffer(w+3), NWW=buffer(w+2), NW=buffer(w+1), N=buffer(w), NE=buffer(w-1), NEE=buffer(w-2), NEEE=buffer(w-3), NEEEE=buffer(w-4);
        NNWWW=buffer(w*2+3), NNWW=buffer(w*2+2), NNW=buffer(w*2+1), NN=buffer(w*2), NNE=buffer(w*2-1), NNEE=buffer(w*2-2), NNEEE=buffer(w*2-3);
        NNNWW=buffer(w*3+2), NNNW=buffer(w*3+1), NNN=buffer(w*3), NNNE=buffer(w*3-1), NNNEE=buffer(w*3-2);
        NNNNW=buffer(w*4+1), NNNN=buffer(w*4), NNNNE=buffer(w*4-1);
        NNNNN=buffer(w*5);
        NNNNNN=buffer(w*6);
        if (prevFramePos>0 && prevFrameWidth==w){
          int offset = prevFramePos+line*w+x;
          prvFrmPx = buf[offset];
          if (gray) {
            sceneOls.Update(W);
            sceneOls.Add(W); sceneOls.Add(NW); sceneOls.Add(N); sceneOls.Add(NE);
            for (int i=-1; i<2; i++) {
              for (int j=-1; j<2; j++)
                sceneOls.Add(buf[offset+i*w+j]);
            }
            prvFrmPred = Clip(int(floor(sceneOls.Predict())));
          }
          else
            prvFrmPred = W;
        }
        else {
          prvFrmPx = prvFrmPred = W;
        }
        sceneMap[0].set_direct(prvFrmPx);
        sceneMap[1].set_direct(prvFrmPred);

        int j = 0;
        jump = jumps[min(x,(int)jumps.size()-1)];
        U64 i= (filterOn ? (filter+1)*64 : 0) + (gray*1024);
        cm.set(hash(++i, (jump!=0)?(0x100|buffer(abs(jump)))*(1-2*(jump<0)):N, line&3));
        if (!gray) {
          for (j=0; j<nPltMaps; j++)
            iCtx[j]+=W;
          iCtx[0]=W|(NE<<8);
          iCtx[1]=W|(N<<8);
          iCtx[2]=W|(WW<<8);
          iCtx[3]=N|(NN<<8);

          cm.set(hash(++i, W, px));
          cm.set(hash(++i, W, px, column[0]));
          cm.set(hash(++i, N, px));
          cm.set(hash(++i, N, px, column[0]));
          cm.set(hash(++i, NW, px));
          cm.set(hash(++i, NW, px, column[0]));
          cm.set(hash(++i, NE, px));
          cm.set(hash(++i, NE, px, column[0]));
          cm.set(hash(++i, NWW, px));
          cm.set(hash(++i, NEE, px));
          cm.set(hash(++i, WW, px));
          cm.set(hash(++i, NN, px));
          cm.set(hash(++i, W, N, px));
          cm.set(hash(++i, W, NW, px));
          cm.set(hash(++i, W, NE, px));
          cm.set(hash(++i, W, NEE, px));
          cm.set(hash(++i, W, NWW, px));
          cm.set(hash(++i, N, NW, px));
          cm.set(hash(++i, N, NE, px));
          cm.set(hash(++i, NW, NE, px));
          cm.set(hash(++i, W, WW, px));
          cm.set(hash(++i, N, NN, px));
          cm.set(hash(++i, NW, NNWW, px));
          cm.set(hash(++i, NE, NNEE, px));
          cm.set(hash(++i, NW, NWW, px));
          cm.set(hash(++i, NW, NNW, px));
          cm.set(hash(++i, NE, NEE, px));
          cm.set(hash(++i, NE, NNE, px));
          cm.set(hash(++i, N, NNW, px));
          cm.set(hash(++i, N, NNE, px));
          cm.set(hash(++i, N, NNN, px));
          cm.set(hash(++i, W, WWW, px));
          cm.set(hash(++i, WW, NEE, px));
          cm.set(hash(++i, WW, NN, px));
          cm.set(hash(++i, W, buffer(w-3), px));
          cm.set(hash(++i, W, buffer(w-4), px));
          cm.set(hash(++i, W, N,NW, px));
          cm.set(hash(++i, N, NN,NNN, px));
          cm.set(hash(++i, W, NE,NEE, px));
          cm.set(hash(++i, W, NW,N,NE, px));
          cm.set(hash(++i, N, NE,NN,NNE, px));
          cm.set(hash(++i, N, NW,NNW,NN, px));
          cm.set(hash(++i, W, WW,NWW,NW, px));
          cm.set(hash(++i, W, NW<<8 | N, WW<<8 | NWW, px));
          cm.set(hash(++i, px, column[0]));
          cm.set(hash(++i, px));
          cm.set(hash(++i, N, px, column[1] ));
          cm.set(hash(++i, W, px, column[1] ));
          for (int j=0; j<nPltMaps; j++)
            cm.set(hash(++i, iCtx[j](), px));

          ctx = min(0x1F,(x-isPNG)/min(0x20,columns[0]));
          res = W;
        }
        else{ // gray
          MapCtxs[j++] = Clamp4(W+N-NW, W, NW, N, NE);
          MapCtxs[j++] = Clip(W+N-NW);
          MapCtxs[j++] = Clamp4(W+NE-N, W, NW, N, NE);
          MapCtxs[j++] = Clip(W+NE-N);
          MapCtxs[j++] = Clamp4(N+NW-NNW, W, NW, N, NE);
          MapCtxs[j++] = Clip(N+NW-NNW);
          MapCtxs[j++] = Clamp4(N+NE-NNE, W, N, NE, NEE);
          MapCtxs[j++] = Clip(N+NE-NNE);
          MapCtxs[j++] = (W+NEE)/2;
          MapCtxs[j++] = Clip(N*3-NN*3+NNN);
          MapCtxs[j++] = Clip(W*3-WW*3+WWW);
          MapCtxs[j++] = (W+Clip(NE*3-NNE*3+buffer(w*3-1)))/2;
          MapCtxs[j++] = (W+Clip(NEE*3-buffer(w*2-3)*3+buffer(w*3-4)))/2;
          MapCtxs[j++] = Clip(NN+buffer(w*4)-buffer(w*6));
          MapCtxs[j++] = Clip(WW+buffer(4)-buffer(6));
          MapCtxs[j++] = Clip((buffer(w*5)-6*buffer(w*4)+15*NNN-20*NN+15*N+Clamp4(W*2-NWW, W, NW, N, NN))/6);
          MapCtxs[j++] = Clip((-3*WW+8*W+Clamp4(NEE*3-NNEE*3+buffer(w*3-2), NE, NEE, buffer(w-3), buffer(w-4)))/6);
          MapCtxs[j++] = Clip(NN+NW-buffer(w*3+1));
          MapCtxs[j++] = Clip(NN+NE-buffer(w*3-1));
          MapCtxs[j++] = Clip((W*2+NW)-(WW+2*NWW)+buffer(w+3));
          MapCtxs[j++] = Clip(((NW+NWW)/2)*3-buffer(w*2+3)*3+(buffer(w*3+4)+buffer(w*3+5))/2);
          MapCtxs[j++] = Clip(NEE+NE-buffer(w*2-3));
          MapCtxs[j++] = Clip(NWW+WW-buffer(w+4));
          MapCtxs[j++] = Clip(((W+NW)*3-NWW*6+buffer(w+3)+buffer(w*2+3))/2);
          MapCtxs[j++] = Clip((NE*2+NNE)-(NNEE+buffer(w*3-2)*2)+buffer(w*4-3));
          MapCtxs[j++] = buffer(w*6);
          MapCtxs[j++] = (buffer(w-4)+buffer(w-6))/2;
          MapCtxs[j++] = (buffer(4)+buffer(6))/2;
          MapCtxs[j++] = (W+N+buffer(w-5)+buffer(w-7))/4;
          MapCtxs[j++] = Clip(buffer(w-3)+W-NEE);
          MapCtxs[j++] = Clip(4*NNN-3*buffer(w*4));
          MapCtxs[j++] = Clip(N+NN-NNN);
          MapCtxs[j++] = Clip(W+WW-WWW);
          MapCtxs[j++] = Clip(W+NEE-NE);
          MapCtxs[j++] = Clip(WW+NEE-N);
          MapCtxs[j++] = (Clip(W*2-NW)+Clip(W*2-NWW)+N+NE)/4;
          MapCtxs[j++] = Clamp4(N*2-NN, W, N, NE, NEE);
          MapCtxs[j++] = (N+NNN)/2;
          MapCtxs[j++] = Clip(NN+W-NNW);
          MapCtxs[j++] = Clip(NWW+N-NNWW);
          MapCtxs[j++] = Clip((4*WWW-15*WW+20*W+Clip(NEE*2-NNEE))/10);
          MapCtxs[j++] = Clip((buffer(w*3-3)-4*NNEE+6*NE+Clip(W*3-NW*3+NNW))/4);
          MapCtxs[j++] = Clip((N*2+NE)-(NN+2*NNE)+buffer(w*3-1));
          MapCtxs[j++] = Clip((NW*2+NNW)-(NNWW+buffer(w*3+2)*2)+buffer(w*4+3));
          MapCtxs[j++] = Clip(NNWW+W-buffer(w*2+3));
          MapCtxs[j++] = Clip((-buffer(w*4)+5*NNN-10*NN+10*N+Clip(W*4-NWW*6+buffer(w*2+3)*4-buffer(w*3+4)))/5);
          MapCtxs[j++] = Clip(NEE+Clip(buffer(w-3)*2-buffer(w*2-4))-buffer(w-4));
          MapCtxs[j++] = Clip(NW+W-NWW);
          MapCtxs[j++] = Clip((N*2+NW)-(NN+2*NNW)+buffer(w*3+1));
          MapCtxs[j++] = Clip(NN+Clip(NEE*2-buffer(w*2-3))-NNE);
          MapCtxs[j++] = Clip((-buffer(4)+5*WWW-10*WW+10*W+Clip(NE*2-NNE))/5);
          MapCtxs[j++] = Clip((-buffer(5)+4*buffer(4)-5*WWW+5*W+Clip(NE*2-NNE))/4);
          MapCtxs[j++] = Clip((WWW-4*WW+6*W+Clip(NE*3-NNE*3+buffer(w*3-1)))/4);
          MapCtxs[j++] = Clip((-NNEE+3*NE+Clip(W*4-NW*6+NNW*4-buffer(w*3+1)))/3);
          MapCtxs[j++] = ((W+N)*3-NW*2)/4;
          for (j=0; j<nOLS; j++) {
            ols[j].Update(W);
            pOLS[j] = Clip(int(floor(ols[j].Predict(ols_ctxs[j]))));
          }

          cm.set(0);
          cm.set(hash(++i, N, px));
          cm.set(hash(++i, N-px));
          cm.set(hash(++i, W, px));
          cm.set(hash(++i, NW, px));
          cm.set(hash(++i, NE, px));
          cm.set(hash(++i, N, NN, px));
          cm.set(hash(++i, W, WW, px));
          cm.set(hash(++i, NE, NNEE, px ));
          cm.set(hash(++i, NW, NNWW, px ));
          cm.set(hash(++i, W, NEE, px));
          cm.set(hash(++i, (Clamp4(W+N-NW,W,NW,N,NE)-px)/2, LogMeanDiffQt(Clip(N+NE-NNE), Clip(N+NW-NNW))));
          cm.set(hash(++i, (W-px)/4, (NE-px)/4, column[0]));
          cm.set(hash(++i, (Clip(W*2-WW)-px)/4, (Clip(N*2-NN)-px)/4));
          cm.set(hash(++i, (Clamp4(N+NE-NNE,W,N,NE,NEE)-px)/4, column[0]));
          cm.set(hash(++i, (Clamp4(N+NW-NNW,W,NW,N,NE)-px)/4, column[0]));
          cm.set(hash(++i, (W+NEE)/4, px, column[0]));
          cm.set(hash(++i, Clip(W+N-NW)-px, column[0]));
          cm.set(hash(++i, Clamp4(N*3-NN*3+NNN,W,N,NN,NE), px, LogMeanDiffQt(W,Clip(NW*2-NNW))));
          cm.set(hash(++i, Clamp4(W*3-WW*3+WWW,W,N,NE,NEE), px, LogMeanDiffQt(N,Clip(NW*2-NWW))));
          cm.set(hash(++i, (W+Clamp4(NE*3-NNE*3+NNNE,W,N,NE,NEE))/2, px, LogMeanDiffQt(N,(NW+NE)/2)));
          cm.set(hash(++i, (N+NNN)/8, Clip(N*3-NN*3+NNN)/4, px));
          cm.set(hash(++i, (W+WWW)/8, Clip(W*3-WW*3+WWW)/4, px));
          cm.set(hash(++i, Clip((-buffer(4)+5*WWW-10*WW+10*W+Clamp4(NE*4-NNE*6+buffer(w*3-1)*4-buffer(w*4-1),N,NE,buffer(w-2),buffer(w-3)))/5)-px));
          cm.set(hash(++i, Clip(N*2-NN)-px, LogMeanDiffQt(N,Clip(NN*2-NNN))));
          cm.set(hash(++i, Clip(W*2-WW)-px, LogMeanDiffQt(NE,Clip(N*2-NW))));

          if (isPNG)
            ctx = ((abs(W-N)>8)<<10)|((W>N)<<9)|((abs(N-NW)>8)<<8)|((N>NW)<<7)|((abs(N-NE)>8)<<6)|((N>NE)<<5)|((W>WW)<<4)|((N>NN)<<3)|min(5,filterOn?filter+1:0);
          else
            ctx = min(0x1F,x/max(1,w/min(32,columns[0])))|( ( ((abs(W-N)*16>W+N)<<1)|(abs(N-NW)>8) )<<5 )|((W+N)&0x180);

          res = Clamp4(W+N-NW,W,NW,N,NE)-px;
        }

        stats->Image.pixels.W = W;
        stats->Image.pixels.N = N;
        stats->Image.pixels.NN = NN;
        stats->Image.pixels.WW = WW;
        stats->Image.ctx = ctx>>gray;
      }
    }
    INJECT_SHARED_c0
    U8 B=(c0<<(8-bpos));
    if (x || !isPNG){
      if (gray) {
        int i=0;
        Map[i++].set_direct(0);
        Map[i++].set_direct((((U8)(Clip(W+N-NW)-px-B))*8+bpos)|(LogMeanDiffQt(Clip(N+NE-NNE), Clip(N+NW-NNW))<<11));

        for (int j=0; j<nSM1; i++, j++)
          Map[i].set_direct((MapCtxs[j]-px-B)*8+bpos);

        for (int j=0; i<nSM; i++, j++)
          Map[i].set_direct((pOLS[j]-px-B)*8+bpos);
      }
      sceneMap[2].set_direct(finalize64(hash(x, line), 19)*8+bpos);
      sceneMap[3].set_direct((prvFrmPx-B)*8+bpos);
      sceneMap[4].set_direct((prvFrmPred-B)*8+bpos);
    }

    // Predict next bit
    if (x || !isPNG){
      cm.mix(m);
      if (gray){
        for (int i=0;i<nSM;i++) {
          Map[i].mix(m);
        }
      }
      else {
        for (int i=0; i<nPltMaps; i++) {
          pltMap[i].set((bpos<<8)|iCtx[i]());
          pltMap[i].mix(m);
        }
      }
      for (int i=0; i<5; i++) {
        const int scale=(prevFramePos>0 && prevFrameWidth==w) ? 64:0;
        sceneMap[i].setscale(scale);
        sceneMap[i].mix(m);
      }

      col=(col+1)&7;
      m.set(5+ctx, 2048+5);
      m.set(col*2+(isPNG && c0==((0x100|res)>>(8-bpos))) + min(5, filterOn?filter+1:0)*16, 6*16);
      m.set(((isPNG?px:N+W)>>4) + min(5, filterOn?filter+1:0)*32, 6*32);
      m.set(c0, 256);
      m.set( ((abs((int)(W-N))>4)<<9)|((abs((int)(N-NE))>4)<<8)|((abs((int)(W-NW))>4)<<7)|((W>N)<<6)|((N>NE)<<5)|((W>NW)<<4)|((W>WW)<<3)|((N>NN)<<2)|((NW>NNWW)<<1)|(NE>NNEE), 1024 );
      m.set(min(63,column[0]), 64);
      m.set(min(127,column[1]), 128);
      m.set( min(255,(x+line)/32), 256);
    }
    else{
      m.add( -2048+((filter>>(7-bpos))&1)*4096 );
      m.set(min(4,filter),MIXERINPUTS);
    }
  }
};

//////////////////////////// Image4bitModel /////////////////////////////////

//  Model for 4-bit image data


class Image4bitModel {
private:
  static constexpr int S = 14; //number of contexts
public:
  static constexpr int MIXERINPUTS = (S*3+1);
  static constexpr int MIXERCONTEXTS = 256+512+512+1024+16+1; //2321
  static constexpr int MIXERCONTEXTSETS = 6;
private:
  const Shared * const shared;
  Random rnd;
  HashTable<16> t;
  StateMap sm;
  StateMap map;
  U8* cp[S]{}; // context pointers
  U8 WW=0, W=0, NWW=0, NW=0, N=0, NE=0, NEE=0, NNWW = 0, NNW=0, NN=0, NNE=0, NNEE=0;
  int w=0, col=0, line=0, run=0, prevColor=0, px=0;
public:
  Image4bitModel(const Shared * const sh, const U64 size) : shared(sh), t(size),
    sm{sh,S,256,1023,true}, //StateMap: s, n, lim, init
    map{sh,1,16,1023,false} //StateMap: s, n, lim, init
  {}
  void setparam(int info0) {
    w=info0;
  }
  void mix(Mixer& m) {
    if (!cp[0]){
      for (int i=0;i<S;i++)
        cp[i]=t[263*i]; //set the initial context to an arbitrary slot in the hashtable
    }
    INJECT_SHARED_y
    for (int i=0;i<S;i++)
      StateTable::update(cp[i],y,rnd); //update hashtable item priorities using predicted counts

    INJECT_SHARED_bpos 
    if (bpos==0 || bpos==4){
      WW=W; NWW=NW; NW=N; N=NE; NE=NEE; NNWW=NWW; NNW=NN; NN=NNE; NNE=NNEE;
      INJECT_SHARED_buf 
      if (bpos==0)
        {INJECT_SHARED_c4 W=c4&0xF; NEE=buf(w-1)>>4; NNEE=buf(w*2-1)>>4;}
      else
        {INJECT_SHARED_c0 W=c0&0xF; NEE=buf(w-1)&0xF; NNEE=buf(w*2-1)&0xF;}
      run=(W!=WW || col==0)?(prevColor=WW,0):min(0xFFF,run+1);
      px=1; //partial pixel (4 bits) with a leading "1"
      U64 i=0; cp[i]=t[hash(i,W,NW,N)];
          i++; cp[i]=t[hash(i,N, min(0xFFF, col/8))];
          i++; cp[i]=t[hash(i,W,NW,N,NN,NE)];
          i++; cp[i]=t[hash(i,W, N, NE+NNE*16, NEE+NNEE*16)];
          i++; cp[i]=t[hash(i,W, N, NW+NNW*16, NWW+NNWW*16)];
          i++; cp[i]=t[hash(i,W, ilog2(run+1), prevColor, col/max(1,w/2) )];
          i++; cp[i]=t[hash(i,NE, min(0x3FF, (col+line)/max(1,w*8)))];
          i++; cp[i]=t[hash(i,NW, (col-line)/max(1,w*8))];
          i++; cp[i]=t[hash(i,WW*16+W,NN*16+N,NNWW*16+NW)];
          i++; cp[i]=t[hash(i,N,NN)];
          i++; cp[i]=t[hash(i,W,WW)];
          i++; cp[i]=t[hash(i,W,NE)];
          i++; cp[i]=t[hash(i,WW,NN,NEE)];
          i++; cp[i]=t[-1];
      assert(++i==S);
      col++;
      if(col==w*2){col=0;line++;}
    }
    else{
      INJECT_SHARED_y
      px+=px+y;
      int j=(y+1)<<(bpos&3);
      for (int i=0;i<S;i++)
        cp[i]+=j;
    }

    // predict
    for (int i=0; i<S; i++) {
      const U8 s = *cp[i];
      const int n0=-!StateTable::next(s, 2);
      const int n1=-!StateTable::next(s, 3);
      const int p1 = sm.p(i,s);
      const int st = stretch(p1)>>1;
      m.add(st);
      m.add((p1-2047)>>2);
      m.add(st*abs(n1-n0));
    }
    m.add(stretch(map.p(0,px))>>1);

    m.set((W<<4) | px, 256);
    m.set(min(31,col/max(1,w/16)) | (N<<5), 512);
    m.set((bpos&3) | (W<<2) | (min(7,ilog2(run+1))<<6), 512);
    m.set(W | (NE<<4) | ((bpos&3)<<8), 1024);
    m.set(px, 16);
    m.set(0,1);
  }
};

//////////////////////////// Image1bitModel /////////////////////////////////

// Model for 1-bit image data

class Image1bitModel {
private:
  static constexpr int S = 11;
public:
  static constexpr int MIXERINPUTS = S;
private:
  const Shared * const shared;
  Random rnd;
  int w=0;
  U32 r0=0, r1=0, r2=0, r3=0;  // last 4 rows, bit 8 is over current pixel
  Array<U8> t{0x23000};  // model: cxt -> state
  int cxt[S]{};  // contexts
  StateMap sm;
public:
  Image1bitModel(const Shared * const sh) : shared(sh),
    sm{sh,S,256,1023,true} // StateMap: s, n, limit, init
  {}
  void setparam(int info0) {
    w=info0;
  }
  void mix(Mixer& m) {
    // update the model
    INJECT_SHARED_y
    for (int i=0; i<S; ++i)
      StateTable::update(&t[cxt[i]],y,rnd);

    INJECT_SHARED_bpos INJECT_SHARED_buf
    // update the contexts (pixels surrounding the predicted one)
    r0+=r0+y;
    r1+=r1+((buf(w-1)>>(7-bpos))&1);
    r2+=r2+((buf(w+w-1)>>(7-bpos))&1);
    r3+=r3+((buf(w+w+w-1)>>(7-bpos))&1);
    cxt[0]=(r0&0x7)|(r1>>4&0x38)|(r2>>3&0xc0);
    cxt[1]=0x100+((r0&1)|(r1>>4&0x3e)|(r2>>2&0x40)|(r3>>1&0x80));
    cxt[2]=0x200+((r0&1)|(r1>>4&0x1d)|(r2>>1&0x60)|(r3&0xC0));
    cxt[3]=0x300+(y|((r0<<1)&4)|((r1>>1)&0xF0)|((r2>>3)&0xA));
    cxt[4]=0x400+((r0>>4&0x2AC)|(r1&0xA4)|(r2&0x349)|(!(r3&0x14D)));
    cxt[5]=0x800+(y|((r1>>4)&0xE)|((r2>>1)&0x70)|((r3<<2)&0x380));
    cxt[6]=0xC00+(((r1&0x30)^(r3&0x0c0c))|(r0&3));
    cxt[7]=0x1000+((!(r0&0x444))|(r1&0xC0C)|(r2&0xAE3)|(r3&0x51C));
    cxt[8]=0x2000+((r0&7)|((r1>>1)&0x3F8)|((r2<<5)&0xC00));
    cxt[9]=0x3000+((r0&0x3f)^(r1&0x3ffe)^(r2<<2&0x7f00)^(r3<<5&0xf800));
    cxt[10]=0x13000+((r0&0x3e)^(r1&0x0c0c)^(r2&0xc800));

    // predict
    for (int i=0; i<S; ++i) {
      const U8 s=t[cxt[i]];
      m.add(stretch(sm.p(i,s)));
    }
  }
};

//////////////////////////// jpegModel /////////////////////////

// Model JPEG. Return 1 if a JPEG file is detected or else 0.
// Only the baseline and 8 bit extended Huffman coded DCT modes are
// supported.  The model partially decodes the JPEG image to provide
// context for the Huffman coded symbols.

// Print a JPEG segment at buf[p...] for debugging
/*
void dump(const char* msg, int p) {
  printf("%s:", msg);
  int len=buf[p+2]*256+buf[p+3];
  for (int i=0; i<len+2; ++i)
    printf(" %02X", buf[p+i]);
  printf("\n");
}
*/

#define finish(success){ \
  U32 length = pos - images[idx].offset; \
  /*if (success && idx && pos-lastPos==1)*/ \
    /*printf("\b\b\b\b\b\b\b\b\b\b\b\b\b\b\bEmbedded JPEG at offset %d, size: %d bytes, level %d\nCompressing... ", images[idx].offset-pos+blpos, length, idx), fflush(stdout);*/ \
  memset(&images[idx], 0, sizeof(JPEGImage)); \
  mcusize=0,dqt_state=-1; \
  idx-=(idx>0); \
  if (images[idx].app <= length) \
    images[idx].app = 0; \
  else images[idx].app-=length; \
}

// Detect invalid JPEG data.  The proper response is to silently
// fall back to a non-JPEG model.
#define jassert(x) if (!(x)) { \
/*  printf("JPEG error at %d, line %d: %s\n", pos, __LINE__, #x); */ \
  if (idx>0) \
    finish(false) \
  else \
    images[idx].jpeg=0; \
  return images[idx].next_jpeg;}

struct HUF {U32 min, max; int val;}; // Huffman decode tables
  // huf[Tc][Th][m] is the minimum, maximum+1, and pointer to codes for
  // coefficient type Tc (0=DC, 1=AC), table Th (0-3), length m+1 (m=0-15)

struct JPEGImage{
  U32 offset, // offset of SOI marker
  jpeg, // 1 if JPEG is header detected, 2 if image data
  next_jpeg, // updated with jpeg on next byte boundary
  app, // Bytes remaining to skip in this marker
  sof, sos, data, // pointers to buf
  htsize; // number of pointers in ht
  int ht[8]; // pointers to Huffman table headers
  U8 qtab[256]; // table
  int qmap[10]; // block -> table number
};

class JpegModel {
private:
  static constexpr int N = 32; // number of contexts
public:
  static constexpr int MIXERINPUTS = 2*N+6; 
  static constexpr int MIXERCONTEXTS = (1+8)+(1+1024)+1024;
  static constexpr int MIXERCONTEXTSETS = 6;
private:
    const Shared * const shared;
    Random rnd;
    // State of parser
    enum {SOF0=0xc0, SOF1, SOF2, SOF3, DHT, RST0=0xd0, SOI=0xd8, EOI, SOS, DQT,
      DNL, DRI, APP0=0xe0, COM=0xfe, FF};  // Second byte of 2 byte codes
    static const int MaxEmbeddedLevel = 3;
    JPEGImage images[MaxEmbeddedLevel]{};
    int idx=-1;
    U32 lastPos=0;

    // Huffman decode state
    U32 huffcode=0;  // Current Huffman code including extra bits
    int huffbits=0;  // Number of valid bits in huffcode
    int huffsize=0;  // Number of bits without extra bits
    int rs=-1;  // Decoded huffcode without extra bits.  It represents
      // 2 packed 4-bit numbers, r=run of zeros, s=number of extra bits for
      // first nonzero code.  huffcode is complete when rs >= 0.
      // rs is -1 prior to decoding incomplete huffcode.
    int hbcount=2;

    int mcupos=0;  // position in MCU (0-639).  The low 6 bits mark
      // the coefficient in zigzag scan order (0=DC, 1-63=AC).  The high
      // bits mark the block within the MCU, used to select Huffman tables.

    // Decoding tables
    Array<HUF> huf{128};  // Tc*64+Th*16+m -> min, max, val
    int mcusize=0;  // number of coefficients in an MCU
    int hufsel[2][10]{{0}};  // DC/AC, mcupos/64 -> huf decode table
    Array<U8> hbuf{2048};  // Tc*1024+Th*256+hufcode -> RS

    // Image state
    Array<int> color{10};  // block -> component (0-3)
    Array<int> pred{4};  // component -> last DC value
    int dc=0;  // DC value of the current block
    int width=0;  // Image width in MCU
    int row=0, column=0;  // in MCU (column 0 to width-1)
    Buf cbuf{0x20000}; // Rotating buffer of coefficients, coded as:
      // DC: level shifted absolute value, low 4 bits discarded, i.e.
      //   [-1023...1024] -> [0...255].
      // AC: as an RS code: a run of R (0-15) zeros followed by an S (0-15)
      //   bit number, or 00 for end of block (in zigzag order).
      //   However if R=0, then the format is ssss11xx where ssss is S,
      //   xx is the first 2 extra bits, and the last 2 bits are 1 (since
      //   this never occurs in a valid RS code).
    int cpos=0;  // position in cbuf
    int rs1=0;  // last RS code
    int rstpos=0,rstlen=0; // reset position
    int ssum=0, ssum1=0, ssum2=0, ssum3=0;
      // sum of S in RS codes in block and sum of S in first component

    RingBuffer<int> cbuf2{0x20000};
    Array<int> adv_pred{4}, sumu{8}, sumv{8}, run_pred{6};
    int prev_coef=0, prev_coef2=0, prev_coef_rs=0;
    Array<int> ls{10};  // block -> distance to previous block
    Array<int> blockW{10}, blockN{10}, SamplingFactors{4};
    Array<int> lcp{7}, zpos{64};

    //for parsing Quantization tables
    int dqt_state = -1;
    U32 dqt_end = 0, qnum = 0;

    // Context model
    BH<9> t; // context hash -> bit history
      // As a cache optimization, the context does not include the last 1-2
      // bits of huffcode if the length (huffbits) is not a multiple of 3.
      // The 7 mapped values are for context+{"", 0, 00, 01, 1, 10, 11}.
    Array<U64> cxt{N};  // context hashes
    Array<U8*> cp{N};  // context pointers
    IndirectMap MJPEGMap; 
    StateMap sm;
    Mixer *m1;
    APM apm1, apm2;

public:
  JpegModel(const Shared * const sh, const U64 size): shared(sh), t(size), 
    MJPEGMap(sh,21,3,128,127), /* BitsOfContext, InputBits, Scale, Limit */
    sm(sh,N,256,1023,true), apm1 (sh,0x8000,24), apm2(sh,0x20000,24)
  {
    m1=MixerFactory::CreateMixer(sh, N+1, 2050, 3);
    m1->set_scalefactor(1024, 128);
  }
  ~JpegModel() {
    delete m1;
  }

  int mix(Mixer& m) {

    static constexpr U8 zzu[64]={  // zigzag coef -> u,v
      0,1,0,0,1,2,3,2,1,0,0,1,2,3,4,5,4,3,2,1,0,0,1,2,3,4,5,6,7,6,5,4,
      3,2,1,0,1,2,3,4,5,6,7,7,6,5,4,3,2,3,4,5,6,7,7,6,5,4,5,6,7,7,6,7};
    static constexpr U8 zzv[64]={
      0,0,1,2,1,0,0,1,2,3,4,3,2,1,0,0,1,2,3,4,5,6,5,4,3,2,1,0,0,1,2,3,
      4,5,6,7,7,6,5,4,3,2,1,2,3,4,5,6,7,7,6,5,4,3,4,5,6,7,7,6,5,6,7,7};

    // Standard Huffman tables (cf. JPEG standard section K.3)
    // IMPORTANT: these are only valid for 8-bit data precision
    static constexpr U8 bits_dc_luminance[16] = {
      0, 1, 5, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0
    };
    static constexpr U8 values_dc_luminance[12] = {
      0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11
    };

    static constexpr U8 bits_dc_chrominance[16] = {
      0, 3, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0
    };
    static constexpr U8 values_dc_chrominance[12] = {
      0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11
    };

    static constexpr U8 bits_ac_luminance[16] = {
      0, 2, 1, 3, 3, 2, 4, 3, 5, 5, 4, 4, 0, 0, 1, 0x7d
    };
    static constexpr U8 values_ac_luminance[162] = {
      0x01, 0x02, 0x03, 0x00, 0x04, 0x11, 0x05, 0x12,
      0x21, 0x31, 0x41, 0x06, 0x13, 0x51, 0x61, 0x07,
      0x22, 0x71, 0x14, 0x32, 0x81, 0x91, 0xa1, 0x08,
      0x23, 0x42, 0xb1, 0xc1, 0x15, 0x52, 0xd1, 0xf0,
      0x24, 0x33, 0x62, 0x72, 0x82, 0x09, 0x0a, 0x16,
      0x17, 0x18, 0x19, 0x1a, 0x25, 0x26, 0x27, 0x28,
      0x29, 0x2a, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39,
      0x3a, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48, 0x49,
      0x4a, 0x53, 0x54, 0x55, 0x56, 0x57, 0x58, 0x59,
      0x5a, 0x63, 0x64, 0x65, 0x66, 0x67, 0x68, 0x69,
      0x6a, 0x73, 0x74, 0x75, 0x76, 0x77, 0x78, 0x79,
      0x7a, 0x83, 0x84, 0x85, 0x86, 0x87, 0x88, 0x89,
      0x8a, 0x92, 0x93, 0x94, 0x95, 0x96, 0x97, 0x98,
      0x99, 0x9a, 0xa2, 0xa3, 0xa4, 0xa5, 0xa6, 0xa7,
      0xa8, 0xa9, 0xaa, 0xb2, 0xb3, 0xb4, 0xb5, 0xb6,
      0xb7, 0xb8, 0xb9, 0xba, 0xc2, 0xc3, 0xc4, 0xc5,
      0xc6, 0xc7, 0xc8, 0xc9, 0xca, 0xd2, 0xd3, 0xd4,
      0xd5, 0xd6, 0xd7, 0xd8, 0xd9, 0xda, 0xe1, 0xe2,
      0xe3, 0xe4, 0xe5, 0xe6, 0xe7, 0xe8, 0xe9, 0xea,
      0xf1, 0xf2, 0xf3, 0xf4, 0xf5, 0xf6, 0xf7, 0xf8,
      0xf9, 0xfa
    };

    static constexpr U8 bits_ac_chrominance[16] = {
      0, 2, 1, 2, 4, 4, 3, 4, 7, 5, 4, 4, 0, 1, 2, 0x77
    };
    static constexpr U8 values_ac_chrominance[162] = {
      0x00, 0x01, 0x02, 0x03, 0x11, 0x04, 0x05, 0x21,
      0x31, 0x06, 0x12, 0x41, 0x51, 0x07, 0x61, 0x71,
      0x13, 0x22, 0x32, 0x81, 0x08, 0x14, 0x42, 0x91,
      0xa1, 0xb1, 0xc1, 0x09, 0x23, 0x33, 0x52, 0xf0,
      0x15, 0x62, 0x72, 0xd1, 0x0a, 0x16, 0x24, 0x34,
      0xe1, 0x25, 0xf1, 0x17, 0x18, 0x19, 0x1a, 0x26,
      0x27, 0x28, 0x29, 0x2a, 0x35, 0x36, 0x37, 0x38,
      0x39, 0x3a, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48,
      0x49, 0x4a, 0x53, 0x54, 0x55, 0x56, 0x57, 0x58,
      0x59, 0x5a, 0x63, 0x64, 0x65, 0x66, 0x67, 0x68,
      0x69, 0x6a, 0x73, 0x74, 0x75, 0x76, 0x77, 0x78,
      0x79, 0x7a, 0x82, 0x83, 0x84, 0x85, 0x86, 0x87,
      0x88, 0x89, 0x8a, 0x92, 0x93, 0x94, 0x95, 0x96,
      0x97, 0x98, 0x99, 0x9a, 0xa2, 0xa3, 0xa4, 0xa5,
      0xa6, 0xa7, 0xa8, 0xa9, 0xaa, 0xb2, 0xb3, 0xb4,
      0xb5, 0xb6, 0xb7, 0xb8, 0xb9, 0xba, 0xc2, 0xc3,
      0xc4, 0xc5, 0xc6, 0xc7, 0xc8, 0xc9, 0xca, 0xd2,
      0xd3, 0xd4, 0xd5, 0xd6, 0xd7, 0xd8, 0xd9, 0xda,
      0xe2, 0xe3, 0xe4, 0xe5, 0xe6, 0xe7, 0xe8, 0xe9,
      0xea, 0xf2, 0xf3, 0xf4, 0xf5, 0xf6, 0xf7, 0xf8,
      0xf9, 0xfa
    };
    
    INJECT_SHARED_pos
    if (idx < 0){
      memset(&images[0], 0, sizeof(images));
      idx = 0;
      lastPos = pos;
    }

    INJECT_SHARED_bpos INJECT_SHARED_buf

    // Be sure to quit on a byte boundary
    if (bpos==0) images[idx].next_jpeg=images[idx].jpeg>1;
    if (bpos!=0 && !images[idx].jpeg) return images[idx].next_jpeg;
    if (bpos==0 && images[idx].app>0){
      --images[idx].app;
      if (idx<MaxEmbeddedLevel && buf(4)==FF && buf(3)==SOI && buf(2)==FF && ((buf(1)&0xFE)==0xC0 || buf(1)==0xC4 || (buf(1)>=0xDB && buf(1)<=0xFE)) )
        memset(&images[++idx], 0, sizeof(JPEGImage));
    }
    if (images[idx].app>0) return images[idx].next_jpeg;
    if (bpos==0) {

      // Parse.  Baseline DCT-Huffman JPEG syntax is:
      // SOI APPx... misc... SOF0 DHT... SOS data EOI
      // SOI (= FF D8) start of image.
      // APPx (= FF Ex) len ... where len is always a 2 byte big-endian length
      //   including the length itself but not the 2 byte preceding code.
      //   Application data is ignored.  There may be more than one APPx.
      // misc codes are DQT, DNL, DRI, COM (ignored).
      // SOF0 (= FF C0) len 08 height width Nf [C HV Tq]...
      //   where len, height, width (in pixels) are 2 bytes, Nf is the repeat
      //   count (1 byte) of [C HV Tq], where C is a component identifier
      //   (color, 0-3), HV is the horizontal and vertical dimensions
      //   of the MCU (high, low bits, packed), and Tq is the quantization
      //   table ID (not used).  An MCU (minimum compression unit) consists
      //   of 64*H*V DCT coefficients for each color.
      // DHT (= FF C4) len [TcTh L1...L16 V1,1..V1,L1 ... V16,1..V16,L16]...
      //   defines Huffman table Th (1-4) for Tc (0=DC (first coefficient)
      //   1=AC (next 63 coefficients)).  L1..L16 are the number of codes
      //   of length 1-16 (in ascending order) and Vx,y are the 8-bit values.
      //   A V code of RS means a run of R (0-15) zeros followed by S (0-15)
      //   additional bits to specify the next nonzero value, negative if
      //   the first additional bit is 0 (e.g. code x63 followed by the
      //   3 bits 1,0,1 specify 7 coefficients: 0, 0, 0, 0, 0, 0, 5.
      //   Code 00 means end of block (remainder of 63 AC coefficients is 0).
      // SOS (= FF DA) len Ns [Cs TdTa]... 0 3F 00
      //   Start of scan.  TdTa specifies DC/AC Huffman tables (0-3, packed
      //   into one byte) for component Cs matching C in SOF0, repeated
      //   Ns (1-4) times.
      // EOI (= FF D9) is end of image.
      // Huffman coded data is between SOI and EOI.  Codes may be embedded:
      // RST0-RST7 (= FF D0 to FF D7) mark the start of an independently
      //   compressed region.
      // DNL (= FF DC) 04 00 height
      //   might appear at the end of the scan (ignored).
      // FF 00 is interpreted as FF (to distinguish from RSTx, DNL, EOI).

      // Detect JPEG (SOI followed by a valid marker)
      if (!images[idx].jpeg && buf(4)==FF && buf(3)==SOI && buf(2)==FF && ((buf(1)&0xFE)==0xC0 || buf(1)==0xC4 || (buf(1)>=0xDB && buf(1)<=0xFE)) ){
        images[idx].jpeg=1;
        images[idx].offset = pos-4;
        images[idx].sos=images[idx].sof=images[idx].htsize=images[idx].data=0, images[idx].app=(buf(1)>>4==0xE)*2;
        mcusize=huffcode=huffbits=huffsize=mcupos=cpos=0, rs=-1;
        memset(&huf[0], 0, sizeof(huf));
        memset(&pred[0], 0, pred.size()*sizeof(int));
        rstpos=rstlen=0;
      }

      // Detect end of JPEG when data contains a marker other than RSTx
      // or byte stuff (00), or if we jumped in position since the last byte seen
      if (images[idx].jpeg && images[idx].data && ((buf(2)==FF && buf(1) && (buf(1)&0xf8)!=RST0) || (pos-lastPos>1)) ) {
        jassert((buf(1)==EOI) || (pos-lastPos>1));
        finish(true);
      }
      lastPos = pos;
      if (!images[idx].jpeg) return images[idx].next_jpeg;

      // Detect APPx, COM or other markers, so we can skip them
      if (!images[idx].data && !images[idx].app && buf(4)==FF && (((buf(3)>0xC1) && (buf(3)<=0xCF) && (buf(3)!=DHT)) || ((buf(3)>=0xDC) && (buf(3)<=0xFE)))){
        images[idx].app=buf(2)*256+buf(1)+2;
        if (idx>0)
          jassert( pos + images[idx].app < images[idx].offset + images[idx-1].app );
      }

      // Save pointers to sof, ht, sos, data,
      if (buf(5)==FF && buf(4)==SOS) {
        int len=buf(3)*256+buf(2);
        if (len==6+2*buf(1) && buf(1) && buf(1)<=4)  // buf(1) is Ns
          images[idx].sos=pos-5, images[idx].data=images[idx].sos+len+2, images[idx].jpeg=2;
      }
      if (buf(4)==FF && buf(3)==DHT && images[idx].htsize<8) images[idx].ht[images[idx].htsize++]=pos-4;
      if (buf(4)==FF && (buf(3)&0xFE)==SOF0) images[idx].sof=pos-4;

      // Parse Quantizazion tables
      if (buf(4)==FF && buf(3)==DQT)
        dqt_end=pos+buf(2)*256+buf(1)-1, dqt_state=0;
      else if (dqt_state>=0) {
        if (pos>=dqt_end)
          dqt_state = -1;
        else {
          if (dqt_state%65==0)
            qnum = buf(1);
          else {
            jassert(buf(1)>0);
            jassert(qnum>=0 && qnum<4);
            images[idx].qtab[qnum*64+((dqt_state%65)-1)]=buf(1)-1;
          }
          dqt_state++;
        }
      }

      // Restart
      if (buf(2)==FF && (buf(1)&0xf8)==RST0) {
        huffcode=huffbits=huffsize=mcupos=0, rs=-1;
        memset(&pred[0], 0, pred.size()*sizeof(int));
        rstlen=column+row*width-rstpos;
        rstpos=column+row*width;
      }
    }

    {
      // Build Huffman tables
      // huf[Tc][Th][m] = min, max+1 codes of length m, pointer to byte values
      if (pos==images[idx].data && bpos==1) {
        for (U32 i=0; i<images[idx].htsize; ++i) {
          U32 p=images[idx].ht[i]+4;  // pointer to current table after length field
          U32 end=p+buf[p-2]*256+buf[p-1]-2;  // end of Huffman table
          U32 count=0;  // sanity check
          while (p<end && end<pos && end<p+2100 && ++count<10) {
            int tc=buf[p]>>4, th=buf[p]&15;
            if (tc>=2 || th>=4) break;
            jassert(tc>=0 && tc<2 && th>=0 && th<4);
            HUF* h=&huf[tc*64+th*16]; // [tc][th][0];
            int val=p+17;  // pointer to values
            int hval=tc*1024+th*256;  // pointer to RS values in hbuf
            int j;
            for (j=0; j<256; ++j) // copy RS codes
              hbuf[hval+j]=buf[val+j];
            int code=0;
            for (j=0; j<16; ++j) {
              h[j].min=code;
              h[j].max=code+=buf[p+j+1];
              h[j].val=hval;
              val+=buf[p+j+1];
              hval+=buf[p+j+1];
              code*=2;
            }
            p=val;
            jassert(hval>=0 && hval<2048);
          }
          jassert(p==end);
        }
        huffcode=huffbits=huffsize=0, rs=-1;

        // load default tables
        if (!images[idx].htsize){
          for (int tc = 0; tc < 2; tc++) {
            for (int th = 0; th < 2; th++) {
              HUF* h = &huf[tc*64+th*16];
              int hval = tc*1024 + th*256;
              int code = 0, c = 0, x = 0;

              for (int i = 0; i < 16; i++) {
                switch (tc*2+th) {
                  case 0: x = bits_dc_luminance[i]; break;
                  case 1: x = bits_dc_chrominance[i]; break;
                  case 2: x = bits_ac_luminance[i]; break;
                  case 3: x = bits_ac_chrominance[i];
                }

                h[i].min = code;
                h[i].max = (code+=x);
                h[i].val = hval;
                hval+=x;
                code+=code;
                c+=x;
              }

              hval = tc*1024 + th*256;
              c--;

              while (c >= 0){
                switch (tc*2+th) {
                  case 0: x = values_dc_luminance[c]; break;
                  case 1: x = values_dc_chrominance[c]; break;
                  case 2: x = values_ac_luminance[c]; break;
                  case 3: x = values_ac_chrominance[c];
                }

                hbuf[hval+c] = x;
                c--;
              }
            }
          }
          images[idx].htsize = 4;
        }

        // Build Huffman table selection table (indexed by mcupos).
        // Get image width.
        if (!images[idx].sof && images[idx].sos) return images[idx].next_jpeg;
        int ns=buf[images[idx].sos+4];
        int nf=buf[images[idx].sof+9];
        jassert(ns<=4 && nf<=4);
        mcusize=0;  // blocks per MCU
        int hmax=0;  // MCU horizontal dimension
        for (int i=0; i<ns; ++i) {
          for (int j=0; j<nf; ++j) {
            if (buf[images[idx].sos+2*i+5]==buf[images[idx].sof+3*j+10]) { // Cs == C ?
              int hv=buf[images[idx].sof+3*j+11];  // packed dimensions H x V
              SamplingFactors[j] = hv;
              if (hv>>4>hmax) hmax=hv>>4;
              hv=(hv&15)*(hv>>4);  // number of blocks in component C
              jassert(hv>=1 && hv+mcusize<=10);
              while (hv) {
                jassert(mcusize<10);
                hufsel[0][mcusize]=buf[images[idx].sos+2*i+6]>>4&15;
                hufsel[1][mcusize]=buf[images[idx].sos+2*i+6]&15;
                jassert (hufsel[0][mcusize]<4 && hufsel[1][mcusize]<4);
                color[mcusize]=i;
                int tq=buf[images[idx].sof+3*j+12];  // quantization table index (0..3)
                jassert(tq>=0 && tq<4);
                images[idx].qmap[mcusize]=tq; // quantizazion table mapping
                --hv;
                ++mcusize;
              }
            }
          }
        }
        jassert(hmax>=1 && hmax<=10);
        int j;
        for (j=0; j<mcusize; ++j) {
          ls[j]=0;
          for (int i=1; i<mcusize; ++i) if (color[(j+i)%mcusize]==color[j]) ls[j]=i;
          ls[j]=(mcusize-ls[j])<<6;
        }
        for (j=0; j<64; ++j) zpos[zzu[j]+8*zzv[j]]=j;
        width=buf[images[idx].sof+7]*256+buf[images[idx].sof+8];  // in pixels
        width=(width-1)/(hmax*8)+1;  // in MCU
        jassert(width>0);
        mcusize*=64;  // coefficients per MCU
        row=column=0;

        // we can have more blocks than components then we have subsampling
        int x=0, y=0;
        for (j = 0; j<(mcusize>>6); j++) {
          int i = color[j];
          int w = SamplingFactors[i]>>4, h = SamplingFactors[i]&0xf;
          blockW[j] = x==0?mcusize-64*(w-1):64;
          blockN[j] = y==0?mcusize*width-64*w*(h-1):w*64;
          x++;
          if (x>=w) { x=0; y++; }
          if (y>=h) { x=0; y=0; }
        }
      }
    }


    // Decode Huffman
    {
      if (mcusize && buf(1+(bpos==0))!=FF) {  // skip stuffed byte
        jassert(huffbits<=32);
        INJECT_SHARED_y
        huffcode+=huffcode+y;
        ++huffbits;
        if (rs<0) {
          jassert(huffbits>=1 && huffbits<=16);
          const int ac=(mcupos&63)>0;
          jassert(mcupos>=0 && (mcupos>>6)<10);
          jassert(ac==0 || ac==1);
          const int sel=hufsel[ac][mcupos>>6];
          jassert(sel>=0 && sel<4);
          const int i=huffbits-1;
          jassert(i>=0 && i<16);
          const HUF *h=&huf[ac*64+sel*16]; // [ac][sel];
          jassert(h[i].min<=h[i].max && h[i].val<2048 && huffbits>0);
          if (huffcode<h[i].max) {
            jassert(huffcode>=h[i].min);
            int k=h[i].val+huffcode-h[i].min;
            jassert(k>=0 && k<2048);
            rs=hbuf[k];
            huffsize=huffbits;
          }
        }
        if (rs>=0) {
          if (huffsize+(rs&15)==huffbits) { // done decoding
            rs1=rs;
            int ex=0;  // decoded extra bits
            if (mcupos&63) {  // AC
              if (rs==0) { // EOB
                mcupos=(mcupos+63)&-64;
                jassert(mcupos>=0 && mcupos<=mcusize && mcupos<=640);
                while (cpos&63) {
                  cbuf2.set(cpos,0);
                  cbuf.set(cpos,(!rs)?0:(63-(cpos&63))<<4);
                  cpos++; rs++;
                }
              }
              else {  // rs = r zeros + s extra bits for the next nonzero value
                      // If first extra bit is 0 then value is negative.
                jassert((rs&15)<=10);
                const int r=rs>>4;
                const int s=rs&15;
                jassert(mcupos>>6==(mcupos+r)>>6);
                mcupos+=r+1;
                ex=huffcode&((1<<s)-1);
                if (s!=0 && (ex>>(s-1))==0) ex-=(1<<s)-1;
                for (int i=r; i>=1; --i) {
                  cbuf2.set(cpos,0);
                  cbuf.set(cpos,(i<<4)|s);
                  cpos++;
                }
                cbuf2.set(cpos,ex);
                cbuf.set(cpos,(s<<4)|(huffcode<<2>>s&3)|12);
                cpos++; ssum+=s;
              }
            }
            else {  // DC: rs = 0S, s<12
              jassert(rs<12);
              ++mcupos;
              ex=huffcode&((1<<rs)-1);
              if (rs!=0 && (ex>>(rs-1))==0) ex-=(1<<rs)-1;
              jassert(mcupos>=0 && mcupos>>6<10);
              const int comp=color[mcupos>>6];
              jassert(comp>=0 && comp<4);
              dc=pred[comp]+=ex;
              jassert((cpos&63)==0);
              cbuf2.set(cpos,dc);
              cbuf.set(cpos,(dc+1023)>>3);
              cpos++;
              if ((mcupos>>6)==0) {
                ssum1=0;
                ssum2=ssum3;
              } else {
                if (color[(mcupos>>6)-1]==color[0]) ssum1+=(ssum3=ssum);
                ssum2=ssum1;
              }
              ssum=rs;
            }
            jassert(mcupos>=0 && mcupos<=mcusize);
            if (mcupos>=mcusize) {
              mcupos=0;
              if (++column==width) column=0, ++row;
            }
            huffcode=huffsize=huffbits=0, rs=-1;

            // UPDATE_ADV_PRED !!!!
            {
              const int acomp=mcupos>>6, q=64*images[idx].qmap[acomp];
              const int zz=mcupos&63, cpos_dc=cpos-zz;
              const bool norst=rstpos!=column+row*width;
              if (zz==0) {
                for (int i=0; i<8; ++i) sumu[i]=sumv[i]=0;
                // position in the buffer of first (DC) coefficient of the block
                // of this same component that is to the west of this one, not
                // necessarily in this MCU
                int offset_DC_W = cpos_dc - blockW[acomp];
                // position in the buffer of first (DC) coefficient of the block
                // of this same component that is to the north of this one, not
                // necessarily in this MCU
                int offset_DC_N = cpos_dc - blockN[acomp];
                for (int i=0; i<64; ++i) {
                  sumu[zzu[i]]+=(zzv[i]&1?-1:1)*(zzv[i]?16*(16+zzv[i]):185)*(images[idx].qtab[q+i]+1)*cbuf2[offset_DC_N+i];
                  sumv[zzv[i]]+=(zzu[i]&1?-1:1)*(zzu[i]?16*(16+zzu[i]):185)*(images[idx].qtab[q+i]+1)*cbuf2[offset_DC_W+i];
                }
              }
              else {
                sumu[zzu[zz-1]]-=(zzv[zz-1]?16*(16+zzv[zz-1]):185)*(images[idx].qtab[q+zz-1]+1)*cbuf2[cpos-1];
                sumv[zzv[zz-1]]-=(zzu[zz-1]?16*(16+zzu[zz-1]):185)*(images[idx].qtab[q+zz-1]+1)*cbuf2[cpos-1];
              }

              for (int i=0; i<3; ++i)
              {
                run_pred[i]=run_pred[i+3]=0;
                for (int st=0; st<10 && zz+st<64; ++st) {
                  const int zz2=zz+st;
                  int p=sumu[zzu[zz2]]*i+sumv[zzv[zz2]]*(2-i);
                  p/=(images[idx].qtab[q+zz2]+1)*185*(16+zzv[zz2])*(16+zzu[zz2])/128;
                  if (zz2==0 && (norst || ls[acomp]==64)) p-=cbuf2[cpos_dc-ls[acomp]];
                  p=(p<0?-1:+1)*ilog(abs(p)+1);
                  if (st==0) {
                    adv_pred[i]=p;
                  }
                  else if (abs(p)>abs(adv_pred[i])+2 && abs(adv_pred[i]) < 210) {
                    if (run_pred[i]==0) run_pred[i]=st*2+(p>0);
                    if (abs(p)>abs(adv_pred[i])+21 && run_pred[i+3]==0) run_pred[i+3]=st*2+(p>0);
                  }
                }
              }
              ex=0;
              for (int i=0; i<8; ++i) ex+=(zzu[zz]<i)*sumu[i]+(zzv[zz]<i)*sumv[i];
              ex=(sumu[zzu[zz]]*(2+zzu[zz])+sumv[zzv[zz]]*(2+zzv[zz])-ex*2)*4/(zzu[zz]+zzv[zz]+16);
              ex/=(images[idx].qtab[q+zz]+1)*185;
              if (zz==0 && (norst || ls[acomp]==64)) ex-=cbuf2[cpos_dc-ls[acomp]];
              adv_pred[3]=(ex<0?-1:+1)*ilog(abs(ex)+1);

              for (int i=0; i<4; ++i) {
                const int a=(i&1?zzv[zz]:zzu[zz]), b=(i&2?2:1);
                if (a<b) ex=65535;
                else {
                  const int zz2=zpos[zzu[zz]+8*zzv[zz]-(i&1?8:1)*b];
                  ex=(images[idx].qtab[q+zz2]+1)*cbuf2[cpos_dc+zz2]/(images[idx].qtab[q+zz]+1);
                  ex=(ex<0?-1:+1)*(ilog(abs(ex)+1)+(ex!=0?17:0));
                }
                lcp[i]=ex;
              }
              if ((zzu[zz]*zzv[zz])!=0){
                const int zz2=zpos[zzu[zz]+8*zzv[zz]-9];
                ex=(images[idx].qtab[q+zz2]+1)*cbuf2[cpos_dc+zz2]/(images[idx].qtab[q+zz]+1);
                lcp[4]=(ex<0?-1:+1)*(ilog(abs(ex)+1)+(ex!=0?17:0));

                ex=(images[idx].qtab[q+zpos[8*zzv[zz]]]+1)*cbuf2[cpos_dc+zpos[8*zzv[zz]]]/(images[idx].qtab[q+zz]+1);
                lcp[5]=(ex<0?-1:+1)*(ilog(abs(ex)+1)+(ex!=0?17:0));

                ex=(images[idx].qtab[q+zpos[zzu[zz]]]+1)*cbuf2[cpos_dc+zpos[zzu[zz]]]/(images[idx].qtab[q+zz]+1);
                lcp[6]=(ex<0?-1:+1)*(ilog(abs(ex)+1)+(ex!=0?17:0));
              }
              else
                lcp[4]=lcp[5]=lcp[6]=65535;

              int prev1=0,prev2=0,cnt1=0,cnt2=0,r=0,s=0;
              prev_coef_rs = cbuf[cpos-64];
              for (int i=0; i<acomp; i++) {
                ex=0;
                ex+=cbuf2[cpos-(acomp-i)*64];
                if (zz==0 && (norst || ls[i]==64)) ex-=cbuf2[cpos_dc-(acomp-i)*64-ls[i]];
                if (color[i]==color[acomp]-1) { prev1+=ex; cnt1++; r+=cbuf[cpos-(acomp-i)*64]>>4; s+=cbuf[cpos-(acomp-i)*64]&0xF; }
                if (color[acomp]>1 && color[i]==color[0]) { prev2+=ex; cnt2++; }
              }
              if (cnt1>0) prev1/=cnt1, r/=cnt1, s/=cnt1, prev_coef_rs=(r<<4)|s;
              if (cnt2>0) prev2/=cnt2;
              prev_coef=(prev1<0?-1:+1)*ilog(11*abs(prev1)+1)+(cnt1<<20);
              prev_coef2=(prev2<0?-1:+1)*ilog(11*abs(prev2)+1);

              if (column==0 && blockW[acomp]>64*acomp) run_pred[1]=run_pred[2], run_pred[0]=0, adv_pred[1]=adv_pred[2], adv_pred[0]=0;
              if (row==0 && blockN[acomp]>64*acomp) run_pred[1]=run_pred[0], run_pred[2]=0, adv_pred[1]=adv_pred[0], adv_pred[2]=0;
            } // !!!!

          }
        }
      }
    }

    // Estimate next bit probability
    if (!images[idx].jpeg || !images[idx].data) return images[idx].next_jpeg;
    if (buf(1+(bpos==0))==FF) {
      m.add(128); //network bias
      m.set(0, 1+8);
      m.set(0, 1+1024);
      m.set(buf(1), 1024);
      return true;
    }
    if (rstlen>0 && rstlen==column+row*width-rstpos && mcupos==0 && (int)huffcode==(1<<huffbits)-1) {
      m.add(2047); //network bias
      m.set(0, 1+8);
      m.set(0, 1+1024);
      m.set(buf(1), 1024);
      return true;
    }

    // Update model
    if (cp[N-1]) {
      INJECT_SHARED_y
      for (int i=0; i<N; ++i)
        StateTable::update(cp[i],y,rnd);
    }

    // Update context
    const int comp=color[mcupos>>6];
    const int coef=(mcupos&63)|comp<<6;
    const int hc=(huffcode*4+((mcupos&63)==0)*2+(comp==0))|1<<(huffbits+2);
    const bool firstcol=column==0 && blockW[mcupos>>6]>mcupos;
    if (++hbcount>2 || huffbits==0) hbcount=0;
    jassert(coef>=0 && coef<256);
    const int zu=zzu[mcupos&63], zv=zzv[mcupos&63];
    if (hbcount==0) {
      U64 n=hc*32;
      int i=0;
      cxt[i++]=hash(++n, coef, adv_pred[2]/12+(run_pred[2]<<8), ssum2>>6, prev_coef/72);
      cxt[i++]=hash(++n, coef, adv_pred[0]/12+(run_pred[0]<<8), ssum2>>6, prev_coef/72);
      cxt[i++]=hash(++n, coef, adv_pred[1]/11+(run_pred[1]<<8), ssum2>>6);
      cxt[i++]=hash(++n, rs1, adv_pred[2]/7, run_pred[5]/2, prev_coef/10);
      cxt[i++]=hash(++n, rs1, adv_pred[0]/7, run_pred[3]/2, prev_coef/10);
      cxt[i++]=hash(++n, rs1, adv_pred[1]/11, run_pred[4]);
      cxt[i++]=hash(++n, adv_pred[2]/14, run_pred[2], adv_pred[0]/14, run_pred[0]);
      cxt[i++]=hash(++n, cbuf[cpos-blockN[mcupos>>6]]>>4, adv_pred[3]/17, run_pred[1], run_pred[5]);
      cxt[i++]=hash(++n, cbuf[cpos-blockW[mcupos>>6]]>>4, adv_pred[3]/17, run_pred[1], run_pred[3]);
      cxt[i++]=hash(++n, lcp[0]/22, lcp[1]/22, adv_pred[1]/7, run_pred[1]);
      cxt[i++]=hash(++n, lcp[0]/22, lcp[1]/22, mcupos&63, lcp[4]/30);
      cxt[i++]=hash(++n, zu/2, lcp[0]/13, lcp[2]/30, prev_coef/40+((prev_coef2/28)<<20));
      cxt[i++]=hash(++n, zv/2, lcp[1]/13, lcp[3]/30, prev_coef/40+((prev_coef2/28)<<20));
      cxt[i++]=hash(++n, rs1, prev_coef/42, prev_coef2/34, lcp[0]/60,lcp[2]/14,lcp[1]/60,lcp[3]/14);
      cxt[i++]=hash(++n, mcupos&63, column>>1);
      cxt[i++]=hash(++n, column>>3, min(5+2*(!comp),zu+zv), lcp[0]/10,lcp[2]/40,lcp[1]/10,lcp[3]/40);
      cxt[i++]=hash(++n, ssum>>3, mcupos&63);
      cxt[i++]=hash(++n, rs1, mcupos&63, run_pred[1]);
      cxt[i++]=hash(++n, coef, ssum2>>5, adv_pred[3]/30, (comp)?hash(prev_coef/22,prev_coef2/50):ssum/((mcupos&0x3F)+1));
      cxt[i++]=hash(++n, lcp[0]/40, lcp[1]/40, adv_pred[1]/28, (comp)?prev_coef/40+((prev_coef2/40)<<20):lcp[4]/22, min(7,zu+zv), ssum/(2*(zu+zv)+1) );
      cxt[i++]=hash(++n, zv, cbuf[cpos-blockN[mcupos>>6]], adv_pred[2]/28, run_pred[2]);
      cxt[i++]=hash(++n, zu, cbuf[cpos-blockW[mcupos>>6]], adv_pred[0]/28, run_pred[0]);
      cxt[i++]=hash(++n, adv_pred[2]/7, run_pred[2]);
      cxt[i++]=hash(  n, adv_pred[0]/7, run_pred[0]);
      cxt[i++]=hash(  n, adv_pred[1]/7, run_pred[1]);
      cxt[i++]=hash(++n, zv, lcp[1]/14, adv_pred[2]/16, run_pred[5]);
      cxt[i++]=hash(++n, zu, lcp[0]/14, adv_pred[0]/16, run_pred[3]);
      cxt[i++]=hash(++n, lcp[0]/14, lcp[1]/14, adv_pred[3]/16);
      cxt[i++]=hash(++n, coef, prev_coef/10, prev_coef2/20);
      cxt[i++]=hash(++n, coef, ssum>>2, prev_coef_rs);
      cxt[i++]=hash(++n, coef, adv_pred[1]/17, lcp[(zu<zv)]/24,lcp[2]/20,lcp[3]/24);
      cxt[i++]=hash(++n, coef, adv_pred[3]/11, lcp[(zu<zv)]/50,lcp[2+3*(zu*zv>1)]/50,lcp[3+3*(zu*zv>1)]/50);
      assert(i==N);
    }

    // Predict next bit
    m1->add(128); //network bias
    assert(hbcount<=2);
    switch(hbcount) {
      case 0: { for (int i=0; i<N; ++i) {
                  cp[i]=t[cxt[i]]+1;
                  const U8 s=*cp[i];
                  const U32 p=sm.p(i,s);
                  m.add(((int)p-2048)>>3);
                  const int st=stretch(p);
                  m1->add(st);
                  m.add(st>>1);
                }
                break;
              }
      case 1: { int hc=1+(huffcode&1)*3;
                for (int i=0; i<N; ++i) {
                  cp[i]+=hc;
                  const U8 s=*cp[i];
                  const U32 p=sm.p(i,s);
                  m.add(((int)p-2048)>>3);
                  const int st=stretch(p);
                  m1->add(st);
                  m.add(st>>1);
                }
                break;
              } 
      default: {int hc=1+(huffcode&1); 
                for (int i=0; i<N; ++i) {
                  cp[i]+=hc;
                  const U8 s=*cp[i];
                  const U32 p=sm.p(i,s);
                  m.add(((int)p-2048)>>3);
                  const int st=stretch(p);
                  m1->add(st);
                  m.add(st>>1);
                }
                break;
              } 
    }
    if (!hbcount) MJPEGMap.set(hash(mcupos, column, row, hc>>2));
    MJPEGMap.mix(*m1);
    m1->set(firstcol, 2);
    m1->set(coef | (min(3,huffbits)<<8), 1024);
    m1->set(((hc&0x1FE)<<1) | min(3,ilog2(zu+zv)), 1024);
    int pr=m1->p();
    m.add(stretch(pr)>>1);
    m.add((pr>>2)-511);
    pr=apm1.p(pr, (hc&511) | (((adv_pred[1]/16)&63)<<9), 1023);
    m.add(stretch(pr)>>1);
    m.add((pr>>2)-511);
    pr=apm2.p(pr, (hc&511) | (coef<<9), 1023);
    m.add(stretch(pr)>>1);
    m.add((pr>>2)-511);
    
    m.set(1 + ((zu+zv<5) | ((huffbits>8)<<1) | (firstcol<<2)), 1+8);
    m.set(1 + ((hc&0xFF) | (min(3,(zu+zv)/3))<<8), 1+1024);
    m.set(coef | (min(3,huffbits/2)<<8), 1024);
    
    return true;
  }
};

//////////////////////////// AudioModel /////////////////////////////////

// Model a 16/8-bit stereo/mono uncompressed .wav file.
// Based on 'An asymptotically Optimal Predictor for Stereo Lossless Audio Compression'
// by Florin Ghido.

#ifdef USE_AUDIOMODEL

// A base class for the common functions
class AudioModel {
protected:
  const Shared * const shared;
  ModelStats *stats;
  int S=0;
  int wmode=0;
  AudioModel(const Shared * const sh, ModelStats *st) : shared(sh), stats(st) {}

  inline int s2(int i) { INJECT_SHARED_buf return int(short(buf(i)+256*buf(i-1))); }
  inline int t2(int i) { INJECT_SHARED_buf return int(short(buf(i-1)+256*buf(i))); }

  inline int X1(int i) {
    INJECT_SHARED_buf
    switch (wmode) {
      case 0: return buf(i)-128;
      case 1: return buf(i<<1)-128;
      case 2: return s2(i<<1);
      case 3: return s2(i<<2);
      case 4: return (buf(i)^128)-128;
      case 5: return (buf(i<<1)^128)-128;
      case 6: return t2(i<<1);
      case 7: return t2(i<<2);
      default: return 0;
    }
  }

  inline int X2(int i) {
    INJECT_SHARED_buf
    switch (wmode) {
      case 0: return buf(i+S)-128;
      case 1: return buf((i<<1)-1)-128;
      case 2: return s2((i+S)<<1);
      case 3: return s2((i<<2)-2);
      case 4: return (buf(i+S)^128)-128;
      case 5: return (buf((i<<1)-1)^128)-128;
      case 6: return t2((i+S)<<1);
      case 7: return t2((i<<2)-2);
      default: return 0;
    }
  }

  inline int signedClip8(const int i) {
    return max(-128, min(127, i));
  }

  inline int signedClip16(const int i) {
    return max(-32768, min(32767, i));
  }
};

class Audio8bitModel : AudioModel {
private:
  static constexpr int nOLS = 8;
  static constexpr int nLMS = 3;
  static constexpr int nSSM = nOLS+nLMS+3;
  static constexpr int nCtx = 3;
public:
  static constexpr int MIXERINPUTS = nCtx*nSSM*SmallStationaryContextMap::MIXERINPUTS;
  static constexpr int MIXERCONTEXTS = 4096+2048+2048+256+10; //8458
  static constexpr int MIXERCONTEXTSETS = 5;
private:
  SmallStationaryContextMap sMap1b[nSSM][nCtx];
  OLS<double, int8_t> ols[nOLS][2]{
    {{128, 24, 0.9975}, {128, 24, 0.9975}},
    {{90, 30, 0.9965}, {90, 30, 0.9965}},
    {{90, 31, 0.996}, {90, 31, 0.996}},
    {{90, 32, 0.995}, {90, 32, 0.995}},
    {{90, 33, 0.995}, {90, 33, 0.995}},
    {{90, 34, 0.9985}, {90, 34, 0.9985}},
    {{28, 4, 0.98}, {28, 4, 0.98}},
    {{28, 3, 0.992}, {28, 3, 0.992}}
  };
  LMS<float, int8_t> lms[nLMS][2]{
    {{1280, 640, 3e-5f, 2e-5f}, {1280, 640, 3e-5f, 2e-5f}},
    {{640, 64, 8e-5f, 1e-5f}, {640, 64, 8e-5f, 1e-5f}},
    {{2450, 8, 1.6e-5f, 1e-6f}, {2450, 8, 1.6e-5f, 1e-6f}}
  };
  int prd[nSSM][2][2]{ 0 }, residuals[nSSM][2]{ 0 };
  int stereo=0, ch=0;
  U32 mask=0, errLog=0, mxCtx=0;
public:
  Audio8bitModel(const Shared * const sh, ModelStats *st) : AudioModel(sh, st),
    sMap1b{ /* SmallStationaryContextMap : BitsOfContext, InputBits, Rate, Scale */
    /*nOLS: 0-3*/ {{sh,11,1,6,128}, {sh,11,1,9,128}, {sh,11,1,7,86}}, {{sh,11,1,6,128}, {sh,11,1,9,128}, {sh,11,1,7,86}}, {{sh,11,1,6,128}, {sh,11,1,9,128}, {sh,11,1,7,86}}, {{sh,11,1,6,86}, {sh,11,1,9,86}, {sh,11,1,7,86}},
    /*nOLS: 4-7*/ {{sh,11,1,6,128}, {sh,11,1,9,128}, {sh,11,1,7,86}}, {{sh,11,1,6,128}, {sh,11,1,9,128}, {sh,11,1,7,86}}, {{sh,11,1,6,128}, {sh,11,1,9,128}, {sh,11,1,7,86}}, {{sh,11,1,6,128}, {sh,11,1,9,128}, {sh,11,1,7,86}},
    /*nLMS: 0-2*/ {{sh,11,1,6,86}, {sh,11,1,9,86}, {sh,11,1,7,86}}, {{sh,11,1,6,86}, {sh,11,1,9,86}, {sh,11,1,7,86}}, {{sh,11,1,6,86}, {sh,11,1,9,86}, {sh,11,1,7,86}}, 
    /*nSSM: 0-2*/ {{sh,11,1,6,86}, {sh,11,1,9,86}, {sh,11,1,7,86}}, {{sh,11,1,6,86}, {sh,11,1,9,86}, {sh,11,1,7,86}}, {{sh,11,1,6,86}, {sh,11,1,9,86}, {sh,11,1,7,86}}
  }
  {}
  void setparam(int info) {
    INJECT_STATS_blpos INJECT_SHARED_bpos
    if (blpos==0 && bpos==0) {
      assert((info&2)==0);
      stereo = (info&1);
      mask = 0;
      stats->Wav = stereo+1;
      wmode=info;
      for (int i=0; i<nLMS; i++)
        lms[i][0].Reset(), lms[i][1].Reset();
    }
  }
  void mix(Mixer& m) {
    INJECT_SHARED_bpos INJECT_SHARED_c0 INJECT_SHARED_c1
    if (bpos==0) {
      INJECT_STATS_blpos
      ch=(stereo)?blpos&1:0;
      const int8_t s = int(((wmode&4)>0)?c1^128:c1)-128;
      const int pCh = ch^stereo;
      int i = 0;
      for (errLog=0; i<nOLS; i++) {
        ols[i][pCh].Update(s);
        residuals[i][pCh] = s-prd[i][pCh][0];
        const U32 absResidual = (U32)abs(residuals[i][pCh]);
        mask+=mask+(absResidual>4);
        errLog+=SQR(absResidual);
      }
      for (int j=0; j<nLMS; j++)
        lms[j][pCh].Update(s);
      for (; i<nSSM; i++)
        residuals[i][pCh] = s-prd[i][pCh][0];
      errLog = min(0xF, ilog2(errLog));
      stats->Audio = mxCtx = ilog2(min(0x1F, BitCount(mask)))*2+ch;

      int k1=90, k2=k1-12*stereo;
      for (int j=(i=1); j<=k1; j++, i+=1<<((j>8)+(j>16)+(j>64))) ols[1][ch].Add(X1(i));
      for (int j=(i=1); j<=k2; j++, i+=1<<((j>5)+(j>10)+(j>17)+(j>26)+(j>37))) ols[2][ch].Add(X1(i));
      for (int j=(i=1); j<=k2; j++, i+=1<<((j>3)+(j>7)+(j>14)+(j>20)+(j>33)+(j>49))) ols[3][ch].Add(X1(i));
      for (int j=(i=1); j<=k2; j++, i+=1+(j>4)+(j>8)) ols[4][ch].Add(X1(i));
      for (int j=(i=1); j<=k1; j++, i+=2+((j>3)+(j>9)+(j>19)+(j>36)+(j>61))) ols[5][ch].Add(X1(i));
      if (stereo) {
        for (i=1; i<=k1-k2; i++) {
          const double s = (double)X2(i);
          ols[2][ch].AddFloat(s);
          ols[3][ch].AddFloat(s);
          ols[4][ch].AddFloat(s);
        }
      }
      k1=28, k2=k1-6*stereo;
      for (i=1; i<=k2; i++) {
        const double s = (double)X1(i);
        ols[0][ch].AddFloat(s);
        ols[6][ch].AddFloat(s);
        ols[7][ch].AddFloat(s);
      }
      for (; i<=96; i++) ols[0][ch].Add(X1(i));
      if (stereo) {
        for (i=1; i<=k1-k2; i++) {
          const double s = (double)X2(i);
          ols[0][ch].AddFloat(s);
          ols[6][ch].AddFloat(s);
          ols[7][ch].AddFloat(s);
        }
        for (; i<=32; i++) ols[0][ch].Add(X2(i));
      }
      else
        for (; i<=128; i++) ols[0][ch].Add(X1(i));

      for (i=0; i<nOLS; i++)
        prd[i][ch][0] = signedClip8((int)floor(ols[i][ch].Predict()));
      for (; i<nOLS+nLMS; i++)
        prd[i][ch][0] = signedClip8((int)floor(lms[i-nOLS][ch].Predict(s)));
      prd[i++][ch][0] = signedClip8(X1(1)*2-X1(2));
      prd[i++][ch][0] = signedClip8(X1(1)*3-X1(2)*3+X1(3));
      prd[i  ][ch][0] = signedClip8(X1(1)*4-X1(2)*6+X1(3)*4-X1(4));
      for (i=0; i<nSSM; i++)
        prd[i][ch][1] = signedClip8(prd[i][ch][0]+residuals[i][pCh]);
    }
    const int8_t B = c0<<(8-bpos);
    for (int i=0; i<nSSM; i++) {
      const U32 ctx = (prd[i][ch][0]-B)*8+bpos;
      sMap1b[i][0].set(ctx);
      sMap1b[i][1].set(ctx);
      sMap1b[i][2].set((prd[i][ch][1]-B)*8+bpos);
      sMap1b[i][0].mix(m);
      sMap1b[i][1].mix(m);
      sMap1b[i][2].mix(m);
    }
    m.set((errLog<<8)|c0, 4096);
    m.set((U8(mask)<<3)|(ch<<2)|(bpos>>1), 2048);
    m.set((mxCtx<<7)|(c1>>1), 1280);
    m.set((errLog<<4)|(ch<<3)|bpos, 256);
    m.set(mxCtx, 10);
  }
};

class Audio16bitModel : AudioModel {
private:
  static constexpr int nOLS = 8;
  static constexpr int nLMS = 3;
  static constexpr int nSSM = nOLS+nLMS+3;
  static constexpr int nCtx = 4;
public:
  static constexpr int MIXERINPUTS = nCtx*nSSM*SmallStationaryContextMap::MIXERINPUTS;
  static constexpr int MIXERCONTEXTS = 8192+4096+2560+256+20; //15124
  static constexpr int MIXERCONTEXTSETS = 5;
private:
  SmallStationaryContextMap sMap1b[nSSM][nCtx];
  OLS<double, short> ols[nOLS][2]{
    {{128, 24, 0.9975}, {128, 24, 0.9975}},
    {{90, 30, 0.997}, {90, 30, 0.997}},
    {{90, 31, 0.996}, {90, 31, 0.996}},
    {{90, 32, 0.995}, {90, 32, 0.995}},
    {{90, 33, 0.995}, {90, 33, 0.995}},
    {{90, 34, 0.9985}, {90, 34, 0.9985}},
    {{28, 4, 0.98}, {28, 4, 0.98}},
    {{32, 3, 0.992}, {32, 3, 0.992}}
  };
  LMS<float, short> lms[nLMS][2]{
    {{1280, 640, 5e-5f, 5e-5f}, {1280, 640, 5e-5f, 5e-5f}},
    {{640, 64, 7e-5f, 1e-5f}, {640, 64, 7e-5f, 1e-5f}},
    {{2450, 8, 2e-5f, 2e-6f}, {2450, 8, 2e-5f, 2e-6f}}
  };
  int prd[nSSM][2][2]{ 0 }, residuals[nSSM][2]{ 0 };
  int stereo=0, ch=0, lsb=0;
  U32 mask=0, errLog=0, mxCtx=0;
  short sample = 0;
public:
  Audio16bitModel(const Shared * const sh, ModelStats *st) : AudioModel(sh, st),
    sMap1b{ /* SmallStationaryContextMap : BitsOfContext, InputBits, Rate, Scale */
    /*nOLS: 0-3*/ {{sh,17,1,7,128},{sh,17,1,10,128},{sh,17,1,6,86},{sh,17,1,6,128}}, {{sh,17,1,7,128},{sh,17,1,10,128},{sh,17,1,6,86},{sh,17,1,6,128}}, {{sh,17,1,7,128},{sh,17,1,10,128},{sh,17,1,6,86},{sh,17,1,6,128}}, {{sh,17,1,7,128},{sh,17,1,10,128},{sh,17,1,6,86},{sh,17,1,6,128}},
    /*nOLS: 4-7*/ {{sh,17,1,7,128},{sh,17,1,10,128},{sh,17,1,6,86},{sh,17,1,6,128}}, {{sh,17,1,7,128},{sh,17,1,10,128},{sh,17,1,6,86},{sh,17,1,6,128}}, {{sh,17,1,7,128},{sh,17,1,10,128},{sh,17,1,6,86},{sh,17,1,6,128}}, {{sh,17,1,7,128},{sh,17,1,10,128},{sh,17,1,6,86},{sh,17,1,6,128}},
    /*nLMS: 0-2*/ {{sh,17,1,7,86}, {sh,17,1,10,86},{sh,17,1,6,64},{sh,17,1,6,86}}, {{sh,17,1,7,86},{sh,17,1,10,86},{sh,17,1,6,64},{sh,17,1,6,86}}, {{sh,17,1,7,86},{sh,17,1,10,86},{sh,17,1,6,64},{sh,17,1,6,86}},
    /*nSSM: 0-2*/ {{sh,17,1,7,86}, {sh,17,1,10,86},{sh,17,1,6,64},{sh,17,1,6,86}}, {{sh,17,1,7,86},{sh,17,1,10,86},{sh,17,1,6,64},{sh,17,1,6,86}}, {{sh,17,1,7,86},{sh,17,1,10,86},{sh,17,1,6,64},{sh,17,1,6,86}}
  }
  {}
  void setparam(int info) {
    INJECT_STATS_blpos INJECT_SHARED_bpos
    if (blpos==0 && bpos==0) {
      info|=4;  // comment this line if skipping the endianness transform
      assert((info&2)!=0);
      stereo = (info&1);
      lsb = (info<4);
      mask = 0;
      stats->Wav = (stereo+1)*2;
      wmode=info;
      for (int i=0; i<nLMS; i++)
        lms[i][0].Reset(), lms[i][1].Reset();
    }
  }
  void mix(Mixer& m) {
    INJECT_SHARED_bpos INJECT_STATS_blpos INJECT_SHARED_c0 INJECT_SHARED_c1
    if (bpos==0 && blpos!=0) {
      ch=(stereo)?(blpos&2)>>1:0;
      lsb=(blpos&1)^(wmode<4);
      if ((blpos&1)==0) {
        sample = (wmode<4)?s2(2):t2(2);
        const int pCh = ch^stereo;
        int i = 0;
        for (errLog=0; i<nOLS; i++) {
          ols[i][pCh].Update(sample);
          residuals[i][pCh] = sample-prd[i][pCh][0];
          const U32 absResidual = (U32)abs(residuals[i][pCh]);
          mask+=mask+(absResidual>128);
          errLog+=SQR(absResidual>>6);
        }
        for (int j=0; j<nLMS; j++)
          lms[j][pCh].Update(sample);
        for (; i<nSSM; i++)
          residuals[i][pCh] = sample-prd[i][pCh][0];
        errLog = min(0xF, ilog2(errLog));

        if (stereo) {
          for (int i=1; i<=24; i++) ols[0][ch].Add(X2(i));
          for (int i=1; i<=104; i++) ols[0][ch].Add(X1(i));
        }
        else
          for (int i=1; i<=128; i++) ols[0][ch].Add(X1(i));

        int k1=90, k2=k1-12*stereo;
        for (int j=(i=1); j<=k1; j++, i+=1<<((j>16)+(j>32)+(j>64))) ols[1][ch].Add(X1(i));
        for (int j=(i=1); j<=k2; j++, i+=1<<((j>5)+(j>10)+(j>17)+(j>26)+(j>37))) ols[2][ch].Add(X1(i));       
        for (int j=(i=1); j<=k2; j++, i+=1<<((j>3)+(j>7)+(j>14)+(j>20)+(j>33)+(j>49))) ols[3][ch].Add(X1(i));
        for (int j=(i=1); j<=k2; j++, i+=1+(j>4)+(j>8)) ols[4][ch].Add(X1(i));
        for (int j=(i=1); j<=k1; j++, i+=2+((j>3)+(j>9)+(j>19)+(j>36)+(j>61))) ols[5][ch].Add(X1(i));

        if (stereo) {
          for (i=1; i<=k1-k2; i++) {
            const double s = (double)X2(i);
            ols[2][ch].AddFloat(s);
            ols[3][ch].AddFloat(s);
            ols[4][ch].AddFloat(s);
          }
        }

        k1=28, k2=k1-6*stereo;
        for (i=1; i<=k2; i++) ols[6][ch].Add(X1(i));
        for (i=1; i<=k1-k2; i++) ols[6][ch].Add(X2(i));

        k1=32, k2=k1-8*stereo;
        for (i=1; i<=k2; i++) ols[7][ch].Add(X1(i));
        for (i=1; i<=k1-k2; i++) ols[7][ch].Add(X2(i));

        for (i=0; i<nOLS; i++)
          prd[i][ch][0] = signedClip16((int)floor(ols[i][ch].Predict()));
        for (; i<nOLS+nLMS; i++)
          prd[i][ch][0] = signedClip16((int)floor(lms[i-nOLS][ch].Predict(sample)));
        prd[i++][ch][0] = signedClip16(X1(1)*2-X1(2));
        prd[i++][ch][0] = signedClip16(X1(1)*3-X1(2)*3+X1(3));
        prd[i  ][ch][0] = signedClip16(X1(1)*4-X1(2)*6+X1(3)*4-X1(4));
        for (i=0; i<nSSM; i++)
          prd[i][ch][1] = signedClip16(prd[i][ch][0]+residuals[i][pCh]);
      }
      stats->Audio = 0x80|(mxCtx = ilog2(min(0x1F, BitCount(mask)))*4+ch*2+lsb);
    }

    const short B = short( (wmode<4)? (lsb)?U8(c0<<(8-bpos)):(c0<<(16-bpos))|c1 : (lsb)?(c1<<8)|U8(c0<<(8-bpos)):c0<<(16-bpos) );

    for (int i=0; i<nSSM; i++) {
      const U32 ctx0 = U16(prd[i][ch][0]-B);
      const U32 ctx1 = U16(prd[i][ch][1]-B);

      const int shift=(!lsb);
      sMap1b[i][0].set( (lsb<<16)|(bpos<<13)|(ctx0>>(3<<shift)) );
      sMap1b[i][1].set( (lsb<<16)|(bpos<<13)|(ctx0>>((!lsb)+(3<<shift))) );
      sMap1b[i][2].set( (lsb<<16)|(bpos<<13)|(ctx0>>((!lsb)*2+(3<<shift))) );
      sMap1b[i][3].set( (lsb<<16)|(bpos<<13)|(ctx1>>((!lsb)+(3<<shift))) );
      sMap1b[i][0].mix(m);
      sMap1b[i][1].mix(m);
      sMap1b[i][2].mix(m);
      sMap1b[i][3].mix(m);
    }

    m.set((errLog<<9)|(lsb<<8)|c0, 8192);
    m.set((U8(mask)<<4)|(ch<<3)|(lsb<<2)|(bpos>>1), 4096);
    m.set((mxCtx<<7)|(c1>>1), 2560);
    m.set((errLog<<4)|(ch<<3)|(lsb<<2)|(bpos>>1), 256);
    m.set(mxCtx, 20);
  }
};

#endif //USE_AUDIOMODEL

//////////////////////////// exeModel /////////////////////////
//
//  Model for x86/x64 code.
//  Based on the previous paq* exe models and on DisFilter (http://www.farbrausch.de/~fg/code/disfilter/) by Fabian Giesen.
//
//  Attemps to parse the input stream as x86/x64 instructions, and quantizes them into 32-bit representations that are then
//  used as context to predict the next bits, and also extracts possible sparse contexts at several previous positions that
//  are relevant to parsing (prefixes, opcode, Mod and R/M fields of the ModRM byte, Scale field of the SIB byte)
//

class ExeModel {
  // formats
  enum InstructionFormat {
    // encoding mode
    fNM = 0x0,      // no ModRM
    fAM = 0x1,      // no ModRM, "address mode" (jumps or direct addresses)
    fMR = 0x2,      // ModRM present
    fMEXTRA = 0x3,  // ModRM present, includes extra bits for opcode
    fMODE = 0x3,    // bitmask for mode

    // no ModRM: size of immediate operand
    fNI = 0x0,      // no immediate
    fBI = 0x4,      // byte immediate
    fWI = 0x8,      // word immediate
    fDI = 0xc,      // dword immediate
    fTYPE = 0xc,    // type mask

    // address mode: type of address operand
    fAD = 0x0,      // absolute address
    fDA = 0x4,      // dword absolute jump target
    fBR = 0x8,      // byte relative jump target
    fDR = 0xc,      // dword relative jump target

    // others
    fERR = 0xf,     // denotes invalid opcodes
  };

  // 1 byte opcodes
  static constexpr U8 Table1[256] = {
    // 0       1       2       3       4       5       6       7       8       9       a       b       c       d       e       f
    fMR|fNI,fMR|fNI,fMR|fNI,fMR|fNI,fNM|fBI,fNM|fDI,fNM|fNI,fNM|fNI,fMR|fNI,fMR|fNI,fMR|fNI,fMR|fNI,fNM|fBI,fNM|fDI,fNM|fNI,fNM|fNI, // 0
    fMR|fNI,fMR|fNI,fMR|fNI,fMR|fNI,fNM|fBI,fNM|fDI,fNM|fNI,fNM|fNI,fMR|fNI,fMR|fNI,fMR|fNI,fMR|fNI,fNM|fBI,fNM|fDI,fNM|fNI,fNM|fNI, // 1
    fMR|fNI,fMR|fNI,fMR|fNI,fMR|fNI,fNM|fBI,fNM|fDI,fNM|fNI,fNM|fNI,fMR|fNI,fMR|fNI,fMR|fNI,fMR|fNI,fNM|fBI,fNM|fDI,fNM|fNI,fNM|fNI, // 2
    fMR|fNI,fMR|fNI,fMR|fNI,fMR|fNI,fNM|fBI,fNM|fDI,fNM|fNI,fNM|fNI,fMR|fNI,fMR|fNI,fMR|fNI,fMR|fNI,fNM|fBI,fNM|fDI,fNM|fNI,fNM|fNI, // 3

    fNM|fNI,fNM|fNI,fNM|fNI,fNM|fNI,fNM|fNI,fNM|fNI,fNM|fNI,fNM|fNI,fNM|fNI,fNM|fNI,fNM|fNI,fNM|fNI,fNM|fNI,fNM|fNI,fNM|fNI,fNM|fNI, // 4
    fNM|fNI,fNM|fNI,fNM|fNI,fNM|fNI,fNM|fNI,fNM|fNI,fNM|fNI,fNM|fNI,fNM|fNI,fNM|fNI,fNM|fNI,fNM|fNI,fNM|fNI,fNM|fNI,fNM|fNI,fNM|fNI, // 5
    fNM|fNI,fNM|fNI,fMR|fNI,fMR|fNI,fNM|fNI,fNM|fNI,fNM|fNI,fNM|fNI,fNM|fDI,fMR|fDI,fNM|fBI,fMR|fBI,fNM|fNI,fNM|fNI,fNM|fNI,fNM|fNI, // 6
    fAM|fBR,fAM|fBR,fAM|fBR,fAM|fBR,fAM|fBR,fAM|fBR,fAM|fBR,fAM|fBR,fAM|fBR,fAM|fBR,fAM|fBR,fAM|fBR,fAM|fBR,fAM|fBR,fAM|fBR,fAM|fBR, // 7

    fMR|fBI,fMR|fDI,fMR|fBI,fMR|fBI,fMR|fNI,fMR|fNI,fMR|fNI,fMR|fNI,fMR|fNI,fMR|fNI,fMR|fNI,fMR|fNI,fMR|fNI,fMR|fNI,fMR|fNI,fMR|fNI, // 8
    fNM|fNI,fNM|fNI,fNM|fNI,fNM|fNI,fNM|fNI,fNM|fNI,fNM|fNI,fNM|fNI,fNM|fNI,fNM|fNI,fAM|fDA,fNM|fNI,fNM|fNI,fNM|fNI,fNM|fNI,fNM|fNI, // 9
    fAM|fAD,fAM|fAD,fAM|fAD,fAM|fAD,fNM|fNI,fNM|fNI,fNM|fNI,fNM|fNI,fNM|fBI,fNM|fDI,fNM|fNI,fNM|fNI,fNM|fNI,fNM|fNI,fNM|fNI,fNM|fNI, // a
    fNM|fBI,fNM|fBI,fNM|fBI,fNM|fBI,fNM|fBI,fNM|fBI,fNM|fBI,fNM|fBI,fNM|fDI,fNM|fDI,fNM|fDI,fNM|fDI,fNM|fDI,fNM|fDI,fNM|fDI,fNM|fDI, // b

    fMR|fBI,fMR|fBI,fNM|fWI,fNM|fNI,fMR|fNI,fMR|fNI,fMR|fBI,fMR|fDI,fNM|fBI,fNM|fNI,fNM|fWI,fNM|fNI,fNM|fNI,fNM|fBI,fERR   ,fNM|fNI, // c
    fMR|fNI,fMR|fNI,fMR|fNI,fMR|fNI,fNM|fBI,fNM|fBI,fNM|fNI,fNM|fNI,fMR|fNI,fMR|fNI,fMR|fNI,fMR|fNI,fMR|fNI,fMR|fNI,fMR|fNI,fMR|fNI, // d
    fAM|fBR,fAM|fBR,fAM|fBR,fAM|fBR,fNM|fBI,fNM|fBI,fNM|fBI,fNM|fBI,fAM|fDR,fAM|fDR,fAM|fAD,fAM|fBR,fNM|fNI,fNM|fNI,fNM|fNI,fNM|fNI, // e
    fNM|fNI,fERR   ,fNM|fNI,fNM|fNI,fNM|fNI,fNM|fNI,fMEXTRA,fMEXTRA,fNM|fNI,fNM|fNI,fNM|fNI,fNM|fNI,fNM|fNI,fNM|fNI,fMEXTRA,fMEXTRA, // f
  };

  // 2 byte opcodes
  static constexpr U8 Table2[256] = {
    // 0       1       2       3       4       5       6       7       8       9       a       b       c       d       e       f
    fERR   ,fERR   ,fERR   ,fERR   ,fERR   ,fERR   ,fNM|fNI,fERR   ,fNM|fNI,fNM|fNI,fERR   ,fERR   ,fERR   ,fERR   ,fERR   ,fERR   , // 0
    fMR|fNI,fMR|fNI,fMR|fNI,fMR|fNI,fMR|fNI,fMR|fNI,fMR|fNI,fMR|fNI,fMR|fNI,fERR   ,fERR   ,fERR   ,fERR   ,fERR   ,fERR   ,fERR   , // 1
    fMR|fNI,fMR|fNI,fMR|fNI,fMR|fNI,fERR   ,fERR   ,fERR   ,fERR   ,fMR|fNI,fMR|fNI,fMR|fNI,fMR|fNI,fMR|fNI,fMR|fNI,fMR|fNI,fMR|fNI, // 2
    fNM|fNI,fNM|fNI,fNM|fNI,fNM|fNI,fNM|fNI,fNM|fNI,fERR   ,fNM|fNI,fERR   ,fERR   ,fERR   ,fERR   ,fERR   ,fERR   ,fERR   ,fERR   , // 3

    fMR|fNI,fMR|fNI,fMR|fNI,fMR|fNI,fMR|fNI,fMR|fNI,fMR|fNI,fMR|fNI,fMR|fNI,fMR|fNI,fMR|fNI,fMR|fNI,fMR|fNI,fMR|fNI,fMR|fNI,fMR|fNI, // 4
    fMR|fNI,fMR|fNI,fMR|fNI,fMR|fNI,fMR|fNI,fMR|fNI,fMR|fNI,fMR|fNI,fMR|fNI,fMR|fNI,fMR|fNI,fMR|fNI,fMR|fNI,fMR|fNI,fMR|fNI,fMR|fNI, // 5
    fMR|fNI,fMR|fNI,fMR|fNI,fMR|fNI,fMR|fNI,fMR|fNI,fMR|fNI,fMR|fNI,fMR|fNI,fMR|fNI,fMR|fNI,fMR|fNI,fMR|fNI,fMR|fNI,fMR|fNI,fMR|fNI, // 6
    fMR|fBI,fMR|fBI,fMR|fBI,fMR|fBI,fMR|fNI,fMR|fNI,fMR|fNI,fNM|fNI,fERR   ,fERR   ,fERR   ,fERR   ,fERR   ,fERR   ,fMR|fNI,fMR|fNI, // 7

    fAM|fDR,fAM|fDR,fAM|fDR,fAM|fDR,fAM|fDR,fAM|fDR,fAM|fDR,fAM|fDR,fAM|fDR,fAM|fDR,fAM|fDR,fAM|fDR,fAM|fDR,fAM|fDR,fAM|fDR,fAM|fDR, // 8
    fMR|fNI,fMR|fNI,fMR|fNI,fMR|fNI,fMR|fNI,fMR|fNI,fMR|fNI,fMR|fNI,fMR|fNI,fMR|fNI,fMR|fNI,fMR|fNI,fMR|fNI,fMR|fNI,fMR|fNI,fMR|fNI, // 9
    fNM|fNI,fNM|fNI,fNM|fNI,fMR|fNI,fMR|fBI,fMR|fNI,fMR|fNI,fMR|fNI,fERR   ,fERR   ,fERR   ,fMR|fNI,fMR|fBI,fMR|fNI,fERR   ,fMR|fNI, // a
    fMR|fNI,fMR|fNI,fMR|fNI,fMR|fNI,fMR|fNI,fMR|fNI,fMR|fNI,fMR|fNI,fERR   ,fERR   ,fERR   ,fMR|fNI,fMR|fNI,fMR|fNI,fMR|fNI,fMR|fNI, // b

    fMR|fNI,fMR|fNI,fMR|fNI,fMR|fNI,fMR|fNI,fMR|fNI,fMR|fNI,fMR|fNI,fNM|fNI,fNM|fNI,fNM|fNI,fNM|fNI,fNM|fNI,fNM|fNI,fNM|fNI,fNM|fNI, // c
    fMR|fNI,fMR|fNI,fMR|fNI,fMR|fNI,fMR|fNI,fMR|fNI,fMR|fNI,fMR|fNI,fMR|fNI,fMR|fNI,fMR|fNI,fMR|fNI,fMR|fNI,fMR|fNI,fMR|fNI,fMR|fNI, // d
    fMR|fNI,fMR|fNI,fMR|fNI,fMR|fNI,fMR|fNI,fMR|fNI,fMR|fNI,fMR|fNI,fMR|fNI,fMR|fNI,fMR|fNI,fMR|fNI,fMR|fNI,fMR|fNI,fMR|fNI,fMR|fNI, // e
    fMR|fNI,fMR|fNI,fMR|fNI,fMR|fNI,fMR|fNI,fMR|fNI,fMR|fNI,fMR|fNI,fMR|fNI,fMR|fNI,fMR|fNI,fMR|fNI,fMR|fNI,fMR|fNI,fMR|fNI,fERR   , // f
  };

  // 3 byte opcodes 0F38XX
  static constexpr U8 Table3_38[256] = {
    // 0       1       2       3       4       5       6       7       8       9       a       b       c       d       e       f
    fMR|fNI,fMR|fNI,fMR|fNI,fMR|fNI,fMR|fNI,fMR|fNI,fMR|fNI,fMR|fNI,fMR|fNI,fMR|fNI,fMR|fNI,fMR|fNI,fERR   ,fERR   ,fERR   ,fERR   , // 0
    fMR|fNI,fERR   ,fERR   ,fERR   ,fMR|fNI,fMR|fNI,fERR   ,fMR|fNI,fERR   ,fERR   ,fERR   ,fERR   ,fMR|fNI,fMR|fNI,fMR|fNI,fERR   , // 1
    fMR|fNI,fMR|fNI,fMR|fNI,fMR|fNI,fMR|fNI,fMR|fNI,fERR   ,fERR   ,fMR|fNI,fMR|fNI,fMR|fNI,fMR|fNI,fERR   ,fERR   ,fERR   ,fERR   , // 2
    fMR|fNI,fMR|fNI,fMR|fNI,fMR|fNI,fMR|fNI,fMR|fNI,fERR   ,fMR|fNI,fMR|fNI,fMR|fNI,fMR|fNI,fMR|fNI,fMR|fNI,fMR|fNI,fMR|fNI,fMR|fNI, // 3
    fMR|fNI,fMR|fNI,fERR   ,fERR   ,fERR   ,fERR   ,fERR   ,fERR   ,fERR   ,fERR   ,fERR   ,fERR   ,fERR   ,fERR   ,fERR   ,fERR   , // 4
    fERR   ,fERR   ,fERR   ,fERR   ,fERR   ,fERR   ,fERR   ,fERR   ,fERR   ,fERR   ,fERR   ,fERR   ,fERR   ,fERR   ,fERR   ,fERR   , // 5
    fERR   ,fERR   ,fERR   ,fERR   ,fERR   ,fERR   ,fERR   ,fERR   ,fERR   ,fERR   ,fERR   ,fERR   ,fERR   ,fERR   ,fERR   ,fERR   , // 6
    fERR   ,fERR   ,fERR   ,fERR   ,fERR   ,fERR   ,fERR   ,fERR   ,fERR   ,fERR   ,fERR   ,fERR   ,fERR   ,fERR   ,fERR   ,fERR   , // 7
    fMR|fNI,fMR|fNI,fERR   ,fERR   ,fERR   ,fERR   ,fERR   ,fERR   ,fERR   ,fERR   ,fERR   ,fERR   ,fERR   ,fERR   ,fERR   ,fERR   , // 8
    fERR   ,fERR   ,fERR   ,fERR   ,fERR   ,fERR   ,fERR   ,fERR   ,fERR   ,fERR   ,fERR   ,fERR   ,fERR   ,fERR   ,fERR   ,fERR   , // 9
    fERR   ,fERR   ,fERR   ,fERR   ,fERR   ,fERR   ,fERR   ,fERR   ,fERR   ,fERR   ,fERR   ,fERR   ,fERR   ,fERR   ,fERR   ,fERR   , // a
    fERR   ,fERR   ,fERR   ,fERR   ,fERR   ,fERR   ,fERR   ,fERR   ,fERR   ,fERR   ,fERR   ,fERR   ,fERR   ,fERR   ,fERR   ,fERR   , // b
    fERR   ,fERR   ,fERR   ,fERR   ,fERR   ,fERR   ,fERR   ,fERR   ,fERR   ,fERR   ,fERR   ,fERR   ,fERR   ,fERR   ,fERR   ,fERR   , // c
    fERR   ,fERR   ,fERR   ,fERR   ,fERR   ,fERR   ,fERR   ,fERR   ,fERR   ,fERR   ,fERR   ,fERR   ,fERR   ,fERR   ,fERR   ,fERR   , // d
    fERR   ,fERR   ,fERR   ,fERR   ,fERR   ,fERR   ,fERR   ,fERR   ,fERR   ,fERR   ,fERR   ,fERR   ,fERR   ,fERR   ,fERR   ,fERR   , // e
    fMR|fNI,fMR|fNI,fERR   ,fERR   ,fERR   ,fERR   ,fERR   ,fERR   ,fERR   ,fERR   ,fERR   ,fERR   ,fERR   ,fERR   ,fERR   ,fERR   , // f
  };

  // 3 byte opcodes 0F3AXX
  static constexpr U8 Table3_3A[256] = {
    // 0       1       2       3       4       5       6       7       8       9       a       b       c       d       e       f
    fERR   ,fERR   ,fERR   ,fERR   ,fERR   ,fERR   ,fERR   ,fERR   ,fMR|fBI,fMR|fBI,fMR|fBI,fMR|fBI,fMR|fBI,fMR|fBI,fMR|fBI,fMR|fBI, // 0
    fERR   ,fERR   ,fERR   ,fERR   ,fMR|fBI,fMR|fBI,fMR|fBI,fMR|fBI,fERR   ,fERR   ,fERR   ,fERR   ,fERR   ,fERR   ,fERR   ,fERR   , // 1
    fMR|fBI,fMR|fBI,fMR|fBI,fERR   ,fERR   ,fERR   ,fERR   ,fERR   ,fERR   ,fERR   ,fERR   ,fERR   ,fERR   ,fERR   ,fERR   ,fERR   , // 2
    fERR   ,fERR   ,fERR   ,fERR   ,fERR   ,fERR   ,fERR   ,fERR   ,fERR   ,fERR   ,fERR   ,fERR   ,fERR   ,fERR   ,fERR   ,fERR   , // 3
    fMR|fBI,fMR|fBI,fMR|fBI,fERR   ,fERR   ,fERR   ,fERR   ,fERR   ,fERR   ,fERR   ,fERR   ,fERR   ,fERR   ,fERR   ,fERR   ,fERR   , // 4
    fERR   ,fERR   ,fERR   ,fERR   ,fERR   ,fERR   ,fERR   ,fERR   ,fERR   ,fERR   ,fERR   ,fERR   ,fERR   ,fERR   ,fERR   ,fERR   , // 5
    fMR|fBI,fMR|fBI,fMR|fBI,fMR|fBI,fERR   ,fERR   ,fERR   ,fERR   ,fERR   ,fERR   ,fERR   ,fERR   ,fERR   ,fERR   ,fERR   ,fERR   , // 6
    fERR   ,fERR   ,fERR   ,fERR   ,fERR   ,fERR   ,fERR   ,fERR   ,fERR   ,fERR   ,fERR   ,fERR   ,fERR   ,fERR   ,fERR   ,fERR   , // 7
    fERR   ,fERR   ,fERR   ,fERR   ,fERR   ,fERR   ,fERR   ,fERR   ,fERR   ,fERR   ,fERR   ,fERR   ,fERR   ,fERR   ,fERR   ,fERR   , // 8
    fERR   ,fERR   ,fERR   ,fERR   ,fERR   ,fERR   ,fERR   ,fERR   ,fERR   ,fERR   ,fERR   ,fERR   ,fERR   ,fERR   ,fERR   ,fERR   , // 9
    fERR   ,fERR   ,fERR   ,fERR   ,fERR   ,fERR   ,fERR   ,fERR   ,fERR   ,fERR   ,fERR   ,fERR   ,fERR   ,fERR   ,fERR   ,fERR   , // a
    fERR   ,fERR   ,fERR   ,fERR   ,fERR   ,fERR   ,fERR   ,fERR   ,fERR   ,fERR   ,fERR   ,fERR   ,fERR   ,fERR   ,fERR   ,fERR   , // b
    fERR   ,fERR   ,fERR   ,fERR   ,fERR   ,fERR   ,fERR   ,fERR   ,fERR   ,fERR   ,fERR   ,fERR   ,fERR   ,fERR   ,fERR   ,fERR   , // c
    fERR   ,fERR   ,fERR   ,fERR   ,fERR   ,fERR   ,fERR   ,fERR   ,fERR   ,fERR   ,fERR   ,fERR   ,fERR   ,fERR   ,fERR   ,fERR   , // d
    fERR   ,fERR   ,fERR   ,fERR   ,fERR   ,fERR   ,fERR   ,fERR   ,fERR   ,fERR   ,fERR   ,fERR   ,fERR   ,fERR   ,fERR   ,fERR   , // e
    fERR   ,fERR   ,fERR   ,fERR   ,fERR   ,fERR   ,fERR   ,fERR   ,fERR   ,fERR   ,fERR   ,fERR   ,fERR   ,fERR   ,fERR   ,fERR   , // f
  };

  // escape opcodes using ModRM byte to get more variants
  static constexpr U8 TableX[32] = {
    // 0       1       2       3       4       5       6       7
    fMR|fBI,fERR   ,fMR|fNI,fMR|fNI,fMR|fNI,fMR|fNI,fMR|fNI,fMR|fNI, // escapes for 0xf6
    fMR|fDI,fERR   ,fMR|fNI,fMR|fNI,fMR|fNI,fMR|fNI,fMR|fNI,fMR|fNI, // escapes for 0xf7
    fMR|fNI,fMR|fNI,fERR   ,fERR   ,fERR   ,fERR   ,fERR   ,fERR   , // escapes for 0xfe
    fMR|fNI,fMR|fNI,fMR|fNI,fERR   ,fMR|fNI,fERR   ,fMR|fNI,fERR   , // escapes for 0xff
  };

  static constexpr U8 InvalidX64Ops[19] = {0x06, 0x07, 0x16, 0x17, 0x1E, 0x1F, 0x27, 0x2F, 0x37, 0x3F, 0x60, 0x61, 0x62, 0x82, 0x9A, 0xD4, 0xD5, 0xD6, 0xEA,};
  static constexpr U8 X64Prefixes[8] = {0x26, 0x2E, 0x36, 0x3E, 0x9B, 0xF0, 0xF2, 0xF3,};

  enum InstructionCategory {
    OP_INVALID              =  0,
    OP_PREFIX_SEGREG        =  1,
    OP_PREFIX               =  2,
    OP_PREFIX_X87FPU        =  3,
    OP_GEN_DATAMOV          =  4,
    OP_GEN_STACK            =  5,
    OP_GEN_CONVERSION       =  6,
    OP_GEN_ARITH_DECIMAL    =  7,
    OP_GEN_ARITH_BINARY     =  8,
    OP_GEN_LOGICAL          =  9,
    OP_GEN_SHF_ROT          = 10,
    OP_GEN_BIT              = 11,
    OP_GEN_BRANCH           = 12,
    OP_GEN_BRANCH_COND      = 13,
    OP_GEN_BREAK            = 14,
    OP_GEN_STRING           = 15,
    OP_GEN_INOUT            = 16,
    OP_GEN_FLAG_CONTROL     = 17,
    OP_GEN_SEGREG           = 18,
    OP_GEN_CONTROL          = 19,
    OP_SYSTEM               = 20,
    OP_X87_DATAMOV          = 21,
    OP_X87_ARITH            = 22,
    OP_X87_COMPARISON       = 23,
    OP_X87_TRANSCENDENTAL   = 24,
    OP_X87_LOAD_CONSTANT    = 25,
    OP_X87_CONTROL          = 26,
    OP_X87_CONVERSION       = 27,
    OP_STATE_MANAGEMENT     = 28,
    OP_MMX                  = 29,
    OP_SSE                  = 30,
    OP_SSE_DATAMOV          = 31,
  };

  static constexpr U8 TypeOp1[256] = {
    OP_GEN_ARITH_BINARY  , OP_GEN_ARITH_BINARY  , OP_GEN_ARITH_BINARY  , OP_GEN_ARITH_BINARY  , //03
    OP_GEN_ARITH_BINARY  , OP_GEN_ARITH_BINARY  , OP_GEN_STACK         , OP_GEN_STACK         , //07
    OP_GEN_LOGICAL       , OP_GEN_LOGICAL       , OP_GEN_LOGICAL       , OP_GEN_LOGICAL       , //0B
    OP_GEN_LOGICAL       , OP_GEN_LOGICAL       , OP_GEN_STACK         , OP_PREFIX            , //0F
    OP_GEN_ARITH_BINARY  , OP_GEN_ARITH_BINARY  , OP_GEN_ARITH_BINARY  , OP_GEN_ARITH_BINARY  , //13
    OP_GEN_ARITH_BINARY  , OP_GEN_ARITH_BINARY  , OP_GEN_STACK         , OP_GEN_STACK         , //17
    OP_GEN_ARITH_BINARY  , OP_GEN_ARITH_BINARY  , OP_GEN_ARITH_BINARY  , OP_GEN_ARITH_BINARY  , //1B
    OP_GEN_ARITH_BINARY  , OP_GEN_ARITH_BINARY  , OP_GEN_STACK         , OP_GEN_STACK         , //1F
    OP_GEN_LOGICAL       , OP_GEN_LOGICAL       , OP_GEN_LOGICAL       , OP_GEN_LOGICAL       , //23
    OP_GEN_LOGICAL       , OP_GEN_LOGICAL       , OP_PREFIX_SEGREG     , OP_GEN_ARITH_DECIMAL , //27
    OP_GEN_ARITH_BINARY  , OP_GEN_ARITH_BINARY  , OP_GEN_ARITH_BINARY  , OP_GEN_ARITH_BINARY  , //2B
    OP_GEN_ARITH_BINARY  , OP_GEN_ARITH_BINARY  , OP_PREFIX_SEGREG     , OP_GEN_ARITH_DECIMAL , //2F
    OP_GEN_LOGICAL       , OP_GEN_LOGICAL       , OP_GEN_LOGICAL       , OP_GEN_LOGICAL       , //33
    OP_GEN_LOGICAL       , OP_GEN_LOGICAL       , OP_PREFIX_SEGREG     , OP_GEN_ARITH_DECIMAL , //37
    OP_GEN_ARITH_BINARY  , OP_GEN_ARITH_BINARY  , OP_GEN_ARITH_BINARY  , OP_GEN_ARITH_BINARY  , //3B
    OP_GEN_ARITH_BINARY  , OP_GEN_ARITH_BINARY  , OP_PREFIX_SEGREG     , OP_GEN_ARITH_DECIMAL , //3F
    OP_GEN_ARITH_BINARY  , OP_GEN_ARITH_BINARY  , OP_GEN_ARITH_BINARY  , OP_GEN_ARITH_BINARY  , //43
    OP_GEN_ARITH_BINARY  , OP_GEN_ARITH_BINARY  , OP_GEN_ARITH_BINARY  , OP_GEN_ARITH_BINARY  , //47
    OP_GEN_ARITH_BINARY  , OP_GEN_ARITH_BINARY  , OP_GEN_ARITH_BINARY  , OP_GEN_ARITH_BINARY  , //4B
    OP_GEN_ARITH_BINARY  , OP_GEN_ARITH_BINARY  , OP_GEN_ARITH_BINARY  , OP_GEN_ARITH_BINARY  , //4F
    OP_GEN_STACK         , OP_GEN_STACK         , OP_GEN_STACK         , OP_GEN_STACK         , //53
    OP_GEN_STACK         , OP_GEN_STACK         , OP_GEN_STACK         , OP_GEN_STACK         , //57
    OP_GEN_STACK         , OP_GEN_STACK         , OP_GEN_STACK         , OP_GEN_STACK         , //5B
    OP_GEN_STACK         , OP_GEN_STACK         , OP_GEN_STACK         , OP_GEN_STACK         , //5F
    OP_GEN_STACK         , OP_GEN_STACK         , OP_GEN_BREAK         , OP_GEN_CONVERSION    , //63
    OP_PREFIX_SEGREG     , OP_PREFIX_SEGREG     , OP_PREFIX            , OP_PREFIX            , //67
    OP_GEN_STACK         , OP_GEN_ARITH_BINARY  , OP_GEN_STACK         , OP_GEN_ARITH_BINARY  , //6B
    OP_GEN_INOUT         , OP_GEN_INOUT         , OP_GEN_INOUT         , OP_GEN_INOUT         , //6F
    OP_GEN_BRANCH_COND   , OP_GEN_BRANCH_COND   , OP_GEN_BRANCH_COND   , OP_GEN_BRANCH_COND   , //73
    OP_GEN_BRANCH_COND   , OP_GEN_BRANCH_COND   , OP_GEN_BRANCH_COND   , OP_GEN_BRANCH_COND   , //77
    OP_GEN_BRANCH_COND   , OP_GEN_BRANCH_COND   , OP_GEN_BRANCH_COND   , OP_GEN_BRANCH_COND   , //7B
    OP_GEN_BRANCH_COND   , OP_GEN_BRANCH_COND   , OP_GEN_BRANCH_COND   , OP_GEN_BRANCH_COND   , //7F
    OP_GEN_ARITH_BINARY  , OP_GEN_ARITH_BINARY  , OP_GEN_ARITH_BINARY  , OP_GEN_ARITH_BINARY  , //83
    OP_GEN_ARITH_BINARY  , OP_GEN_ARITH_BINARY  , OP_GEN_DATAMOV       , OP_GEN_DATAMOV       , //87
    OP_GEN_DATAMOV       , OP_GEN_DATAMOV       , OP_GEN_DATAMOV       , OP_GEN_DATAMOV       , //8B
    OP_GEN_DATAMOV       , OP_GEN_DATAMOV       , OP_GEN_DATAMOV       , OP_GEN_STACK         , //8F
    OP_GEN_DATAMOV       , OP_GEN_DATAMOV       , OP_GEN_DATAMOV       , OP_GEN_DATAMOV       , //93
    OP_GEN_DATAMOV       , OP_GEN_DATAMOV       , OP_GEN_DATAMOV       , OP_GEN_DATAMOV       , //97
    OP_GEN_CONVERSION    , OP_GEN_CONVERSION    , OP_GEN_BRANCH        , OP_PREFIX_X87FPU     , //9B
    OP_GEN_STACK         , OP_GEN_STACK         , OP_GEN_DATAMOV       , OP_GEN_DATAMOV       , //9F
    OP_GEN_DATAMOV       , OP_GEN_DATAMOV       , OP_GEN_DATAMOV       , OP_GEN_DATAMOV       , //A3
    OP_GEN_STRING        , OP_GEN_STRING        , OP_GEN_STRING        , OP_GEN_STRING        , //A7
    OP_GEN_LOGICAL       , OP_GEN_LOGICAL       , OP_GEN_STRING        , OP_GEN_STRING        , //AB
    OP_GEN_STRING        , OP_GEN_STRING        , OP_GEN_STRING        , OP_GEN_STRING        , //AF
    OP_GEN_DATAMOV       , OP_GEN_DATAMOV       , OP_GEN_DATAMOV       , OP_GEN_DATAMOV       , //B3
    OP_GEN_DATAMOV       , OP_GEN_DATAMOV       , OP_GEN_DATAMOV       , OP_GEN_DATAMOV       , //B7
    OP_GEN_DATAMOV       , OP_GEN_DATAMOV       , OP_GEN_DATAMOV       , OP_GEN_DATAMOV       , //BB
    OP_GEN_DATAMOV       , OP_GEN_DATAMOV       , OP_GEN_DATAMOV       , OP_GEN_DATAMOV       , //BF
    OP_GEN_SHF_ROT       , OP_GEN_SHF_ROT       , OP_GEN_BRANCH        , OP_GEN_BRANCH        , //C3
    OP_GEN_DATAMOV       , OP_GEN_DATAMOV       , OP_GEN_DATAMOV       , OP_GEN_DATAMOV       , //C7
    OP_GEN_STACK         , OP_GEN_STACK         , OP_GEN_BRANCH        , OP_GEN_BRANCH        , //CB
    OP_GEN_BREAK         , OP_GEN_BREAK         , OP_GEN_BREAK         , OP_GEN_BREAK         , //CF
    OP_GEN_SHF_ROT       , OP_GEN_SHF_ROT       , OP_GEN_SHF_ROT       , OP_GEN_SHF_ROT       , //D3
    OP_GEN_ARITH_DECIMAL , OP_GEN_ARITH_DECIMAL , OP_GEN_DATAMOV       , OP_GEN_DATAMOV       , //D7
    OP_X87_ARITH         , OP_X87_DATAMOV       , OP_X87_ARITH         , OP_X87_DATAMOV       , //DB
    OP_X87_ARITH         , OP_X87_DATAMOV       , OP_X87_ARITH         , OP_X87_DATAMOV       , //DF
    OP_GEN_BRANCH_COND   , OP_GEN_BRANCH_COND   , OP_GEN_BRANCH_COND   , OP_GEN_BRANCH_COND   , //E3
    OP_GEN_INOUT         , OP_GEN_INOUT         , OP_GEN_INOUT         , OP_GEN_INOUT         , //E7
    OP_GEN_BRANCH        , OP_GEN_BRANCH        , OP_GEN_BRANCH        , OP_GEN_BRANCH        , //EB
    OP_GEN_INOUT         , OP_GEN_INOUT         , OP_GEN_INOUT         , OP_GEN_INOUT         , //EF
    OP_PREFIX            , OP_GEN_BREAK         , OP_PREFIX            , OP_PREFIX            , //F3
    OP_SYSTEM            , OP_GEN_FLAG_CONTROL  , OP_GEN_ARITH_BINARY  , OP_GEN_ARITH_BINARY  , //F7
    OP_GEN_FLAG_CONTROL  , OP_GEN_FLAG_CONTROL  , OP_GEN_FLAG_CONTROL  , OP_GEN_FLAG_CONTROL  , //FB
    OP_GEN_FLAG_CONTROL  , OP_GEN_FLAG_CONTROL  , OP_GEN_ARITH_BINARY  , OP_GEN_BRANCH        , //FF
  };

  static constexpr U8 TypeOp2[256] = {
    OP_SYSTEM            , OP_SYSTEM            , OP_SYSTEM            , OP_SYSTEM            , //03
    OP_INVALID           , OP_SYSTEM            , OP_SYSTEM            , OP_SYSTEM            , //07
    OP_SYSTEM            , OP_SYSTEM            , OP_INVALID           , OP_GEN_CONTROL       , //0B
    OP_INVALID           , OP_GEN_CONTROL       , OP_INVALID           , OP_INVALID           , //0F
    OP_SSE_DATAMOV       , OP_SSE_DATAMOV       , OP_SSE_DATAMOV       , OP_SSE_DATAMOV       , //13
    OP_SSE               , OP_SSE               , OP_SSE_DATAMOV       , OP_SSE_DATAMOV       , //17
    OP_SSE               , OP_GEN_CONTROL       , OP_GEN_CONTROL       , OP_GEN_CONTROL       , //1B
    OP_GEN_CONTROL       , OP_GEN_CONTROL       , OP_GEN_CONTROL       , OP_GEN_CONTROL       , //1F
    OP_SYSTEM            , OP_SYSTEM            , OP_SYSTEM            , OP_SYSTEM            , //23
    OP_SYSTEM            , OP_INVALID           , OP_SYSTEM            , OP_INVALID           , //27
    OP_SSE_DATAMOV       , OP_SSE_DATAMOV       , OP_SSE               , OP_SSE               , //2B
    OP_SSE               , OP_SSE               , OP_SSE               , OP_SSE               , //2F
    OP_SYSTEM            , OP_SYSTEM            , OP_SYSTEM            , OP_SYSTEM            , //33
    OP_SYSTEM            , OP_SYSTEM            , OP_INVALID           , OP_INVALID           , //37
    OP_PREFIX            , OP_INVALID           , OP_PREFIX            , OP_INVALID           , //3B
    OP_INVALID           , OP_INVALID           , OP_INVALID           , OP_INVALID           , //3F
    OP_GEN_DATAMOV       , OP_GEN_DATAMOV       , OP_GEN_DATAMOV       , OP_GEN_DATAMOV       , //43
    OP_GEN_DATAMOV       , OP_GEN_DATAMOV       , OP_GEN_DATAMOV       , OP_GEN_DATAMOV       , //47
    OP_GEN_DATAMOV       , OP_GEN_DATAMOV       , OP_GEN_DATAMOV       , OP_GEN_DATAMOV       , //4B
    OP_GEN_DATAMOV       , OP_GEN_DATAMOV       , OP_GEN_DATAMOV       , OP_GEN_DATAMOV       , //4F
    OP_SSE_DATAMOV       , OP_SSE               , OP_SSE               , OP_SSE               , //53
    OP_SSE               , OP_SSE               , OP_SSE               , OP_SSE               , //57
    OP_SSE               , OP_SSE               , OP_SSE               , OP_SSE               , //5B
    OP_SSE               , OP_SSE               , OP_SSE               , OP_SSE               , //5F
    OP_MMX               , OP_MMX               , OP_MMX               , OP_MMX               , //63
    OP_MMX               , OP_MMX               , OP_MMX               , OP_MMX               , //67
    OP_MMX               , OP_MMX               , OP_MMX               , OP_MMX               , //6B
    OP_INVALID           , OP_INVALID           , OP_MMX               , OP_MMX               , //6F
    OP_SSE               , OP_MMX               , OP_MMX               , OP_MMX               , //73
    OP_MMX               , OP_MMX               , OP_MMX               , OP_MMX               , //77
    OP_INVALID           , OP_INVALID           , OP_INVALID           , OP_INVALID           , //7B
    OP_INVALID           , OP_INVALID           , OP_MMX               , OP_MMX               , //7F
    OP_GEN_BRANCH_COND   , OP_GEN_BRANCH_COND   , OP_GEN_BRANCH_COND   , OP_GEN_BRANCH_COND   , //83
    OP_GEN_BRANCH_COND   , OP_GEN_BRANCH_COND   , OP_GEN_BRANCH_COND   , OP_GEN_BRANCH_COND   , //87
    OP_GEN_BRANCH_COND   , OP_GEN_BRANCH_COND   , OP_GEN_BRANCH_COND   , OP_GEN_BRANCH_COND   , //8B
    OP_GEN_BRANCH_COND   , OP_GEN_BRANCH_COND   , OP_GEN_BRANCH_COND   , OP_GEN_BRANCH_COND   , //8F
    OP_GEN_DATAMOV       , OP_GEN_DATAMOV       , OP_GEN_DATAMOV       , OP_GEN_DATAMOV       , //93
    OP_GEN_DATAMOV       , OP_GEN_DATAMOV       , OP_GEN_DATAMOV       , OP_GEN_DATAMOV       , //97
    OP_GEN_DATAMOV       , OP_GEN_DATAMOV       , OP_GEN_DATAMOV       , OP_GEN_DATAMOV       , //9B
    OP_GEN_DATAMOV       , OP_GEN_DATAMOV       , OP_GEN_DATAMOV       , OP_GEN_DATAMOV       , //9F
    OP_GEN_STACK         , OP_GEN_STACK         , OP_GEN_CONTROL       , OP_GEN_BIT           , //A3
    OP_GEN_SHF_ROT       , OP_GEN_SHF_ROT       , OP_INVALID           , OP_INVALID           , //A7
    OP_GEN_STACK         , OP_GEN_STACK         , OP_SYSTEM            , OP_GEN_BIT           , //AB
    OP_GEN_SHF_ROT       , OP_GEN_SHF_ROT       , OP_STATE_MANAGEMENT  , OP_GEN_ARITH_BINARY  , //AF
    OP_GEN_DATAMOV       , OP_GEN_DATAMOV       , OP_GEN_DATAMOV       , OP_GEN_BIT           , //B3
    OP_GEN_DATAMOV       , OP_GEN_DATAMOV       , OP_GEN_CONVERSION    , OP_GEN_CONVERSION    , //B7
    OP_INVALID           , OP_GEN_CONTROL       , OP_GEN_BIT           , OP_GEN_BIT           , //BB
    OP_GEN_BIT           , OP_GEN_BIT           , OP_GEN_CONVERSION    , OP_GEN_CONVERSION    , //BF
    OP_GEN_DATAMOV       , OP_GEN_DATAMOV       , OP_SSE               , OP_SSE               , //C3
    OP_SSE               , OP_SSE               , OP_SSE               , OP_GEN_DATAMOV       , //C7
    OP_GEN_DATAMOV       , OP_GEN_DATAMOV       , OP_GEN_DATAMOV       , OP_GEN_DATAMOV       , //CB
    OP_GEN_DATAMOV       , OP_GEN_DATAMOV       , OP_GEN_DATAMOV       , OP_GEN_DATAMOV       , //CF
    OP_INVALID           , OP_MMX               , OP_MMX               , OP_MMX               , //D3
    OP_SSE               , OP_MMX               , OP_INVALID           , OP_SSE               , //D7
    OP_MMX               , OP_MMX               , OP_SSE               , OP_MMX               , //DB
    OP_MMX               , OP_MMX               , OP_SSE               , OP_MMX               , //DF
    OP_SSE               , OP_MMX               , OP_SSE               , OP_MMX               , //E3
    OP_SSE               , OP_MMX               , OP_INVALID           , OP_SSE               , //E7
    OP_MMX               , OP_MMX               , OP_SSE               , OP_MMX               , //EB
    OP_MMX               , OP_MMX               , OP_SSE               , OP_MMX               , //EF
    OP_INVALID           , OP_MMX               , OP_MMX               , OP_MMX               , //F3
    OP_SSE               , OP_MMX               , OP_SSE               , OP_SSE               , //F7
    OP_MMX               , OP_MMX               , OP_MMX               , OP_SSE               , //FB
    OP_MMX               , OP_MMX               , OP_MMX               , OP_INVALID           , //FF
  };

  static constexpr U8 TypeOp3_38[256] = {
    OP_SSE               , OP_SSE               , OP_SSE               , OP_SSE               , //03
    OP_SSE               , OP_SSE               , OP_SSE               , OP_SSE               , //07
    OP_SSE               , OP_SSE               , OP_SSE               , OP_SSE               , //0B
    OP_INVALID           , OP_INVALID           , OP_INVALID           , OP_INVALID           , //0F
    OP_SSE               , OP_INVALID           , OP_INVALID           , OP_INVALID           , //13
    OP_SSE               , OP_SSE               , OP_INVALID           , OP_SSE               , //17
    OP_INVALID           , OP_INVALID           , OP_INVALID           , OP_INVALID           , //1B
    OP_SSE               , OP_SSE               , OP_SSE               , OP_INVALID           , //1F
    OP_SSE               , OP_SSE               , OP_SSE               , OP_SSE               , //23
    OP_SSE               , OP_SSE               , OP_INVALID           , OP_INVALID           , //27
    OP_SSE               , OP_SSE               , OP_SSE               , OP_SSE               , //2B
    OP_INVALID           , OP_INVALID           , OP_INVALID           , OP_INVALID           , //2F
    OP_SSE               , OP_SSE               , OP_SSE               , OP_SSE               , //33
    OP_SSE               , OP_SSE               , OP_INVALID           , OP_SSE               , //37
    OP_SSE               , OP_SSE               , OP_SSE               , OP_SSE               , //3B
    OP_SSE               , OP_SSE               , OP_SSE               , OP_SSE               , //3F
    OP_SSE               , OP_SSE               , OP_INVALID           , OP_INVALID           , //43
    OP_INVALID           , OP_INVALID           , OP_INVALID           , OP_INVALID           , //47
    OP_INVALID           , OP_INVALID           , OP_INVALID           , OP_INVALID           , //4B
    OP_INVALID           , OP_INVALID           , OP_INVALID           , OP_INVALID           , //4F
    OP_INVALID           , OP_INVALID           , OP_INVALID           , OP_INVALID           , //53
    OP_INVALID           , OP_INVALID           , OP_INVALID           , OP_INVALID           , //57
    OP_INVALID           , OP_INVALID           , OP_INVALID           , OP_INVALID           , //5B
    OP_INVALID           , OP_INVALID           , OP_INVALID           , OP_INVALID           , //5F
    OP_INVALID           , OP_INVALID           , OP_INVALID           , OP_INVALID           , //63
    OP_INVALID           , OP_INVALID           , OP_INVALID           , OP_INVALID           , //67
    OP_INVALID           , OP_INVALID           , OP_INVALID           , OP_INVALID           , //6B
    OP_INVALID           , OP_INVALID           , OP_INVALID           , OP_INVALID           , //6F
    OP_INVALID           , OP_INVALID           , OP_INVALID           , OP_INVALID           , //73
    OP_INVALID           , OP_INVALID           , OP_INVALID           , OP_INVALID           , //77
    OP_INVALID           , OP_INVALID           , OP_INVALID           , OP_INVALID           , //7B
    OP_INVALID           , OP_INVALID           , OP_INVALID           , OP_INVALID           , //7F
    OP_INVALID           , OP_INVALID           , OP_INVALID           , OP_INVALID           , //83
    OP_INVALID           , OP_INVALID           , OP_INVALID           , OP_INVALID           , //87
    OP_INVALID           , OP_INVALID           , OP_INVALID           , OP_INVALID           , //8B
    OP_INVALID           , OP_INVALID           , OP_INVALID           , OP_INVALID           , //8F
    OP_INVALID           , OP_INVALID           , OP_INVALID           , OP_INVALID           , //93
    OP_INVALID           , OP_INVALID           , OP_INVALID           , OP_INVALID           , //97
    OP_INVALID           , OP_INVALID           , OP_INVALID           , OP_INVALID           , //9B
    OP_INVALID           , OP_INVALID           , OP_INVALID           , OP_INVALID           , //9F
    OP_INVALID           , OP_INVALID           , OP_INVALID           , OP_INVALID           , //A3
    OP_INVALID           , OP_INVALID           , OP_INVALID           , OP_INVALID           , //A7
    OP_INVALID           , OP_INVALID           , OP_INVALID           , OP_INVALID           , //AB
    OP_INVALID           , OP_INVALID           , OP_INVALID           , OP_INVALID           , //AF
    OP_INVALID           , OP_INVALID           , OP_INVALID           , OP_INVALID           , //B3
    OP_INVALID           , OP_INVALID           , OP_INVALID           , OP_INVALID           , //B7
    OP_INVALID           , OP_INVALID           , OP_INVALID           , OP_INVALID           , //BB
    OP_INVALID           , OP_INVALID           , OP_INVALID           , OP_INVALID           , //BF
    OP_INVALID           , OP_INVALID           , OP_INVALID           , OP_INVALID           , //C3
    OP_INVALID           , OP_INVALID           , OP_INVALID           , OP_INVALID           , //C7
    OP_INVALID           , OP_INVALID           , OP_INVALID           , OP_INVALID           , //CB
    OP_INVALID           , OP_INVALID           , OP_INVALID           , OP_INVALID           , //CF
    OP_INVALID           , OP_INVALID           , OP_INVALID           , OP_INVALID           , //D3
    OP_INVALID           , OP_INVALID           , OP_INVALID           , OP_INVALID           , //D7
    OP_INVALID           , OP_INVALID           , OP_INVALID           , OP_INVALID           , //DB
    OP_INVALID           , OP_INVALID           , OP_INVALID           , OP_INVALID           , //DF
    OP_INVALID           , OP_INVALID           , OP_INVALID           , OP_INVALID           , //E3
    OP_INVALID           , OP_INVALID           , OP_INVALID           , OP_INVALID           , //E7
    OP_INVALID           , OP_INVALID           , OP_INVALID           , OP_INVALID           , //EB
    OP_INVALID           , OP_INVALID           , OP_INVALID           , OP_INVALID           , //EF
    OP_GEN_DATAMOV       , OP_GEN_DATAMOV       , OP_INVALID           , OP_INVALID           , //F3
    OP_INVALID           , OP_INVALID           , OP_INVALID           , OP_INVALID           , //F7
    OP_INVALID           , OP_INVALID           , OP_INVALID           , OP_INVALID           , //FB
    OP_INVALID           , OP_INVALID           , OP_INVALID           , OP_INVALID           , //FF
  };

  static constexpr U8 TypeOp3_3A[256] = {
    OP_INVALID           , OP_INVALID           , OP_INVALID           , OP_INVALID           , //03
    OP_INVALID           , OP_INVALID           , OP_INVALID           , OP_INVALID           , //07
    OP_SSE               , OP_SSE               , OP_SSE               , OP_SSE               , //0B
    OP_SSE               , OP_SSE               , OP_SSE               , OP_SSE               , //0F
    OP_INVALID           , OP_INVALID           , OP_INVALID           , OP_INVALID           , //13
    OP_SSE               , OP_SSE               , OP_SSE               , OP_SSE               , //17
    OP_INVALID           , OP_INVALID           , OP_INVALID           , OP_INVALID           , //1B
    OP_INVALID           , OP_INVALID           , OP_INVALID           , OP_INVALID           , //1F
    OP_SSE               , OP_SSE               , OP_SSE               , OP_INVALID           , //23
    OP_INVALID           , OP_INVALID           , OP_INVALID           , OP_INVALID           , //27
    OP_INVALID           , OP_INVALID           , OP_INVALID           , OP_INVALID           , //2B
    OP_INVALID           , OP_INVALID           , OP_INVALID           , OP_INVALID           , //2F
    OP_INVALID           , OP_INVALID           , OP_INVALID           , OP_INVALID           , //33
    OP_INVALID           , OP_INVALID           , OP_INVALID           , OP_INVALID           , //37
    OP_INVALID           , OP_INVALID           , OP_INVALID           , OP_INVALID           , //3B
    OP_INVALID           , OP_INVALID           , OP_INVALID           , OP_INVALID           , //3F
    OP_SSE               , OP_SSE               , OP_SSE               , OP_INVALID           , //43
    OP_INVALID           , OP_INVALID           , OP_INVALID           , OP_INVALID           , //47
    OP_INVALID           , OP_INVALID           , OP_INVALID           , OP_INVALID           , //4B
    OP_INVALID           , OP_INVALID           , OP_INVALID           , OP_INVALID           , //4F
    OP_INVALID           , OP_INVALID           , OP_INVALID           , OP_INVALID           , //53
    OP_INVALID           , OP_INVALID           , OP_INVALID           , OP_INVALID           , //57
    OP_INVALID           , OP_INVALID           , OP_INVALID           , OP_INVALID           , //5B
    OP_INVALID           , OP_INVALID           , OP_INVALID           , OP_INVALID           , //5F
    OP_SSE               , OP_SSE               , OP_SSE               , OP_SSE               , //63
    OP_INVALID           , OP_INVALID           , OP_INVALID           , OP_INVALID           , //67
    OP_INVALID           , OP_INVALID           , OP_INVALID           , OP_INVALID           , //6B
    OP_INVALID           , OP_INVALID           , OP_INVALID           , OP_INVALID           , //6F
    OP_INVALID           , OP_INVALID           , OP_INVALID           , OP_INVALID           , //73
    OP_INVALID           , OP_INVALID           , OP_INVALID           , OP_INVALID           , //77
    OP_INVALID           , OP_INVALID           , OP_INVALID           , OP_INVALID           , //7B
    OP_INVALID           , OP_INVALID           , OP_INVALID           , OP_INVALID           , //7F
    OP_INVALID           , OP_INVALID           , OP_INVALID           , OP_INVALID           , //83
    OP_INVALID           , OP_INVALID           , OP_INVALID           , OP_INVALID           , //87
    OP_INVALID           , OP_INVALID           , OP_INVALID           , OP_INVALID           , //8B
    OP_INVALID           , OP_INVALID           , OP_INVALID           , OP_INVALID           , //8F
    OP_INVALID           , OP_INVALID           , OP_INVALID           , OP_INVALID           , //93
    OP_INVALID           , OP_INVALID           , OP_INVALID           , OP_INVALID           , //97
    OP_INVALID           , OP_INVALID           , OP_INVALID           , OP_INVALID           , //9B
    OP_INVALID           , OP_INVALID           , OP_INVALID           , OP_INVALID           , //9F
    OP_INVALID           , OP_INVALID           , OP_INVALID           , OP_INVALID           , //A3
    OP_INVALID           , OP_INVALID           , OP_INVALID           , OP_INVALID           , //A7
    OP_INVALID           , OP_INVALID           , OP_INVALID           , OP_INVALID           , //AB
    OP_INVALID           , OP_INVALID           , OP_INVALID           , OP_INVALID           , //AF
    OP_INVALID           , OP_INVALID           , OP_INVALID           , OP_INVALID           , //B3
    OP_INVALID           , OP_INVALID           , OP_INVALID           , OP_INVALID           , //B7
    OP_INVALID           , OP_INVALID           , OP_INVALID           , OP_INVALID           , //BB
    OP_INVALID           , OP_INVALID           , OP_INVALID           , OP_INVALID           , //BF
    OP_INVALID           , OP_INVALID           , OP_INVALID           , OP_INVALID           , //C3
    OP_INVALID           , OP_INVALID           , OP_INVALID           , OP_INVALID           , //C7
    OP_INVALID           , OP_INVALID           , OP_INVALID           , OP_INVALID           , //CB
    OP_INVALID           , OP_INVALID           , OP_INVALID           , OP_INVALID           , //CF
    OP_INVALID           , OP_INVALID           , OP_INVALID           , OP_INVALID           , //D3
    OP_INVALID           , OP_INVALID           , OP_INVALID           , OP_INVALID           , //D7
    OP_INVALID           , OP_INVALID           , OP_INVALID           , OP_INVALID           , //DB
    OP_INVALID           , OP_INVALID           , OP_INVALID           , OP_INVALID           , //DF
    OP_INVALID           , OP_INVALID           , OP_INVALID           , OP_INVALID           , //E3
    OP_INVALID           , OP_INVALID           , OP_INVALID           , OP_INVALID           , //E7
    OP_INVALID           , OP_INVALID           , OP_INVALID           , OP_INVALID           , //EB
    OP_INVALID           , OP_INVALID           , OP_INVALID           , OP_INVALID           , //EF
    OP_INVALID           , OP_INVALID           , OP_INVALID           , OP_INVALID           , //F3
    OP_INVALID           , OP_INVALID           , OP_INVALID           , OP_INVALID           , //F7
    OP_INVALID           , OP_INVALID           , OP_INVALID           , OP_INVALID           , //FB
    OP_INVALID           , OP_INVALID           , OP_INVALID           , OP_INVALID           , //FF
  };

  static constexpr U8 TypeOpX[32] = {
    // escapes for F6
    OP_GEN_LOGICAL       , OP_GEN_LOGICAL       , OP_GEN_LOGICAL       , OP_GEN_ARITH_BINARY  ,
    OP_GEN_ARITH_BINARY  , OP_GEN_ARITH_BINARY  , OP_GEN_ARITH_BINARY  , OP_GEN_ARITH_BINARY  ,
    // escapes for F7
    OP_GEN_LOGICAL       , OP_GEN_LOGICAL       , OP_GEN_LOGICAL       , OP_GEN_ARITH_BINARY  ,
    OP_GEN_ARITH_BINARY  , OP_GEN_ARITH_BINARY  , OP_GEN_ARITH_BINARY  , OP_GEN_ARITH_BINARY  ,
    // escapes for FE
    OP_GEN_ARITH_BINARY  , OP_GEN_ARITH_BINARY  , OP_INVALID           , OP_INVALID           ,
    OP_INVALID           , OP_INVALID           , OP_INVALID           , OP_INVALID           ,
    // escapes for FF
    OP_GEN_ARITH_BINARY  , OP_GEN_ARITH_BINARY  , OP_GEN_BRANCH        , OP_GEN_BRANCH        ,
    OP_GEN_BRANCH        , OP_GEN_BRANCH        , OP_GEN_STACK         , OP_INVALID           ,
  };

  enum Prefixes {
    ES_OVERRIDE = 0x26,
    CS_OVERRIDE = 0x2E,
    SS_OVERRIDE = 0x36,
    DS_OVERRIDE = 0x3E,
    FS_OVERRIDE = 0x64,
    GS_OVERRIDE = 0x65,
    AD_OVERRIDE = 0x67,
    WAIT_FPU    = 0x9B,
    LOCK        = 0xF0,
    REP_N_STR   = 0xF2,
    REP_STR     = 0xF3,
  };

  enum Opcodes {
    // 1-byte opcodes of special interest (for one reason or another)
    OP_2BYTE  = 0x0f,     // start of 2-byte opcode
    OP_OSIZE  = 0x66,     // operand size prefix
    OP_CALLF  = 0x9a,
    OP_RETNI  = 0xc2,     // ret near+immediate
    OP_RETN   = 0xc3,
    OP_ENTER  = 0xc8,
    OP_INT3   = 0xcc,
    OP_INTO   = 0xce,
    OP_CALLN  = 0xe8,
    OP_JMPF   = 0xea,
    OP_ICEBP  = 0xf1,
  };

  enum ExeState {
    Start             =  0,
    Pref_Op_Size      =  1,
    Pref_MultiByte_Op =  2,
    ParseFlags        =  3,
    ExtraFlags        =  4,
    ReadModRM         =  5,
    Read_OP3_38       =  6,
    Read_OP3_3A       =  7,
    ReadSIB           =  8,
    Read8             =  9,
    Read16            = 10,
    Read32            = 11,
    Read8_ModRM       = 12,
    Read16_f          = 13,
    Read32_ModRM      = 14,
    Error             = 15,
  };

  struct OpCache {
    U32 Op[CacheSize];
    U32 Index;
  };

  struct Instruction {
    U32 Data;
    U8 Prefix, Code, ModRM, SIB, REX, Flags, BytesRead, Size, Category;
    bool MustCheckREX, Decoding, o16, imm8;
  };

  static constexpr int CodeShift           = 3;
  static constexpr U32 CodeMask            = 0xFF<<CodeShift;       // 0x000007F8
  static constexpr U32 ClearCodeMask       = 0xFFFFFFFF^CodeMask;   // 0xFFFFF807
  static constexpr U32 PrefixMask          = (1<<CodeShift)-1;      // 0x07
  static constexpr U32 OperandSizeOverride = 0x01<<(8+CodeShift);   // 0x00000800
  static constexpr U32 MultiByteOpcode     = 0x02<<(8+CodeShift);   // 0x00001000
  static constexpr U32 PrefixREX           = 0x04<<(8+CodeShift);   // 0x00002000
  static constexpr U32 Prefix38            = 0x08<<(8+CodeShift);   // 0x00004000
  static constexpr U32  Prefix3A           = 0x10<<(8+CodeShift);   // 0x00008000
  static constexpr U32 HasExtraFlags       = 0x20<<(8+CodeShift);   // 0x00010000
  static constexpr U32 HasModRM            = 0x40<<(8+CodeShift);   // 0x00020000
  static constexpr U32 ModRMShift          = 7+8+CodeShift;         // 18
  static constexpr U32 SIBScaleShift       = ModRMShift+8-6;        // 20
  static constexpr U32 RegDWordDisplacement= 1<<(8+SIBScaleShift);  // 0x10000000
  static constexpr U32 AddressMode         = 2<<(8+SIBScaleShift);  // 0x20000000
  static constexpr U32 TypeShift           = 2+8+SIBScaleShift;     // 30
  static constexpr U32 CategoryShift       = 5;
  static constexpr U32 CategoryMask        = ((1<<CategoryShift)-1); //0x1F (31)
  static constexpr U8  ModRM_mod           = 0xC0;
  static constexpr U8  ModRM_reg           = 0x38;
  static constexpr U8  ModRM_rm            = 0x07;
  static constexpr U8  SIB_scale           = 0xC0;
  static constexpr U8  SIB_index           = 0x38; //unused
  static constexpr U8  SIB_base            = 0x07;
  static constexpr U8  REX_w               = 0x08;

  static constexpr U32 MinRequired         = 8; // minimum required consecutive valid instructions to be considered as code
private:
  static constexpr int nCM1=10, nCM2=10, nIM=1;
public:
  static constexpr int MIXERINPUTS = (nCM1+nCM2)*ContextMap2::MIXERINPUTS + nIM*IndirectMap::MIXERINPUTS; // 142
  static constexpr int MIXERCONTEXTS = 1024+1024+1024+8192+8192+8192; // 27648
  static constexpr int MIXERCONTEXTSETS = 6;
private:
  const Shared * const shared;
  const ModelStats * const stats;
  ContextMap2 cm;
  IndirectMap iMap;
  OpCache Cache;
  U32 StateBH[256];
  ExeState pState, State;
  Instruction Op;
  U32 TotalOps, OpMask, OpCategMask, Context;
  U64 BrkCtx; // hash
  bool Valid;
  inline bool IsInvalidX64Op(const U8 Op) {
    for (int i=0; i<19; i++) {
      if (Op == InvalidX64Ops[i])
        return true;
    }
    return false;
  }
  inline bool IsValidX64Prefix(const U8 Prefix) {
    for (int i=0; i<8; i++) {
      if (Prefix == X64Prefixes[i])
        return true;
    }
    return ((Prefix>=0x40 && Prefix<=0x4F) || (Prefix>=0x64 && Prefix<=0x67));
  }
  void ProcessMode(Instruction &Op, ExeState &State) {
    if ((Op.Flags&fMODE)==fAM) {
      Op.Data|=AddressMode;
      Op.BytesRead = 0;
      switch (Op.Flags&fTYPE) {
        case fDR : Op.Data|=(2<<TypeShift);
        case fDA : Op.Data|=(1<<TypeShift);
        case fAD : {
          State = Read32;
          break;
        }
        case fBR : {
          Op.Data|=(2<<TypeShift);
          State = Read8;
        }
      }
    }
    else{
      switch (Op.Flags&fTYPE) {
        case fBI : State = Read8; break;
        case fWI : {
          State = Read16;
          Op.Data|=(1<<TypeShift);
          Op.BytesRead = 0;
          break;
        }
        case fDI : {
          // x64 Move with 8byte immediate? [REX.W is set, opcodes 0xB8+r]
          Op.imm8=((Op.REX & REX_w)>0 && (Op.Code&0xF8)==0xB8);
          if (!Op.o16 || Op.imm8){
            State = Read32;
            Op.Data|=(2<<TypeShift);
          }
          else{
            State = Read16;
            Op.Data|=(3<<TypeShift);
          }
          Op.BytesRead = 0;
          break;
        }
        default: State = Start; /*no immediate*/
      }
    }
  }
  void ProcessFlags2(Instruction &Op, ExeState &State) {
    //if arriving from state ExtraFlags, we've already read the ModRM byte
    if ((Op.Flags&fMODE)==fMR && State!=ExtraFlags) {
      State = ReadModRM;
      return;
    }
    ProcessMode(Op, State);
  }
  void ProcessFlags(Instruction &Op, ExeState &State) {
    if (Op.Code==OP_CALLF || Op.Code==OP_JMPF || Op.Code==OP_ENTER) {
      Op.BytesRead = 0;
      State = Read16_f;
      return; //must exit, ENTER has ModRM too
    }
    ProcessFlags2(Op, State);
  }
  void CheckFlags(Instruction &Op, ExeState &State) {
    //must peek at ModRM byte to read the REG part, so we can know the opcode
    if (Op.Flags==fMEXTRA)
      State = ExtraFlags;
    else if (Op.Flags==fERR) {
      memset(&Op, 0, sizeof(Instruction));
      State = Error;
    }
    else
      ProcessFlags(Op, State);
  }
  void ReadFlags(Instruction &Op, ExeState &State) {
    Op.Flags = Table1[Op.Code];
    Op.Category = TypeOp1[Op.Code];
    CheckFlags(Op, State);
  }
  void ProcessModRM(Instruction &Op, ExeState &State) {
    if ((Op.ModRM & ModRM_mod)==0x40)
      State = Read8_ModRM; //register+byte displacement
    else if ((Op.ModRM & ModRM_mod)==0x80 || (Op.ModRM & (ModRM_mod|ModRM_rm))==0x05 || (Op.ModRM<0x40 && (Op.SIB & SIB_base)==0x05) ){
      State = Read32_ModRM; //register+dword displacement
      Op.BytesRead = 0;
    }
    else
      ProcessMode(Op, State);
  }
  void ApplyCodeAndSetFlag(Instruction &Op, const U32 Flag = 0) {
    Op.Data&=ClearCodeMask; 
    Op.Data|=(Op.Code<<CodeShift)|Flag;
  }
  inline U32 OpN(OpCache &Cache, const U32 n) {
    return Cache.Op[ (Cache.Index-n)&(CacheSize-1) ];
  }
  inline U32 OpNCateg(U32 &Mask, const U32 n) {
    return ((Mask>>(CategoryShift*(n-1)))&CategoryMask);
  }
  inline int pref(const int i) {
    INJECT_SHARED_buf
    return (buf(i)==0x0f)+2*(buf(i)==0x66)+3*(buf(i)==0x67);
  }
  // Get context at buf(i) relevant to parsing 32-bit x86 code
  U32 execxt(int i, int x=0) {
    INJECT_SHARED_buf
    int prefix=0, opcode=0, modrm=0, sib=0;
    if (i) prefix+=4*pref(i--);
    if (i) prefix+=pref(i--);
    if (i) opcode+=buf(i--);
    if (i) modrm+=buf(i--)&(ModRM_mod|ModRM_rm);
    if (i && ((modrm&ModRM_rm)==4) && (modrm<ModRM_mod)) sib=buf(i)&SIB_scale;
    return prefix|opcode<<4|modrm<<12|x<<20|sib<<(28-6);
  }
  void Update();
public:
  ExeModel(const Shared * const  sh, const ModelStats * const st, const U64 size) : shared(sh), stats(st),
    cm(sh,size,nCM1+nCM2,64), iMap(sh,20,1,64,1023),
    pState (Start), State(Start), TotalOps(0), OpMask(0), OpCategMask(0), Context(0), BrkCtx(0), Valid(false) 
  {
    assert(ispowerof2(size));
    memset(&Cache, 0, sizeof(OpCache));
    memset(&Op, 0, sizeof(Instruction));
    memset(&StateBH, 0, sizeof(StateBH));
  }
  void mix(Mixer& m);
};

void ExeModel::Update() {
  INJECT_SHARED_buf INJECT_STATS_blpos INJECT_SHARED_c1
  pState = State;
  Op.Size++;
  switch (State) {
    case Start: case Error: {
      // previous code may have just been a REX prefix
      bool Skip = false;
      if (Op.MustCheckREX) {
        Op.MustCheckREX = false;
        // valid x64 code?
        if (!IsInvalidX64Op(c1) && !IsValidX64Prefix(c1)) {
          Op.REX = Op.Code;
          Op.Code = c1;
          Op.Data = PrefixREX|(Op.Code<<CodeShift)|(Op.Data&PrefixMask);
          Skip = true;
        }
      }

      Op.ModRM = Op.SIB = Op.REX = Op.Flags = Op.BytesRead = 0;
      if (!Skip) {
        Op.Code = c1;
        // possible REX prefix?
        Op.MustCheckREX = ((Op.Code&0xF0)==0x40) && (!(Op.Decoding && ((Op.Data&PrefixMask)==1)));

        // check prefixes
        Op.Prefix = (Op.Code==ES_OVERRIDE || Op.Code==CS_OVERRIDE || Op.Code==SS_OVERRIDE || Op.Code==DS_OVERRIDE) + //invalid in x64
                    (Op.Code==FS_OVERRIDE)*2 +
                    (Op.Code==GS_OVERRIDE)*3 +
                    (Op.Code==AD_OVERRIDE)*4 +
                    (Op.Code==WAIT_FPU)*5 +
                    (Op.Code==LOCK)*6 +
                    (Op.Code==REP_N_STR || Op.Code==REP_STR)*7;

        if (!Op.Decoding) {
          TotalOps+=(Op.Data!=0)-(Cache.Index && Cache.Op[ Cache.Index&(CacheSize-1) ]!=0);
          OpMask = (OpMask<<1)|(State!=Error);
          OpCategMask = (OpCategMask<<CategoryShift)|(Op.Category);
          Op.Size = 0;

          Cache.Op[ Cache.Index&(CacheSize-1) ] = Op.Data;
          Cache.Index++;

          if (!Op.Prefix)
            Op.Data = Op.Code<<CodeShift;
          else {
            Op.Data = Op.Prefix;
            Op.Category = TypeOp1[Op.Code];
            Op.Decoding = true;
            BrkCtx = hash(0, Op.Prefix, OpCategMask&CategoryMask);
            break;
          }
        }
        else {
          // we only have enough bits for one prefix, so the
          // instruction will be encoded with the last one
          if (!Op.Prefix) {
            Op.Data|=(Op.Code<<CodeShift);
            Op.Decoding = false;
          }
          else {
            Op.Data = Op.Prefix;
            Op.Category = TypeOp1[Op.Code];
            BrkCtx = hash(1, Op.Prefix, OpCategMask&CategoryMask);
            break;
          }
        }
      }

      if ((Op.o16=(Op.Code==OP_OSIZE)))
        State = Pref_Op_Size;
      else if (Op.Code==OP_2BYTE)
        State = Pref_MultiByte_Op;
      else
        ReadFlags(Op, State);
      BrkCtx = hash(2, State, Op.Code, (OpCategMask&CategoryMask), OpN(Cache,1)&((ModRM_mod|ModRM_reg|ModRM_rm)<<ModRMShift));
      break;
    }
    case Pref_Op_Size : {
      Op.Code = c1;
      ApplyCodeAndSetFlag(Op, OperandSizeOverride);
      ReadFlags(Op, State);
      BrkCtx = hash(3, State);
      break;
    }
    case Pref_MultiByte_Op : {
      Op.Code = c1;
      Op.Data|=MultiByteOpcode;

      if (Op.Code==0x38)
        State = Read_OP3_38;
      else if (Op.Code==0x3A)
        State = Read_OP3_3A;
      else {
        ApplyCodeAndSetFlag(Op);
        Op.Flags = Table2[Op.Code];
        Op.Category = TypeOp2[Op.Code];
        CheckFlags(Op, State);
      }
      BrkCtx = hash(4, State);
      break;
    }
    case ParseFlags : {
      ProcessFlags(Op, State);
      BrkCtx = hash(5, State);
      break;
    }
    case ExtraFlags : case ReadModRM : {
      Op.ModRM = c1;
      Op.Data|=(Op.ModRM<<ModRMShift)|HasModRM;
      Op.SIB = 0;
      if (Op.Flags==fMEXTRA) {
        Op.Data|=HasExtraFlags;
        int i = ((Op.ModRM>>3)&0x07) | ((Op.Code&0x01)<<3) | ((Op.Code&0x08)<<1);
        Op.Flags = TableX[i];
        Op.Category = TypeOpX[i];
        if (Op.Flags==fERR) {
          memset(&Op, 0, sizeof(Instruction));
          State = Error;
          BrkCtx = hash(6, State);
          break;
        }
        ProcessFlags(Op, State);
        BrkCtx = hash(7, State);
        break;
      }

      if ((Op.ModRM & ModRM_rm)==4 && Op.ModRM<ModRM_mod) {
        State = ReadSIB;
        BrkCtx = hash(8, State);
        break;
      }

      ProcessModRM(Op, State);
      BrkCtx = hash(9, State, Op.Code );
      break;
    }
    case Read_OP3_38 : case Read_OP3_3A : {
      Op.Code = c1;
      ApplyCodeAndSetFlag(Op, Prefix38<<(State-Read_OP3_38));
      if (State==Read_OP3_38) {
        Op.Flags = Table3_38[Op.Code];
        Op.Category = TypeOp3_38[Op.Code];
      }
      else {
        Op.Flags = Table3_3A[Op.Code];
        Op.Category = TypeOp3_3A[Op.Code];
      }
      CheckFlags(Op, State);
      BrkCtx = hash(10, State);
      break;
    }
    case ReadSIB : {
      Op.SIB = c1;
      Op.Data|=((Op.SIB & SIB_scale)<<SIBScaleShift);
      ProcessModRM(Op, State);
      BrkCtx = hash(11, State, Op.SIB&SIB_scale);
      break;
    }
    case Read8 : case Read16 : case Read32 : {
      if (++Op.BytesRead>=((State-Read8)<<int(Op.imm8+1))) {
        Op.BytesRead = 0;
        Op.imm8 = false;
        State = Start;
      }
      BrkCtx = hash(12, State, Op.Flags&fMODE, Op.BytesRead, ((Op.BytesRead>1)?(buf(Op.BytesRead)<<8):0)|((Op.BytesRead)?c1:0) );
      break;
    }
    case Read8_ModRM : {
      ProcessMode(Op, State);
      BrkCtx = hash(13, State);
      break;
    }
    case Read16_f : {
      if (++Op.BytesRead==2) {
        Op.BytesRead = 0;
        ProcessFlags2(Op, State);
      }
      BrkCtx = hash(14, State);
      break;
    }
    case Read32_ModRM : {
      Op.Data|=RegDWordDisplacement;
      if (++Op.BytesRead==4) {
        Op.BytesRead = 0;
        ProcessMode(Op, State);
      }
      BrkCtx = hash(15, State);
      break;
    }
  }
  Valid = (TotalOps>2*MinRequired) && ((OpMask&((1<<MinRequired)-1))==((1<<MinRequired)-1));
  Context = State+16*Op.BytesRead+16*(Op.REX & REX_w);
  StateBH[Context] = (StateBH[Context]<<8)|c1;

  bool Forced=stats->blockType==EXE;  
  if (Valid || Forced) {
    U32 mask=0, count0=0;
    U32 i=0;
    while (i<nCM1) {
      if (i>1) {mask = mask*2 + (buf(i-1)==0); count0+=mask&1;}
      int j = (i<4) ? i+1 : 5+(i-4)*(2+(i>6));
      cm.set(hash(i, execxt(j, buf(1)*(j>6)), ((1<<nCM1)|mask)*(count0*nCM1/2>=i), (0x08|(blpos&0x07))*(i<4)));
      i++;
    }

    cm.set(BrkCtx);

    mask = PrefixMask|(0xF8<<CodeShift)|MultiByteOpcode|Prefix38|Prefix3A;
    cm.set(hash(++i,OpN(Cache, 1)&(mask|RegDWordDisplacement|AddressMode), State+16*Op.BytesRead, Op.Data&mask, Op.REX, Op.Category));

    mask = 0x04|(0xFE<<CodeShift)|MultiByteOpcode|Prefix38|Prefix3A|((ModRM_mod|ModRM_reg)<<ModRMShift);
    cm.set(hash(++i,
      OpN(Cache, 1)&mask, OpN(Cache, 2)&mask, OpN(Cache, 3)&mask,
      Context+256*((Op.ModRM & ModRM_mod)==ModRM_mod),
      Op.Data&((mask|PrefixREX)^(ModRM_mod<<ModRMShift))
    ));

    mask = 0x04|CodeMask;
    cm.set(hash(++i,OpN(Cache, 1)&mask, OpN(Cache, 2)&mask, OpN(Cache, 3)&mask, OpN(Cache, 4)&mask, (Op.Data&mask)|(State<<11)|(Op.BytesRead<<15)));

    mask = 0x04|(0xFC<<CodeShift)|MultiByteOpcode|Prefix38|Prefix3A;
    cm.set(hash(++i,State+16*Op.BytesRead, Op.Data&mask, Op.Category*8 + (OpMask&0x07), Op.Flags, ((Op.SIB & SIB_base)==5)*4+((Op.ModRM & ModRM_reg)==ModRM_reg)*2+((Op.ModRM & ModRM_mod)==0)));

    mask = PrefixMask|CodeMask|OperandSizeOverride|MultiByteOpcode|PrefixREX|Prefix38|Prefix3A|HasExtraFlags|HasModRM|((ModRM_mod|ModRM_rm)<<ModRMShift);
    cm.set(hash(++i,Op.Data&mask, State+16*Op.BytesRead, Op.Flags));

    mask = PrefixMask|CodeMask|OperandSizeOverride|MultiByteOpcode|Prefix38|Prefix3A|HasExtraFlags|HasModRM;
    cm.set(hash(++i,OpN(Cache, 1)&mask, State, Op.BytesRead*2+((Op.REX&REX_w)>0), Op.Data&((U16)(mask^OperandSizeOverride))));

    mask = 0x04|(0xFE<<CodeShift)|MultiByteOpcode|Prefix38|Prefix3A|(ModRM_reg<<ModRMShift);
    cm.set(hash(++i,OpN(Cache, 1)&mask, OpN(Cache, 2)&mask, State+16*Op.BytesRead, Op.Data&(mask|PrefixMask|CodeMask)));

    cm.set(hash(++i,State+16*Op.BytesRead));

    cm.set(hash(++i,
      (0x100|c1)*(Op.BytesRead>0),
      State+16*pState+256*Op.BytesRead,
      ((Op.Flags&fMODE)==fAM)*16 + (Op.REX & REX_w) + (Op.o16)*4 + ((Op.Code & 0xFE)==0xE8)*2 + ((Op.Data & MultiByteOpcode)!=0 && (Op.Code & 0xF0)==0x80)
    ));
  }
}

void ExeModel::mix(Mixer& m) {
  INJECT_SHARED_bpos 
  bool Forced=stats->blockType==EXE;
  if (bpos==0)
    Update();

  if (Valid || Forced){
    cm.setscale(Forced?128:64);
    cm.mix(m);
    iMap.set(hash(BrkCtx, bpos));
    iMap.setscale(Forced?128:64);
    iMap.mix(m);
  }
  else {
      for (int i=0; i<MIXERINPUTS; ++i)
        m.add(0);
  }
  INJECT_SHARED_c0
  U8 s = ((StateBH[Context]>>(28-bpos))&0x08) |
         ((StateBH[Context]>>(21-bpos))&0x04) |
         ((StateBH[Context]>>(14-bpos))&0x02) |
         ((StateBH[Context]>>( 7-bpos))&0x01) |
         ((Op.Category==OP_GEN_BRANCH)<<4)|
         (((c0&((1<<bpos)-1))==0)<<5);

  m.set(Context*4+(s>>4), 1024);
  m.set(State*64+bpos*8+(Op.BytesRead>0)*4+(s>>4), 1024);
  m.set((BrkCtx&0x1FF)|((s&0x20)<<4), 1024);
  m.set(finalize64(hash(Op.Code, State, OpN(Cache, 1)&CodeMask),13), 8192);
  m.set(finalize64(hash(State, bpos, Op.Code, Op.BytesRead),13), 8192);
  m.set(finalize64(hash(State, (bpos<<2)|(c0&3), OpCategMask&CategoryMask, ((Op.Category==OP_GEN_BRANCH)<<2)|(((Op.Flags&fMODE)==fAM)<<1)|(Op.BytesRead>0)),13), 8192);
}

//////////////////////////// IndirectModel /////////////////////

// The context is a byte string history that occurs within a
// 1 or 2 byte context.

class IndirectModel  {
private:
  static constexpr int nCM = 15;
public:
  static constexpr int MIXERINPUTS = nCM*ContextMap::MIXERINPUTS; // 75
  static constexpr int MIXERCONTEXTS = 0;
  static constexpr int MIXERCONTEXTSETS = 0;
private:
  const Shared * const shared;
  ContextMap cm;
  Array<U32> t1{256};
  Array<U16> t2{0x10000};
  Array<U16> t3{0x8000};
  Array<U16> t4{0x8000};
  IndirectContext<U32> iCtx{16,8}; /* BitsPerContext, InputBits */
public:
  IndirectModel(const Shared * const sh, const U64 size) : shared(sh), cm(sh,size,nCM){}
  void mix(Mixer& m) {
    INJECT_SHARED_bpos
    if(bpos==0) {
      INJECT_SHARED_c4 INJECT_SHARED_buf
      U32 d=c4&0xffff, c=d&255, d2=(buf(1)&31)+32*(buf(2)&31)+1024*(buf(3)&31);
      U32 d3=(buf(1)>>3&31)+32*(buf(3)>>3&31)+1024*(buf(4)>>3&31);
      U32& r1=t1[d>>8];
      r1=r1<<8|c;
      U16& r2=t2[c4>>8&0xffff];
      r2=r2<<8|c;
      U16& r3=t3[(buf(2)&31)+32*(buf(3)&31)+1024*(buf(4)&31)];
      r3=r3<<8|c;
      U16& r4=t4[(buf(2)>>3&31)+32*(buf(4)>>3&31)+1024*(buf(5)>>3&31)];
      r4=r4<<8|c;
      const U32 t=c|t1[c]<<8;
      const U32 t0=d|t2[d]<<16;
      const U32 ta=d2|t3[d2]<<16;
      const U32 tc=d3|t4[d3]<<16;
      const U8 pc=tolower(U8(c4>>8));
      iCtx+=(c=tolower(c)), iCtx=(pc<<8)|c;
      const U32 ctx0=iCtx(), mask=(U8(t1[c])==U8(t2[d]))|
        ((U8(t1[c])==U8(t3[d2]))<<1)|
        ((U8(t1[c])==U8(t4[d3]))<<2)|
        ((U8(t1[c])==U8(ctx0))<<3);
      U64 i=0;
      cm.set(hash(++i,t));
      cm.set(hash(++i,t0));
      cm.set(hash(++i,ta));
      cm.set(hash(++i,tc));
      cm.set(hash(++i,t&0xff00, mask));
      cm.set(hash(++i,t0&0xff0000));
      cm.set(hash(++i,ta&0xff0000));
      cm.set(hash(++i,tc&0xff0000));
      cm.set(hash(++i,t&0xffff));
      cm.set(hash(++i,t0&0xffffff));
      cm.set(hash(++i,ta&0xffffff));
      cm.set(hash(++i,tc&0xffffff));
      cm.set(hash(++i, ctx0&0xff, c));
      cm.set(hash(++i, ctx0&0xffff));
      cm.set(hash(++i, ctx0&0x7f7fff));
    }
    cm.mix(m);
  }
};

//////////////////////////// dmcModel //////////////////////////

// Model using DMC (Dynamic Markov Compression).
//
// The bitwise context is represented by a state graph.
//
// See the original paper: http://webhome.cs.uvic.ca/~nigelh/Publications/DMC.pdf
// See the original DMC implementation: http://maveric0.uwaterloo.ca/ftp/dmc/
//
// Main differences:
// - Instead of floats we use fixed point arithmetic.
// - The threshold for cloning a state increases gradually as memory is used up.
// - For probability estimation each state maintains both a 0,1 count ("c0" and "c1") 
//   and a bit history ("state"). The 0,1 counts are updated adaptively favoring newer events.
//   The bit history state is mapped to a probability adaptively using a StateMap.
// - The predictions of multiple "dmcModel"s are combined and stiblilized in "dmcForest". See below.

struct DMCNode { // 12 bytes
  // c0,c1: adaptive counts of zeroes and ones; 
  //        fixed point numbers with 6 integer and 10 fractional bits, i.e. scaling factor = 1024;
  //        thus the values 0 .. 65535 represent real counts of 0.0 .. 63.999
  // nx0, nx1: indexes of next DMC nodes in the state graph
  // state: bit history state - as in a contextmap
public:
  U16 c0, c1;
private:
  U32 _nx0, _nx1; // packed: their higher 28 bits are nx0, nx1; the lower 4+4 bits give the bit history state byte

public:
  U8   get_state() const        {return U8(((_nx0&0xf)<<4) | (_nx1&0xf));}
  void set_state(const U8 state) {_nx0=(_nx0&0xfffffff0) | (state>>4); _nx1=(_nx1&0xfffffff0) | (state&0xf);}
  U32  get_nx0() const          {return _nx0>>4;}
  void set_nx0(const U32 nx0)    {assert((nx0>>28)==0);_nx0=(_nx0&0xf) | (nx0<<4);}
  U32  get_nx1() const          {return _nx1>>4;}
  void set_nx1(const U32 nx1)    {assert((nx1>>28)==0);_nx1=(_nx1&0xf) | (nx1<<4);}
};

#define DMC_NODES_BASE (255*256) // = 65280
#define DMC_NODES_MAX  ((U64(1)<<31)/sizeof(DMCNode)) // = 178 956 970

class dmcModel {
private:
  const Shared * const shared;
  Random rnd;
  Array<DMCNode> t;     // state graph
  StateMap sm;          // statemap for bit history states
  U32 top, curr;        // index of first unallocated node (i.e. number of allocated nodes); index of current node
  U32 threshold;        // cloning threshold parameter: fixed point number like c0,c1
  U32 threshold_fine;   // "threshold" scaled by 11 bits used for increasing the threshold in finer steps
  U32 extra;            // this value is used for approximating stategraph maturity level when the state graph is already full 
                        // this is the number of skipped cloning events when the counts were already large enough (>1.0)

  // helper function: adaptively increment a counter
  U32 increment_counter (const U32 x, const U32 increment) const { // x is a fixed point number as c0,c1 ; "increment"  is 0 or 1
    return (((x<<6)-x)>>6)+(increment<<10); // x * (1-1/64) + increment
  }

public: 
  dmcModel(const Shared * const sh, const U64 dmc_nodes, const U32 th_start) : shared(sh), 
    t(min(dmc_nodes+DMC_NODES_BASE,DMC_NODES_MAX)), 
    sm(sh,1,256,256 /*64-512 are all fine*/ ,true) //StateMap: s, n, limit, init
  {
    resetstategraph(th_start);
  }

  // Initialize the state graph to a bytewise order 1 model
  // See an explanation of the initial structure in:
  // http://wing.comp.nus.edu.sg/~junping/docs/njp-icita2005.pdf
  void resetstategraph(const U32 th_start) {
    assert(((t.size()-1)>>28)==0); // the top 4 bits must be unused by nx0 and nx1 for storing the 4+4 bits of the bit history state byte
    top=curr=extra=0;
    threshold=th_start;
    threshold_fine=th_start<<11;
    for (int j=0; j<256; ++j) { //256 trees
      for (int i=0; i<255; ++i) { //255 nodes in each tree
        if (i<127) { //internal tree nodes
          t[top].set_nx0(top+i+1); // left node 
          t[top].set_nx1(top+i+2); // right node
        }
        else { // 128 leaf nodes - they each references a root node of tree(i)
          int linked_tree_root=(i-127)*2*255;
          t[top].set_nx0(linked_tree_root);     // left node  -> root of tree 0,2,4... 
          t[top].set_nx1(linked_tree_root+255); // right node -> root of tree 1,3,5...
        }
        t[top].c0=t[top].c1 = th_start<1024 ? 2048 : 512; // 2.0  0.5
        t[top].set_state(0);
        top++;
      }
    }
  }

  //update stategraph
  void update() {

    U32 c0=t[curr].c0;
    U32 c1=t[curr].c1;
    INJECT_SHARED_y
    const U32 n = y==0 ? c0 : c1;

    // update counts, state
    t[curr].c0=increment_counter(c0,1-y);
    t[curr].c1=increment_counter(c1,y);
    t[curr].set_state( StateTable::next(t[curr].get_state(),y,rnd) );

    // clone next state when threshold is reached
    if(n>threshold) {

      const U32 next = y==0 ? t[curr].get_nx0() : t[curr].get_nx1();
      c0=t[next].c0;
      c1=t[next].c1;
      const U32 nn=c0+c1;

      if(nn>n+threshold) {
        if(top!=t.size()) { // state graph is not yet full, let's clone
          U32 c0_top=U64(c0)*U64(n)/U64(nn);
          U32 c1_top=U64(c1)*U64(n)/U64(nn);
          assert(c0>=c0_top);
          assert(c1>=c1_top);
          c0-=c0_top;
          c1-=c1_top;

          t[top].c0=c0_top;
          t[top].c1=c1_top;
          t[next].c0=c0;
          t[next].c1=c1;

          t[top].set_nx0(t[next].get_nx0());
          t[top].set_nx1(t[next].get_nx1());
          t[top].set_state(t[next].get_state());
          if(y==0) t[curr].set_nx0(top);
          else t[curr].set_nx1(top);

          ++top;

          if(threshold<8*1024)threshold=(++threshold_fine)>>11;
        }
        else // state graph was full
          extra += nn>>10;
      }
    }

    if(y==0) curr=t[curr].get_nx0();
    else     curr=t[curr].get_nx1();
  }

  bool isfull() const {return extra>>7 > U32(t.size());}
  int pr1() const {
    const U32 n0=t[curr].c0+1;
    const U32 n1=t[curr].c1+1;
    return (n1<<12)/(n0+n1);
  }
  int pr2() {
    const U8 state=t[curr].get_state();
    return sm.p(0,state);
  }
  int st() {
    update();
    return stretch(pr1()) + stretch(pr2()); // average the predictions for stability
  }
};

// This class solves two problems of the DMC model
// 1) The DMC model is a memory hungry algorithm. In theory it works best when it can clone
//    nodes forever. But when the state graph is full you can't clone nodes anymore. 
//    You can either i) reset the model (the state graph) and start over
//    or ii) you can keep updating the counts forever in the already fixed state graph. Both
//    choices are troublesome: i) resetting the model degrades the predictive power significantly
//    until the graph becomes large enough again and ii) a fixed structure can't adapt anymore.
//    To solve this issue:
//    Ten models with different arguments work in tandem. Only eight of the ten models
//    are reset periodically. Due to their different cloning threshold arguments and 
//    different state graph sizes they are reset at different points in time. 
//    The remaining two models (having the highest threshold and largest stategraph) are
//    never reset and are beneficial for semi-stationary files.
// 2) The DMC model is sensitive to the cloning threshold parameter. Some files prefer
//    a smaller threshold other files prefer a larger threshold.
//    The difference in terms of compression is significant.
//    To solve this issue DMC models with different thresholds are used and their 
//    predictions are combined.
//
//    Disadvantages: with the same memory requirements we have less number of nodes
//    in each model. Also keeping more models updated at all times requires more
//    calculations and more memory access than updating one model only.
//    Advantage: more stable and better compression - even with reduced number of nodes.
//
// Further notes: 
//    Extremely small initial threshold arguments (i) help the state graph become large faster
//    and model longer input bit sequences sooner. Moreover (ii) when using a small threshold 
//    parameter the split counts c0 and c1 will be small after cloning, and after updating them
//    with 0 and 1 the prediction p=c1/(c0+c1) will be biased towards these latest events.

class DmcForest {
private:
  static constexpr U32 MODELS = 10; // 8 fast and 2 slow models
public:
  static constexpr int MIXERINPUTS = 2 + 8/2; // 6 : fast models (2 individually) + slow models (8 combined pairwise)
  static constexpr int MIXERCONTEXTS = 0;
  static constexpr int MIXERCONTEXTSETS = 0;
private:
  static constexpr U32 dmcparams [MODELS] = {2,32, 64,4, 128,8, 256,16, 1024,1536};
  static constexpr U64 dmcmem [MODELS]    = {6,10, 11,7,  12,8,  13, 9,    2,   2};
  const Shared * const shared;
  Array<dmcModel*> dmcmodels;
public:
  DmcForest(const Shared * const sh, const U64 size) : shared(sh), dmcmodels(MODELS) {
    for(int i=MODELS-1;i>=0;i--) 
      dmcmodels[i]=new dmcModel(sh,size/dmcmem[i],dmcparams[i]);
  }
  ~DmcForest() {
    for(int i=MODELS-1;i>=0;i--)
      delete dmcmodels[i];
  }

  // update and predict
  void mix(Mixer &m) {
    int i=MODELS;
    // the slow models predict individually
    m.add(dmcmodels[--i]->st()>>3);
    m.add(dmcmodels[--i]->st()>>3);
    // the fast models are combined for better stability
    while(i>0) {
      const int pr1=dmcmodels[--i]->st();
      const int pr2=dmcmodels[--i]->st();
      m.add((pr1+pr2)>>4);
    }

    // reset models when their structure can't adapt anymore
    // the two slow models are never reset
    INJECT_SHARED_bpos
    if(bpos==0)
      for(int i=MODELS-3;i>=0;i--) 
        if(dmcmodels[i]->isfull())
          dmcmodels[i]->resetstategraph(dmcparams[i]);
  }
};


//////////////////////////// NestModel //////////////////////////
//
//

class NestModel {
private:
  static constexpr int nCM = 12;
public:
  static constexpr int MIXERINPUTS = nCM*ContextMap::MIXERINPUTS; // 60
  static constexpr int MIXERCONTEXTS = 0;
  static constexpr int MIXERCONTEXTSETS = 0;
private:
  const Shared * const shared;
  int ic=0, bc=0, pc=0,vc=0, qc=0, lvc=0, wc=0, ac=0, ec=0, uc=0, sense1=0, sense2=0, w=0;
  ContextMap cm;
public:
  NestModel(const Shared * const sh, const U64 size) : shared(sh), cm(sh,size,nCM) {}
  void mix(Mixer& m) {
    INJECT_SHARED_bpos 
    if (bpos==0) {
      INJECT_SHARED_c1
      int c=c1, matched=1, vv;
      w*=((vc&7)>0 && (vc&7)<3);
      if (c&0x80) w = w*11*32 + c;
      const int lc = (c >= 'A' && c <= 'Z'?c+'a'-'A':c);
      if (lc == 'a' || lc == 'e' || lc == 'i' || lc == 'o' || lc == 'u'){ vv = 1; w = w*997*8 + (lc/4-22); } else
      if (lc >= 'a' && lc <= 'z'){ vv = 2; w = w*271*32 + lc-97; } else
      if (lc == ' ' || lc == '.' || lc == ',' || lc == '\n') vv = 3; else
      if (lc >= '0' && lc <= '9') vv = 4; else
      if (lc == 'y') vv = 5; else
      if (lc == '\'') vv = 6; else vv=(c&32)?7:0;
      vc = (vc << 3) | vv;
      if (vv != lvc) {
        wc = (wc << 3) | vv;
        lvc = vv;
      }
      INJECT_SHARED_c4
      switch(c) {
        case ' ': qc = 0; break;
        case '(': ic += 31; break;
        case ')': ic -= 31; break;
        case '[': ic += 11; break;
        case ']': ic -= 11; break;
        case '<': ic += 23; qc += 34; break;
        case '>': ic -= 23; qc /= 5; break;
        case ':': pc = 20; break;
        case '{': ic += 17; break;
        case '}': ic -= 17; break;
        case '|': pc += 223; break;
        case '"': pc += 0x40; break;
        case '\'': pc += 0x42; if (c!=(U8)(c4>>8)) sense2^=1; else ac+=(2*sense2-1); break;
        case '\n': pc = qc = 0; break;
        case '.': pc = 0; break;
        case '!': pc = 0; break;
        case '?': pc = 0; break;
        case '#': pc += 0x08; break;
        case '%': pc += 0x76; break;
        case '$': pc += 0x45; break;
        case '*': pc += 0x35; break;
        case '-': pc += 0x3; break;
        case '@': pc += 0x72; break;
        case '&': qc += 0x12; break;
        case ';': qc /= 3; break;
        case '\\': pc += 0x29; break;
        case '/': pc += 0x11;
                  if (c1 == '<') qc += 74;
                  break;
        case '=': pc += 87; if (c!=(U8)(c4>>8)) sense1^=1; else ec+=(2*sense1-1); break;
        default: matched = 0;
      }
      if (c4==0x266C743B) uc=min(7,uc+1);  //&lt;
      else if (c4==0x2667743B) uc-=(uc>0); //&gt;
      if (matched) bc = 0; else bc += 1;
      if (bc > 300) bc = ic = pc = qc = uc = 0;
      U64 i=0;
      cm.set(hash(++i, (vv>0 && vv<3)?0:(lc|0x100), ic&0x3FF, ec&0x7, ac&0x7, uc ));
      cm.set(hash(++i, ic, w, ilog2(bc+1)));
      cm.set(hash(++i, (3*vc+77*pc+373*ic+qc)&0xffff));
      cm.set(hash(++i, (31*vc+27*pc+281*qc)&0xffff));
      cm.set(hash(++i, (13*vc+271*ic+qc+bc)&0xffff));
      cm.set(hash(++i, (17*pc+7*ic)&0xffff));
      cm.set(hash(++i, (13*vc+ic)&0xffff));
      cm.set(hash(++i, (vc/3+pc)&0xffff));
      cm.set(hash(++i, (7*wc+qc)&0xffff));
      cm.set(hash(++i, vc&0xffff,c4&0xff));
      cm.set(hash(++i, (3*pc)&0xffff,c4&0xff));
      cm.set(hash(++i, ic&0xffff,c4&0xff));

    }
    cm.mix(m);
  }
};

//////////////////////////// XMLModel //////////////////////////
//
//

struct XMLAttribute {
  U32 Name, Value, Length;
};

struct XMLContent {
  U32 Data, Length, Type;
};

struct XMLTag {
  U32 Name, Length;
  int Level;
  bool EndTag, Empty;
  XMLContent Content;
  struct XMLAttributes {
    XMLAttribute Items[4];
    U32 Index;
  } Attributes;
};

struct XMLTagCache {
  XMLTag Tags[CacheSize];
  U32 Index;
};

enum ContentFlags {
  Text        = 0x001,
  Number      = 0x002,
  Date        = 0x004,
  Time        = 0x008,
  URL         = 0x010,
  Link        = 0x020,
  Coordinates = 0x040,
  Temperature = 0x080,
  ISBN        = 0x100,
};

enum XMLState {
  None               = 0,
  ReadTagName        = 1,
  ReadTag            = 2,
  ReadAttributeName  = 3,
  ReadAttributeValue = 4,
  ReadContent        = 5,
  ReadCDATA          = 6,
  ReadComment        = 7,
};

class XMLModel {
private:
  static constexpr int nCM = 4;
public:
  static constexpr int MIXERINPUTS = nCM*ContextMap::MIXERINPUTS; //20
  static constexpr int MIXERCONTEXTS = 0;
  static constexpr int MIXERCONTEXTSETS = 0;
private:
  const Shared * const shared;
  ContextMap cm;
  XMLTagCache Cache{};
  U32 StateBH[8]{};
  XMLState State = None, pState = None;
  U32 WhiteSpaceRun = 0, pWSRun = 0, IndentTab = 0, IndentStep = 2, LineEnding = 2;

  void DetectContent(XMLContent *Content){
    INJECT_SHARED_c4 INJECT_SHARED_c8 INJECT_SHARED_buf
    if ((c4&0xF0F0F0F0)==0x30303030) { //may be 4 digits (dddd)
      int i = 0, j = 0;
      while ((i<4) && ( (j=(c4>>(8*i))&0xFF)>=0x30 && j<=0x39 ))
        i++;
      //
      if (i==4 /*????dddd*/ && ( ((c8&0xFDF0F0FD)==0x2D30302D && buf(9)>=0x30 && buf(9)<=0x39 /*d-dd-dddd or d.dd.dddd*/) || ((c8&0xF0FDF0FD)==0x302D302D) /*d-d-dddd or d.d.dddd*/))
        (*Content).Type |= ContentFlags::Date;
    }
    else if (((c8&0xF0F0FDF0)==0x30302D30 /*dd.d???? or dd-d????*/ || (c8&0xF0F0F0FD)==0x3030302D) && buf(9)>=0x30 && buf(9)<=0x39 /*dddd-???? or dddd.????*/){
      int i = 2, j = 0;
      while ((i<4) && ( (j=(c8>>(8*i))&0xFF)>=0x30 && j<=0x39 ))
        i++;

      if (i==4 && (c4&0xF0FDF0F0)==0x302D3030) //dd??d.dd or dd??d-dd
        (*Content).Type |= ContentFlags::Date;
    }

    if ((c4&0xF0FFF0F0)==0x303A3030 && buf(5)>=0x30 && buf(5)<=0x39 && ((buf(6)<0x30 || buf(6)>0x39) /*?dd:dd*/ || ((c8&0xF0F0FF00)==0x30303A00 && (buf(9)<0x30 || buf(9)>0x39) /*?dd:dd:dd*/)))
      (*Content).Type |= ContentFlags::Time;

    if ((*Content).Length>=8 && (c8&0x80808080)==0 && (c4&0x80808080)==0) //8 ~ascii
      (*Content).Type |= ContentFlags::Text;

    if ((c8&0xF0F0FF)==0x3030C2 && (c4&0xFFF0F0FF)==0xB0303027){ //dd {utf8 C2B0: degree sign} dd {apostrophe}
      int i = 2;
      while ((i<7) && buf(i)>=0x30 && buf(i)<=0x39)
        i+=(i&1)*2+1;

      if (i==10)
        (*Content).Type |= ContentFlags::Coordinates;
    }

    if ((c4&0xFFFFFA)==0xC2B042 && (c4&0xff)!=0x47 && (((c4>>24)>=0x30 && (c4>>24)<=0x39) || ((c4>>24)==0x20 && (buf(5)>=0x30 && buf(5)<=0x39))))
      (*Content).Type |= ContentFlags::Temperature;
    
    INJECT_SHARED_c1
    if (c1>=0x30 && c1<=0x39)
      (*Content).Type |= ContentFlags::Number;

    if (c4==0x4953424E && (c8&0xff)==0x20) // " ISBN"
      (*Content).Type |= ContentFlags::ISBN;
  }
public:
  XMLModel(const Shared * const sh, const U64 size) : shared(sh), cm(sh,size,nCM) {}
  void update() {
    INJECT_SHARED_c1 INJECT_SHARED_c4 INJECT_SHARED_c8
    XMLTag *pTag = &Cache.Tags[ (Cache.Index-1)&(CacheSize-1) ], *Tag = &Cache.Tags[ Cache.Index&(CacheSize-1) ];
    XMLAttribute *Attribute = &((*Tag).Attributes.Items[ (*Tag).Attributes.Index&3 ]);
    XMLContent *Content = &(*Tag).Content;
    pState = State;
    if ((c1==TAB || c1==SPACE) && (c1==(U8)(c4>>8) || !WhiteSpaceRun)){
      WhiteSpaceRun++;
      IndentTab = (c1==TAB);
    }
    else{
      if ((State==None || (State==ReadContent && (*Content).Length<=LineEnding+WhiteSpaceRun)) && WhiteSpaceRun>1+IndentTab && WhiteSpaceRun!=pWSRun){
        IndentStep=abs((int)(WhiteSpaceRun-pWSRun));
        pWSRun = WhiteSpaceRun;
      }
      WhiteSpaceRun=0;
    }
    if (c1==NEW_LINE)
      LineEnding = 1+((U8)(c4>>8)==CARRIAGE_RETURN);

    switch (State){
      case None : {
        if (c1==0x3C){
          State = ReadTagName;
          memset(Tag, 0, sizeof(XMLTag));
          (*Tag).Level = ((*pTag).EndTag || (*pTag).Empty)?(*pTag).Level:(*pTag).Level+1;
        }
        if ((*Tag).Level>1)
          DetectContent(Content);

        cm.set(hash(pState, State, ((*pTag).Level+1)*IndentStep - WhiteSpaceRun));
        break;
      }
      case ReadTagName : {
        if ((*Tag).Length>0 && (c1==TAB || c1==NEW_LINE || c1==CARRIAGE_RETURN || c1==SPACE))
          State = ReadTag;
        else if ((c1==0x3A || (c1>='A' && c1<='Z') || c1==0x5F || (c1>='a' && c1<='z')) || ((*Tag).Length>0 && (c1==0x2D || c1==0x2E || (c1>='0' && c1<='9')))){
          (*Tag).Length++;
          (*Tag).Name = (*Tag).Name * 263 * 32 + (c1&0xDF);
        }
        else if (c1 == 0x3E){
          if ((*Tag).EndTag){
            State = None;
            Cache.Index++;
          }
          else
            State = ReadContent;
        }
        else if (c1!=0x21 && c1!=0x2D && c1!=0x2F && c1!=0x5B){
          State = None;
          Cache.Index++;
        }
        else if ((*Tag).Length==0){
          if (c1==0x2F){
            (*Tag).EndTag = true;
            (*Tag).Level = max(0,(*Tag).Level-1);
          }
          else if (c4==0x3C212D2D){
            State = ReadComment;
            (*Tag).Level = max(0,(*Tag).Level-1);
          }
        }

        if ((*Tag).Length==1 && (c4&0xFFFF00)==0x3C2100){
          memset(Tag, 0, sizeof(XMLTag));
          State = None;
        }
        else if ((*Tag).Length==5 && c8==0x215B4344 && c4==0x4154415B){
          State = ReadCDATA;
          (*Tag).Level = max(0,(*Tag).Level-1);
        }

        int i = 1;
        do{
          pTag = &Cache.Tags[ (Cache.Index-i)&(CacheSize-1) ];
          i+=1+((*pTag).EndTag && Cache.Tags[ (Cache.Index-i-1)&(CacheSize-1) ].Name==(*pTag).Name);
        }
        while ( i<CacheSize && ((*pTag).EndTag || (*pTag).Empty) );

        cm.set(hash(pState, State, (*Tag).Name, (*Tag).Level, (*pTag).Name, (*pTag).Level!=(*Tag).Level ));
        break;
      }
      case ReadTag : {
        if (c1==0x2F)
          (*Tag).Empty = true;
        else if (c1==0x3E){
          if ((*Tag).Empty){
            State = None;
            Cache.Index++;
          }
          else
            State = ReadContent;
        }
        else if (c1!=TAB && c1!=NEW_LINE && c1!=CARRIAGE_RETURN && c1!=SPACE){
          State = ReadAttributeName;
          (*Attribute).Name = c1&0xDF;
        }
        cm.set(hash(pState, State, (*Tag).Name, c1, (*Tag).Attributes.Index ));
        break;
      }
      case ReadAttributeName : {
        if ((c4&0xFFF0)==0x3D20 && (c1==0x22 || c1==0x27)){
          State = ReadAttributeValue;
          if ((c8&0xDFDF)==0x4852 && (c4&0xDFDF0000)==0x45460000)
            (*Content).Type |= Link;
        }
        else if (c1!=0x22 && c1!=0x27 && c1!=0x3D)
          (*Attribute).Name = (*Attribute).Name * 263 * 32 + (c1&0xDF);

        cm.set(hash(pState, State, (*Attribute).Name, (*Tag).Attributes.Index, (*Tag).Name, (*Content).Type ));
        break;
      }
      case ReadAttributeValue : {
        if (c1==0x22 || c1==0x27){
          (*Tag).Attributes.Index++;
          State = ReadTag;
        }
        else{
          (*Attribute).Value = (*Attribute).Value* 263 * 32 + (c1&0xDF);
          (*Attribute).Length++;
          if ((c8&0xDFDFDFDF)==0x48545450 && ((c4>>8)==0x3A2F2F || c4==0x733A2F2F))
            (*Content).Type |= URL;
        }
        cm.set(hash(pState, State, (*Attribute).Name, (*Content).Type ));
        break;
      }
      case ReadContent : {
        if (c1==0x3C){
          State = ReadTagName;
          Cache.Index++;
          memset(&Cache.Tags[ Cache.Index&(CacheSize-1) ], 0, sizeof(XMLTag));
          Cache.Tags[ Cache.Index&(CacheSize-1) ].Level = (*Tag).Level+1;
        }
        else{
          (*Content).Length++;
          (*Content).Data = (*Content).Data * 997*16 + (c1&0xDF);
          DetectContent(Content);
        }
        cm.set(hash(pState, State, (*Tag).Name, c4&0xC0FF ));
        break;
      }
      case ReadCDATA : {
        if ((c4&0xFFFFFF)==0x5D5D3E){
          State = None;
          Cache.Index++;
        }
        cm.set(hash(pState, State));
        break;
      }
      case ReadComment : {
        if ((c4&0xFFFFFF)==0x2D2D3E){
          State = None;
          Cache.Index++;
        }
        cm.set(hash(pState, State));
        break;
      }
    }

    StateBH[pState] = (StateBH[pState]<<8)|c1;
    pTag = &Cache.Tags[ (Cache.Index-1)&(CacheSize-1) ];
    U64 i=64;
    cm.set(hash(++i,State, (*Tag).Level, pState*2+(*Tag).EndTag, (*Tag).Name));
    cm.set(hash(++i,(*pTag).Name, State*2+(*pTag).EndTag, (*pTag).Content.Type, (*Tag).Content.Type));
    cm.set(hash(++i,State*2+(*Tag).EndTag, (*Tag).Name, (*Tag).Content.Type, c4&0xE0FF));
  }
  void mix(Mixer &m) {
    INJECT_SHARED_bpos
    if (bpos==0)
      update();
    cm.mix(m);
  }
};


//////////////////////// Order-N Model //////////////////
// Model for order 0-14 contexts
// Contexts are hashes of previous 0..14 bytes
// Order 0..6, 8 and 14 are used for prediction
// Note: order 7+ contexts are modeled by matchModel as well

class NormalModel { 
private:
  static constexpr int nCM = 9;
  static constexpr int nRCM = 3;
  static constexpr int nSM = 2;
public:
  static constexpr int MIXERINPUTS = nCM*ContextMap2::MIXERINPUTS + nRCM*RunContextMap::MIXERINPUTS + nSM; //68
  static constexpr int MIXERCONTEXTS = 64 + 8+1024+256+256+256+256+1536; //3656
  static constexpr int MIXERCONTEXTSETS = 7;
private:
  const Shared * const shared;
  ModelStats *stats;
  ContextMap2 cm;
  RunContextMap rcm7, rcm9, rcm10;
  StateMap StateMaps[2];
  U64 cxt[15]{}; // context hashes
public:
  NormalModel(const Shared * const sh, ModelStats *st,const U64 cmsize, const U64 rcmsize):
    shared(sh), stats(st),
    cm(sh,cmsize,nCM,64), rcm7(sh,rcmsize), rcm9(sh,rcmsize), rcm10(sh,rcmsize),
    StateMaps{ /* StateMap: s, n, lim, init */
      {sh,1,256,1023,false}, {sh,1,256*256,1023,false}
    }
  {
    assert(ispowerof2(cmsize) && ispowerof2(rcmsize));
  }

  void reset() {
    memset(&cxt[0], 0, sizeof(cxt));
  }

  void update_hashes() {
    // update order 1..14 context hashes
    // note: order 0 context does not need an update so its hash never changes
    INJECT_SHARED_c1
    for (int i=14; i>0; --i)
      cxt[i]=combine64(cxt[i-1],c1);
  }

  void mix(Mixer& m) {
    INJECT_SHARED_bpos
    if(bpos==0) {
      update_hashes();
      for (int i=0; i<=6; ++i) 
        cm.set(cxt[i]);
      cm.set(cxt[8]);
      cm.set(cxt[14]);
      rcm7.set(cxt[7]);
      rcm9.set(cxt[10]);
      rcm10.set(cxt[12]);
    }
    cm.mix(m);
    rcm7.mix(m);
    rcm9.mix(m);
    rcm10.mix(m);
    
    INJECT_SHARED_c0 INJECT_SHARED_c1 
    m.add((stretch(StateMaps[0].p(0,c0))+1)>>1);
    m.add((stretch(StateMaps[1].p(0,c0|(c1<<8)))+1)>>1);

    const int order=max(0, cm.order-(nCM-7)); //0-7
    assert(0<=order && order<=7);
    m.set(order<<3 | bpos, 64); 
    stats->order=order;
  }
  
  //setting more mixer contexts after skipping the special blocktypes
  void mix_post(Mixer& m) {
    INJECT_SHARED_c4 
    U32 c2=(c4>>8)&0xff, c3=(c4>>16)&0xff, c;

    INJECT_SHARED_c0 INJECT_SHARED_c1 INJECT_SHARED_bpos 
    m.set(8+(c1 | (bpos>5)<<8 |  ( ((c0&((1<<bpos)-1))==0) || (c0==((2<<bpos)-1)) )<<9), 8+1024);
    m.set(c0, 256);
    m.set(stats->order | ((c4>>6)&3)<<3 | (bpos==0)<<5 | (c1==c2)<<6 | (stats->blockType==EXE)<<7, 256);
    m.set(c2, 256);
    m.set(c3, 256);
    if (bpos!=0)
    {
      c=c0<<(8-bpos); if (bpos==1)c|=c3>>1;
      c=min(bpos,5)<<8 | c1>>5 | (c2>>5)<<3 | (c&192);
    }
    else c=c3>>7 | (c4>>31)<<1 | (c2>>6)<<2 | (c1&240);
    m.set(c, 1536);
  }
};

// this is a factory class for lazy object creation for models
// objects created within this class are instantiated on first use and guaranteed to be destroyed
class Models {
private: 
  const Shared * const shared; //read only 
  ModelStats *stats; //read-write
public:
  Models(const Shared * const sh, ModelStats *st): shared(sh), stats(st) {}
public:
  NormalModel& normalModel() {static NormalModel instance{shared, stats, MEM*32,MEM};return instance;}
  DmcForest& dmcForest() {static DmcForest instance{shared, MEM};return instance;}
  CharGroupModel& charGroupModel() {static CharGroupModel instance{shared, MEM/2};return instance;}
  RecordModel& recordModel() {static RecordModel instance{shared, stats, MEM*2};return instance;}
  SparseModel& sparseModel() {static SparseModel instance{shared, MEM*2};return instance;}
  MatchModel& matchModel() {static MatchModel instance{shared, stats, MEM*4};return instance;}
  SparseMatchModel& sparseMatchModel() {static SparseMatchModel instance{shared, MEM};return instance;}
  IndirectModel& indirectModel() {static IndirectModel instance{shared, MEM};return instance;}

#ifdef USE_TEXTMODEL
  TextModel& textModel() {static TextModel instance{shared, stats, MEM*16};return instance;}
  WordModel& wordModel() {static WordModel instance{shared, stats, MEM*16};return instance;}
#endif //USE_TEXTMODEL
  NestModel& nestModel() {static NestModel instance{shared, MEM};return instance;}
  XMLModel& xmlModel() {static XMLModel instance{shared, MEM/4};return instance;}

  ExeModel& exeModel() {static ExeModel instance{shared, stats, MEM*4};return instance;}
  LinearPredictionModel& linearPredictionModel() {static LinearPredictionModel instance{shared};return instance;}

  JpegModel& jpegModel() {static JpegModel instance{shared, MEM};return instance;}

  Image24bitModel& image24bitModel() {static Image24bitModel instance{shared, stats, MEM*4};return instance;}
  Image8bitModel& image8bitModel() {static Image8bitModel instance{shared, stats, MEM*4};return instance;}
  Image4bitModel& image4bitModel() {static Image4bitModel instance{shared, MEM/2};return instance;}
  Image1bitModel& image1bitModel() {static Image1bitModel instance{shared};return instance;}

#ifdef USE_AUDIOMODEL
  Audio8bitModel& audio8bitModel() {static Audio8bitModel instance{shared,stats};return instance;}
  Audio16bitModel& audio16bitModel() {static Audio16bitModel instance{shared,stats};return instance;}
#endif //USE_AUDIOMODEL
};

//////////////////////////// contextModel //////////////////////

// This combines all the context models with a Mixer.

class ContextModel {
  const Shared * const shared;
  ModelStats *stats;
  Models models;
  Mixer *m;
  Blocktype next_blocktype=DEFAULT, blocktype=DEFAULT;
  int blocksize=0, blockinfo=0, bytesread=0;
  bool readsize=false;
public:
  ContextModel(const Shared * const sh, ModelStats *st, Models &mdls) : shared(sh), stats(st), models(mdls) {
    m=MixerFactory::CreateMixer(
      sh, 
      1 +  //bias
      MatchModel::MIXERINPUTS  + NormalModel::MIXERINPUTS + SparseMatchModel::MIXERINPUTS + 
      SparseModel::MIXERINPUTS + RecordModel::MIXERINPUTS + CharGroupModel::MIXERINPUTS + 
      TextModel::MIXERINPUTS   + WordModel::MIXERINPUTS   + IndirectModel::MIXERINPUTS + 
      DmcForest::MIXERINPUTS   + NestModel::MIXERINPUTS   + XMLModel::MIXERINPUTS + 
      LinearPredictionModel::MIXERINPUTS  +  ExeModel::MIXERINPUTS
      ,
      MatchModel::MIXERCONTEXTS  + NormalModel::MIXERCONTEXTS + SparseMatchModel::MIXERCONTEXTS +
      SparseModel::MIXERCONTEXTS + RecordModel::MIXERCONTEXTS + CharGroupModel::MIXERCONTEXTS + 
      TextModel::MIXERCONTEXTS   + WordModel::MIXERCONTEXTS   + IndirectModel::MIXERCONTEXTS +
      DmcForest::MIXERCONTEXTS   + NestModel::MIXERCONTEXTS   + XMLModel::MIXERCONTEXTS +
      LinearPredictionModel::MIXERCONTEXTS  +  ExeModel::MIXERCONTEXTS
      ,
      MatchModel::MIXERCONTEXTSETS  + NormalModel::MIXERCONTEXTSETS + SparseMatchModel::MIXERCONTEXTSETS +
      SparseModel::MIXERCONTEXTSETS + RecordModel::MIXERCONTEXTSETS + CharGroupModel::MIXERCONTEXTSETS +
      TextModel::MIXERCONTEXTSETS   + WordModel::MIXERCONTEXTSETS   + IndirectModel::MIXERCONTEXTSETS +
      DmcForest::MIXERCONTEXTSETS   + NestModel::MIXERCONTEXTSETS   + XMLModel::MIXERCONTEXTSETS +
      LinearPredictionModel::MIXERCONTEXTSETS  +  ExeModel::MIXERCONTEXTSETS
    );
  }

  ~ContextModel() {
    delete m;
  }
  int p();
};

int ContextModel::p(){
  U32 &blpos=stats->blpos;
  // Parse block type and block size
  INJECT_SHARED_bpos 
  if (bpos==0) {
    --blocksize;
    blpos++;
    INJECT_SHARED_c1
    if (blocksize==-1) {
      next_blocktype=(Blocktype)c1; //got blocktype but don't switch (we don't have all the info yet)
      bytesread=0;
      readsize=true;
    }
    else if (blocksize<0) {
      if (readsize) {
        bytesread|=int(c1&0x7F)<<((-blocksize-2)*7);
        if ((c1>>7)==0) {
          readsize=false;
          if (!hasInfo(next_blocktype)) {
            blocksize=bytesread;
            if (hasRecursion(next_blocktype))
              blocksize=0;
            blpos=0;
          }
          else
            blocksize=-1;
        }
      }
      else if (blocksize==-5) {
        blocksize=bytesread;
        INJECT_SHARED_c4
        blockinfo=c4;
        blpos=0;
      }
    }

    if (blpos==0) blocktype=next_blocktype; //got all the info - switch to next blocktype
    if (blocksize==0) blocktype=DEFAULT;

    stats->blockType = blocktype;
  }

  m->add(256); //network bias

  MatchModel &matchModel=models.matchModel();
  matchModel.mix(*m);
  NormalModel &normalModel=models.normalModel();
  normalModel.mix(*m);

  // Test for special block types
  switch(blocktype) {
    case IMAGE1: {
      Image1bitModel &image1bitModel=models.image1bitModel();
      image1bitModel.setparam(blockinfo);
      image1bitModel.mix(*m);
      break;
    }
    case IMAGE4: {
      Image4bitModel &image4bitModel=models.image4bitModel();
      image4bitModel.setparam(blockinfo);
      m->set_scalefactor(2048,256);
      return image4bitModel.mix(*m), m->p();
    }
    case IMAGE8: {
      Image8bitModel &image8bitModel=models.image8bitModel();
      image8bitModel.setparam(blockinfo,0,0);
      m->set_scalefactor(2048,128);
      return image8bitModel.mix(*m), m->p();
    }
    case IMAGE8GRAY: {
      Image8bitModel &image8bitModel=models.image8bitModel();
      image8bitModel.setparam(blockinfo,1,0);
      m->set_scalefactor(2048,128);
      return image8bitModel.mix(*m), m->p();
    }
    case IMAGE24: {
      Image24bitModel &image24bitModel=models.image24bitModel();
      image24bitModel.setparam(blockinfo,0,0);
      m->set_scalefactor(1024,128);
      return image24bitModel.mix(*m), m->p();
    }
    case IMAGE32: {
      Image24bitModel &image24bitModel=models.image24bitModel();
      image24bitModel.setparam(blockinfo,1,0);
      m->set_scalefactor(2048,128);
      return image24bitModel.mix(*m), m->p();
    }
    case PNG8: {
      Image8bitModel &image8bitModel=models.image8bitModel();
      image8bitModel.setparam(blockinfo,0,1);
      m->set_scalefactor(2048,128);
      return image8bitModel.mix(*m), m->p();
    }
    case PNG8GRAY: {
      Image8bitModel &image8bitModel=models.image8bitModel();
      image8bitModel.setparam(blockinfo,1,1);
      m->set_scalefactor(2048,128);
      return image8bitModel.mix(*m), m->p();
    }
    case PNG24: {
      Image24bitModel &image24bitModel=models.image24bitModel();
      image24bitModel.setparam(blockinfo,0,1);
      m->set_scalefactor(1024,128);
      return image24bitModel.mix(*m), m->p();
    }
    case PNG32: {
      Image24bitModel &image24bitModel=models.image24bitModel();
      image24bitModel.setparam(blockinfo,1,1);
      m->set_scalefactor(2048,128);
      return image24bitModel.mix(*m), m->p();
    }
    #ifdef USE_AUDIOMODEL
    case AUDIO:
    case AUDIO_LE: {
      RecordModel &recordModel=models.recordModel();
      recordModel.mix(*m);
      if ((blockinfo&2)==0) {
        Audio8bitModel &audio8bitModel=models.audio8bitModel();
        audio8bitModel.setparam(blockinfo);
        m->set_scalefactor(1024,128);
        return audio8bitModel.mix(*m), m->p();
      }
      else {
        Audio16bitModel &audio16bitModel=models.audio16bitModel();
        audio16bitModel.setparam(blockinfo);
        m->set_scalefactor(1024,128);
        return audio16bitModel.mix(*m), m->p();
      }
    }
    #endif //USE_AUDIOMODEL
    case JPEG: {
      JpegModel &jpegModel=models.jpegModel();
      m->set_scalefactor(1024,256);
      if (jpegModel.mix(*m)) return m->p();
    }
    case DEFAULT:
    case HDR: 
    case FILECONTAINER:
    case EXE:
    case CD:
    case ZLIB:
    case BASE64:
    case GIF:
    case TEXT:
    case TEXT_EOL:
    case RLE:
    case LZW:
      break;
  }

  normalModel.mix_post(*m);

  if (blocktype!=IMAGE1) {
    SparseMatchModel &sparseMatchModel=models.sparseMatchModel();
    sparseMatchModel.mix(*m);
    SparseModel &sparseModel=models.sparseModel();
    sparseModel.mix(*m);
    RecordModel &recordModel=models.recordModel();
    recordModel.mix(*m);
    CharGroupModel &charGroupModel=models.charGroupModel();
    charGroupModel.mix(*m);
    #ifdef USE_TEXTMODEL
    TextModel &textModel=models.textModel();
    textModel.mix(*m);
    WordModel &wordModel=models.wordModel();
    wordModel.mix(*m);
    #endif //USE_TEXTMODEL
    IndirectModel &indirectModel=models.indirectModel();
    indirectModel.mix(*m);
    DmcForest &dmcForest=models.dmcForest();
    dmcForest.mix(*m);
    NestModel &nestModel=models.nestModel();
    nestModel.mix(*m);
    XMLModel &xmlModel=models.xmlModel();
    xmlModel.mix(*m);
    if (blocktype!=TEXT && blocktype!=TEXT_EOL) {
      LinearPredictionModel &linearPredictionModel=models.linearPredictionModel();
      linearPredictionModel.mix(*m);
      ExeModel &exeModel=models.exeModel();
      exeModel.mix(*m);
    }
  }

  m->set_scalefactor(1024,128);
  return m->p();
}

/////////////////// Secondary Symbol Estimation ////////////////
//
// Filter the context model with APMs

class SSE {
private:
  const Shared * const shared;
  ModelStats *stats;
  struct {
    APM APMs[4];
    APM1 APM1s[3];
  } Text;
  struct {
    struct {
      APM APMs[4];
      APM1 APM1s[2];
    } Color, Palette;
    struct {
      APM APMs[3];
    } Gray;
  } Image;
  struct {
    APM1 APM1s[7];
  } Generic;
public:
  SSE(const Shared * const sh, ModelStats *st) : shared(sh), stats(st),
  Text {
      { /*APM:*/  {sh,0x10000,24}, {sh,0x10000,24}, {sh,0x10000,24}, {sh,0x10000,24}}, /* APM: contexts, steps */
      { /*APM1:*/ {sh,0x10000,7}, {sh,0x10000,6}, {sh,0x10000,6}} /* APM1: contexts, rate */
    },
    Image { 
      { /*APM:*/ {{sh,0x1000,24}, {sh,0x10000,24}, {sh,0x10000,24}, {sh,0x10000,24}}, /*APM1:*/ {{sh,0x10000,7}, {sh,0x10000,7}} }, // color
      { /*APM:*/ {{sh,0x1000,24}, {sh,0x10000,24}, {sh,0x10000,24}, {sh,0x10000,24}}, /*APM1:*/ {{sh,0x10000,5}, {sh,0x10000,6}} }, // palette
      { /*APM:*/ {{sh,0x1000,24}, {sh,0x10000,24}, {sh,0x10000,24}} } //gray
    },
    Generic 
      { /*APM1:*/ {{sh,0x2000,7}, {sh,0x10000,7}, {sh,0x10000,7}, {sh,0x10000,7}, {sh,0x10000,7}, {sh,0x10000,7}, {sh,0x10000,7}} 
    }
  {}
public:
  int p(int pr0) {
    INJECT_STATS_blpos INJECT_SHARED_c0 INJECT_SHARED_bpos INJECT_SHARED_c4
    int pr;
    int pr1, pr2, pr3;
    switch (stats->blockType) {
      case TEXT: case TEXT_EOL: {
        int limit=0x3FF>>((blpos<0xFFF)*2);
        pr  = Text.APMs[0].p(pr0, (c0<<8)|(stats->Text.mask&0xF)|((stats->Misses&0xF)<<4), limit);
        pr1 = Text.APMs[1].p(pr0, finalize64(hash(bpos, stats->Misses&3, c4&0xffff, stats->Text.mask>>4),16), limit);
        pr2 = Text.APMs[2].p(pr0, finalize64(hash(c0, stats->Match.expectedByte, stats->Match.length3),16), limit);
        pr3 = Text.APMs[3].p(pr0, finalize64(hash(c0, c4&0xffff, stats->Text.firstLetter),16), limit);

        pr0 = (pr0+pr1+pr2+pr3+2)>>2;

        pr1 = Text.APM1s[0].p(pr0, finalize64(hash(stats->Match.expectedByte, stats->Match.length3, c4&0xff),16));
        pr2 = Text.APM1s[1].p(pr, finalize64(hash(c0, c4&0x00ffffff),16));
        pr3 = Text.APM1s[2].p(pr, finalize64(hash(c0, c4&0xffffff00),16));

        pr = (pr+pr1+pr2+pr3+2)>>2;
        pr = (pr+pr0+1)>>1;
        break;
      }
      case IMAGE24: case IMAGE32: {
        int limit=0x3FF>>((blpos<0xFFF)*4);
        pr  = Image.Color.APMs[0].p(pr0, (c0<<4)|(stats->Misses&0xF), limit);
        pr1 = Image.Color.APMs[1].p(pr0, finalize64(hash(c0, stats->Image.pixels.W, stats->Image.pixels.WW),16), limit);
        pr2 = Image.Color.APMs[2].p(pr0, finalize64(hash(c0, stats->Image.pixels.N, stats->Image.pixels.NN),16), limit);
        pr3 = Image.Color.APMs[3].p(pr0, (c0<<8)|stats->Image.ctx, limit);

        pr0 = (pr0+pr1+pr2+pr3+2)>>2;
        pr1 = Image.Color.APM1s[0].p(pr, finalize64(hash(c0, stats->Image.pixels.W, (c4&0xff)-stats->Image.pixels.Wp1, stats->Image.plane),16));
        pr2 = Image.Color.APM1s[1].p(pr, finalize64(hash(c0, stats->Image.pixels.N, (c4&0xff)-stats->Image.pixels.Np1, stats->Image.plane),16));

        pr=(pr*2+pr1*3+pr2*3+4)>>3;
        pr = (pr+pr0+1)>>1;
        break;
      }
      case IMAGE8GRAY: {
        int limit=0x3FF>>((blpos<0xFFF)*4);
        pr  = Image.Gray.APMs[0].p(pr0, (c0<<4)|(stats->Misses&0xF), limit);
        pr1 = Image.Gray.APMs[1].p(pr, (c0<<8)|stats->Image.ctx, limit);
        pr2 = Image.Gray.APMs[2].p(pr0, bpos|(stats->Image.ctx&0xF8)|(stats->Match.expectedByte<<8), limit);

        pr0 = (2*pr0+pr1+pr2+2)>>2;
        pr = (pr+pr0+1)>>1;
        break;
      }
      case IMAGE8: {
        int limit=0x3FF>>((blpos<0xFFF)*4);
        pr  = Image.Palette.APMs[0].p(pr0, (c0<<4)|(stats->Misses&0xF), limit);
        pr1 = Image.Palette.APMs[1].p(pr0, finalize64(hash(c0 | stats->Image.pixels.W<<8 | stats->Image.pixels.N <<16),16), limit);
        pr2 = Image.Palette.APMs[2].p(pr0, finalize64(hash(c0 | stats->Image.pixels.N<<8 | stats->Image.pixels.NN<<16),16), limit);
        pr3 = Image.Palette.APMs[3].p(pr0, finalize64(hash(c0 | stats->Image.pixels.W<<8 | stats->Image.pixels.WW<<16),16), limit);

        pr0 = (pr0+pr1+pr2+pr3+2)>>2;
        pr1 = Image.Palette.APM1s[0].p(pr0, finalize64(hash(c0 | stats->Match.expectedByte<<8 | stats->Image.pixels.N<<16),16));
        pr2 = Image.Palette.APM1s[1].p(pr , finalize64(hash(c0 | stats->Image.pixels.W<<8     | stats->Image.pixels.N<<16),16));

        pr = (pr*2+pr1+pr2+2)>>2;
        pr = (pr+pr0+1)>>1;
        break;
      }
      default: {
        pr  = Generic.APM1s[0].p(pr0, (stats->Match.length3)<<11 | c0<<3 | (stats->Misses&0x7));
        const U16 ctx1 = c0 | (c4&0xff)<<8;
        const U16 ctx2 = c0 ^ finalize64(hash(c4&0xffff),16);
        const U16 ctx3 = c0 ^ finalize64(hash(c4&0xffffff),16);
        pr1 = Generic.APM1s[1].p(pr0, ctx1);
        pr2 = Generic.APM1s[2].p(pr0, ctx2);
        pr3 = Generic.APM1s[3].p(pr0, ctx3);

        pr0 = (pr0+pr1+pr2+pr3+2)>>2;
        pr1 = Generic.APM1s[4].p(pr, (stats->Match.expectedByte<<8)|(c4&0xff));
        pr2 = Generic.APM1s[5].p(pr, ctx2);
        pr3 = Generic.APM1s[6].p(pr, ctx3);

        pr = (pr+pr1+pr2+pr3+2)>>2;
        pr = (pr+pr0+1)>>1;
      }
    }
    return pr;
  }
};

//////////////////////////// Predictor /////////////////////////

// A Predictor estimates the probability that the next bit of
// uncompressed data is 1.  Methods:
// p() returns P(1) as a 12 bit number (0-4095).
// update() trains the models with the actual bit (0 or 1).

class Predictor {
  Shared shared;
  ModelStats stats;
  Models models;
  ContextModel contextModel;
  SSE sse;
  int pr;  // next prediction, scaled by 12 bits (0-4095)
  
  void trainText(const char * const Dictionary, int Iterations) {
    NormalModel &normalModel=models.normalModel();
    WordModel &wordModel=models.wordModel();
    DummyMixer m_dummy(&shared,
      normalModel.MIXERINPUTS+wordModel.MIXERINPUTS,
      normalModel.MIXERCONTEXTS+wordModel.MIXERCONTEXTS,
      normalModel.MIXERCONTEXTSETS+wordModel.MIXERCONTEXTSETS
    );
    assert(shared.buf.getpos()==0 && stats.blpos==0);
    FileDisk f;
    printf("Pre-training models with text...");
    OpenFromMyFolder::anotherfile(&f, Dictionary);
    int c;
    int training_bytecount=0;
    while (Iterations-->0) {
      f.setpos(0);
      c=SPACE;
      training_bytecount=0;
      do {
        training_bytecount++;
        U8 c1 = c==NEW_LINE ? SPACE : c;
        if(c!=CARRIAGE_RETURN) {
          for (int bpos=0; bpos<8; bpos++) {
            normalModel.mix(m_dummy); //update (train) model
              wordModel.mix(m_dummy); //update (train) model
            m_dummy.p();
            shared.y=(c1>>(7-bpos))&1;
            shared.update();
            updater.broadcast_update();
          }
        } 
        // emulate a space before and after each word/expression
        // reset models in between
        if(c==NEW_LINE) {
          normalModel.reset();
          wordModel.reset();
          for (int bpos=0; bpos<8; bpos++) {
            normalModel.mix(m_dummy); //update (train) model
            wordModel.mix(m_dummy); //update (train) model
            m_dummy.p();
            shared.y=(c1>>(7-bpos))&1;
            shared.update();
            updater.broadcast_update();
          }
        }
      } while ((c=f.getchar())!=EOF);
    }
    normalModel.reset();
    wordModel.reset();
    shared.reset();
    stats.reset();
    printf(" done [%s, %d bytes]\n", Dictionary, training_bytecount);
    f.close();
  }
  
  void trainExe() {
    ExeModel &exeModel=models.exeModel();
    DummyMixer dummy_m(&shared,exeModel.MIXERINPUTS,exeModel.MIXERCONTEXTS,exeModel.MIXERCONTEXTSETS);
    assert(shared.buf.getpos()==0 && stats.blpos==0);
    FileDisk f;
    printf("Pre-training x86/x64 model...");
    OpenFromMyFolder::myself(&f);
    int c=0;
    int training_bytecount=0;
    do {
      training_bytecount++;
      for (int bpos=0; bpos<8; bpos++) {
        exeModel.mix(dummy_m); //update (train) model
        dummy_m.p();
        shared.y=(c>>(7-bpos))&1;
        shared.update();
        updater.broadcast_update();
      }
    } while ((c=f.getchar())!=EOF);
    printf(" done [%d bytes]\n",training_bytecount);
    f.close();
    shared.reset();
    stats.reset();
  }
  
public:
  Predictor(): shared(), stats(), models(&shared,&stats), contextModel(&shared, &stats, models), sse(&shared,&stats), pr(2048) {
    shared.reset();
    shared.buf.setsize(MEM*8);
    //initiate pre-traing
    if(options & OPTION_TRAINTXT) {
      trainText("english.dic",3);
      trainText("english.exp",1);
    }
    if(options & OPTION_TRAINEXE)
      trainExe();
  }
  int p() const {return pr;}
  void update(U8 y) {
    stats.Misses+=stats.Misses+((pr>>11)!=y);
    
    // Update global context: pos, bpos, c0, c4, c8, buf
    shared.y=y;
    shared.update(); 
    // Broadcast to all current subscribers: y (and c0, c1, c4, etc) is known
    updater.broadcast_update();

    const U8 bpos=shared.bpos;
    const U8 c0=shared.c0;
    const U8 chargrp = (bpos>0)?AsciiGroupC0[0][(1<<bpos)-2+(c0&((1<<bpos)-1))]:0;
    stats.Text.chargrp = chargrp;

    // Predict
    pr=contextModel.p();

    // SSE Stage
    pr=sse.p(pr);
  }
};

//////////////////////////// Encoder ////////////////////////////

// An Encoder does arithmetic encoding.  Methods:
// Encoder(COMPRESS, f) creates encoder for compression to archive f, which
//   must be open past any header for writing in binary mode.
// Encoder(DECOMPRESS, f) creates encoder for decompression from archive f,
//   which must be open past any header for reading in binary mode.
// code(i) in COMPRESS mode compresses bit i (0 or 1) to file f.
// code() in DECOMPRESS mode returns the next decompressed bit from file f.
//   Global y is set to the last bit coded or decoded by code().
// compress(c) in COMPRESS mode compresses one byte.
// decompress() in DECOMPRESS mode decompresses and returns one byte.
// flush() should be called exactly once after compression is done and
//   before closing f.  It does nothing in DECOMPRESS mode.
// size() returns current length of archive
// setFile(f) sets alternate source to File *f for decompress() in COMPRESS
//   mode (for testing transforms).
// If level (global) is 0, then data is stored without arithmetic coding.

typedef enum {COMPRESS, DECOMPRESS} Mode;
class Encoder {
private:
  Predictor predictor;
  const Mode mode;       // Compress or decompress?
  File *archive;         // Compressed data file
  U32 x1, x2;            // Range, initially [0, 1), scaled by 2^32
  U32 x;                 // Decompress mode: last 4 input bytes of archive
  File *alt;             // decompress() source in COMPRESS mode
  float p1, p2;          // percentages for progress indicator: 0.0 .. 1.0

  // Compress bit y or return decompressed bit
  int code(int i=0) {
    int p=predictor.p();
    if(p==0)p++;
    assert(p>0 && p<4096);
    U32 xmid=x1 + ((x2-x1)>>12)*p + (((x2-x1)&0xfff)*p>>12);
    assert(xmid>=x1 && xmid<x2);
    U8 y = (mode==DECOMPRESS) ? x<=xmid : i;
    y ? (x2=xmid) : (x1=xmid+1);
    while (((x1^x2)&0xff000000)==0) {  // pass equal leading bytes of range
      if (mode==COMPRESS) archive->putchar(x2>>24);
      x1<<=8;
      x2=(x2<<8)+255;
      if (mode==DECOMPRESS) x=(x<<8)+(archive->getchar()&255);  // EOF is OK
    }
    predictor.update(y);
    return y;
  }

public:
  Encoder(Mode m, File *f);
  Mode getMode() const {return mode;}
  U64 size() const {return archive->curpos();}  // length of archive so far
  void flush();  // call this when compression is finished
  void setFile(File *f) {alt=f;}

  // Compress one byte
  void compress(int c) {
    assert(mode==COMPRESS);
    if (level==0)
      archive->putchar(c);
    else
      for (int i=7; i>=0; --i)
        code((c>>i)&1);
  }

  // Decompress and return one byte
  int decompress() {
    if (mode==COMPRESS) {
      assert(alt);
      return alt->getchar();
    }
    else if (level==0)
      return archive->getchar();
    else {
      int c=0;
      for (int i=0; i<8; ++i)
        c+=c+code();
      return c;
    }
  }

  void encode_blocksize(U64 blocksize) {
    //TODO: Large file support
    while (blocksize>0x7F) {
      compress(0x80|(blocksize&0x7F));
      blocksize>>=7;
    }
    compress(U8(blocksize));
  }

  U64 decode_blocksize() {
    //TODO: Large file support
    U64 blocksize=0;
    U8 B=0;
    int i=0;
    do {
      B=decompress();
      blocksize|=U64((B&0x7F)<<i);
      i+=7;
    } while ((B>>7)>0);
    return blocksize;
  }

  void set_status_range(float perc1, float perc2) { p1=perc1; p2=perc2; }
  void print_status(U64 n, U64 size) {
    fprintf(stderr,"%6.2f%%\b\b\b\b\b\b\b", (p1+(p2-p1)*n/(size+1))*100);
    fflush(stderr);
  }
  void print_status() {
    fprintf(stderr,"%6.2f%%\b\b\b\b\b\b\b", float(size())/(p2+1)*100);
    fflush(stderr);
  }
};

Encoder::Encoder(Mode m, File *f):
    mode(m), archive(f), x1(0), x2(0xffffffff), x(0), alt(0) {
  if (mode==DECOMPRESS) {
    U64 start=size();
    archive->setend();
    U64 end=size();
    if (end>=(UINT64_C(1)<<31))quit("Large archives not yet supported.");
    set_status_range(0.0, (float)end);
    archive->setpos(start);
  }
  if (level>0 && mode==DECOMPRESS) {  // x = first 4 bytes of archive
    for (int i=0; i<4; ++i)
      x=(x<<8)+(archive->getchar()&255);
  }
}

void Encoder::flush() {
  if (mode==COMPRESS && level>0)
    archive->putchar(x1>>24);  // Flush first unequal byte of range
}

/////////////////////////// Filters /////////////////////////////////
//
// Before compression, data is encoded in blocks with the following format:
//
//   <type> <size> <encoded-data>
//
// Type is 1 byte (type Blocktype): DEFAULT=0, JPEG, EXE
// Size is 4 bytes in big-endian format.
// Encoded-data decodes to <size> bytes.  The encoded size might be
// different.  Encoded data is designed to be more compressible.
//
//   void encode(File *in, File *out, int n);
//
// Reads n bytes of in (open in "rb" mode) and encodes one or
// more blocks to temporary file out (open in "wb+" mode).
// The file pointer of in is advanced n bytes.  The file pointer of
// out is positioned after the last byte written.
//
//   en.setFile(File *out);
//   int decode(Encoder& en);
//
// Decodes and returns one byte.  Input is from en.decompress(), which
// reads from out if in COMPRESS mode.  During compression, n calls
// to decode() must exactly match n bytes of in, or else it is compressed
// as type 0 without encoding.
//
//   Blocktype detect(File *in, int n, Blocktype type);
//
// Reads n bytes of in, and detects when the type changes to
// something else.  If it does, then the file pointer is repositioned
// to the start of the change and the new type is returned.  If the type
// does not change, then it repositions the file pointer n bytes ahead
// and returns the old type.
//
// For each type X there are the following 2 functions:
//
//   void encode_X(File *in, File *out, int n, ...);
//
// encodes n bytes from in to out.
//
//   int decode_X(Encoder& en);
//
// decodes one byte from en and returns it.  decode() and decode_X()
// maintain state information using static variables.


#define IMG_DET(type,start_pos,header_len,width,height) return dett=(type),\
deth=(header_len),detd=(width)*(height),info=(width),\
in->setpos(start+(start_pos)),HDR

#define AUD_DET(type,start_pos,header_len,data_len,wmode) return dett=(type),\
deth=(header_len),detd=(data_len),info=(wmode),\
in->setpos(start+(start_pos)),HDR


// Function ecc_compute(), edc_compute() and eccedc_init() taken from
// ** UNECM - Decoder for ECM (Error Code Modeler) format.
// ** Version 1.0
// ** Copyright (C) 2002 Neill Corlett

/* LUTs used for computing ECC/EDC */
static U8 ecc_f_lut[256];
static U8 ecc_b_lut[256];
static U32 edc_lut[256];
static int luts_init=0;

void eccedc_init(void) {
  if (luts_init) return;
  U32 i, j, edc;
  for(i = 0; i < 256; i++) {
    j = (i << 1) ^ (i & 0x80 ? 0x11D : 0);
    ecc_f_lut[i] = j;
    ecc_b_lut[i ^ j] = i;
    edc = i;
    for(j = 0; j < 8; j++) edc = (edc >> 1) ^ (edc & 1 ? 0xD8018001 : 0);
    edc_lut[i] = edc;
  }
  luts_init=1;
}

void ecc_compute(U8 *src, U32 major_count, U32 minor_count, U32 major_mult, U32 minor_inc, U8 *dest) {
  U32 size = major_count * minor_count;
  U32 major, minor;
  for(major = 0; major < major_count; major++) {
    U32 index = (major >> 1) * major_mult + (major & 1);
    U8 ecc_a = 0;
    U8 ecc_b = 0;
    for(minor = 0; minor < minor_count; minor++) {
      U8 temp = src[index];
      index += minor_inc;
      if(index >= size) index -= size;
      ecc_a ^= temp;
      ecc_b ^= temp;
      ecc_a = ecc_f_lut[ecc_a];
    }
    ecc_a = ecc_b_lut[ecc_f_lut[ecc_a] ^ ecc_b];
    dest[major              ] = ecc_a;
    dest[major + major_count] = ecc_a ^ ecc_b;
  }
}

U32 edc_compute(const U8  *src, int size) {
  U32 edc = 0;
  while(size--) edc = (edc >> 8) ^ edc_lut[(edc ^ (*src++)) & 0xFF];
  return edc;
}

int expand_cd_sector(U8 *data, int address, int test) {
  U8 d2[2352];
  eccedc_init();
  //sync pattern: 00 FF FF FF FF FF FF FF FF FF FF 00
  d2[0]=d2[11]=0;
  for (int i=1; i<11; i++) d2[i]=255;
  //determine Mode and Form
  int mode=(data[15]!=1?2:1);
  int form=(data[15]==3?2:1);
  //address (Minutes, Seconds, Sectors)
  if (address==-1) for (int i=12; i<15; i++) d2[i]=data[i]; else {
    int c1=(address&15)+((address>>4)&15)*10;
    int c2=((address>>8)&15)+((address>>12)&15)*10;
    int c3=((address>>16)&15)+((address>>20)&15)*10;
    c1=(c1+1)%75;
    if (c1==0) {
      c2=(c2+1)%60;
      if (c2==0) c3++;
    }
    d2[12]=(c3%10)+16*(c3/10);
    d2[13]=(c2%10)+16*(c2/10);
    d2[14]=(c1%10)+16*(c1/10);
  }
  d2[15]=mode;
  if (mode==2) for (int i=16; i<24; i++) d2[i]=data[i-4*(i>=20)]; //8 byte subheader
  if (form==1) {
    if (mode==2) {
      d2[1]=d2[12],d2[2]=d2[13],d2[3]=d2[14];
      d2[12]=d2[13]=d2[14]=d2[15]=0;
    } else {
      for(int i=2068; i<2076; i++) d2[i]=0; //Mode1: reserved 8 (zero) bytes
    }
    for (int i=16+8*(mode==2); i<2064+8*(mode==2); i++) d2[i]=data[i]; //data bytes
    U32 edc=edc_compute(d2+16*(mode==2), 2064-8*(mode==2));
    for (int i=0; i<4; i++) d2[2064+8*(mode==2)+i]=(edc>>(8*i))&0xff;
    ecc_compute(d2+12, 86, 24,  2, 86, d2+2076);
    ecc_compute(d2+12, 52, 43, 86, 88, d2+2248);
    if (mode==2) {
      d2[12]=d2[1],d2[13]=d2[2],d2[14]=d2[3],d2[15]=2;
      d2[1]=d2[2]=d2[3]=255;
    }
  }
  for (int i=0; i<2352; i++) if (d2[i]!=data[i] && test) form=2;
  if (form==2) {
    for (int i=24; i<2348; i++) d2[i]=data[i]; //data bytes
    U32 edc=edc_compute(d2+16, 2332);
    for (int i=0; i<4; i++) d2[2348+i]=(edc>>(8*i))&0xff; //EDC
  }
  for (int i=0; i<2352; i++) if (d2[i]!=data[i] && test) return 0; else data[i]=d2[i];
  return mode+form-1;
}

#ifdef USE_ZLIB
int parse_zlib_header(int header) {
    switch (header) {
        case 0x2815 : return 0;  case 0x2853 : return 1;  case 0x2891 : return 2;  case 0x28cf : return 3;
        case 0x3811 : return 4;  case 0x384f : return 5;  case 0x388d : return 6;  case 0x38cb : return 7;
        case 0x480d : return 8;  case 0x484b : return 9;  case 0x4889 : return 10; case 0x48c7 : return 11;
        case 0x5809 : return 12; case 0x5847 : return 13; case 0x5885 : return 14; case 0x58c3 : return 15;
        case 0x6805 : return 16; case 0x6843 : return 17; case 0x6881 : return 18; case 0x68de : return 19;
        case 0x7801 : return 20; case 0x785e : return 21; case 0x789c : return 22; case 0x78da : return 23;
    }
    return -1;
}
int zlib_inflateInit(z_streamp strm, int zh) {
    if (zh==-1) return inflateInit2(strm, -MAX_WBITS); else return inflateInit(strm);
}
#endif //USE_ZLIB

bool IsGrayscalePalette(File *in, int n = 256, int isRGBA = 0){
  U64 offset = in->curpos();
  int stride = 3+isRGBA, res = (n>0)<<8, order=1;
  for (int i = 0; (i < n*stride) && (res>>8); i++) {
    int b = in->getchar();
    if (b==EOF){
      res = 0;
      break;
    }
    if (!i) {
      res = 0x100|b;
      order = 1-2*(b>int(ilog2(n)/4));
      continue;
    }
    //"j" is the index of the current byte in this color entry
    int j = i%stride;
    if (!j){
      // load first component of this entry
      int k = (b-(res&0xFF))*order;
      res = res&((k>=0 && k<=8)<<8);
      res|=(res)?b:0;
    }
    else if (j==3)
      res&=((!b || (b==0xFF))*0x1FF); // alpha/attribute component must be zero or 0xFF
    else
      res&=((b==(res&0xFF))*0x1FF);
  }
  in->setpos(offset);
  return (res>>8)>0;
}


#define TEXT_MIN_SIZE 1500   // size of minimum allowed text block (in bytes)
#define TEXT_MAX_MISSES 2    // threshold: max allowed number of invalid UTF8 sequences seen recently before reporting "fail"
#define TEXT_ADAPT_RATE 256  // smaller (like 32) = illegal sequences are allowed to come more often, larger (like 1024) = more rigorous detection

struct TextParserStateInfo {
  Array<U64> _start;
  Array<U64> _end;      // position of last char with a valid UTF8 state: marks the end of the detected TEXT block
  Array<U32> _EOLType;  // 0: none or CR-only;   1: CRLF-only (applicable to EOL transform);   2: mixed or LF-only
  U32 invalidCount;     // adaptive count of invalid UTF8 sequences seen recently
  U32 UTF8State;        // state of utf8 parser; 0: valid;  12: invalid;  any other value: yet uncertain (more bytes must be read)
  TextParserStateInfo(): _start(0), _end(0), _EOLType(0) {}
  void reset(U64 startpos) {
    _start.resize(1);
    _end.resize(1);
    _start[0]=startpos;
    _end[0]=startpos-1;
    _EOLType.resize(1);
    _EOLType[0]=0;
    invalidCount=0;
    UTF8State=0;
  }
  U64 start(){return _start[_start.size()-1];}
  U64 end(){return _end[_end.size()-1];}
  void setend(U64 end){_end[_end.size()-1]=end;}
  U32 EOLType(){return _EOLType[_EOLType.size()-1];}
  void setEOLType(U32 EOLType){_EOLType[_EOLType.size()-1]=EOLType;}
  U64 validlength(){return end() - start() + 1;}
  void next(U64 startpos) {
    _start.push_back(startpos);
    _end.push_back(startpos-1);
    _EOLType.push_back(0);
    invalidCount=0;
    UTF8State=0;
  }
  void removefirst() {
    if(_start.size()==1)
      reset(0);
    else {
      for(int i=0;i<(int)_start.size()-1;i++){
        _start[i]=_start[i+1];
        _end[i]=_end[i+1];
        _EOLType[i]=_EOLType[i+1];
      }
      _start.pop_back();
      _end.pop_back();
      _EOLType.pop_back();
    }
  }
} textparser;


// Detect blocks
Blocktype detect(File *in, U64 blocksize, Blocktype type, int &info) {
  //TODO: Large file support
  int n = (int)blocksize;
  U32 buf3=0, buf2=0, buf1=0, buf0=0;  // last 16 bytes
  U64 start= in->curpos();

  // For EXE detection
  Array<int> abspos(256),  // CALL/JMP abs. addr. low byte -> last offset
    relpos(256);    // CALL/JMP relative addr. low byte -> last offset
  int e8e9count=0;  // number of consecutive CALL/JMPs
  int e8e9pos=0;    // offset of first CALL or JMP instruction
  int e8e9last=0;   // offset of most recent CALL or JMP

  int soi=0, sof=0, sos=0, app=0;  // For JPEG detection - position where found
#ifdef USE_AUDIOMODEL
  int wavi=0,wavsize=0,wavch=0,wavbps=0,wavm=0,wavtype=0,wavlen=0,wavlist=0;  // For WAVE detection
  int aiff=0,aiffm=0,aiffs=0;  // For AIFF detection
  int s3mi=0,s3mno=0,s3mni=0;  // For S3M detection
#endif //  USE_AUDIOMODEL
  U32 bmpi=0,dibi=0,imgbpp=0,bmpx=0,bmpy=0,bmpof=0,bmps=0,n_colors=0;  // For BMP detection
  int rgbi=0,rgbx=0,rgby=0;  // For RGB detection
  int tga=0,tgax=0,tgay=0,tgaz=0,tgat=0,tgaid=0,tgamap=0;  // For TGA detection
  // ascii images are currently not supported
  int pgm=0,pgmcomment=0,pgmw=0,pgmh=0,pgm_ptr=0,pgmc=0,pgmn=0,pamatr=0,pamd=0,pgmdata=0,pgmdatasize=0;  // For PBM, PGM, PPM, PAM detection
  char pgm_buf[32];
  int cdi=0,cda=0,cdm=0;  // For CD sectors detection
  U32 cdf=0;
  unsigned char zbuf[256+32] = {0}, zin[1<<16] = {0}, zout[1<<16] = {0}; // For ZLIB stream detection
  int zbufpos=0,zzippos=-1, histogram[256]={0};
  int pdfim=0,pdfimw=0,pdfimh=0,pdfimb=0,pdfgray=0,pdfimp=0;
  int b64s=0,b64i=0,b64line=0,b64nl=0; // For base64 detection
  int gif=0,gifa=0,gifi=0,gifw=0,gifc=0,gifb=0,gifplt=0; // For GIF detection
  static int gifgray=0;
  int png=0, pngw=0, pngh=0, pngbps=0, pngtype=0, pnggray=0, lastchunk=0, nextchunk=0; // For PNG detection
  // For image detection
  static int deth=0,detd=0;  // detected header/data size in bytes
  static Blocktype dett;  // detected block type
  if (deth) {in->setpos(start+deth);deth=0;return dett;}
  else if (detd) {in->setpos(start+min(blocksize, (U64)detd));detd=0;return DEFAULT;}

  textparser.reset(0);
  for (int i=0; i<n; ++i) {
    int c=in->getchar();
    if (c==EOF) return (Blocktype)(-1);
    buf3=buf3<<8|buf2>>24;
    buf2=buf2<<8|buf1>>24;
    buf1=buf1<<8|buf0>>24;
    buf0=buf0<<8|c;

    // detect PNG images
    if (!png && buf3==0x89504E47 /*%PNG*/ && buf2==0x0D0A1A0A && buf1==0x0000000D && buf0==0x49484452) png=i, pngtype=-1, lastchunk=buf3;
    if (png){
      const int p=i-png;
      if (p==12){
        pngw = buf2;
        pngh = buf1;
        pngbps = buf0>>24;
        pngtype = (U8)(buf0>>16);
        pnggray = 0;
        png*=((buf0&0xFFFF)==0 && pngw && pngh && pngbps==8 && (!pngtype || pngtype==2 || pngtype==3 || pngtype==4 || pngtype==6));
      }
      else if (p>12 && pngtype<0)
        png = 0;
      else if (p==17){
        png*=((buf1&0xFF)==0);
        nextchunk =(png)?i+8:0;
      }
      else if (p>17 && i==nextchunk){
        nextchunk+=buf1+4/*CRC*/+8/*Chunk length+Id*/;
        lastchunk = buf0;
        png*=(lastchunk!=0x49454E44/*IEND*/);
        if (lastchunk==0x504C5445/*PLTE*/){
          png*=(buf1%3==0);
          pnggray = (png && IsGrayscalePalette(in, buf1/3));
        }
      }
    }

    // ZLIB stream detection
    #ifdef USE_ZLIB
    histogram[c]++;
    if (i>=256)
      histogram[zbuf[zbufpos]]--;
    zbuf[zbufpos] = c;
    if (zbufpos<32)
      zbuf[zbufpos+256] = c;
    zbufpos=(zbufpos+1)&0xFF;

    int zh=parse_zlib_header(((int)zbuf[(zbufpos-32)&0xFF])*256+(int)zbuf[(zbufpos-32+1)&0xFF]);
    bool valid = (i>=31 && zh!=-1);
    if (!valid && options&OPTION_BRUTE && i>=255){
      U8 BTYPE = (zbuf[zbufpos]&7)>>1;
      if ((valid=(BTYPE==1 || BTYPE==2))){
        int maximum=0, used=0, offset=zbufpos;
        for (int i=0;i<4;i++,offset+=64){
          for (int j=0;j<64;j++){
            int freq = histogram[zbuf[(offset+j)&0xFF]];
            used+=(freq>0);
            maximum+=(freq>maximum);
          }
          if (maximum>=((12+i)<<i) || used*(6-i)<(i+1)*64){
            valid = false;
            break;
          }
        }
      }
    }
    if (valid || zzippos==i) {
      int streamLength=0, ret=0, brute=(zh==-1 && zzippos!=i);

      // Quick check possible stream by decompressing first 32 bytes
      z_stream strm;
      strm.zalloc=Z_NULL; strm.zfree=Z_NULL; strm.opaque=Z_NULL;
      strm.next_in=Z_NULL; strm.avail_in=0;
      if (zlib_inflateInit(&strm,zh)==Z_OK) {
        strm.next_in=&zbuf[(zbufpos-(brute?0:32))&0xFF]; strm.avail_in=32;
        strm.next_out=zout; strm.avail_out=1<<16;
        ret=inflate(&strm, Z_FINISH);
        ret=(inflateEnd(&strm)==Z_OK && (ret==Z_STREAM_END || ret==Z_BUF_ERROR) && strm.total_in>=16);
      }
      if (ret) {
        // Verify valid stream and determine stream length
        const U64 savedpos=in->curpos();
        strm.zalloc=Z_NULL; strm.zfree=Z_NULL; strm.opaque=Z_NULL;
        strm.next_in=Z_NULL; strm.avail_in=0; strm.total_in=strm.total_out=0;
        if (zlib_inflateInit(&strm,zh)==Z_OK) {
          for (int j=i-(brute?255:31); j<n; j+=1<<16) {
            unsigned int blsize=min(n-j,1<<16);
            in->setpos(start+j);
            if (in->blockread(zin, blsize)!=blsize) break;
            strm.next_in=zin; strm.avail_in=blsize;
            do {
              strm.next_out=zout; strm.avail_out=1<<16;
              ret=inflate(&strm, Z_FINISH);
            } while (strm.avail_out==0 && ret==Z_BUF_ERROR);
            if (ret==Z_STREAM_END) streamLength=strm.total_in;
            if (ret!=Z_BUF_ERROR) break;
          }
          if (inflateEnd(&strm)!=Z_OK) streamLength=0;
        }
        in->setpos(savedpos);
      }
      if (streamLength>(brute<<7)) {
        info=0;
        if (pdfimw>0 && pdfimw<0x1000000 && pdfimh>0) {
          if (pdfimb==8 && (int)strm.total_out==pdfimw*pdfimh) info=((pdfgray?IMAGE8GRAY:IMAGE8)<<24)|pdfimw;
          if (pdfimb==8 && (int)strm.total_out==pdfimw*pdfimh*3) info=(IMAGE24<<24)|pdfimw*3;
          if (pdfimb==4 && (int)strm.total_out==((pdfimw+1)/2)*pdfimh) info=(IMAGE4<<24)|((pdfimw+1)/2);
          if (pdfimb==1 && (int)strm.total_out==((pdfimw+7)/8)*pdfimh) info=(IMAGE1<<24)|((pdfimw+7)/8);
          pdfgray=0;
        }
        else if (png && pngw<0x1000000 && lastchunk==0x49444154/*IDAT*/){
          if (pngbps==8 && pngtype==2 && (int)strm.total_out==(pngw*3+1)*pngh) info=(PNG24<<24)|(pngw*3), png=0;
          else if (pngbps==8 && pngtype==6 && (int)strm.total_out==(pngw*4+1)*pngh) info=(PNG32<<24)|(pngw*4), png=0;
          else if (pngbps==8 && (!pngtype || pngtype==3) && (int)strm.total_out==(pngw+1)*pngh) info=(((!pngtype || pnggray)?PNG8GRAY:PNG8)<<24)|(pngw), png=0;
        }
        in->setpos(start+i-(brute?255:31));
        detd=streamLength;
        return ZLIB;
      }
    }
    if (zh==-1 && zbuf[(zbufpos-32)&0xFF]=='P' && zbuf[(zbufpos-32+1)&0xFF]=='K' && zbuf[(zbufpos-32+2)&0xFF]=='\x3'
      && zbuf[(zbufpos-32+3)&0xFF]=='\x4' && zbuf[(zbufpos-32+8)&0xFF]=='\x8' && zbuf[(zbufpos-32+9)&0xFF]=='\0') {
        int nlen=(int)zbuf[(zbufpos-32+26)&0xFF]+((int)zbuf[(zbufpos-32+27)&0xFF])*256
                +(int)zbuf[(zbufpos-32+28)&0xFF]+((int)zbuf[(zbufpos-32+29)&0xFF])*256;
        if (nlen<256 && i+30+nlen<n) zzippos=i+30+nlen;
    }
    #endif //USE_ZLIB

    if (i-pdfimp>1024) pdfim=pdfimw=pdfimh=pdfimb=pdfgray=0; // fail
    if (pdfim>1 && !(isspace(c) || isdigit(c))) pdfim=1;
    if (pdfim==2 && isdigit(c)) pdfimw=pdfimw*10+(c-'0');
    if (pdfim==3 && isdigit(c)) pdfimh=pdfimh*10+(c-'0');
    if (pdfim==4 && isdigit(c)) pdfimb=pdfimb*10+(c-'0');
    if ((buf0&0xffff)==0x3c3c) pdfimp=i,pdfim=1; // <<
    if (pdfim && (buf1&0xffff)==0x2f57 && buf0==0x69647468) pdfim=2,pdfimw=0; // /Width
    if (pdfim && (buf1&0xffffff)==0x2f4865 && buf0==0x69676874) pdfim=3,pdfimh=0; // /Height
    if (pdfim && buf3==0x42697473 && buf2==0x50657243 && buf1==0x6f6d706f
       && buf0==0x6e656e74 && zbuf[(zbufpos-32+15)&0xFF]=='/') pdfim=4,pdfimb=0; // /BitsPerComponent
    if (pdfim && (buf2&0xFFFFFF)==0x2F4465 && buf1==0x76696365 && buf0==0x47726179) pdfgray=1; // /DeviceGray

    // CD sectors detection (mode 1 and mode 2 form 1+2 - 2352 bytes)
    if (buf1==0x00ffffff && buf0==0xffffffff && !cdi) cdi=i,cda=-1,cdm=0;
    if (cdi && i>cdi) {
      const int p=(i-cdi)%2352;
      if (p==8 && (buf1!=0xffffff00 || ((buf0&0xff)!=1 && (buf0&0xff)!=2))) cdi=0;
      else if (p==16 && i+2336<n) {
        U8 data[2352];
        const U64 savedpos=in->curpos();
        in->setpos(start+i-23);
        in->blockread(data, 2352);
        in->setpos(savedpos);
        int t=expand_cd_sector(data, cda, 1);
        if (t!=cdm) cdm=t*(i-cdi<2352);
        if (cdm && cda!=10 && (cdm==1 || buf0==buf1)) {
          if (type!=CD) return info=cdm, in->setpos(start+cdi-7), CD;
          cda=(data[12]<<16)+(data[13]<<8)+data[14];
          if (cdm!=1 && i-cdi>2352 && buf0!=cdf) cda=10;
          if (cdm!=1) cdf=buf0;
        } else cdi=0;
      }
      if (!cdi && type==CD) {
        in->setpos(start+i-p-7);
        return DEFAULT;
      }
    }
    if (type==CD) continue;

    // Detect JPEG by code SOI APPx (FF D8 FF Ex) followed by
    // SOF0 (FF C0 xx xx 08) and SOS (FF DA) within a reasonable distance.
    // Detect end by any code other than RST0-RST7 (FF D9-D7) or
    // a byte stuff (FF 00).

    if (!soi && i>=3 && (buf0&0xffffff00)==0xffd8ff00 && ((buf0&0xFE)==0xC0 || (U8)buf0==0xC4 || ((U8)buf0>=0xDB && (U8)buf0<=0xFE) )) soi=i, app=i+2, sos=sof=0;
    if (soi) {
      if (app==i && (buf0>>24)==0xff &&
         ((buf0>>16)&0xff)>0xc1 && ((buf0>>16)&0xff)<0xff) app=i+(buf0&0xffff)+2;
      if (app<i && (buf1&0xff)==0xff && (buf0&0xfe0000ff)==0xc0000008) sof=i;
      if (sof && sof>soi && i-sof<0x1000 && (buf0&0xffff)==0xffda) {
        sos=i;
        if (type!=JPEG) return in->setpos(start+soi-3), JPEG;
      }
      if (i-soi>0x40000 && !sos) soi=0;
    }
    if (type==JPEG && sos && i>sos && (buf0&0xff00)==0xff00
        && (buf0&0xff)!=0 && (buf0&0xf8)!=0xd0) return DEFAULT;
#ifdef USE_AUDIOMODEL
    // Detect .wav file header
    if (buf0==0x52494646) wavi=i,wavm=wavlen=0; //"RIFF"
    if (wavi) {
      int p=i-wavi;
      if (p==4) wavsize=bswap(buf0); //filesize
      else if (p==8){
        wavtype=(buf0==0x57415645 /*WAVE*/)?1:(buf0==0x7366626B /*sfbk*/)?2:0; 
        if (!wavtype) wavi=0;
      }
      else if (wavtype){
        if (wavtype==1) {
          if (p==16+wavlen && (buf1!=0x666d7420 /*"fmt "*/|| ((wavm=bswap(buf0)-16)&0xFFFFFFFD)!=0)) wavlen=((bswap(buf0)+1)&(-2))+8, wavi*=(buf1==0x666d7420 /*"fmt "*/ && (wavm&0xFFFFFFFD)!=0);
          else if (p==22+wavlen) wavch=bswap(buf0)&0xffff; // number of channels: 1 or 2
          else if (p==34+wavlen) wavbps=bswap(buf0)&0xffff; // bits per smaple: 8 or 16
          else if (p==40+wavlen+wavm && buf1!=0x64617461 /*"data"*/) wavm+=((bswap(buf0)+1)&(-2))+8,wavi=(wavm>0xfffff?0:wavi);
          else if (p==40+wavlen+wavm) {
            int wavd=bswap(buf0); // size of data section
            wavlen=0;
            if ((wavch==1 || wavch==2) && (wavbps==8 || wavbps==16) && wavd>0 && wavsize>=wavd+36
               && wavd%((wavbps/8)*wavch)==0) AUD_DET((wavbps==8)?AUDIO:AUDIO_LE,wavi-3,44+wavm,wavd,wavch+wavbps/4-3);
            wavi=0;
          }
        }
        else{
          if ((p==16 && buf1!=0x4C495354 /*LIST*/) || (p==20 && buf0!=0x494E464F /*INFO*/))
            wavi=0;
          else if (p>20 && buf1==0x4C495354 /*LIST*/ && (wavi*=(buf0!=0))){
            wavlen = bswap(buf0);
            wavlist = i;
          }
          else if (wavlist){
            p=i-wavlist;
            if (p==8 && (buf1!=0x73647461 /*sdta*/ || buf0!=0x736D706C /*smpl*/))
              wavi=0;
            else if (p==12){
              int wavd = bswap(buf0);
              if (wavd && (wavd+12)==wavlen)
                AUD_DET(AUDIO_LE,wavi-3,(12+wavlist-(wavi-3)+1)&~1,wavd,1+16/4-3 /*mono, 16-bit*/);
              wavi=0;
            }
          }
        }
      }
    }

    // Detect .aiff file header
    if (buf0==0x464f524d) aiff=i,aiffs=0; // FORM
    if (aiff) {
      const int p=i-aiff;
      if (p==12 && (buf1!=0x41494646 || buf0!=0x434f4d4d)) aiff=0; // AIFF COMM
      else if (p==24) {
        const int bits=buf0&0xffff, chn=buf1>>16;
        if ((bits==8 || bits==16) && (chn==1 || chn==2)) aiffm=chn+bits/4-3+4; else aiff=0;
      } else if (p==42+aiffs && buf1!=0x53534e44) aiffs+=(buf0+8)+(buf0&1),aiff=(aiffs>0x400?0:aiff);
      else if (p==42+aiffs) AUD_DET(AUDIO,aiff-3,54+aiffs,buf0-8,aiffm);
    }

    // Detect .mod file header
    if ((buf0==0x4d2e4b2e || buf0==0x3643484e || buf0==0x3843484e  // M.K. 6CHN 8CHN
       || buf0==0x464c5434 || buf0==0x464c5438) && (buf1&0xc0c0c0c0)==0 && i>=1083) {
      const U64 savedpos=in->curpos();
      const int chn=((buf0>>24)==0x36?6:(((buf0>>24)==0x38 || (buf0&0xff)==0x38)?8:4));
      int len=0; // total length of samples
      int numpat=1; // number of patterns
      for (int j=0; j<31; j++) {
        in->setpos(start+i-1083+42+j*30);
        const int i1=in->getchar();
        const int i2=in->getchar();
        len+=i1*512+i2*2;
      }
      in->setpos(start+i-131);
      for (int j=0; j<128; j++) {
        int x=in->getchar();
        if (x+1>numpat) numpat=x+1;
      }
      if (numpat<65) AUD_DET(AUDIO,i-1083,1084+numpat*256*chn,len,4 /*mono, 8-bit*/);
      in->setpos(savedpos);
    }

    // Detect .s3m file header
    if (buf0==0x1a100000) s3mi=i,s3mno=s3mni=0; //0x1A: signature byte, 0x10: song type, 0x0000: reserved
    if (s3mi) {
      const int p=i-s3mi;
      if (p==4) {
        s3mno=bswap(buf0)&0xffff; //Number of entries in the order table, should be even 
        s3mni=(bswap(buf0)>>16);  //Number of instruments in the song
      }
      else if (p==16 && (((buf1>>16)&0xff)!=0x13 || buf0!=0x5343524d /*SCRM*/)) s3mi=0;
      else if (p==16) {
        const U64 savedpos=in->curpos();
        int b[31],sam_start=(1<<16),sam_end=0,ok=1;
        for (int j=0;j<s3mni;j++) {
          in->setpos(start+s3mi-31+0x60+s3mno+j*2);
          int i1=in->getchar();
          i1+=in->getchar()*256;
          in->setpos(start+s3mi-31+i1*16);
          i1=in->getchar();
          if (i1==1) { // type: sample
            for (int k=0;k<31;k++) b[k]=in->getchar();
            int len=b[15]+(b[16]<<8);
            int ofs=b[13]+(b[14]<<8);
            if (b[30]>1) ok=0;
            if (ofs*16<sam_start) sam_start=ofs*16;
            if (ofs*16+len>sam_end) sam_end=ofs*16+len;
          }
        }
        if (ok && sam_start<(1<<16)) AUD_DET(AUDIO,s3mi-31,sam_start,sam_end-sam_start,0 /*mono, 8-bit*/);
        s3mi=0;
        in->setpos(savedpos);
      }
    }
#endif //  USE_AUDIOMODEL
    
    // Detect .bmp image
    if (!bmpi && !dibi) {
      if((buf0&0xffff)==16973) { // 'BM'
        bmpi=i; // header start: bmpi-1
        dibi=i-1+18; // we expect a DIB header to come
      }
      else if ((buf0==0x28000000))  // headerless (DIB-only)
        dibi=i+1;
    }
    else {
      const U32 p=i-dibi+1+18;
           if (p==10+4) bmpof=bswap(buf0),bmpi=(bmpof<54 || start+blocksize<bmpi-1+bmpof)?(dibi=0):bmpi; //offset of pixel data (this field is still located in the BMP Header)
      else if (p==14+4 && buf0!=0x28000000) bmpi=dibi=0; //BITMAPINFOHEADER (0x28)
      else if (p==18+4) bmpx=bswap(buf0),bmpi=(((bmpx&0xff000000)!=0 || bmpx==0)?(dibi=0):bmpi); //width
      else if (p==22+4) bmpy=abs(int(bswap(buf0))),bmpi=(((bmpy&0xff000000)!=0 || bmpy==0)?(dibi=0):bmpi); //height
      else if (p==26+2) bmpi=((bswap(buf0<<16))!=1)?(dibi=0):bmpi; //number of color planes (must be 1)
      else if (p==28+2) imgbpp=bswap(buf0<<16),bmpi=((imgbpp!=1 && imgbpp!=4 && imgbpp!=8 && imgbpp!=24 && imgbpp!=32)?(dibi=0):bmpi); //color depth
      else if (p==30+4) bmpi=((buf0!=0)?(dibi=0):bmpi); //compression method must be: BI_RGB (uncompressed)
      else if (p==34+4) bmps=bswap(buf0); //image size or 0
    //else if (p==38+4) ; // the horizontal resolution of the image (ignored)
    //else if (p==42+4) ; // the vertical resolution of the image (ignored)
      else if (p==46+4) { 
        n_colors=bswap(buf0); // the number of colors in the color palette, or 0 to default to (1<<imgbpp)
        if(n_colors==0 && imgbpp<=8) n_colors=1<<imgbpp;
        if(n_colors>(1u<<imgbpp) || (imgbpp>8 && n_colors>0))bmpi=dibi=0;
      }
      else if (p==50+4) { //the number of important colors used
        if (bswap(buf0)<=n_colors || bswap(buf0)==0x10000000) {
          if (bmpi==0 /*headerless*/ && (bmpx*2==bmpy) && imgbpp>1 && // possible icon/cursor?
             (
              (bmps>0 && bmps==( (bmpx*bmpy*(imgbpp+1))>>4 )) ||
              ((!bmps || bmps<((bmpx*bmpy*imgbpp)>>3)) && (
               (bmpx==8)   || (bmpx==10) || (bmpx==14) || (bmpx==16) || (bmpx==20) ||
               (bmpx==22)  || (bmpx==24) || (bmpx==32) || (bmpx==40) || (bmpx==48) ||
               (bmpx==60)  || (bmpx==64) || (bmpx==72) || (bmpx==80) || (bmpx==96) ||
               (bmpx==128) || (bmpx==256)
              ))
             )
          )
            bmpy=bmpx;

          Blocktype blocktype=DEFAULT;
          U32 width_in_bytes=0;
          if      (imgbpp==1)  {blocktype=IMAGE1; width_in_bytes=(((bmpx-1)>>5)+1)*4;}
          else if (imgbpp==4)  {blocktype=IMAGE4; width_in_bytes=((bmpx*4+31)>>5)*4;}
          else if (imgbpp==8)  {blocktype=IMAGE8; width_in_bytes=(bmpx+3)&-4;}
          else if (imgbpp==24) {blocktype=IMAGE24;width_in_bytes=((bmpx*3)+3)&-4;}
          else if (imgbpp==32) {blocktype=IMAGE32;width_in_bytes=bmpx*4;}
          
          if (imgbpp==8){
            const U64 color_palette_pos=dibi-18+54;
            const U64 savedpos=in->curpos();
            in->setpos(color_palette_pos);
            if(IsGrayscalePalette(in, n_colors, 1)) blocktype=IMAGE8GRAY;
            in->setpos(savedpos);
          }

          const U32 headerpos=bmpi>0 ? bmpi-1 : dibi-4;
          const U32 minheadersize=(bmpi>0 ? 54: 54-14)+n_colors*4;
          const U32 headersize= bmpi>0 ? bmpof : minheadersize;
          
          // some final sanity checks
          if(bmps!=0 && bmps<width_in_bytes*bmpy) { /*printf("\nBMP guard: image is larger than reported in header\n",bmps,width_in_bytes*bmpy);*/ }
          else if(start+blocksize<headerpos+headersize+width_in_bytes*bmpy) {/*printf("\nBMP guard: cropped data\n");*/ }
          else if(headersize==(bmpi>0 ? 54: 54-14) && n_colors>0) { /*printf("\nBMP guard: missing palette\n");*/ }
          else if(bmpi>0 && bmpof<minheadersize) { /*printf("\nBMP guard: overlapping color palette\n");*/ }
          else if(bmpi>0 && U64(bmpi)-1+bmpof+width_in_bytes*bmpy>start+blocksize) { /*printf("\nBMP guard: reported pixel data offset is incorrect\n");*/ }
          else if(width_in_bytes*bmpy<=64){ /*printf("\nBMP guard: too small\n");*/ }  // too small - not worthy to use the image models
          else IMG_DET(blocktype,headerpos,headersize,width_in_bytes,bmpy);
        }
        bmpi=dibi=0;
      }
    }

    // Detect binary .pbm .pgm .ppm .pam images
    if ((buf0&0xfff0ff)==0x50300a) { //"Px" + line feed, where "x" shall be a number
      pgmn=(buf0&0xf00)>>8; // extract "x"
      if ((pgmn>=4 && pgmn<=6) || pgmn==7) pgm=i,pgm_ptr=pgmw=pgmh=pgmc=pgmcomment=pamatr=pamd=pgmdata=pgmdatasize=0; // "P4" (pbm), "P5" (pgm), "P6" (ppm), "P7" (pam)
    }
    if (pgm) {
      if (pgmdata==0) { // parse header
        if (i-pgm==1 && c==0x23) pgmcomment=1; // # (pgm comment)
        if (!pgmcomment && pgm_ptr) {
          int s=0;
          if (pgmn==7) {
             if ((buf1&0xdfdf)==0x5749 && (buf0&0xdfdfdfff)==0x44544820) pgm_ptr=0, pamatr=1; // WIDTH
             if ((buf1&0xdfdfdf)==0x484549 && (buf0&0xdfdfdfff)==0x47485420) pgm_ptr=0, pamatr=2; // HEIGHT
             if ((buf1&0xdfdfdf)==0x4d4158 && (buf0&0xdfdfdfff)==0x56414c20) pgm_ptr=0, pamatr=3; // MAXVAL
             if ((buf1&0xdfdf)==0x4445 && (buf0&0xdfdfdfff)==0x50544820) pgm_ptr=0, pamatr=4; // DEPTH
             if ((buf2&0xdf)==0x54 && (buf1&0xdfdfdfdf)==0x55504c54 && (buf0&0xdfdfdfff)==0x59504520) pgm_ptr=0, pamatr=5; // TUPLTYPE
             if ((buf1&0xdfdfdf)==0x454e44 && (buf0&0xdfdfdfff)==0x4844520a) pgm_ptr=0, pamatr=6; // ENDHDR
             if (c==0x0a) {
               if (pamatr==0) pgm=0;
               else if (pamatr<5) s=pamatr;
               if (pamatr!=6) pamatr=0;
             }
          } else if (c==0x20 && !pgmw) s=1;
          else if (c==0x0a && !pgmh) s=2;
          else if (c==0x0a && !pgmc && pgmn!=4) s=3;
          if (s) {
            pgm_buf[pgm_ptr++]=0;
            int v=atoi(pgm_buf); // parse width/height/depth/maxval value
            if (s==1) pgmw=v; else if (s==2) pgmh=v; else if (s==3) pgmc=v; else if (s==4) pamd=v;
            if (v==0 || (s==3 && v>255)) pgm=0; else pgm_ptr=0;
          }
        }
        if (!pgmcomment) pgm_buf[pgm_ptr++]=c;
        if (pgm_ptr>=32) pgm=0;
        if (pgmcomment && c==0x0a) pgmcomment=0;
        if (pgmw && pgmh && !pgmc && pgmn==4) {pgmdata=i;pgmdatasize=(pgmw+7)/8*pgmh;}
        if (pgmw && pgmh && pgmc && (pgmn==5 || (pgmn==7 && pamd==1 && pamatr==6))) {pgmdata=i;pgmdatasize=pgmw*pgmh;}
        if (pgmw && pgmh && pgmc && (pgmn==6 || (pgmn==7 && pamd==3 && pamatr==6))) {pgmdata=i;pgmdatasize=pgmw*3*pgmh;}
        if (pgmw && pgmh && pgmc && (pgmn==7 && pamd==4 && pamatr==6)) {pgmdata=i;pgmdatasize=pgmw*4*pgmh;}
      }
      else { // pixel data
        if(textparser.start()==U32(i) ||         // for any sign of non-text data in pixel area
           (pgm-2==0 && n-pgmdatasize==i))        // or the image is the whole file/block -> finish (success)
        {
          if (pgmw && pgmh && !pgmc && pgmn==4) IMG_DET(IMAGE1,pgm-2,pgmdata-pgm+3,(pgmw+7)/8,pgmh);
          if (pgmw && pgmh && pgmc && (pgmn==5 || (pgmn==7 && pamd==1 && pamatr==6))) IMG_DET(IMAGE8GRAY,pgm-2,pgmdata-pgm+3,pgmw,pgmh);
          if (pgmw && pgmh && pgmc && (pgmn==6 || (pgmn==7 && pamd==3 && pamatr==6))) IMG_DET(IMAGE24,pgm-2,pgmdata-pgm+3,pgmw*3,pgmh);
          if (pgmw && pgmh && pgmc && (pgmn==7 && pamd==4 && pamatr==6)) IMG_DET(IMAGE32,pgm-2,pgmdata-pgm+3,pgmw*4,pgmh);
        }
        else if((--pgmdatasize)==0)pgm=0; // all data was probably text in pixel area: fail
      }
    }

    // Detect .rgb image
    if ((buf0&0xffff)==0x01da) rgbi=i,rgbx=rgby=0;
    if (rgbi) {
      const int p=i-rgbi;
      if (p==1 && c!=0) rgbi=0;
      else if (p==2 && c!=1) rgbi=0;
      else if (p==4 && (buf0&0xffff)!=1 && (buf0&0xffff)!=2 && (buf0&0xffff)!=3) rgbi=0;
      else if (p==6) rgbx=buf0&0xffff,rgbi=(rgbx==0?0:rgbi);
      else if (p==8) rgby=buf0&0xffff,rgbi=(rgby==0?0:rgbi);
      else if (p==10) {
        int z=buf0&0xffff;
        if (rgbx && rgby && (z==1 || z==3 || z==4)) IMG_DET(IMAGE8,rgbi-1,512,rgbx,rgby*z);
        rgbi=0;
      }
    }

    // Detect .tiff file header (2/8/24 bit color, not compressed).
    if (buf1==0x49492a00 && n>i+(int)bswap(buf0)) {
      const U64 savedpos=in->curpos();
      in->setpos(start+i+ (U64)bswap(buf0)-7);

      // read directory
      int dirsize=in->getchar();
      int tifx=0,tify=0,tifz=0,tifzb=0,tifc=0,tifofs=0,tifofval=0,tifsize=0,b[12];
      if (in->getchar()==0) {
        for (int i=0; i<dirsize; i++) {
          for (int j=0; j<12; j++) b[j]=in->getchar();
          if (b[11]==EOF) break;
          int tag=b[0]+(b[1]<<8);
          int tagfmt=b[2]+(b[3]<<8);
          int taglen=b[4]+(b[5]<<8)+(b[6]<<16)+(b[7]<<24);
          int tagval=b[8]+(b[9]<<8)+(b[10]<<16)+(b[11]<<24);
          if (tagfmt==3||tagfmt==4) {
            if (tag==256) tifx=tagval;
            else if (tag==257) tify=tagval;
            else if (tag==258) tifzb=taglen==1?tagval:8; // bits per component
            else if (tag==259) tifc=tagval; // 1 = no compression
            else if (tag==273 && tagfmt==4) tifofs=tagval,tifofval=(taglen<=1);
            else if (tag==277) tifz=tagval; // components per pixel
            else if (tag==279 && taglen==1) tifsize=tagval;
          }
        }
      }
      if (tifx && tify && tifzb && (tifz==1 || tifz==3) && ((tifc==1) || (tifc==5/*LZW*/ && tifsize>0)) && (tifofs && tifofs+i<n)) {
        if (!tifofval) {
          in->setpos(start+i+tifofs-7);
          for (int j=0; j<4; j++) b[j]=in->getchar();
          tifofs=b[0]+(b[1]<<8)+(b[2]<<16)+(b[3]<<24);
        }
        if (tifofs && tifofs<(1<<18) && tifofs+i<n) {
          if (tifc==1) {
            if (tifz==1 && tifzb==1) IMG_DET(IMAGE1,i-7,tifofs,((tifx-1)>>3)+1,tify);
            else if (tifz==1 && tifzb==8) IMG_DET(IMAGE8,i-7,tifofs,tifx,tify);
            else if (tifz==3 && tifzb==8) IMG_DET(IMAGE24,i-7,tifofs,tifx*3,tify);
          }
          else if (tifc==5 && tifsize>0) {
            tifx=((tifx+8-tifzb)/(9-tifzb))*tifz;
            info=tifz*tifzb;
            info=(((info==1)?IMAGE1:((info==8)?IMAGE8:IMAGE24))<<24)|tifx;
            detd=tifsize;
            in->setpos(start+i-7+tifofs);
            return dett=LZW;
          }
        }
      }
      in->setpos(savedpos);
    }

    // Detect .tga image (8-bit 256 colors or 24-bit uncompressed)
    if ((buf1&0xFFF7FF)==0x00010100 && (buf0&0xFFFFFFC7)==0x00000100 && (c==16 || c==24 || c==32)) tga=i,tgax=tgay,tgaz=8,tgat=(buf1>>8)&0xF,tgaid=buf1>>24,tgamap=c/8;
    else if ((buf1&0xFFFFFF)==0x00000200 && buf0==0x00000000) tga=i,tgax=tgay,tgaz=24,tgat=2;
    else if ((buf1&0xFFF7FF)==0x00000300 && buf0==0x00000000) tga=i,tgax=tgay,tgaz=8,tgat=(buf1>>8)&0xF;
    if (tga) {
      if (i-tga==8) tga=(buf1==0?tga:0),tgax=(bswap(buf0)&0xffff),tgay=(bswap(buf0)>>16);
      else if (i-tga==10) {
        if ((buf0&0xFFF7)==32<<8)
          tgaz=32;
        if ((tgaz<<8)==(int)(buf0&0xFFD7) && tgax && tgay && U32(tgax*tgay)<0xFFFFFFF) {
          if (tgat==1){
            in->setpos(start+tga+11+tgaid);
            IMG_DET( (IsGrayscalePalette(in))?IMAGE8GRAY:IMAGE8,tga-7,18+tgaid+256*tgamap,tgax,tgay);
          }
          else if (tgat==2) IMG_DET((tgaz==24)?IMAGE24:IMAGE32,tga-7,18+tgaid,tgax*(tgaz>>3),tgay);
          else if (tgat==3) IMG_DET(IMAGE8GRAY,tga-7,18+tgaid,tgax,tgay);
          else if (tgat==9 || tgat==11) {
            const U64 savedpos=in->curpos();
            in->setpos(start+tga+11+tgaid);
            if (tgat==9) {
              info=(IsGrayscalePalette(in)?IMAGE8GRAY:IMAGE8)<<24;
              in->setpos(start+tga+11+tgaid+256*tgamap);
            }
            else
              info=IMAGE8GRAY<<24;
            info|=tgax;
            // now detect compressed image data size
            detd=0;
            int c=in->getchar(), b=0, total=tgax*tgay, line=0;
            while (total>0 && c>=0 && (++detd, b=in->getchar())>=0){
              if (c==0x80) { c=b; continue; }
              else if (c>0x7F) {
                total-=(c=(c&0x7F)+1); line+=c;
                c=in->getchar();
                detd++;
              }
              else {
                in->setpos(in->curpos()+c); 
                detd+=++c; total-=c; line+=c;
                c=in->getchar();
              }
              if (line>tgax) break;
              else if (line==tgax) line=0;
            }
            if (total==0) {
              in->setpos(start+tga+11+tgaid+256*tgamap);
              return dett=RLE;
            }
            else
              in->setpos(savedpos);
          }
        }
        tga=0;
      }
    }

    // Detect .gif
    if (type==DEFAULT && dett==GIF && i==0) {
      dett=DEFAULT;
      if (c==0x2c || c==0x21) gif=2,gifi=2;
      else gifgray=0;
    }
    if (!gif && (buf1&0xffff)==0x4749 && (buf0==0x46383961 || buf0==0x46383761)) gif=1,gifi=i+5;
    if (gif) {
      if (gif==1 && i==gifi) gif=2, gifi = i+5+(gifplt=(c&128)?(3*(2<<(c&7))):0);
      if (gif==2 && gifplt && i==gifi-gifplt-3) gifgray = IsGrayscalePalette(in, gifplt/3), gifplt = 0;
      if (gif==2 && i==gifi) {
        if ((buf0&0xff0000)==0x210000) gif=5,gifi=i;
        else if ((buf0&0xff0000)==0x2c0000) gif=3,gifi=i;
        else gif=0;
      }
      if (gif==3 && i==gifi+6) gifw=(bswap(buf0)&0xffff);
      if (gif==3 && i==gifi+7) gif=4,gifc=gifb=0,gifa=gifi=i+2+(gifplt=((c&128)?(3*(2<<(c&7))):0));
      if (gif==4 && gifplt) gifgray = IsGrayscalePalette(in, gifplt/3), gifplt = 0;
      if (gif==4 && i==gifi) {
        if (c>0 && gifb && gifc!=gifb) gifw=0;
        if (c>0) gifb=gifc,gifc=c,gifi+=c+1;
        else if (!gifw) gif=2,gifi=i+3;
        else return in->setpos(start+gifa-1),detd=i-gifa+2,info=((gifgray?IMAGE8GRAY:IMAGE8)<<24)|gifw,dett=GIF;
      }
      if (gif==5 && i==gifi) {
        if (c>0) gifi+=c+1; else gif=2,gifi=i+3;
      }
    }

    // Detect EXE if the low order byte (little-endian) XX is more
    // recently seen (and within 4K) if a relative to absolute address
    // conversion is done in the context CALL/JMP (E8/E9) XX xx xx 00/FF
    // 4 times in a row.  Detect end of EXE at the last
    // place this happens when it does not happen for 64KB.

    if (((buf1&0xfe)==0xe8 || (buf1&0xfff0)==0x0f80) && ((buf0+1)&0xfe)==0) {
      int r=buf0>>24;  // relative address low 8 bits
      int a=((buf0>>24)+i)&0xff;  // absolute address low 8 bits
      int rdist=i-relpos[r];
      int adist=i-abspos[a];
      if (adist<rdist && adist<0x800 && abspos[a]>5) {
        e8e9last=i;
        ++e8e9count;
        if (e8e9pos==0 || e8e9pos>abspos[a]) e8e9pos=abspos[a];
      }
      else e8e9count=0;
      if (type==DEFAULT && e8e9count>=4 && e8e9pos>5)
        return in->setpos(start+e8e9pos-5), EXE;
      abspos[a]=i;
      relpos[r]=i;
    }
    if (i-e8e9last>0x4000) {
      //TODO: Large file support
      if (type==EXE) {
        info=(int)start;
        in->setpos(start+e8e9last);
        return DEFAULT;
      }
      e8e9count=e8e9pos=0;
    }

    // Detect base64 encoded data
    if (b64s==0 && buf0==0x73653634 && ((buf1&0xffffff)==0x206261 || (buf1&0xffffff)==0x204261)) b64s=1,b64i=i-6; //' base64' ' Base64'
    if (b64s==0 && ((buf1==0x3b626173 && buf0==0x6536342c) || (buf1==0x215b4344 && buf0==0x4154415b))) b64s=3,b64i=i+1; // ';base64,' '![CDATA['
    if (b64s>0) {
      if (b64s==1 && buf0==0x0d0a0d0a) b64i=i+1,b64line=0,b64s=2;
      else if (b64s==2 && (buf0&0xffff)==0x0d0a && b64line==0) b64line=i+1-b64i,b64nl=i;
      else if (b64s==2 && (buf0&0xffff)==0x0d0a && b64line>0 && (buf0&0xffffff)!=0x3d0d0a) {
         if (i-b64nl<b64line && buf0!=0x0d0a0d0a) i-=1,b64s=5;
         else if (buf0==0x0d0a0d0a) i-=3,b64s=5;
         else if (i-b64nl==b64line) b64nl=i;
         else b64s=0;
      }
      else if (b64s==2 && (buf0&0xffffff)==0x3d0d0a) i-=1,b64s=5; // '=' or '=='
      else if (b64s==2 && !(isalnum(c) || c=='+' || c=='/' || c==10 || c==13 || c=='=')) b64s=0;
      if (b64line>0 && (b64line<=4 || b64line>255)) b64s=0;
      if (b64s==3 && i>=b64i && !(isalnum(c) || c=='+' || c=='/' || c=='=')) b64s=4;
      if ((b64s==4 && i-b64i>128) || (b64s==5 && i-b64i>512 && i-b64i<(1<<27))) return in->setpos(start+b64i),detd=i-b64i,BASE64;
      if (b64s>3) b64s=0;
      if (b64s==1 && i-b64i>=128)b64s=0; // detect false positives after 128 bytes
    }


    // Detect text
    // This is different from the above detection routines: it's a negative detection (it detects a failure)
    U32 t = utf8_state_table[c];
    textparser.UTF8State = utf8_state_table[256 + textparser.UTF8State + t];
    if(textparser.UTF8State == UTF8_ACCEPT) { // proper end of a valid utf8 sequence
      if (c==NEW_LINE) {
        if (((buf0>>8)&0xff) != CARRIAGE_RETURN)
          textparser.setEOLType(2); // mixed or LF-only
        else 
          if (textparser.EOLType()==0)textparser.setEOLType(1); // CRLF-only
      }
      textparser.invalidCount=textparser.invalidCount*(TEXT_ADAPT_RATE-1)/TEXT_ADAPT_RATE;
      if(textparser.invalidCount==0)
        textparser.setend(i); // a possible end of block position
    }
    else
    if(textparser.UTF8State == UTF8_REJECT) { // illegal state
      textparser.invalidCount = textparser.invalidCount*(TEXT_ADAPT_RATE-1)/TEXT_ADAPT_RATE + TEXT_ADAPT_RATE;
      textparser.UTF8State = UTF8_ACCEPT; // reset state
      if (textparser.validlength()<TEXT_MIN_SIZE) {
        textparser.reset(i+1); // it's not text (or not long enough) - start over
      }
      else if (textparser.invalidCount >= TEXT_MAX_MISSES*TEXT_ADAPT_RATE) {
        if (textparser.validlength()<TEXT_MIN_SIZE)
          textparser.reset(i+1); // it's not text (or not long enough) - start over
        else // Commit text block validation
          textparser.next(i+1);
      }
    }
  }
  return type;
}


typedef enum {FDECOMPRESS, FCOMPARE, FDISCARD} FMode;

void encode_cd(File *in, File *out, U64 size, int info) {
  //TODO: Large file support
  const int BLOCK=2352;
  U8 blk[BLOCK];
  U64 blockresidual=size%BLOCK;
  assert(blockresidual<65536);
  out->putchar((blockresidual>>8)&255);
  out->putchar(blockresidual &255);
  for (U64 offset=0; offset<size; offset+=BLOCK) {
    if (offset+BLOCK > size) { //residual
      in->blockread(&blk[0], size-offset);
      out->blockwrite(&blk[0], size-offset);
    } else { //normal sector
      in->blockread(&blk[0], BLOCK);
      if (info==3) blk[15]=3; //indicate Mode2/Form2
      if (offset==0) out->blockwrite(&blk[12], 4+4*(blk[15]!=1)); //4-byte address + 4 bytes from the 8-byte subheader goes only to the first sector
      out->blockwrite(&blk[16+8*(blk[15]!=1)], 2048+276*(info==3)); //user data goes to all sectors
      if (offset+BLOCK*2 > size && blk[15]!=1) out->blockwrite(&blk[16], 4); //in Mode2 4 bytes from the 8-byte subheader goes after the last sector
    }
  }
}

U64 decode_cd(File *in, U64 size, File *out, FMode mode, U64 &diffFound) {
  //TODO: Large file support
  const int BLOCK=2352;
  U8 blk[BLOCK];
  U64 i=0; //*in position
  U64 nextblockpos=0;
  int address=-1;
  int datasize=0;
  U64 residual=(in->getchar() << 8) + in->getchar();
  size -=2;
  while (i<size) {
    if (size-i == residual) { //residual data after last sector
      in->blockread(blk, residual);
      if (mode==FDECOMPRESS) out->blockwrite(blk, residual);
      else if (mode==FCOMPARE) for (int j=0; j<(int)residual; ++j) if (blk[j]!= out->getchar() && !diffFound) diffFound=nextblockpos+j+1;
      return nextblockpos+residual;
    } else if (i==0) { //first sector
      in->blockread(blk+12, 4); //header (4 bytes) consisting of address (Minutes, Seconds, Sectors) and mode (1 = Mode1, 2 = Mode2/Form1, 3 = Mode2/Form2)
      if (blk[15]!=1) in->blockread(blk+16, 4);  //Mode2: 4 bytes from the read 8-byte subheader
      datasize=2048+(blk[15]==3)*276; //user data bytes: Mode1 and Mode2/Form1: 2048 (ECC is present) or Mode2/Form2: 2048+276=2324 bytes (ECC is not present)
      i+=4+4*(blk[15]!=1); //4 byte header + ( Mode2: 4 bytes from the 8-byte subheader )
    } else { //normal sector
      address=(blk[12]<<16)+(blk[13]<<8)+blk[14]; //3-byte address (Minutes, Seconds, Sectors)
    }
    in->blockread(blk+16+(blk[15]!=1)*8, datasize);  //read data bytes, but skip 8-byte subheader in Mode 2 (which we processed alread above)
    i+=datasize;
    if (datasize>2048) blk[15]=3; //indicate Mode2/Form2
    if (blk[15]!=1 && size-residual-i==4) { //Mode 2: we are at the last sector - grab the 4 subheader bytes
      in->blockread(blk+16, 4);
      i+=4;
    }
    expand_cd_sector(blk, address, 0);
    if (mode==FDECOMPRESS) out->blockwrite(blk, BLOCK);
    else if (mode==FCOMPARE) for (int j=0; j<BLOCK; ++j) if (blk[j]!= out->getchar() && !diffFound) diffFound=nextblockpos+j+1;
    nextblockpos+=BLOCK;
  }
  return nextblockpos;
}


// 24-bit image data transforms, controlled by OPTION_SKIPRGB:
// - simple color transform (b, g, r) -> (g, g-r, g-b)
// - channel reorder only (b, g, r) -> (g, r, b)
// Detects RGB565 to RGB888 conversions

#define RGB565_MIN_RUN 63

void encode_bmp(File *in, File *out, U64 len, int width) {
  int r, g, b, total=0;
  bool isPossibleRGB565 = true;
  for (int i=0; i<(int)(len/width); i++) {
    for (int j=0; j<width/3; j++) {
      b=in->getchar();
      g=in->getchar();
      r=in->getchar();
      if (isPossibleRGB565) {
        int pTotal=total;
        total=std::min<int>(total+1, 0xFFFF)*((b&7)==((b&8)-((b>>3)&1)) && (g&3)==((g&4)-((g>>2)&1)) && (r&7)==((r&8)-((r>>3)&1)));
        if (total>RGB565_MIN_RUN || pTotal>=RGB565_MIN_RUN) {
          b^=(b&8)-((b>>3)&1);
          g^=(g&4)-((g>>2)&1);
          r^=(r&8)-((r>>3)&1);
        }
        isPossibleRGB565=total>0;
      }
      out->putchar(g);
      out->putchar(options&OPTION_SKIPRGB?r:g-r);
      out->putchar(options&OPTION_SKIPRGB?b:g-b);
    }
    for (int j=0; j<width%3; j++) out->putchar(in->getchar());
  }
  for (int i=len%width; i>0; i--) out->putchar(in->getchar());
}

U64 decode_bmp(Encoder& en, U64 size, int width, File *out, FMode mode, U64 &diffFound) {
  int r, g, b, p, total=0;
  bool isPossibleRGB565 = true;
  for (int i=0; i<(int)(size/width); i++) {
    p=i*width;
    for (int j=0; j<width/3; j++) {
      g=en.decompress();
      r=en.decompress();
      b=en.decompress();
      if (!(options&OPTION_SKIPRGB))
        r=g-r, b=g-b;
      if (isPossibleRGB565){
        if (total>=RGB565_MIN_RUN) {
          b^=(b&8)-((b>>3)&1);
          g^=(g&4)-((g>>2)&1);
          r^=(r&8)-((r>>3)&1);
        }
        total=std::min<int>(total+1, 0xFFFF)*((b&7)==((b&8)-((b>>3)&1)) && (g&3)==((g&4)-((g>>2)&1)) && (r&7)==((r&8)-((r>>3)&1)));
        isPossibleRGB565=total>0;
      }
      if (mode==FDECOMPRESS) {
        out->putchar(b);
        out->putchar(g);
        out->putchar(r);
        if (!j && !(i&0xf)) en.print_status();
      }
      else if (mode==FCOMPARE) {
        if ((b&255)!=out->getchar() && !diffFound) diffFound=p+1;
        if (g!=out->getchar() && !diffFound) diffFound=p+2;
        if ((r&255)!=out->getchar() && !diffFound) diffFound=p+3;
        p+=3;
      }
    }
    for (int j=0; j<width%3; j++) {
      if (mode==FDECOMPRESS) {
        out->putchar(en.decompress());
      }
      else if (mode==FCOMPARE) {
        if (en.decompress()!= out->getchar() && !diffFound) diffFound=p+j+1;
      }
    }
  }
  for (int i=size%width; i>0; i--) {
    if (mode==FDECOMPRESS) {
      out->putchar(en.decompress());
    }
    else if (mode==FCOMPARE) {
      if (en.decompress()!=out->getchar()&&!diffFound) { diffFound=size-i; break; }
    }
  }
  return size;
}

// 32-bit image
void encode_im32(File *in, File *out, U64 len, int width) {
  int r,g,b,a;
  for (int i=0; i<(int)(len/width); i++) {
    for (int j=0; j<width/4; j++) {
      b=in->getchar();
      g=in->getchar();
      r=in->getchar();
      a=in->getchar();
      out->putchar(g);
      out->putchar(options&OPTION_SKIPRGB?r:g-r);
      out->putchar(options&OPTION_SKIPRGB?b:g-b);
      out->putchar(a);
    }
    for (int j=0; j<width%4; j++) out->putchar(in->getchar());
  }
  for (int i=len%width; i>0; i--) out->putchar(in->getchar());
}

U64 decode_im32(Encoder& en, U64 size, int width, File *out, FMode mode, U64 &diffFound) {
  int r,g,b,a,p;
  bool rgb = (width&(1<<31))>0;
  if (rgb) width^=(1<<31);
  for (int i=0; i<(int)(size/width); i++) {
    p=i*width;
    for (int j=0; j<width/4; j++) {
      b=en.decompress(), g=en.decompress(), r=en.decompress(), a=en.decompress();
      if (mode==FDECOMPRESS) {
        out->putchar(options&OPTION_SKIPRGB?r:b-r); out->putchar(b); out->putchar(options&OPTION_SKIPRGB?g:b-g); out->putchar(a);
        if (!j && !(i&0xf)) en.print_status();
      }
      else if (mode==FCOMPARE) {
        if (((options&OPTION_SKIPRGB?r:b-r)&255)!=out->getchar() && !diffFound) diffFound=p+1;
        if (b!=out->getchar() && !diffFound) diffFound=p+2;
        if (((options&OPTION_SKIPRGB?g:b-g)&255)!=out->getchar() && !diffFound) diffFound=p+3;
        if (((a)&255)!=out->getchar() && !diffFound) diffFound=p+4;
        p+=4;
      }
    }
    for (int j=0; j<width%4; j++) {
      if (mode==FDECOMPRESS) {
        out->putchar(en.decompress());
      }
      else if (mode==FCOMPARE) {
        if (en.decompress()!=out->getchar() && !diffFound) diffFound=p+j+1;
      }
    }
  }
  for (int i=size%width; i>0; i--) {
    if (mode==FDECOMPRESS) {
      out->putchar(en.decompress());
    }
    else if (mode==FCOMPARE) {
      if (en.decompress()!= out->getchar() && !diffFound){ diffFound=size-i; break; }
    }
  }
  return size;
}

void encode_endianness16b(File *in, File *out, U64 size) {
  for (U64 i=0, l=size>>1; i<l; i++) {
    U8 B = in->getchar();
    out->putchar(in->getchar());
    out->putchar(B);
  }
  if ((size&1)>0)
    out->putchar(in->getchar());
}

U64 decode_endianness16b(Encoder& en, U64 size, File *out, FMode mode, U64 &diffFound) {
  for (U64 i=0, l=size>>1; i<l; i++) {
    U8 B1 = en.decompress(), B2 = en.decompress();
    if (mode==FDECOMPRESS) {
      out->putchar(B2);
      out->putchar(B1);
    }
    else if (mode==FCOMPARE){
      bool ok = out->getchar()==B2;
      ok&=out->getchar()==B1;
      if (!ok && !diffFound){
        diffFound=size-i*2;
        break;
      }
    }
    if (mode==FDECOMPRESS && !(i&0x7FF))
      en.print_status();
  }
  if (!diffFound && (size&1)>0) {
    if (mode==FDECOMPRESS)
      out->putchar(en.decompress());
    else if (mode==FCOMPARE) {
      if (out->getchar()!=en.decompress())
        diffFound=size-1;
    }
  }
  return size;
}

// EOL transform

void encode_eol(File *in, File *out, U64 len) {
  U8 B=0, pB=0;
  for (int i=0; i<(int)len; i++){
    B = in->getchar();
    if (pB==CARRIAGE_RETURN && B!=NEW_LINE)
      out->putchar(pB);
    if (B!=CARRIAGE_RETURN)
      out->putchar(B);
    pB = B;
  }
  if (B==CARRIAGE_RETURN)
    out->putchar(B);
}

U64 decode_eol(Encoder& en, U64 size, File *out, FMode mode, U64 &diffFound) {
  U8 B;
  U64 count = 0;
  for (int i=0; i<(int)size; i++, count++){
    if ((B=en.decompress())==NEW_LINE){
      if (mode==FDECOMPRESS)
        out->putchar(CARRIAGE_RETURN);
      else if (mode == FCOMPARE){
        if (out->getchar()!=CARRIAGE_RETURN && !diffFound){
          diffFound=size-i;
          break;
        }
      }
      count++;
    }
    if (mode==FDECOMPRESS)
      out->putchar(B);
    else if (mode==FCOMPARE){
      if (B!=out->getchar() && !diffFound){
        diffFound=size-i;
        break;
      }
    }
    if (mode==FDECOMPRESS && !(i&0xFFF))
      en.print_status();
  }
  return count;
}

void encode_rle(File *in, File *out, U64 size, int info, int &hdrsize) {
  U8 b, c = in->getchar();
  int i = 1, maxBlockSize = info&0xFFFFFF;
  out->putVLI(maxBlockSize);
  hdrsize=VLICost(maxBlockSize);
  while (i<(int)size) {
    b = in->getchar(), i++;
    if (c==0x80) { c = b; continue; }
    else if (c>0x7F) {
      for (int j=0; j<=(c&0x7F); j++) out->putchar(b);
      c = in->getchar(), i++;
    }
    else {
      for (int j=0; j<=c; j++, i++) { out->putchar(b), b = in->getchar(); }
      c = b;
    }
  }
}

#define rleOutputRun { \
  while (run > 128) { \
    *outPtr++ = 0xFF, *outPtr++ = byte; \
    run-=128; \
  } \
  *outPtr++ = (U8)(0x80|(run-1)), *outPtr++ = byte; \
}

U64 decode_rle(File *in, U64 size, File *out, FMode mode, U64 &diffFound) {
  U8 inBuffer[0x10000]={0};
  U8 outBuffer[0x10200]={0};
  U64 pos = 0;
  int maxBlockSize = (int)in->getVLI();
  enum { BASE, LITERAL, RUN, LITERAL_RUN } state;
  do {
    U64 remaining = in->blockread(&inBuffer[0], maxBlockSize);
    U8 *inPtr = (U8*)inBuffer;
    U8 *outPtr= (U8*)outBuffer;
    U8 *lastLiteral = nullptr;
    state = BASE;
    while (remaining>0){
      U8 byte = *inPtr++, loop = 0;
      int run = 1;
      for (remaining--; remaining>0 && byte==*inPtr; remaining--, run++, inPtr++);
      do {
        loop = 0;
        switch (state) {
          case BASE: case RUN: {
            if (run>1) {
              state = RUN;
              rleOutputRun;
            }
            else {
              lastLiteral = outPtr;
              *outPtr++ = 0, *outPtr++ = byte;
              state = LITERAL;
            }
            break;
          }
          case LITERAL: {
            if (run>1) {
              state = LITERAL_RUN;
              rleOutputRun;
            }
            else {
              if (++(*lastLiteral)==127)
                state = BASE;
              *outPtr++ = byte;
            }
            break;
          }
          case LITERAL_RUN: {
            if (outPtr[-2]==0x81 && *lastLiteral<(125)) {
              state = (((*lastLiteral)+=2)==127)?BASE:LITERAL;
              outPtr[-2] = outPtr[-1];
            }
            else
              state = RUN;
            loop = 1;
          }
        }
      } while (loop);
    }

    U64 length = outPtr-(U8*)(&outBuffer[0]);
    if (mode==FDECOMPRESS)
      out->blockwrite(&outBuffer[0], length);
    else if (mode==FCOMPARE) {
      for (int j=0; j<(int)length; ++j) {
        if (outBuffer[j]!=out->getchar() && !diffFound) {
          diffFound = pos+j+1;
          break; 
        }
      }
    }
    pos+=length;
  } while (!in->eof() && !diffFound);
  return pos;
}

struct LZWentry{
  short prefix;
  short suffix;
};

#define LZW_RESET_CODE 256
#define LZW_EOF_CODE   257

class LZWDictionary{
private:
  static constexpr int HashSize = 9221;
  LZWentry dictionary[4096];
  short table[HashSize];
  U8 buffer[4096];
public:
  int index;
  LZWDictionary(): index(0){ reset(); }
  void reset(){
    memset(&dictionary, 0xFF, sizeof(dictionary));
    memset(&table, 0xFF, sizeof(table));
    for (int i=0; i<256; i++){
      table[-findEntry(-1, i)-1] = (short)i;
      dictionary[i].suffix = i;
    }
    index = 258; //2 extra codes, one for resetting the dictionary and one for signaling EOF
  }
  int findEntry(const int prefix, const int suffix){
    int i = finalize64(hash(prefix, suffix), 13);
    int offset = (i>0)?HashSize-i:1;
    while (true){
      if (table[i]<0) //free slot?
        return -i-1;
      else if (dictionary[table[i]].prefix==prefix && dictionary[table[i]].suffix==suffix) //is it the entry we want?
        return table[i];
      i-=offset;
      if (i<0)
        i+=HashSize;
    }
  }
  void addEntry(const int prefix, const int suffix, const int offset = -1){
    if (prefix==-1 || prefix>=index || index>4095 || offset>=0)
      return;
    dictionary[index].prefix = prefix;
    dictionary[index].suffix = suffix;
    table[-offset-1] = index;
    index+=(index<4096);
  }
  int dumpEntry(File *f, int code){
    int n = 4095;
    while (code>256 && n>=0){
      buffer[n] = U8(dictionary[code].suffix);
      n--;
      code = dictionary[code].prefix;
    }
    buffer[n] = U8(code);
    f->blockwrite(&buffer[n], 4096-n);
    return code;
  }
};

int encode_lzw(File *in, File *out, U64 size, int &hdrsize) {
  LZWDictionary dic;
  int parent=-1, code=0, buffer=0, bitsPerCode=9, bitsUsed=0;
  bool done = false;
  while (!done) {
    buffer = in->getchar();
    if (buffer<0) { return 0; }
    for (int j=0; j<8; j++ ) {
      code+=code+((buffer>>(7-j))&1), bitsUsed++;
      if (bitsUsed>=bitsPerCode) {
        if (code==LZW_EOF_CODE){ done=true; break; }
        else if (code==LZW_RESET_CODE){
          dic.reset();
          parent=-1; bitsPerCode=9;
        }
        else{
          if (code<dic.index){
            if (parent!=-1)
              dic.addEntry(parent, dic.dumpEntry(out, code));
            else
              out->putchar(code);
          }
          else if (code==dic.index){
            int a = dic.dumpEntry(out, parent);
            out->putchar(a);
            dic.addEntry(parent,a);
          }
          else return 0;
          parent = code;
        }
        bitsUsed=0; code=0;
        if ((1<<bitsPerCode)==dic.index+1 && dic.index<4096)
          bitsPerCode++;
      }
    }
  }
  return 1;
}

inline void writeCode(File *f, const FMode mode, int *buffer, U64 *pos, int *bitsUsed, const int bitsPerCode, const int code, U64 *diffFound){
  *buffer<<=bitsPerCode; *buffer|=code;
  (*bitsUsed)+=bitsPerCode;
  while ((*bitsUsed)>7) {
    const U8 B = *buffer>>(*bitsUsed-=8);
    (*pos)++;
    if (mode==FDECOMPRESS) f->putchar(B);
    else if (mode==FCOMPARE && B!=f->getchar()) *diffFound=*pos;
  }
}

U64 decode_lzw(File *in, U64 size, File *out, FMode mode, U64 &diffFound) {
  LZWDictionary dic;
  U64 pos=0;
  int parent=-1, code=0, buffer=0, bitsPerCode=9, bitsUsed=0;
  writeCode(out, mode, &buffer, &pos, &bitsUsed, bitsPerCode, LZW_RESET_CODE, &diffFound);
  while ((code=in->getchar())>=0 && diffFound==0) {
    int index = dic.findEntry(parent, code);
    if (index<0){ // entry not found
      writeCode(out, mode, &buffer, &pos, &bitsUsed, bitsPerCode, parent, &diffFound);
      if (dic.index>4092){
        writeCode(out, mode, &buffer, &pos, &bitsUsed, bitsPerCode, LZW_RESET_CODE, &diffFound);
        dic.reset();
        bitsPerCode = 9;
      }
      else{
        dic.addEntry(parent, code, index);
        if (dic.index>=(1<<bitsPerCode))
          bitsPerCode++;
      }
      parent = code;
    }
    else
      parent = index;
  }
  if (parent>=0)
    writeCode(out, mode, &buffer, &pos, &bitsUsed, bitsPerCode, parent, &diffFound);
  writeCode(out, mode, &buffer, &pos, &bitsUsed, bitsPerCode, LZW_EOF_CODE, &diffFound);
  if (bitsUsed>0) { // flush buffer
    pos++;
    if (mode==FDECOMPRESS) out->putchar(U8(buffer));
    else if (mode==FCOMPARE && U8(buffer)!=out->getchar()) diffFound=pos;
  }
  return pos;
}

// EXE transform: <encoded-size> <begin> <block>...
// Encoded-size is 4 bytes, MSB first.
// begin is the offset of the start of the input file, 4 bytes, MSB first.
// Each block applies the e8e9 transform to strings falling entirely
// within the block starting from the end and working backwards.
// The 5 byte pattern is E8/E9 xx xx xx 00/FF (x86 CALL/JMP xxxxxxxx)
// where xxxxxxxx is a relative address LSB first.  The address is
// converted to an absolute address by adding the offset mod 2^25
// (in range +-2^24).

void encode_exe(File *in, File *out, U64 len, U64 begin) {
  const int BLOCK=0x10000;
  Array<U8> blk(BLOCK);
  out->putVLI(begin); //TODO: Large file support

  // Transform
  for (U64 offset=0; offset<len; offset+=BLOCK) {
    U32 size=min(U32(len-offset), BLOCK);
    int bytesRead=(int)in->blockread(&blk[0], size);
    if (bytesRead!=(int)size) quit("encode_exe read error");
    for (int i=bytesRead-1; i>=5; --i) {
      if ((blk[i-4]==0xe8 || blk[i-4]==0xe9 || (blk[i-5]==0x0f && (blk[i-4]&0xf0)==0x80))
         && (blk[i]==0||blk[i]==0xff)) {
        int a=(blk[i-3]|blk[i-2]<<8|blk[i-1]<<16|blk[i]<<24)+(int)(offset+begin)+i+1;
        a<<=7;
        a>>=7;
        blk[i]=a>>24;
        blk[i-1]=a^176;
        blk[i-2]=(a>>8)^176;
        blk[i-3]=(a>>16)^176;
      }
    }
    out->blockwrite(&blk[0], bytesRead);
  }
}

U64 decode_exe(Encoder& en, U64 size, File *out, FMode mode, U64 &diffFound) {
  const int BLOCK=0x10000;  // block size
  int begin, offset=6, a;
  U8 c[6];
  begin=(int)en.decode_blocksize(); //TODO: Large file support
  size-=VLICost(U64(begin));
  for (int i=4; i>=0; i--) c[i]=en.decompress();  // Fill queue

  while (offset<(int)size+6) {
    memmove(c+1, c, 5);
    if (offset<=(int)size) c[0]=en.decompress();
    // E8E9 transform: E8/E9 xx xx xx 00/FF -> subtract location from x
    if ((c[0]==0x00 || c[0]==0xFF) && (c[4]==0xE8 || c[4]==0xE9 || (c[5]==0x0F && (c[4]&0xF0)==0x80))
     && (((offset-1)^(offset-6))&-BLOCK)==0 && offset<=(int)size) { // not crossing block boundary
      a=((c[1]^176)|(c[2]^176)<<8|(c[3]^176)<<16|c[0]<<24)-offset-begin;
      a<<=7;
      a>>=7;
      c[3]=a;
      c[2]=a>>8;
      c[1]=a>>16;
      c[0]=a>>24;
    }
    if (mode==FDECOMPRESS) out->putchar(c[5]);
    else if (mode==FCOMPARE && c[5]!=out->getchar() && !diffFound) diffFound=offset-6+1;
    if (mode==FDECOMPRESS && !(offset&0xfff)) en.print_status();
    offset++;
  }
  return size;
}

#ifdef USE_ZLIB
MTFList  MTF(81);

int encode_zlib(File *in, File *out, U64 len, int &hdrsize) {
  const int BLOCK=1<<16, LIMIT=128;
  U8 zin[BLOCK*2],zout[BLOCK],zrec[BLOCK*2], diffByte[81*LIMIT];
  U64 diffPos[81*LIMIT];

  // Step 1 - parse offset type form zlib stream header
  U64 pos_backup=in->curpos();
  U32 h1=in->getchar(), h2=in->getchar();
  in->setpos(pos_backup);
  int zh=parse_zlib_header(h1*256+h2);
  int memlevel,clevel,window=zh==-1?0:MAX_WBITS+10+zh/4,ctype=zh%4;
  int minclevel=window==0?1:ctype==3?7:ctype==2?6:ctype==1?2:1;
  int maxclevel=window==0?9:ctype==3?9:ctype==2?6:ctype==1?5:1;
  int index=-1, nTrials=0;
  bool found=false;

  // Step 2 - check recompressiblitiy, determine parameters and save differences
  z_stream main_strm, rec_strm[81];
  int diffCount[81], recpos[81], main_ret=Z_STREAM_END;
  main_strm.zalloc=Z_NULL; main_strm.zfree=Z_NULL; main_strm.opaque=Z_NULL;
  main_strm.next_in=Z_NULL; main_strm.avail_in=0;
  if (zlib_inflateInit(&main_strm,zh)!=Z_OK) return false;
  for (int i=0; i<81; i++) {
    clevel=(i/9)+1;
    // Early skip if invalid parameter
    if (clevel<minclevel || clevel>maxclevel){
      diffCount[i]=LIMIT;
      continue;
    }
    memlevel=(i%9)+1;
    rec_strm[i].zalloc=Z_NULL; rec_strm[i].zfree=Z_NULL; rec_strm[i].opaque=Z_NULL;
    rec_strm[i].next_in=Z_NULL; rec_strm[i].avail_in=0;
    int ret=deflateInit2(&rec_strm[i], clevel, Z_DEFLATED, window-MAX_WBITS, memlevel, Z_DEFAULT_STRATEGY);
    diffCount[i]=(ret==Z_OK)?0:LIMIT;
    recpos[i]=BLOCK*2;
    diffPos[i*LIMIT]=0xFFFFFFFFFFFFFFFF;
    diffByte[i*LIMIT]=0;
  }

  for (U64 i=0; i<len; i+=BLOCK) {
    U32 blsize=min(U32(len-i),BLOCK);
    nTrials=0;
    for (int j=0; j<81; j++) {
      if (diffCount[j]==LIMIT) continue;
      nTrials++;
      if (recpos[j]>=BLOCK)
        recpos[j]-=BLOCK;
    }
    // early break if nothing left to test
    if (nTrials==0)
      break;
    memmove(&zrec[0], &zrec[BLOCK], BLOCK);
    memmove(&zin[0], &zin[BLOCK], BLOCK);
    in->blockread(&zin[BLOCK], blsize); // Read block from input file

    // Decompress/inflate block
    main_strm.next_in=&zin[BLOCK]; main_strm.avail_in=blsize;
    do {
      main_strm.next_out=&zout[0]; main_strm.avail_out=BLOCK;
      main_ret=inflate(&main_strm, Z_FINISH);
      nTrials=0;

      // Recompress/deflate block with all possible parameters
      for (int j=MTF.GetFirst(); j>=0; j=MTF.GetNext()){
        if (diffCount[j]==LIMIT) continue;
        nTrials++;
        rec_strm[j].next_in=&zout[0];  rec_strm[j].avail_in=BLOCK-main_strm.avail_out;
        rec_strm[j].next_out=&zrec[recpos[j]]; rec_strm[j].avail_out=BLOCK*2-recpos[j];
        int ret=deflate(&rec_strm[j], main_strm.total_in == len ? Z_FINISH : Z_NO_FLUSH);
        if (ret!=Z_BUF_ERROR && ret!=Z_STREAM_END && ret!=Z_OK) { diffCount[j]=LIMIT; continue; }

        // Compare
        int end=2*BLOCK-(int)rec_strm[j].avail_out;
        int tail=max(main_ret==Z_STREAM_END ? (int)len-(int)rec_strm[j].total_out : 0,0);
        for (int k=recpos[j]; k<end+tail; k++) {
          if ((k<end && i+k-BLOCK<len && zrec[k]!=zin[k]) || k>=end) {
            if (++diffCount[j]<LIMIT) {
              const int p=j*LIMIT+diffCount[j];
              diffPos[p]=i+k-BLOCK;
              assert(k < int(sizeof(zin)/sizeof(*zin)));
              diffByte[p]=zin[k];
            }
          }
        }
        // Early break on perfect match
        if (main_ret==Z_STREAM_END && diffCount[j]==0){
          index=j;
          found=true;
          break;
        }
        recpos[j]=2*BLOCK-rec_strm[j].avail_out;
      }
    } while (main_strm.avail_out==0 && main_ret==Z_BUF_ERROR && nTrials>0);
    if ((main_ret!=Z_BUF_ERROR && main_ret!=Z_STREAM_END) || nTrials==0) break;
  }
  int minCount=(found)?0:LIMIT;
  for (int i=80; i>=0; i--) {
    clevel=(i/9)+1;
    if (clevel>=minclevel && clevel<=maxclevel)
      deflateEnd(&rec_strm[i]);
    if (!found && diffCount[i]<minCount)
      minCount=diffCount[index=i];
  }
  inflateEnd(&main_strm);
  if (minCount==LIMIT) return false;
  MTF.MoveToFront(index);

  // Step 3 - write parameters, differences and precompressed (inflated) data
  out->putchar(diffCount[index]);
  out->putchar(window);
  out->putchar(index);
  for (int i=0; i<=diffCount[index]; i++) {
    const int v=i==diffCount[index] ? int(len-diffPos[index*LIMIT+i])
                                    : int(diffPos[index*LIMIT+i+1]-diffPos[index*LIMIT+i])-1;
    out->put32(v);
  }
  for (int i=0; i<diffCount[index]; i++) out->putchar(diffByte[index*LIMIT+i+1]);

  in->setpos(pos_backup);
  main_strm.zalloc=Z_NULL; main_strm.zfree=Z_NULL; main_strm.opaque=Z_NULL;
  main_strm.next_in=Z_NULL; main_strm.avail_in=0;
  if (zlib_inflateInit(&main_strm,zh)!=Z_OK) return false;
  for (U64 i=0; i<len; i+=BLOCK) {
    U32 blsize=min(U32(len-i),BLOCK);
    in->blockread(&zin[0], blsize);
    main_strm.next_in=&zin[0]; main_strm.avail_in=blsize;
    do {
      main_strm.next_out=&zout[0]; main_strm.avail_out=BLOCK;
      main_ret=inflate(&main_strm, Z_FINISH);
      out->blockwrite(&zout[0], BLOCK-main_strm.avail_out);
    } while (main_strm.avail_out==0 && main_ret==Z_BUF_ERROR);
    if (main_ret!=Z_BUF_ERROR && main_ret!=Z_STREAM_END) break;
  }
  inflateEnd(&main_strm);
  hdrsize=diffCount[index]*5+7;
  return main_ret==Z_STREAM_END;
}

int decode_zlib(File *in, U64 size, File *out, FMode mode, U64 &diffFound) {
  const int BLOCK=1<<16, LIMIT=128;
  U8 zin[BLOCK],zout[BLOCK];
  int diffCount=min(in->getchar(),LIMIT-1);
  int window=in->getchar()-MAX_WBITS;
  int index=in->getchar();
  int memlevel=(index%9)+1;
  int clevel=(index/9)+1;
  int len=0;
  int diffPos[LIMIT];
  diffPos[0]=-1;
  for (int i=0; i<=diffCount; i++) {
    int v=in->get32();
    if (i==diffCount) len=v+diffPos[i]; else diffPos[i+1]=v+diffPos[i]+1;
  }
  U8 diffByte[LIMIT];
  diffByte[0]=0;
  for (int i=0; i<diffCount; i++) diffByte[i+1]=in->getchar();
  size-=7+5*diffCount;

  z_stream rec_strm;
  int diffIndex=1,recpos=0;
  rec_strm.zalloc=Z_NULL; rec_strm.zfree=Z_NULL; rec_strm.opaque=Z_NULL;
  rec_strm.next_in=Z_NULL; rec_strm.avail_in=0;
  int ret=deflateInit2(&rec_strm, clevel, Z_DEFLATED, window, memlevel, Z_DEFAULT_STRATEGY);
  if (ret!=Z_OK) return 0;
  for (U64 i=0; i<size; i+=BLOCK) {
    U32 blsize=min(U32(size-i),BLOCK);
    in->blockread(&zin[0], blsize);
    rec_strm.next_in=&zin[0];  rec_strm.avail_in=blsize;
    do {
      rec_strm.next_out=&zout[0]; rec_strm.avail_out=BLOCK;
      ret=deflate(&rec_strm, i+blsize==size ? Z_FINISH : Z_NO_FLUSH);
      if (ret!=Z_BUF_ERROR && ret!=Z_STREAM_END && ret!=Z_OK) break;
      const int have=min(BLOCK-rec_strm.avail_out,len-recpos);
      while (diffIndex<=diffCount && diffPos[diffIndex]>=recpos && diffPos[diffIndex]<recpos+have) {
        zout[diffPos[diffIndex]-recpos]=diffByte[diffIndex];
        diffIndex++;
      }
      if (mode==FDECOMPRESS) out->blockwrite(&zout[0], have);
      else if (mode==FCOMPARE) for (int j=0; j<have; j++) if (zout[j]!=out->getchar() && !diffFound) diffFound=recpos+j+1;
      recpos+=have;

    } while (rec_strm.avail_out==0);
  }
  while (diffIndex<=diffCount) {
    if (mode==FDECOMPRESS) out->putchar(diffByte[diffIndex]);
    else if (mode==FCOMPARE) if (diffByte[diffIndex]!=out->getchar() && !diffFound) diffFound=recpos+1;
    diffIndex++;
    recpos++;
  }
  deflateEnd(&rec_strm);
  return recpos==len ? len : 0;
}
#endif //USE_ZLIB

//
// decode/encode base64
//
static const char  table1[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
bool isbase64(unsigned char c) {
    return (isalnum(c) || (c == '+') || (c == '/')|| (c == 10) || (c == 13));
}

U64 decode_base64(File *in, File *out, FMode mode, U64 &diffFound){
    U8 inn[3];
    int i, len=0, blocksout = 0;
    int fle=0;
    int linesize=0;
    int outlen=0;
    int tlf=0;
    linesize=in->getchar();
    outlen=in->getchar();
    outlen+=(in->getchar()<<8);
    outlen+=(in->getchar()<<16);
    tlf=(in->getchar());
    outlen+=((tlf&63)<<24);
    Array<U8> ptr((outlen>>2)*4+10);
    tlf=(tlf&192);
    if (tlf==128)
       tlf=10;        // LF: 10
    else if (tlf==64)
       tlf=13;        // LF: 13
    else
       tlf=0;

    while(fle<outlen){
      len=0;
      for(i=0;i<3;i++){
        int c=in->getchar();
        if(c!=EOF) {
          inn[i]=c;
          len++;
        }
        else
          inn[i]=0;
      }
      if(len){
        U8 in0,in1,in2;
        in0=inn[0],in1=inn[1],in2=inn[2];
        ptr[fle++]=(table1[in0>>2]);
        ptr[fle++]=(table1[((in0&0x03)<<4)|((in1&0xf0)>>4)]);
        ptr[fle++]=((len>1?table1[((in1&0x0f)<<2)|((in2&0xc0)>>6)]:'='));
        ptr[fle++]=((len>2?table1[in2&0x3f]:'='));
        blocksout++;
      }
      if(blocksout>=(linesize/4) && linesize!=0){ //no lf if linesize==0
        if( blocksout && !in->eof() && fle<=outlen) { //no lf if eof
          if (tlf) ptr[fle++]=(tlf);
          else ptr[fle++]=13,ptr[fle++]=10;
        }
        blocksout = 0;
      }
    }
    //Write out or compare
    if (mode==FDECOMPRESS){
      out->blockwrite(&ptr[0], outlen);
    }
    else if (mode==FCOMPARE){
      for(i=0;i<outlen;i++){
        U8 b=ptr[i];
        if (b!=out->getchar() && !diffFound) diffFound=(int)out->curpos();
      }
    }
    return outlen;
}

inline char valueb(char c){
  const char *p = strchr(table1, c);
  if (p) return (char)(p - table1);
  return 0;
}

void encode_base64(File *in, File *out, U64 len64) {
  int len=(int)len64;
  int in_len = 0;
  int i = 0;
  int j = 0;
  int b=0;
  int lfp=0;
  int tlf=0;
  char src[4];
  int b64mem=(len>>2)*3+10;
  Array<U8> ptr(b64mem);
  int olen=5;

  while (b=in->getchar(),in_len++ , ( b != '=') && isbase64(b) && in_len<=len) {
    if (b==13 || b==10) {
       if (lfp==0) lfp=in_len ,tlf=b;
       if (tlf!=b) tlf=0;
       continue;
    }
    src[i++] = b;
    if (i ==4){
          for (j = 0; j <4; j++) src[j] = valueb(src[j]);
          src[0] = (src[0] << 2) + ((src[1] & 0x30) >> 4);
          src[1] = ((src[1] & 0xf) << 4) + ((src[2] & 0x3c) >> 2);
          src[2] = ((src[2] & 0x3) << 6) + src[3];

          ptr[olen++]=src[0];
          ptr[olen++]=src[1];
          ptr[olen++]=src[2];
      i = 0;
    }
  }

  if (i){
    for (j=i;j<4;j++)
      src[j] = 0;

    for (j=0;j<4;j++)
      src[j] = valueb(src[j]);

    src[0] = (src[0] << 2) + ((src[1] & 0x30) >> 4);
    src[1] = ((src[1] & 0xf) << 4) + ((src[2] & 0x3c) >> 2);
    src[2] = ((src[2] & 0x3) << 6) + src[3];

    for (j=0;(j<i-1);j++) {
        ptr[olen++]=src[j];
    }
  }
  ptr[0]=lfp&255; //nl length
  ptr[1]=len&255;
  ptr[2]=(len>>8)&255;
  ptr[3]=(len>>16)&255;
  if (tlf!=0) {
    if (tlf==10) ptr[4]=128;
    else ptr[4]=64;
  }
  else
      ptr[4]=(len>>24)&63; //1100 0000
  out->blockwrite(&ptr[0], olen);
}

#define LZW_TABLE_SIZE 9221

#define lzw_find(k) {\
  offset = (int)finalize64(k*PHI64,13); \
  int stride = (offset>0)?LZW_TABLE_SIZE-offset:1; \
  while (true){ \
    if ((index=table[offset])<0){ index=-offset-1; break; } \
    else if (dict[index]==int(k)){ break; } \
    offset-=stride; \
    if (offset<0) \
      offset+=LZW_TABLE_SIZE; \
  } \
}

#define lzw_reset { for (int i=0; i<LZW_TABLE_SIZE; table[i]=-1, i++); }

int encode_gif(File *in, File *out, U64 len, int &hdrsize) {
  int codesize=in->getchar(),diffpos=0,clearpos=0,bsize=0,code,offset=0;
  U64 beginin=in->curpos(),beginout=out->curpos();
  Array<U8> output(4096);
  hdrsize=6;
  out->putchar(hdrsize>>8);
  out->putchar(hdrsize&255);
  out->putchar(bsize);
  out->putchar(clearpos>>8);
  out->putchar(clearpos&255);
  out->putchar(codesize);
  Array<int> table(LZW_TABLE_SIZE);  
  for (int phase=0; phase<2; phase++) {
    in->setpos(beginin);
    int bits=codesize+1,shift=0,buffer=0;
    int blocksize=0,maxcode=(1<<codesize)+1,last=-1;
    Array<int> dict(4096);
    lzw_reset;
    bool end=false;
    while ((blocksize=in->getchar())>0 && in->curpos()-beginin<len && !end) {
      for (int i=0; i<blocksize; i++) {
        buffer|=in->getchar()<<shift;
        shift+=8;
        while (shift>=bits && !end) {
          code=buffer&((1<<bits)-1);
          buffer>>=bits;
          shift-=bits;
          if (!bsize && code!=(1<<codesize)) {
            hdrsize+=4; out->put32(0);
          }
          if (!bsize) bsize=blocksize;
          if (code==(1<<codesize)) {
            if (maxcode>(1<<codesize)+1) {
              if (clearpos && clearpos!=69631-maxcode) return 0;
              clearpos=69631-maxcode;
            }
            bits=codesize+1, maxcode=(1<<codesize)+1, last=-1;
            lzw_reset;
          }
          else if (code==(1<<codesize)+1) end=true;
          else if (code>maxcode+1) return 0;
          else {
            int j=(code<=maxcode?code:last),size=1;
            while (j>=(1<<codesize)) {
              output[4096-(size++)]=dict[j]&255;
              j=dict[j]>>8;
            }
            output[4096-size]=j;
            if (phase==1) out->blockwrite(&output[4096-size], size); else diffpos+=size;
            if (code==maxcode+1) { if (phase==1) out->putchar(j); else diffpos++; }
            if (last!=-1) {
              if (++maxcode>=8191) return 0;
              if (maxcode<=4095)
              {
                int key=(last<<8)+j, index=-1;
                lzw_find(key);
                dict[maxcode]=key;
                table[(index<0)?-index-1:offset]=maxcode;
                if (phase==0 && index>0) {
                  hdrsize+=4;
                  j=diffpos-size-(code==maxcode);
                  out->put32(j);
                  diffpos=size+(code==maxcode);
                }
              }
              if (maxcode>=((1<<bits)-1) && bits<12) bits++;
            }
            last=code;
          }
        }
      }
    }
  }
  diffpos=(int)out->curpos();
  out->setpos(beginout);
  out->putchar(hdrsize>>8);
  out->putchar(hdrsize&255);
  out->putchar(255-bsize);
  out->putchar((clearpos>>8)&255);
  out->putchar(clearpos&255);
  out->setpos(diffpos);
  return in->curpos()-beginin==len-1;
}

#define gif_write_block(count) { output[0]=(count);\
if (mode==FDECOMPRESS) out->blockwrite(&output[0], (count)+1);\
else if (mode==FCOMPARE) for (int j=0; j<(count)+1; j++) if (output[j]!=out->getchar() && !diffFound) { diffFound=outsize+j+1; return 1; } \
outsize+=(count)+1; blocksize=0; }

#define gif_write_code(c) { buffer+=(c)<<shift; shift+=bits;\
while (shift>=8) { output[++blocksize]=buffer&255; buffer>>=8;shift-=8;\
if (blocksize==bsize) gif_write_block(bsize); }}

int decode_gif(File *in, U64 size, File *out, FMode mode, U64 &diffFound) {
  int diffcount=in->getchar(), curdiff=0;
  Array<int> diffpos(4096);
  diffcount=((diffcount<<8)+in->getchar()-6)/4;
  int bsize=255-in->getchar();
  int clearpos=in->getchar(); clearpos=(clearpos<<8)+in->getchar();
  clearpos=(69631-clearpos)&0xffff;
  int codesize=in->getchar(),bits=codesize+1,shift=0,buffer=0,blocksize=0;
  if (diffcount>4096 || clearpos<=(1<<codesize)+2) return 1;
  int maxcode=(1<<codesize)+1,input,code,offset=0;
  Array<int> dict(4096);
  Array<int> table(LZW_TABLE_SIZE);
  lzw_reset;
  for (int i=0; i<diffcount; i++) {
    diffpos[i]=in->getchar();
    diffpos[i]=(diffpos[i]<<8)+in->getchar();
    diffpos[i]=(diffpos[i]<<8)+in->getchar();
    diffpos[i]=(diffpos[i]<<8)+in->getchar();
    if (i>0) diffpos[i]+=diffpos[i-1];
  }
  Array<U8> output(256);
  size-=6+diffcount*4;
  int last=in->getchar(),total=(int)size+1,outsize=1;
  if (mode==FDECOMPRESS) out->putchar(codesize);
  else if (mode==FCOMPARE) if (codesize!=out->getchar() && !diffFound) diffFound=1;
  if (diffcount==0 || diffpos[0]!=0) gif_write_code(1<<codesize) else curdiff++;
  while (size!=0 && (input=in->getchar())!=EOF) {
    size--;
    int key=(last<<8)+input, index=(code=-1);
    if (last<0) index=input; else lzw_find(key);
    code = index;
    if (curdiff<diffcount && total-(int)size>diffpos[curdiff]) curdiff++, code=-1;
    if (code<0) {
      gif_write_code(last);
      if (maxcode==clearpos) { gif_write_code(1<<codesize); bits=codesize+1, maxcode=(1<<codesize)+1; lzw_reset }
      else
      {
        ++maxcode;
        if (maxcode<=4095) { dict[maxcode]=key; table[(index<0)?-index-1:offset]=maxcode; }
        if (maxcode>=(1<<bits) && bits<12) bits++;
      }
      code=input;
    }
    last=code;
  }
  gif_write_code(last);
  gif_write_code((1<<codesize)+1);
  if (shift>0) {
    output[++blocksize]=buffer&255;
    if (blocksize==bsize) gif_write_block(bsize);
  }
  if (blocksize>0) gif_write_block(blocksize);
  if (mode==FDECOMPRESS) out->putchar(0);
  else if (mode==FCOMPARE) if (0!=out->getchar() && !diffFound) diffFound=outsize+1;
  return outsize+1;
}


//////////////////// Compress, Decompress ////////////////////////////

void direct_encode_block(Blocktype type, File *in, U64 len, Encoder &en, int info=-1) {
  //TODO: Large file support
  en.compress(type);
  en.encode_blocksize(len);
  if (info!=-1) {
    en.compress((info>>24)&0xFF);
    en.compress((info>>16)&0xFF);
    en.compress((info>>8)&0xFF);
    en.compress((info)&0xFF);
  }
  fprintf(stderr,"Compressing... ");
  for (U64 j=0; j<len; ++j) {
    if ((j&0xfff)==0) en.print_status(j, len);
    en.compress(in->getchar());
  }
  fprintf(stderr,"\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b");
}

void compressRecursive(File *in, U64 blocksize, Encoder &en, String &blstr, int recursion_level, float p1, float p2);

U64 decode_func(Blocktype type, Encoder &en, File *tmp, U64 len, int info, File *out, FMode mode, U64 &diffFound) {
  if (type==IMAGE24) return decode_bmp(en, len, info, out, mode, diffFound);
  else if (type==IMAGE32) return decode_im32(en, len, info, out, mode, diffFound);
  else if (type==AUDIO_LE) return decode_endianness16b(en, len, out, mode, diffFound);
  else if (type==EXE) return decode_exe(en, len, out, mode, diffFound);
  else if (type == TEXT_EOL) return decode_eol(en, len, out, mode, diffFound);
  else if (type==CD) return decode_cd(tmp, len, out, mode, diffFound);
  #ifdef USE_ZLIB
  else if (type==ZLIB) return decode_zlib(tmp, len, out, mode, diffFound);
  #endif //USE_ZLIB
  else if (type==BASE64) return decode_base64(tmp, out, mode, diffFound);
  else if (type==GIF) return decode_gif(tmp, len, out, mode, diffFound);
  else if (type==RLE) return decode_rle(tmp, len, out, mode, diffFound);
  else if (type==LZW) return decode_lzw(tmp, len, out, mode, diffFound);
  else assert(false);
  return 0;
}

U64 encode_func(Blocktype type, File *in, File *tmp, U64 len, int info, int &hdrsize) {
  if (type==IMAGE24) encode_bmp(in, tmp, len, info);
  else if (type==IMAGE32) encode_im32(in, tmp, len, info);
  else if (type==AUDIO_LE) encode_endianness16b(in, tmp, len);
  else if (type==EXE) encode_exe(in, tmp, len, info);
  else if (type == TEXT_EOL) encode_eol(in, tmp, len);
  else if (type==CD) encode_cd(in, tmp, len, info);
  #ifdef USE_ZLIB
  else if (type==ZLIB) return encode_zlib(in, tmp, len, hdrsize)?0:1;
  #endif //USE_ZLIB
  else if (type==BASE64) encode_base64(in, tmp, len);
  else if (type==GIF) return encode_gif(in, tmp, len, hdrsize)?0:1;
  else if (type==RLE) encode_rle(in, tmp, len, info, hdrsize);
  else if (type==LZW) return encode_lzw(in, tmp, len, hdrsize)?0:1;
  else assert(false);
  return 0;
}

void transform_encode_block(Blocktype type, File *in, U64 len, Encoder &en, int info, String &blstr, int recursion_level, float p1, float p2, U64 begin) {
  if (hasTransform(type)) {
    FileTmp tmp;
    int hdrsize=0;
    U64 diffFound=encode_func(type, in, &tmp, len, info, hdrsize);
    const U64 tmpsize=tmp.curpos();
    tmp.setpos(tmpsize); //switch to read mode
    if (diffFound==0) {
      tmp.setpos(0);
      en.setFile(&tmp);
      in->setpos(begin);
      decode_func(type, en, &tmp, tmpsize, info, in, FCOMPARE, diffFound);
    }
    // Test fails, compress without transform
    if (diffFound>0 || tmp.getchar()!=EOF) {
      printf("Transform fails at %" PRIu64 ", skipping...\n", diffFound-1);
      in->setpos(begin);
      direct_encode_block(DEFAULT, in, len, en);
    } else {
      tmp.setpos(0);
      if (hasRecursion(type)) {
        //TODO: Large file support
        en.compress(type);
        en.encode_blocksize(tmpsize);
        Blocktype type2=(Blocktype)((info>>24)&0xFF);
        if (type2!=DEFAULT) {
          String blstr_sub0;blstr_sub0+=blstr.c_str();blstr_sub0+="->";
          String blstr_sub1;blstr_sub1+=blstr.c_str();blstr_sub1+="-->";
          String blstr_sub2;blstr_sub2+=blstr.c_str();blstr_sub2+="-->";
          printf(" %-11s | ->  exploded     |%10d bytes [%d - %d]\n",blstr_sub0.c_str(),int(tmpsize),0,int(tmpsize-1));
          printf(" %-11s | --> added header |%10d bytes [%d - %d]\n",blstr_sub1.c_str(),hdrsize,0,hdrsize-1);
          direct_encode_block(HDR, &tmp, hdrsize, en);
          printf(" %-11s | --> data         |%10d bytes [%d - %d]\n",blstr_sub2.c_str(),int(tmpsize-hdrsize),hdrsize,int(tmpsize-1));
          transform_encode_block(type2, &tmp, tmpsize-hdrsize, en, info&0xffffff, blstr, recursion_level, p1, p2, hdrsize);
        } else {
          compressRecursive(&tmp, tmpsize, en, blstr, recursion_level+1, p1, p2);
        }
      } else {
        direct_encode_block(type, &tmp, tmpsize, en, hasInfo(type)?info:-1);
      }
    }
    tmp.close();
  } else {
    direct_encode_block(type, in, len, en, hasInfo(type)?info:-1);
  }
}

void compressRecursive(File *in, const U64 blocksize, Encoder &en, String &blstr, int recursion_level, float p1, float p2) {
  static const char* typenames[25]={"default", "filecontainer", "jpeg", "hdr", "1b-image", "4b-image", "8b-image", "8b-img-grayscale",
    "24b-image", "32b-image", "audio", "audio - le", "exe", "cd", "zlib", "base64", "gif", "png-8b", "png-8b-grayscale", "png-24b", "png-32b", "text", "text - eol", "rle", "lzw"};
  static const char* audiotypes[4]={"8b-mono", "8b-stereo", "16b-mono","16b-stereo"};
  Blocktype type=DEFAULT;
  int blnum=0, info=0;  // image width or audio type
  U64 begin=in->curpos();
  U64 block_end=begin+blocksize;
  if (recursion_level==5) {
    direct_encode_block(DEFAULT, in, blocksize, en);
    return;
  }
  float pscale=blocksize>0?(p2-p1)/blocksize:0;

  // Transform and test in blocks
  U64 nextblock_start;
  U64 textstart;
  U64 textend=0;
  Blocktype nextblock_type;
  Blocktype nextblock_type_bak=DEFAULT; //initialized only to suppress a compiler warning, will be overwritten
  U64 bytes_to_go=blocksize;
  while (bytes_to_go>0) {
    if(type==TEXT || type==TEXT_EOL) { // it was a split block in the previous iteration: TEXT -> DEFAULT -> ...
      nextblock_type=nextblock_type_bak;
      nextblock_start=textend+1;
    }
    else {
      nextblock_type=detect(in, bytes_to_go, type, info);
      nextblock_start=in->curpos();
      in->setpos(begin);
    }

    // override (any) next block detection by a preceding text block
    textstart=begin+textparser._start[0];
    textend=begin+textparser._end[0];
    if(textend>nextblock_start-1)textend=nextblock_start-1;
    if(type==DEFAULT && textstart<textend) { // only DEFAULT blocks may be overridden
      if(textstart==begin && textend == nextblock_start-1) { // whole first block is text
        type=(textparser._EOLType[0]==1)?TEXT_EOL:TEXT; // DEFAULT -> TEXT
      }
      else if (textend - textstart + 1 >= TEXT_MIN_SIZE) { // we have one (or more) large enough text portion that splits DEFAULT
        if (textstart != begin) { // text is not the first block 
          nextblock_start=textstart; // first block is still DEFAULT
          nextblock_type_bak=nextblock_type;
          nextblock_type=(textparser._EOLType[0]==1)?TEXT_EOL:TEXT; //next block is text
          textparser.removefirst();
        } else {
          type=(textparser._EOLType[0]==1)?TEXT_EOL:TEXT; // first block is text
          nextblock_type=DEFAULT;     // next block is DEFAULT
          nextblock_start=textend+1; 
        }
      }
      // no text block is found, still DEFAULT
    }

    if (nextblock_start>block_end) {  // if a detection reports a larger size than the actual block size, fall back
      nextblock_start=begin+1;
      type=nextblock_type=DEFAULT;
    }

    U64 len=nextblock_start-begin;
    if (len>0) {
      en.set_status_range(p1,p2=p1+pscale*len);

      //Compose block enumeration string
      String blstr_sub;
      blstr_sub+=blstr.c_str();
      if (blstr_sub.strsize()!=0) blstr_sub+="-";
      blstr_sub+=U64(blnum);
      blnum++;

      printf(" %-11s | %-16s |%10" PRIu64 " bytes [%" PRIu64 " - %" PRIu64 "]",blstr_sub.c_str(),typenames[(type==ZLIB && isPNG(Blocktype(info>>24)))?info>>24:type],len,begin,nextblock_start-1);
      if (type==AUDIO || type==AUDIO_LE) printf(" (%s)", audiotypes[info%4]);
      else if (type==IMAGE1 || type==IMAGE4 || type==IMAGE8 || type==IMAGE8GRAY || type==IMAGE24 || type==IMAGE32 || (type==ZLIB && isPNG(Blocktype(info>>24)))) printf(" (width: %d)", (type==ZLIB)?(info&0xFFFFFF):info);
      else if (hasRecursion(type) && (info>>24)!=DEFAULT) printf(" (%s)",typenames[info>>24]);
      else if (type==CD) printf(" (mode%d/form%d)", info==1?1:2, info!=3?1:2);
      printf("\n");
      transform_encode_block(type, in, len, en, info, blstr_sub, recursion_level, p1, p2, begin);
      p1=p2;
      bytes_to_go-=len;
    }
    type=nextblock_type;
    begin=nextblock_start;
  }
}

// Compress a file. Split filesize bytes into blocks by type.
// For each block, output
// <type> <size> and call encode_X to convert to type X.
// Test transform and compress.
void compressfile(const char* filename, U64 filesize, Encoder& en, bool verbose) {
  assert(en.getMode()==COMPRESS);
  assert(filename && filename[0]);

  en.compress(FILECONTAINER);
  U64 start=en.size();
  en.encode_blocksize(filesize);

  FileDisk in;
  in.open(filename, true);
  printf("Block segmentation:\n");
  String blstr;
  compressRecursive(&in, filesize, en, blstr, 0, 0.0f, 1.0f);
  in.close();

  if(options & OPTION_MULTIPLE_FILE_MODE) { //multiple file mode
    if(verbose)
    printf("File size to encode   : 4\n"); //This string must be long enough. "Compressing ..." is still on screen, we need to overwrite it.
    printf("File input size       : %" PRIu64 "\n",filesize);
    printf("File compressed size  : %" PRIu64 "\n",en.size()-start);
  }
}

U64 decompressRecursive(File *out, U64 blocksize, Encoder& en, FMode mode, int recursion_level) {
  Blocktype type;
  U64 len, i=0;
  U64 diffFound=0;
  int info=0;
  while (i<blocksize) {
    type=(Blocktype)en.decompress();
    len=en.decode_blocksize();
    if (hasInfo(type)) {
      info=0; for (int i=0; i<4; ++i) { info<<=8; info+=en.decompress(); }
    }
    if (hasRecursion(type)) {
      FileTmp tmp;
      decompressRecursive(&tmp, len, en, FDECOMPRESS, recursion_level+1);
      if (mode!=FDISCARD) {
        tmp.setpos(0);
        if (hasTransform(type)) len=decode_func(type, en, &tmp, len, info, out, mode, diffFound);
      }
      tmp.close();
    } else if (hasTransform(type)) {
      len=decode_func(type, en, NULL, len, info, out, mode, diffFound);
    } else {
      for (U64 j=0; j<len; ++j) {
        if (!(j&0xfff)) en.print_status();
        if (mode==FDECOMPRESS) out->putchar(en.decompress());
        else if (mode==FCOMPARE) {
          if (en.decompress()!=out->getchar() && !diffFound) {
            mode=FDISCARD;
            diffFound=i+j+1;
          }
        } else en.decompress();
      }
    }
    i+=len;
  }
  return diffFound;
}

// Decompress or compare a file
void decompressfile(const char* filename, FMode fmode, Encoder& en) {
  assert(en.getMode()==DECOMPRESS);
  assert(filename && filename[0]);

  Blocktype blocktype=(Blocktype)en.decompress();
  if(blocktype!=FILECONTAINER)quit("Bad archive.");
  U64 filesize=en.decode_blocksize();

  FileDisk f;
  if(fmode==FCOMPARE){
    f.open(filename,true);
    printf("Comparing");
  }
  else { //mode==FDECOMPRESS;
    f.create(filename);
    printf("Extracting");
  }
  printf(" %s %" PRIu64 " bytes -> ", filename, filesize);

  // Decompress/Compare
  U64 r=decompressRecursive(&f, filesize, en, fmode, 0);
  if (fmode==FCOMPARE && !r && f.getchar()!=EOF) printf("file is longer\n");
  else if (fmode==FCOMPARE && r) printf("differ at %" PRIu64 "\n",r-1);
  else if (fmode==FCOMPARE) printf("identical\n");
  else printf("done   \n");
  f.close();
}


//////////////////////////// User Interface ////////////////////////////


class ListOfFiles {
private:
  typedef enum{IN_HEADER,FINISHED_A_FILENAME,FINISHED_A_LINE,PROCESSING_FILENAME} STATE;
  STATE state; //parsing state
  FileName basepath;
  String list_of_files; //path/file list in first column, columns separated by tabs, rows separated by newlines, with header in 1st row
  Array<FileName*> names;  //all filenames parsed from list_of_files
public:
  ListOfFiles():state(IN_HEADER),names(0) {}
  ~ListOfFiles() {for(int i=0;i<(int)names.size();i++)delete names[i];}
  void setbasepath(const char *s) {
    assert(basepath.strsize()==0);
    basepath+=s;
  }
  void addchar(char c) {
    if(c!=EOF)list_of_files+=c;
    if(c==10 || c==13 || c==EOF) //got a newline
      state=FINISHED_A_LINE; //empty lines / extra newlines (cr, crlf or lf) are ignored
    else if(state==IN_HEADER)return; //ignore anything in header
    else if(c==TAB) //got a tab
      state=FINISHED_A_FILENAME; //ignore the rest (other columns)
    else { // got a character
      if(state==FINISHED_A_FILENAME)return; //ignore the rest (other columns)
      if(state==FINISHED_A_LINE) {
        names.push_back(new FileName(basepath.c_str()));
        state=PROCESSING_FILENAME;
        if(c=='/' || c=='\\')
          quit("For security reasons absolute paths are not allowed in the file list.");
        //TODO: prohibit parent folder references in path ('/../')
      }
      if(c==0 || c==':' || c=='?' || c=='*') {printf("\nIllegal character ('%c') in file list.",c);quit();}
      if(c==BADSLASH)c=GOODSLASH;
      (*names[names.size()-1])+=c;
    }
  }
  int getcount() {return (int)names.size();}
  const char * getfilename(int i) {return names[i]->c_str();}
  String * getstring() {return &list_of_files;}
};

int main_utf8(int argc, char** argv) {
  try {

    if(!to_screen) //we need a minimal feedback when redirected
      fprintf(stderr,PROGNAME " archiver v" PROGVERSION " (C) " PROGYEAR ", Matt Mahoney et al.\n");
    printf(PROGNAME " archiver v" PROGVERSION " (C) " PROGYEAR ", Matt Mahoney et al.\n");

    // Print help message
    if (argc<2) {
      printf(
        "\n"
        "Free under GPL, http://www.gnu.org/licenses/gpl.txt\n\n"
        "To compress:\n"
        "\n"
        "  " PROGNAME " -LEVEL[SWITCHES] INPUTSPEC [OUTPUTSPEC]\n"
        "\n"
        "    -LEVEL:\n"
        "      -0 = store (uses 300 MB)\n"
        "      -1 -2 -3 = faster (uses 392, 406, 435 MB)\n"
        "      -4 -5 -6 -7 -8 -9 = smaller (uses 492, 605, 833, 1288, 2198, 4018 MB)\n"
        "    The listed memory requirements are indicative, actual usage may vary\n"
        "    depending on several factors including need for temporary files,\n"
        "    temporary memory needs of some preprocessing (transformations), etc.\n"
        "\n"
        "    Optional compression SWITCHES:\n"
        "      b = Brute-force detection of DEFLATE streams\n"
        "      e = Pre-train x86/x64 model\n"
        "      t = Pre-train main model with word and expression list\n"
        "          (english.dic, english.exp)\n"
        "      a = Adaptive learning rate\n"
        "      s = Skip the color transform, just reorder the RGB channels\n"
        "    INPUTSPEC:\n"
        "    The input may be a FILE or a PATH/FILE or a [PATH/]@FILELIST.\n"
        "    Only file content and the file size is kept in the archive. Filename,\n"
        "    path, date and any file attributes or permissions are not stored.\n"
        "    When a @FILELIST is provided the FILELIST file will be considered\n"
        "    implicitly as the very first input file. It will be compressed and upon\n"
        "    decompression it will be extracted. The FILELIST is a tab separated text\n"
        "    file where the first column contains the names and optionally the relative\n"
        "    paths of the files to be compressed. The paths should be relative to the\n"
        "    FILELIST file. In the other columns you may store any information you wish\n"
        "    to keep about the files (timestamp, owner, attributes or your own remarks).\n"
        "    These extra columns will be ignored by the compressor and the decompressor\n"
        "    but you may restore full file information using them with a 3rd party\n"
        "    utility. The FILELIST file must contain a header but will be ignored.\n"
        "\n"
        "    OUTPUTSPEC:\n"
        "    When omitted: the archive will be created in the same folder where the\n"
        "    input file resides. The archive filename will be constructed from the\n"
        "    input file name by appending ." PROGNAME PROGVERSION " extension\n"
        "    to it.\n"
        "    When OUTPUTSPEC is a filename (with an optional path) it will be\n"
        "    used as the archive filename.\n"
        "    When OUTPUTSPEC is a folder the archive file will be generated from\n"
        "    the input filename and will be created in the specified folder.\n"
        "    If the archive file already exists it will be overwritten.\n"
        "\n"
        "    Examples:\n"
        "      " PROGNAME " -8 enwik8\n"
        "      " PROGNAME " -8ba b64sample.xml\n"
        "      " PROGNAME " -8 @myfolder/myfilelist.txt\n"
        "      " PROGNAME " -8a benchmark/enwik8 results/enwik8_a_" PROGNAME PROGVERSION "\n"
        "\n"
        "To extract (decompress contents):\n"
        "\n"
        "  " PROGNAME " -d [INPUTPATH/]ARCHIVEFILE [[OUTPUTPATH/]OUTPUTFILE]\n"
        "    If an output folder is not provided the output file will go to the input\n"
        "    folder. If an output filename is not provided output filename will be the\n"
        "    same as ARCHIVEFILE without the last extension (e.g. without ." PROGNAME PROGVERSION")\n"
        "    When OUTPUTPATH does not exist it will be created.\n"
        "    When the archive contains multiple files, first the @LISTFILE is extracted\n"
        "    then the rest of the files. Any required folders will be created.\n"
        "\n"
        "To test:\n"
        "\n"
        "  " PROGNAME " -t [INPUTPATH/]ARCHIVEFILE [[OUTPUTPATH/]OUTPUTFILE]\n"
        "    Tests contents of the archive by decompressing it (to memory) and comparing\n"
        "    the result to the original file(s). If a file fails the test, the first\n"
        "    mismatched position will be printed to screen.\n"
        "\n"
        "To list archive contents:\n"
        "\n"
        "  " PROGNAME " -l [INPUTFOLDER/]ARCHIVEFILE\n"
        "    Extracts @FILELIST from archive (to memory) and prints its content\n"
        "    to screen. This command is only applicable to multi-file archives.\n"
        "\n"
        "Additional optional swithes:\n"
        "\n"
        "    -v\n"
        "    Print more detailed (verbose) information to screen.\n"
        "\n"
        "    -log LOGFILE\n"
        "    Logs (appends) compression results in the specified tab separated LOGFILE.\n"
        "    Logging is only applicable for compression.\n"
        "\n"
        "    -simd [NONE|SSE2|AVX2]\n"
        "    Overrides detected SIMD instruction set for neural network operations\n"
        "\n"
        "Remark: the command line arguments may be used in any order except the input\n"
        "and output: always the input comes first then (the optional) output.\n"
        "\n"
        "    Example:\n"
        "      " PROGNAME " -8 enwik8 folder/ -v -log logfile.txt -simd sse2\n"
        "    is equivalent to:\n"
        "      " PROGNAME " -v -simd sse2 enwik8 -log logfile.txt folder/ -8\n"
      );
      quit();
    }

    // Parse command line arguments
    typedef enum {doNone, doCompress, doExtract, doCompare, doList} WHATTODO;
    WHATTODO whattodo=doNone;
    bool verbose=false;
    int c;
    int simd_iset=-1; //simd instruction set to use

    FileName input;
    FileName output;
    FileName inputpath;
    FileName outputpath;
    FileName archiveName;
    FileName logfile;

    for(int i=1;i<argc;i++) {
      int arg_len = (int)strlen(argv[i]);
      if(argv[i][0]=='-') {
        if(arg_len==1)quit("Empty command.");
        if (argv[i][1]>='0' && argv[i][1]<='9') {
          if(whattodo != doNone)quit("Only one command may be specified.");
          whattodo = doCompress;
          level = argv[i][1]-'0';
          //process optional compression switches
          for(int j=2;j<arg_len;j++) {
            switch (argv[i][j]&0xDF) {
              case 'B': options |= OPTION_BRUTE; break;
              case 'E': options |= OPTION_TRAINEXE; break;
              case 'T': options |= OPTION_TRAINTXT; break;
              case 'A': options |= OPTION_ADAPTIVE; break;
              case 'S': options |= OPTION_SKIPRGB; break;
              default: {printf("Invalid compression switch: %c",argv[1][j]); quit();}
            }
          }
        }
        else if (strcasecmp(argv[i],"-d")==0) {
          if(whattodo != doNone)quit("Only one command may be specified.");
          whattodo=doExtract;
        }
        else if (strcasecmp(argv[i],"-t")==0) {
          if(whattodo != doNone)quit("Only one command may be specified.");
          whattodo=doCompare;
        }
        else if (strcasecmp(argv[i],"-l")==0) {
          if(whattodo != doNone)quit("Only one command may be specified.");
          whattodo=doList;
        }
        else if (strcasecmp(argv[i],"-v")==0) {
          verbose=true;
        }
        else if (strcasecmp(argv[i],"-log")==0) {
          if(logfile.strsize()!=0)quit("Only one logfile may be specified.");
          if(++i==argc)quit("The -log switch requires a filename.");
          logfile+=argv[i];
        }
        else if (strcasecmp(argv[i],"-simd")==0) {
          if(++i==argc)quit("The -simd switch requires an instruction set name (NONE,SSE2,AVX2).");
          if(strcasecmp(argv[i],"NONE")==0)simd_iset=0;
          else if(strcasecmp(argv[i],"SSE2")==0)simd_iset=3;
          else if(strcasecmp(argv[i],"AVX2")==0)simd_iset=9;
          else quit("Invalid -simd option. Use -simd NONE, -simd SSE2 or -simd AVX2.");
        }
        else {
          printf("Invalid command: %s",argv[i]);
          quit();
        }
      } else { //this parameter does not begin with a dash ("-") -> it must be a folder/filename
        if(input.strsize()==0) {
          input+=argv[i];
          input.replaceslashes();
        }
        else if(output.strsize()==0) {
          output+=argv[i];
          output.replaceslashes();
        }
        else
          quit("More than two filenames specified. Only an input and an output is needed.");
      }
    }

    if(verbose) {
      //print compiled-in modules
      printf("\n");
      printf("Build: ");
      #ifdef USE_ZLIB
      printf("USE_ZLIB ");
      #else
      printf("NO_ZLIB ");
      #endif

      #ifdef USE_AUDIOMODEL
      printf("USE_AUDIOMODEL ");
      #else
      printf("NO_WAVMODEL ");
      #endif

      #ifdef USE_TEXTMODEL
      printf("USE_TEXTMODEL ");
      #else
      printf("NO_TEXTMODEL ");
      #endif

      printf("\n");
    }

    // Determine CPU's (and OS) support for SIMD vectorization istruction set 
    int detected_simd_iset = simd_detect();
    if(simd_iset==-1)simd_iset=detected_simd_iset;
    if(simd_iset>detected_simd_iset)printf("\nOverriding system highest vectorization support. Expect a crash.");

    // Print anything only if the user wants/needs to know
    if(verbose || simd_iset!=detected_simd_iset) {
      printf("\nHighest SIMD vectorization support on this system: ");
      if(detected_simd_iset<0 || detected_simd_iset>9)quit("Oops, sorry. Unexpected result.");
      static const char* vectorization_string[10]={"None","MMX","SSE","SSE2","SSE3","SSSE3","SSE4.1","SSE4.2","AVX","AVX2"};
      printf("%s.\n", vectorization_string[detected_simd_iset]);

      printf("Using ");
      if(simd_iset>=9) printf("AVX2");
      else if(simd_iset>=3) printf("SSE2");  
      else printf("non-vectorized");  
      printf(" neural network functions.\n");  
    }

    // Set highest or user selected vectorization mode
    if(simd_iset>=9) MixerFactory::set_simd(SIMD_AVX2);
    else if(simd_iset>=3) MixerFactory::set_simd(SIMD_SSE2);
    else MixerFactory::set_simd(SIMD_NONE);

    if(verbose) {
      printf("\n");
      printf(" Command line   =");
      for(int i=0;i<argc;i++)printf(" %s",argv[i]);
      printf("\n");
    }

    // Successfully parsed command line arguments
    // Let's check their validity
    if(whattodo==doNone)
      quit("A command switch is required: -0..-9 to compress, -d to decompress, -t to test, -l to list.");
    if(input.strsize()==0) {
      printf("\nAn %s is required %s.\n",whattodo==doCompress ? "input file or filelist" : "archive filename",
      whattodo==doCompress ? "for compressing" :
      whattodo==doExtract ? "for decompressing" :
      whattodo==doCompare ? "for testing" :
      whattodo==doList ? "to list its contents":""
      );
      quit();
    }
    if(whattodo==doList && output.strsize()!=0)
      quit("The list command needs only one file parameter.");

    // File list supplied?
    if(input.beginswith("@")) {
      if(whattodo==doCompress) {
        options|=OPTION_MULTIPLE_FILE_MODE;
        input.stripstart(1);
      } else
        quit("A file list (a file name prefixed by '@') may only be specified when compressing.");
    }

    int pathtype;

    //Logfile supplied?
    if(logfile.strsize()!=0) {
      if(whattodo!=doCompress)
        quit("A log file may only be specified for compression.");
      pathtype=examinepath(logfile.c_str());
      if(pathtype==2 || pathtype==4)
        quit("Specified log file should be a file not a directory.");
      if(pathtype==0) {
        printf("\nThere is a problem with the log file: %s",logfile.c_str());
        quit();
      }
    }

    // Separate paths from input filename/directory name
    pathtype=examinepath(input.c_str());
    if(pathtype==2 || pathtype==4) {
      printf("\nSpecified input is a directory but should be a file: %s",input.c_str());quit();
    }
    if(pathtype==3) {
      printf("\nSpecified input file does not exist: %s",input.c_str());quit();
    }
    if(pathtype==0) {
      printf("\nThere is a problem with the specified input file: %s",input.c_str());quit();
    }
    if(input.lastslashpos()>=0) {
      inputpath+=input.c_str();
      inputpath.keeppath();
      input.keepfilename();
    }

    // Separate paths from output filename/directory name
    if(output.strsize()>0) {
      pathtype=examinepath(output.c_str());
      if(pathtype==1 || pathtype==3) { //is an existing file, or looks like a file
        if(output.lastslashpos()>=0) {
          outputpath+=output.c_str();
          outputpath.keeppath();
          output.keepfilename();
        }
      }
      else if (pathtype==2 || pathtype==4) {//is an existing directory, or looks like a directory
        outputpath+=output.c_str();
        if(!outputpath.endswith("/") && !outputpath.endswith("\\"))
          outputpath+=GOODSLASH;
        //output file is not specified
        output.resize(0);
        output.push_back(0);
      }
      else {
        printf("\nThere is a problem with the specified output: %s",output.c_str());
        quit();
      }
    }

    //determine archive name
    if(whattodo==doCompress) {
      archiveName+=outputpath.c_str();
      if(output.strsize()==0) { // If no archive name is provided, construct it from input (append PROGNAME extension to input filename)
        archiveName+=input.c_str();
        archiveName+="." PROGNAME PROGVERSION;
      }
      else
        archiveName+=output.c_str();
    } else { // extract/compare/list: archivename is simply the input
      archiveName+=inputpath.c_str();
      archiveName+=input.c_str();
    }
    if(verbose) {
      printf(" Archive        = %s\n",archiveName.c_str());
      printf(" Input folder   = %s\n",inputpath.strsize()==0 ? "." : inputpath.c_str());
      printf(" Output folder  = %s\n",outputpath.strsize()==0 ? "." : outputpath.c_str());
    }

    Mode mode = whattodo==doCompress ? COMPRESS : DECOMPRESS;

    ListOfFiles listoffiles;

    //set basepath for filelist
    listoffiles.setbasepath(whattodo==doCompress ? inputpath.c_str() : outputpath.c_str());

    // Process file list (in multiple file mode)
    if(options & OPTION_MULTIPLE_FILE_MODE) { //multiple file mode
      assert(whattodo==doCompress);
      // Read and parse filelist file
      FileDisk f;
      FileName fn(inputpath.c_str());fn+=input.c_str();
      f.open(fn.c_str(),true);
      while(true) {c=f.getchar();listoffiles.addchar(c);if(c==EOF)break;}
      f.close();
      //Verify input files
      for(int i=0;i<listoffiles.getcount();i++)
        getfilesize(listoffiles.getfilename(i)); // Does file exist? Is it readable? (we don't actually need the filesize now)
    } else { //single file mode or extract/compare/list
      FileName fn(inputpath.c_str());fn+=input.c_str();
      getfilesize(fn.c_str()); // Does file exist? Is it readable? (we don't actually need the filesize now)
    }

    FileDisk archive;  // compressed file

    if(mode==DECOMPRESS) {
      archive.open(archiveName.c_str(),true);
      // Verify archive header, get level and options
      int len=(int)strlen(PROGNAME);
      for(int i=0;i<len;i++)
        if(archive.getchar()!=PROGNAME[i]) {printf("%s: not a valid %s file.", archiveName.c_str(), PROGNAME); quit();}
      level=archive.getchar();
      c=archive.getchar();
      if(c==EOF) printf("Unexpected end of archive file.\n");
      options=(U8)c;
    }

    if(verbose) {
      // Print specified command
      printf(" To do          = ");
      if(whattodo==doNone)printf("-");
      if(whattodo==doCompress)printf("Compress");
      if(whattodo==doExtract)printf("Extract");
      if(whattodo==doCompare)printf("Compare");
      if(whattodo==doList)printf("List");
      printf("\n");
      // Print specified options
      printf(" Level          = %d\n",level);
      printf(" Brute      (b) = %s\n",options&OPTION_BRUTE    ? "On  (Brute-force detection of DEFLATE streams)" : "Off"); //this is a compression-only option, but we put/get it for reproducibility
      printf(" Train exe  (e) = %s\n",options&OPTION_TRAINEXE ? "On  (Pre-train x86/x64 model)" : "Off");
      printf(" Train txt  (t) = %s\n",options&OPTION_TRAINTXT ? "On  (Pre-train main model with word and expression list)" : "Off");
      printf(" Adaptive   (a) = %s\n",options&OPTION_ADAPTIVE ? "On  (Adaptive learning rate)" : "Off");
      printf(" Skip RGB   (s) = %s\n",options&OPTION_SKIPRGB  ? "On  (Skip the color transform, just reorder the RGB channels)" : "Off");
      printf(" File mode      = %s\n",options&OPTION_MULTIPLE_FILE_MODE ? "Multiple": "Single");
    }
    printf("\n");

    int number_of_files=1; //default for single file mode

    // Write archive header to archive file
    if (mode==COMPRESS) {
      if(options & OPTION_MULTIPLE_FILE_MODE) { //multiple file mode
        number_of_files=listoffiles.getcount();
        printf("Creating archive %s in multiple file mode with %d file%s...\n",archiveName.c_str(), number_of_files, number_of_files>1?"s":"");
      } else //single file mode
        printf("Creating archive %s in single file mode...\n",archiveName.c_str());
      archive.create(archiveName.c_str());
      archive.append(PROGNAME);
      archive.putchar(level);
      archive.putchar(options);
    }

    // In single file mode with no output filename specified we must construct it from the supplied archive filename
    if((options & OPTION_MULTIPLE_FILE_MODE)==0) { //single file mode
      if((whattodo==doExtract || whattodo==doCompare) && output.strsize()==0) {
        output+=input.c_str();
        const char* file_extension="." PROGNAME PROGVERSION;
        if(output.endswith(file_extension))
          output.stripend((int)strlen(file_extension));
        else {
          printf("Can't construct output filename from archive filename.\nArchive file extension must be: '%s'",file_extension);
          quit();
        }
      }
    }

    // Set globals according to requested compression level
    assert(level>=0 && level<=9);
    Encoder en(mode, &archive);
    U64 content_size=0;
    U64 total_size=0;

    // Compress list of files
    if (mode==COMPRESS) {
      U64 start=en.size(); //header size (=8)
      if(verbose)
      printf("Writing header : %" PRIu64 " bytes\n", start);
      total_size+=start;
      if ((options & OPTION_MULTIPLE_FILE_MODE)!=0) { //multiple file mode

        en.compress(TEXT);
        U64 len1=input.size(); //ASCIIZ filename of listfile - with ending zero
        const String * const s=listoffiles.getstring();
        U64 len2=s->size(); //ASCIIZ filenames of files to compress - with ending zero
        en.encode_blocksize(len1+len2);
        
        for (U64 i=0; i<len1; i++)
          en.compress(input[i]); //ASCIIZ filename of listfile
        for (U64 i=0; i<len2; i++)
          en.compress((*s)[i]); //ASCIIZ filenames of files to compress

        printf("1/2 - Filename of listfile : %" PRIu64 " bytes\n", len1);
        printf("2/2 - Content of listfile  : %" PRIu64 " bytes\n", len2);
        printf("----- Compressed to        : %" PRIu64 " bytes\n", en.size()-start);
        total_size+=len1+len2;
      }
    }

    // Decompress list of files
    if (mode==DECOMPRESS && (options & OPTION_MULTIPLE_FILE_MODE)!=0) {
      const char* errmsg_invalid_char="Invalid character or unexpected end of archive file.";
      // name of listfile
      FileName list_filename(outputpath.c_str());
      if(output.strsize()!=0)
        quit("Output filename must not be specified when extracting multiple files.");
      if((c=en.decompress())!=TEXT)quit(errmsg_invalid_char);
      en.decode_blocksize(); //we don't really need it
      while((c=en.decompress())!=0) {
        if(c==255)quit(errmsg_invalid_char);
        list_filename+=(char)c;
      }
      while((c=en.decompress())!=0) {
        if(c==255)quit(errmsg_invalid_char);
        listoffiles.addchar((char)c);
      }
      if (whattodo==doList)
        printf("File list of %s archive:\n", archiveName.c_str());

      number_of_files=listoffiles.getcount();

      //write filenames to screen or listfile or verify (compare) contents
      if (whattodo==doList)
        printf("%s\n",listoffiles.getstring()->c_str());
      else if (whattodo==doExtract) {
        FileDisk f;
        f.create(list_filename.c_str());
        String *s=listoffiles.getstring();
        f.blockwrite((U8*)(&(*s)[0]),s->strsize());
        f.close();
      }
      else if (whattodo==doCompare) {
        FileDisk f;
        f.open(list_filename.c_str(),true);
        String *s=listoffiles.getstring();
        for(U64 i=0;i<s->strsize();i++)
          if(f.getchar()!=(U8)(*s)[i])
            quit("Mismatch in list of files.");
        if(f.getchar()!=EOF) printf("Filelist on disk is larger than in archive.\n");
        f.close();
      }
    }

    if(whattodo==doList && (options & OPTION_MULTIPLE_FILE_MODE)==0)
      quit("Can't list. Filenames are not stored in single file mode.\n");

    // Compress or decompress files
    if (mode==COMPRESS) {
      if(!to_screen) //we need a minimal feedback when redirected
        fprintf(stderr,"Output is redirected - only minimal feedback is on screen\n");
      if ((options & OPTION_MULTIPLE_FILE_MODE)!=0) { //multiple file mode
        for (int i=0; i<number_of_files; i++) {
          const char* fname=listoffiles.getfilename(i);
          U64 fsize=getfilesize(fname);
          if(!to_screen) //we need a minimal feedback when redirected
            fprintf(stderr,"\n%d/%d - Filename: %s (%" PRIu64 " bytes)\n", i+1, number_of_files, fname, fsize);
          printf("\n%d/%d - Filename: %s (%" PRIu64 " bytes)\n", i+1, number_of_files, fname, fsize);
          compressfile(fname, fsize, en, verbose);
          total_size+=fsize+4; //4: file size information
          content_size+=fsize;
        }
      } else { //single file mode
        FileName fn;fn+=inputpath.c_str();fn+=input.c_str();
        const char * fname=fn.c_str();
        U64 fsize=getfilesize(fname);
        if(!to_screen) //we need a minimal feedback when redirected
          fprintf(stderr,"\nFilename: %s (%" PRIu64 " bytes)\n",fname, fsize);
        printf("\nFilename: %s (%" PRIu64 " bytes)\n",fname, fsize);
        compressfile(fname, fsize, en, verbose);
        total_size+=fsize+4; //4: file size information
        content_size+=fsize;
      }

      U64 pre_flush=en.size();
      en.flush();
      total_size+=en.size()-pre_flush; //we consider padding bytes as auxiliary bytes
      printf("-----------------------\n");
      printf("Total input size     : %" PRIu64 "\n", content_size);
      if(verbose)
      printf("Total metadata bytes : %" PRIu64 "\n", total_size-content_size);
      printf("Total archive size   : %" PRIu64 "\n", en.size());
      printf("\n");
      // Log compression results
      if(logfile.strsize()!=0) {
        String results;
        pathtype=examinepath(logfile.c_str());
        //Write header if needed
        if(pathtype==3 /*does not exist*/ || (pathtype==1 && getfilesize(logfile.c_str())==0)/*exists but does not contain a header*/)
          results+="PROG_NAME\tPROG_VERSION\tCOMMAND_LINE\tLEVEL\tINPUT_FILENAME\tORIGINAL_SIZE_BYTES\tCOMPRESSED_SIZE_BYTES\tRUNTIME_MS\n";
        //Write results to logfile
        results+=PROGNAME "\t" PROGVERSION "\t";
        for(int i=1;i<argc;i++){if(i!=0)results+=' ';results+=argv[i];}results+="\t";
        results+=U64(level);results+="\t";
        results+=input.c_str();results+="\t";
        results+=content_size;results+="\t";
        results+=en.size();results+="\t";
        results+=U64(programChecker.get_runtime()*1000.0);results+="\t";
        results+="\n";
        appendtofile(logfile.c_str(),results.c_str());
        printf("Results logged to file '%s'\n",logfile.c_str());
        printf("\n");
      }
    } else { //decompress
      if(whattodo==doExtract || whattodo==doCompare) {
        FMode fmode= whattodo==doExtract ? FDECOMPRESS : FCOMPARE;
        if ((options & OPTION_MULTIPLE_FILE_MODE)!=0) { //multiple file mode
          for (int i=0; i<number_of_files; i++) {
            const char* fname=listoffiles.getfilename(i);
            decompressfile(fname, fmode, en);
          }
        } else { //single file mode
          FileName fn;fn+=outputpath.c_str();fn+=output.c_str();
          const char * fname=fn.c_str();
          decompressfile(fname, fmode, en);
        }
      }
    }

    archive.close();
    if (whattodo!=doList) programChecker.print();
  }
  // we catch only the intentional exceptions from quit() to exit gracefully
  // any other exception should result in a crash and must be investigated
  catch(IntentionalException const&) {}

  return 0;
}

int main(int argc, char** argv) {
  #ifdef WINDOWS
    // On Windows, argv is encoded in the effective codepage, therefore unsuitable for acquiring command line arguments (file names
    // in our case) not representable in that particular codepage.
    // -> We will recreate argv as UTF8 (like in Linux)
    U32 oldcp = GetConsoleOutputCP();
    SetConsoleOutputCP(CP_UTF8);
    wchar_t **szArglist;
    int argc_utf8;
    char** argv_utf8;
    if( (szArglist = CommandLineToArgvW(GetCommandLineW(), &argc_utf8)) == NULL) {
      printf("CommandLineToArgvW failed\n");
      return 0;
    } else {
      if(argc!=argc_utf8)quit("Error preparing command line arguments.");
      argv_utf8=new char*[argc_utf8+1];
      for(int i=0; i<argc_utf8; i++) { 
        wchar_t *s=szArglist[i];
        int buffersize = WideCharToMultiByte(CP_UTF8,0,s,-1,NULL,0,NULL,NULL);
        argv_utf8[i] = new char[buffersize];
        WideCharToMultiByte(CP_UTF8,0,s,-1,argv_utf8[i],buffersize,NULL,NULL);
        //printf("%d: %s\n", i, argv_utf8[i]); //debug: see if conversion is successful
      }
      argv_utf8[argc_utf8]=nullptr;
      int retval=main_utf8(argc_utf8, argv_utf8);
      for(int i=0; i<argc_utf8; i++)
        delete[] argv_utf8[i]; 
      delete[] argv_utf8;
      SetConsoleOutputCP(oldcp);
      return retval;
    }
  #else
    return main_utf8(argc, argv);
  #endif
}
