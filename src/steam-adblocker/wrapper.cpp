//
//  version.dll
//
//  Copyright (c) 2018, Takayuki Matsuoka <takayuki.matsuoka@gmail.com>
//
//  Permission is hereby granted, free of charge, to any person obtaining a copy
//  of this software and associated documentation files (the "Software"), to deal
//  in the Software without restriction, including without limitation the rights
//  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
//  copies of the Software, and to permit persons to whom the Software is
//  furnished to do so, subject to the following conditions:
//
//  The above copyright notice and this permission notice shall be included in all
//  copies or substantial portions of the Software.
//
//  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
//  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
//  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
//  SOFTWARE.
//

#include <SDKDDKVer.h>
#include <windows.h>
#include <tchar.h>
#include <stdlib.h>     // _countof()

static HMODULE hModuleVersionDll = nullptr;

bool initialize_wrapper() {
	if (hModuleVersionDll == nullptr) {
		const TCHAR* dllName = _T("version.dll");

		// systemDllName : "C:\Windows\System32"
		// fullpathDllName : "C:\Windows\System32\version.dll"
		TCHAR systemDirectory[MAX_PATH] = {};
		GetSystemDirectory(systemDirectory, _countof(systemDirectory));
		TCHAR fullpathDllName[MAX_PATH] = {};
		_sntprintf_s(fullpathDllName, _countof(fullpathDllName), _T("%s\\%s"), systemDirectory, dllName);

		// Load "genuine" version.dll
		hModuleVersionDll = LoadLibrary(fullpathDllName);

		if (!hModuleVersionDll) {
			return 0;
		}
		return 1;
	}
	return 0;
}


bool deinitialize_wrapper() {
	if (hModuleVersionDll != nullptr) {
		FreeLibrary(hModuleVersionDll);
		hModuleVersionDll = nullptr;
		return 1;
	}
	return 0;
}


template<typename T>
void setup(T*& funcPtr, const char* funcName) {
	if (funcPtr != nullptr) {
		return;
	}
	funcPtr = reinterpret_cast<T*>(GetProcAddress(hModuleVersionDll, funcName));
}


// Function Name     : GetFileVersionInfoA
// Ordinal           : 1 (0x1)
namespace P { BOOL(WINAPI* GetFileVersionInfoA)(LPCSTR lptstrFilename, DWORD dwHandle, DWORD dwLen, LPVOID lpData); }
extern "C"    BOOL WINAPI  GetFileVersionInfoA(LPCSTR lptstrFilename, DWORD dwHandle, DWORD dwLen, LPVOID lpData) {
	setup(P::GetFileVersionInfoA, __FUNCTION__);
	return  P::GetFileVersionInfoA(lptstrFilename, dwHandle, dwLen, lpData);
#pragma comment(linker, "/EXPORT:" __FUNCTION__ "=" __FUNCDNAME__)
}



// Function Name     : GetFileVersionInfoByHandle
// Ordinal           : 2 (0x2)
namespace P { int(WINAPI* GetFileVersionInfoByHandle)(int hMem, LPCWSTR lpFileName, int v2, int v3); }
extern "C"    int WINAPI  GetFileVersionInfoByHandle(int hMem, LPCWSTR lpFileName, int v2, int v3) {
	setup(P::GetFileVersionInfoByHandle, __FUNCTION__);
	return P::GetFileVersionInfoByHandle(hMem, lpFileName, v2, v3);
#pragma comment(linker, "/EXPORT:" __FUNCTION__ "=" __FUNCDNAME__)
}


// Function Name     : GetFileVersionInfoExA
// Ordinal           : 3 (0x3)
namespace P { BOOL(WINAPI* GetFileVersionInfoExA)(DWORD dwFlags, LPCSTR lpwstrFilename, DWORD dwHandle, DWORD dwLen, LPVOID lpData); }
extern "C"    BOOL WINAPI  GetFileVersionInfoExA(DWORD dwFlags, LPCSTR lpwstrFilename, DWORD dwHandle, DWORD dwLen, LPVOID lpData) {
	setup(P::GetFileVersionInfoExA, __FUNCTION__);
	return  P::GetFileVersionInfoExA(dwFlags, lpwstrFilename, dwHandle, dwLen, lpData);
#pragma comment(linker, "/EXPORT:" __FUNCTION__ "=" __FUNCDNAME__)
}


