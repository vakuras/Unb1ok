///
/// vUtil Implementation
///
/// Written by Vadim Kuras. 2010.
///

#include <crtdbg.h>
#include <stdarg.h>
#include "vutil.h"
#include "unzip.h"

#define TRACEMAXSTRING 1024

bool vutil::LogToFile;
FILE* vutil::LogFile = NULL;
PTCHAR vutil::AppName = L"";

EXTERN_C IMAGE_DOS_HEADER __ImageBase;

void vutil::SetAppName(PTCHAR appName)
{
	vutil::AppName = appName;
}

void vutil::GetDllNameChangeExt(PTCHAR path, const PTCHAR ext, DWORD dwSize)
{
	GetModuleFileName((HINSTANCE)&__ImageBase, path, dwSize);
	PTCHAR pChr = _tcsrchr(path, '.');
	
	if (pChr)
		*(pChr+1) = NULL;

	_tcscat_s(path, dwSize, ext);
}

void vutil::InitTraceFileTarget()
{
	TCHAR path[MAX_PATH];
	GetDllNameChangeExt(path, L"log", MAX_PATH);

	if (_tfopen_s(&LogFile, path, TEXT("w, ccs=UNICODE")))
	{
		LogToFile = false;
		return;
	}

	LogToFile = true;
}

void vutil::DestryTraceFileTarget()
{
	LogToFile = false;

	if (LogFile)
		fclose(LogFile);

	LogFile = NULL;
}


void vutil::TraceImpl(const PCHAR functionName, int lineNumber, DWORD errCode, const PTCHAR format,...)
{
	#define TRACEMAXSTRING 1024
	static TCHAR szBuffer1[TRACEMAXSTRING];
	static TCHAR szBuffer2[TRACEMAXSTRING];

    va_list args;
    va_start(args,format);
    _vsntprintf_s(szBuffer1, TRACEMAXSTRING, format, args);
    va_end(args);

	if (errCode)
		_stprintf_s(szBuffer2, TRACEMAXSTRING, _T("%s | %S(%d) | %s | Error[%d]\n"), AppName, functionName, lineNumber, szBuffer1, errCode);
	else
		_stprintf_s(szBuffer2, TRACEMAXSTRING, _T("%s | %S(%d) | %s\n"), AppName, functionName, lineNumber, szBuffer1);

    OutputDebugString(szBuffer2);

	if (!LogToFile)
		return;

	if (_ftprintf(LogFile, _T("%s"), szBuffer2) > -1)
		return;
		
	LogToFile = false;
	DestryTraceFileTarget();
}

LPVOID vutil::GetResourceBuffer(HINSTANCE hInstance, LPCTSTR lpName, LPCTSTR lpType, PDWORD length)
{
	HRSRC hRSRC = FindResource(hInstance, lpName, lpType);

	if (!hRSRC)
	{
		TraceErr(TEXT("FindResource failed"));
		return NULL;
	}

	HANDLE hResouce = LoadResource(hInstance, hRSRC);

	if (!hResouce)
	{
		TraceErr(TEXT("LoadResource failed"));
		return NULL;
	}

	LPVOID resource = LockResource(hResouce);
	*length = SizeofResource(hInstance, hRSRC);

	return resource;
}

HZIP vutil::OpenZip(LPVOID buffer, DWORD length)
{
	return OpenZip(buffer, length, NULL);
}

HZIP vutil::OpenZip(LPVOID buffer, DWORD length, const char* password)
{
	return ::OpenZip(buffer, length, password);
}

ZRESULT vutil::UnZip(HZIP hz, LPVOID* buffer, const PTCHAR name, PLONG size)
{
	ZIPENTRY zipEntry;
	int zipIndex;

	ZRESULT result = FindZipItem(hz, name, true, &zipIndex, &zipEntry);

	if (result != ZR_OK)
		return result;

	*buffer = new char[zipEntry.unc_size];

	result = UnzipItem(hz, zipIndex, *buffer, zipEntry.unc_size);

	if (result != ZR_OK)
		delete [] *buffer;

	*size = zipEntry.unc_size;

	return result;
}

void vutil::FreeBuffer(LPVOID* buffer)
{
	delete [] *buffer;
	*buffer = NULL;
}

ZRESULT vutil::FreeZip(HZIP handle)
{
	return ::CloseZip(handle);
}

errno_t vutil::ReadStructFromFile(PTCHAR fileName, LPVOID buffer, DWORD length)
{
	errno_t result = 0;
	FILE * file = NULL;

	__try
	{
		result = _tfopen_s(&file, fileName, TEXT("rb"));

		if (result)
		{
			Trace(TEXT("_tfopen_s failed"));
			return result;
		}
	
		if (fread_s(buffer, length, length, 1, file) != 1)
		{
			result = errno;
			TraceErrWithCode(result, TEXT("fread_s failed"));
		}

		return result;
	}
	__finally
	{
		if (file)
			fclose(file);
	}
}

errno_t vutil::WriteStructToFile(PTCHAR fileName, LPVOID buffer, DWORD length)
{
	errno_t result = 0;
	FILE * file = NULL;

	__try
	{
		result = _tfopen_s(&file, fileName, TEXT("wb"));

		if (result)
		{
			Trace(TEXT("_tfopen_s failed"));
			return result;
		}
	
		if (fwrite(buffer, length, 1, file) != 1)
		{
			result = errno;
			TraceErrWithCode(result, TEXT("fwrite failed"));
		}

		return result;
	}
	__finally
	{
		if (file)
			fclose(file);
	}
}