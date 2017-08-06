///
/// vUtil Header
///
/// Written by Vadim Kuras. 2010.
///

#ifndef __VUTIL_HEADER
#define __VUTIL_HEADER

#include <windows.h>
#include <tchar.h>
#include <string.h>
#include <stdio.h>

#define Trace(format, ...) vutil::TraceImpl(__FUNCTION__, __LINE__, 0, format, __VA_ARGS__)
#define TraceErrWithCode(errCode, format, ...) vutil::TraceImpl(__FUNCTION__, __LINE__, errCode, format, __VA_ARGS__)
#define TraceErr(format, ...) vutil::TraceImpl(__FUNCTION__, __LINE__, GetLastError(), format, __VA_ARGS__)

DECLARE_HANDLE(HZIP);

typedef DWORD ZRESULT;

// These are the result codes:
#define ZR_OK         0x00000000     // nb. the pseudo-code zr-recent is never returned,
#define ZR_RECENT     0x00000001     // but can be passed to FormatZipMessage.
// The following come from general system stuff (e.g. files not openable)
#define ZR_GENMASK    0x0000FF00
#define ZR_NODUPH     0x00000100     // couldn't duplicate the handle
#define ZR_NOFILE     0x00000200     // couldn't create/open the file
#define ZR_NOALLOC    0x00000300     // failed to allocate some resource
#define ZR_WRITE      0x00000400     // a general error writing to the file
#define ZR_NOTFOUND   0x00000500     // couldn't find that file in the zip
#define ZR_MORE       0x00000600     // there's still more data to be unzipped
#define ZR_CORRUPT    0x00000700     // the zipfile is corrupt or not a zipfile
#define ZR_READ       0x00000800     // a general error reading the file
#define ZR_PASSWORD   0x00001000     // we didn't get the right password to unzip the file
// The following come from mistakes on the part of the caller
#define ZR_CALLERMASK 0x00FF0000
#define ZR_ARGS       0x00010000     // general mistake with the arguments
#define ZR_NOTMMAP    0x00020000     // tried to ZipGetMemory, but that only works on mmap zipfiles, which yours wasn't
#define ZR_MEMSIZE    0x00030000     // the memory size is too small
#define ZR_FAILED     0x00040000     // the thing was already failed when you called this function
#define ZR_ENDED      0x00050000     // the zip creation has already been closed
#define ZR_MISSIZE    0x00060000     // the indicated input file size turned out mistaken
#define ZR_PARTIALUNZ 0x00070000     // the file had already been partially unzipped
#define ZR_ZMODE      0x00080000     // tried to mix creating/opening a zip 
// The following come from bugs within the zip library itself
#define ZR_BUGMASK    0xFF000000
#define ZR_NOTINITED  0x01000000     // initialisation didn't work
#define ZR_SEEK       0x02000000     // trying to seek in an unseekable file
#define ZR_NOCHANGE   0x04000000     // changed its mind on storage, but not allowed
#define ZR_FLATE      0x05000000     // an internal error in the de/inflation code

class vutil
{
private:
	static bool								LogToFile;
	static FILE*							LogFile;
	static PTCHAR							AppName;

public:
	static void								SetAppName(PTCHAR appName);
	static void								TraceImpl(const PCHAR functionName, int lineNumber, DWORD errCode, const PTCHAR format,...);
	static void								InitTraceFileTarget();
	static void								DestryTraceFileTarget();
	static void								GetDllNameChangeExt(PTCHAR path, const PTCHAR ext, DWORD dwSize);
	
	//resource
	static LPVOID GetResourceBuffer(HINSTANCE hInstance, LPCTSTR lpName, LPCTSTR lpType, PDWORD length);

	//zip
	static HZIP OpenZip(LPVOID buffer, DWORD length);
	static HZIP OpenZip(LPVOID buffer, DWORD length, const char* password);
	static ZRESULT FreeZip(HZIP handle);
	static ZRESULT UnZip(HZIP hz, LPVOID* buffer, const PTCHAR name, PLONG size);
	static void FreeBuffer(LPVOID* buffer);

	//file
	static errno_t ReadStructFromFile(PTCHAR fileName, LPVOID buffer, DWORD length);
	static errno_t WriteStructToFile(PTCHAR fileName, LPVOID buffer, DWORD length);
};

#endif //__VUTIL_HEADER