// Function Name     : GetFileVersionInfoExW
// Ordinal           : 4 (0x4)
namespace P { BOOL(WINAPI* GetFileVersionInfoExW)(DWORD dwFlags, LPCWSTR lpwstrFilename, DWORD dwHandle, DWORD dwLen, LPVOID lpData); }
extern "C"    BOOL WINAPI  GetFileVersionInfoExW(DWORD dwFlags, LPCWSTR lpwstrFilename, DWORD dwHandle, DWORD dwLen, LPVOID lpData) {
	setup(P::GetFileVersionInfoExW, __FUNCTION__);
	return  P::GetFileVersionInfoExW(dwFlags, lpwstrFilename, dwHandle, dwLen, lpData);
#pragma comment(linker, "/EXPORT:" __FUNCTION__ "=" __FUNCDNAME__)
}


// Function Name     : GetFileVersionInfoSizeA
// Ordinal           : 5 (0x5)
namespace P { DWORD(WINAPI* GetFileVersionInfoSizeA)(LPCSTR lptstrFilename, LPDWORD lpdwHandle); }
extern "C"    DWORD WINAPI  GetFileVersionInfoSizeA(LPCSTR lptstrFilename, LPDWORD lpdwHandle) {
	setup(P::GetFileVersionInfoSizeA, __FUNCTION__);
	return  P::GetFileVersionInfoSizeA(lptstrFilename, lpdwHandle);
#pragma comment(linker, "/EXPORT:" __FUNCTION__ "=" __FUNCDNAME__)
}


// Function Name     : GetFileVersionInfoSizeExA
// Ordinal           : 6 (0x6)
namespace P { DWORD(WINAPI* GetFileVersionInfoSizeExA)(DWORD dwFlags, LPCSTR lpwstrFilename, LPDWORD lpdwHandle); }
extern "C"    DWORD WINAPI  GetFileVersionInfoSizeExA(DWORD dwFlags, LPCSTR lpwstrFilename, LPDWORD lpdwHandle) {
	setup(P::GetFileVersionInfoSizeExA, __FUNCTION__);
	return   P::GetFileVersionInfoSizeExA(dwFlags, lpwstrFilename, lpdwHandle);
#pragma comment(linker, "/EXPORT:" __FUNCTION__ "=" __FUNCDNAME__)
}


// Function Name     : GetFileVersionInfoSizeExW
// Ordinal           : 7 (0x7)
#undef F
#define F GetFileVersionInfoSizeExW
namespace P { DWORD(WINAPI* GetFileVersionInfoSizeExW)(DWORD dwFlags, LPCWSTR lpwstrFilename, LPDWORD lpdwHandle); }
extern "C"    DWORD  WINAPI  GetFileVersionInfoSizeExW(DWORD dwFlags, LPCWSTR lpwstrFilename, LPDWORD lpdwHandle) {
	setup(P::GetFileVersionInfoSizeExW, __FUNCTION__);
	return    P::GetFileVersionInfoSizeExW(dwFlags, lpwstrFilename, lpdwHandle);
#pragma comment(linker, "/EXPORT:" __FUNCTION__ "=" __FUNCDNAME__)
}


// Function Name     : GetFileVersionInfoSizeW
// Ordinal           : 8 (0x8)
namespace P { DWORD(WINAPI *GetFileVersionInfoSizeW)(LPCWSTR lptstrFilename, LPDWORD lpdwHandle); }
extern "C"    DWORD WINAPI   GetFileVersionInfoSizeW(LPCWSTR lptstrFilename, LPDWORD lpdwHandle) {
	setup(P::GetFileVersionInfoSizeW, __FUNCTION__);
	return    P::GetFileVersionInfoSizeW(lptstrFilename, lpdwHandle);
#pragma comment(linker, "/EXPORT:" __FUNCTION__ "=" __FUNCDNAME__)
}


// Function Name     : GetFileVersionInfoW
// Ordinal           : 9 (0x9)
namespace P { BOOL(WINAPI* GetFileVersionInfoW)(LPCWSTR lptstrFilename, DWORD dwHandle, DWORD dwLen, LPVOID lpData); }
extern "C"    BOOL  WINAPI  GetFileVersionInfoW(LPCWSTR lptstrFilename, DWORD dwHandle, DWORD dwLen, LPVOID lpData) {
	setup(P::GetFileVersionInfoW, __FUNCTION__);
	return   P::GetFileVersionInfoW(lptstrFilename, dwHandle, dwLen, lpData);
#pragma comment(linker, "/EXPORT:" __FUNCTION__ "=" __FUNCDNAME__)
}


// Function Name     : VerFindFileA
// Ordinal           : 10 (0xa)
namespace P { DWORD(WINAPI* VerFindFileA)(DWORD uFlags, LPCSTR szFileName, LPCSTR szWinDir, LPCSTR szAppDir, LPSTR szCurDir, PUINT lpuCurDirLen, LPSTR szDestDir, PUINT lpuDestDirLen); }
extern "C"    DWORD  WINAPI  VerFindFileA(DWORD uFlags, LPCSTR szFileName, LPCSTR szWinDir, LPCSTR szAppDir, LPSTR szCurDir, PUINT lpuCurDirLen, LPSTR szDestDir, PUINT lpuDestDirLen) {
	setup(P::VerFindFileA, __FUNCTION__);
	return    P::VerFindFileA(uFlags, szFileName, szWinDir, szAppDir, szCurDir, lpuCurDirLen, szDestDir, lpuDestDirLen);
#pragma comment(linker, "/EXPORT:" __FUNCTION__ "=" __FUNCDNAME__)
}


// Function Name     : VerFindFileW
// Ordinal           : 11 (0xb)
namespace P { DWORD(WINAPI* VerFindFileW)(DWORD uFlags, LPCWSTR szFileName, LPCWSTR szWinDir, LPCWSTR szAppDir, LPWSTR szCurDir, PUINT lpuCurDirLen, LPWSTR szDestDir, PUINT lpuDestDirLen); }
extern "C"    DWORD  WINAPI  VerFindFileW(DWORD uFlags, LPCWSTR szFileName, LPCWSTR szWinDir, LPCWSTR szAppDir, LPWSTR szCurDir, PUINT lpuCurDirLen, LPWSTR szDestDir, PUINT lpuDestDirLen) {
	setup(P::VerFindFileW, __FUNCTION__);
	return    P::VerFindFileW(uFlags, szFileName, szWinDir, szAppDir, szCurDir, lpuCurDirLen, szDestDir, lpuDestDirLen);
#pragma comment(linker, "/EXPORT:" __FUNCTION__ "=" __FUNCDNAME__)
}


// Function Name     : VerInstallFileA
// Ordinal           : 12 (0xc)
namespace P { DWORD(WINAPI* VerInstallFileA)(DWORD uFlags, LPCSTR szSrcFileName, LPCSTR szDestFileName, LPCSTR szSrcDir, LPCSTR szDestDir, LPCSTR szCurDir, LPSTR szTmpFile, PUINT lpuTmpFileLen); }
extern "C"    DWORD  WINAPI  VerInstallFileA(DWORD uFlags, LPCSTR szSrcFileName, LPCSTR szDestFileName, LPCSTR szSrcDir, LPCSTR szDestDir, LPCSTR szCurDir, LPSTR szTmpFile, PUINT lpuTmpFileLen) {
	setup(P::VerInstallFileA, __FUNCTION__);
	return    P::VerInstallFileA(uFlags, szSrcFileName, szDestFileName, szSrcDir, szDestDir, szCurDir, szTmpFile, lpuTmpFileLen);
#pragma comment(linker, "/EXPORT:" __FUNCTION__ "=" __FUNCDNAME__)
}


// Function Name     : VerInstallFileW
// Ordinal           : 13 (0xd)
namespace P { DWORD(WINAPI* VerInstallFileW)(DWORD uFlags, LPCWSTR szSrcFileName, LPCWSTR szDestFileName, LPCWSTR szSrcDir, LPCWSTR szDestDir, LPCWSTR szCurDir, LPWSTR szTmpFile, PUINT lpuTmpFileLen); }
extern "C"    DWORD  WINAPI  VerInstallFileW(DWORD uFlags, LPCWSTR szSrcFileName, LPCWSTR szDestFileName, LPCWSTR szSrcDir, LPCWSTR szDestDir, LPCWSTR szCurDir, LPWSTR szTmpFile, PUINT lpuTmpFileLen) {
	setup(P::VerInstallFileW, __FUNCTION__);
	return    P::VerInstallFileW(uFlags, szSrcFileName, szDestFileName, szSrcDir, szDestDir, szCurDir, szTmpFile, lpuTmpFileLen);
#pragma comment(linker, "/EXPORT:" __FUNCTION__ "=" __FUNCDNAME__)
}


// Function Name     : VerLanguageNameA
// Ordinal           : 14 (0xe)
namespace P { DWORD(WINAPI* VerLanguageNameA)(DWORD wLang, LPSTR szLang, DWORD cchLang); }
extern "C"    DWORD  WINAPI  VerLanguageNameA(DWORD wLang, LPSTR szLang, DWORD cchLang) {
	setup(P::VerLanguageNameA, __FUNCTION__);
	return    P::VerLanguageNameA(wLang, szLang, cchLang);
#pragma comment(linker, "/EXPORT:" __FUNCTION__ "=" __FUNCDNAME__)
}


// Function Name     : VerLanguageNameW
// Ordinal           : 15 (0xf)
namespace P { DWORD(WINAPI* VerLanguageNameW)(DWORD wLang, LPWSTR szLang, DWORD cchLang); }
extern "C"    DWORD  WINAPI  VerLanguageNameW(DWORD wLang, LPWSTR szLang, DWORD cchLang) {
	setup(P::VerLanguageNameW, __FUNCTION__);
	return    P::VerLanguageNameW(wLang, szLang, cchLang);
#pragma comment(linker, "/EXPORT:" __FUNCTION__ "=" __FUNCDNAME__)
}


// Function Name     : VerQueryValueA
// Ordinal           : 16 (0x10)
namespace P { BOOL(WINAPI* VerQueryValueA)(LPCVOID pBlock, LPCSTR lpSubBlock, LPVOID * lplpBuffer, PUINT puLen); }
extern "C"    BOOL  WINAPI  VerQueryValueA(LPCVOID pBlock, LPCSTR lpSubBlock, LPVOID * lplpBuffer, PUINT puLen) {
	setup(P::VerQueryValueA, __FUNCTION__);
	return   P::VerQueryValueA(pBlock, lpSubBlock, lplpBuffer, puLen);
#pragma comment(linker, "/EXPORT:" __FUNCTION__ "=" __FUNCDNAME__)
}


// Function Name     : VerQueryValueW
// Ordinal           : 17 (0x11)
namespace P { BOOL(WINAPI* VerQueryValueW)(LPCVOID pBlock, LPCWSTR lpSubBlock, LPVOID * lplpBuffer, PUINT puLen); }
extern "C"    BOOL  WINAPI  VerQueryValueW(LPCVOID pBlock, LPCWSTR lpSubBlock, LPVOID * lplpBuffer, PUINT puLen) {
	setup(P::VerQueryValueW, __FUNCTION__);
	return   P::VerQueryValueW(pBlock, lpSubBlock, lplpBuffer, puLen);
#pragma comment(linker, "/EXPORT:" __FUNCTION__ "=" __FUNCDNAME__)
}