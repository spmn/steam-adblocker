#include <Windows.h>
#include <stdlib.h>

#ifdef WIN32
#endif

#define WRAPPER_FUNC(lib, name) orig_##name = GetProcAddress(lib, ###name)
#define WRAPPER_GENFUNC(name) \
	FARPROC orig_##name; \
	__declspec(naked) void _##name() \
	{ \
		__asm jmp[orig_##name] \
	}

WRAPPER_GENFUNC(GetFileVersionInfoA);
WRAPPER_GENFUNC(GetFileVersionInfoByHandle);
WRAPPER_GENFUNC(GetFileVersionInfoExW);
WRAPPER_GENFUNC(GetFileVersionInfoSizeA);
WRAPPER_GENFUNC(GetFileVersionInfoSizeExW);
WRAPPER_GENFUNC(GetFileVersionInfoSizeW);
WRAPPER_GENFUNC(GetFileVersionInfoW);
WRAPPER_GENFUNC(VerFindFileA);
WRAPPER_GENFUNC(VerFindFileW);
WRAPPER_GENFUNC(VerInstallFileA);
WRAPPER_GENFUNC(VerInstallFileW);
WRAPPER_GENFUNC(VerLanguageNameA);
WRAPPER_GENFUNC(VerLanguageNameW);
WRAPPER_GENFUNC(VerQueryValueA);
WRAPPER_GENFUNC(VerQueryValueW);

bool initialize_wrapper()
{
	wchar_t version_path[MAX_PATH];
	GetSystemDirectoryW(version_path, _countof(version_path));
	wcscat_s(version_path, L"\\version.dll");

	HMODULE version_dll = LoadLibraryW(version_path);
	if (version_dll)
	{
		WRAPPER_FUNC(version_dll, GetFileVersionInfoA);
		WRAPPER_FUNC(version_dll, GetFileVersionInfoByHandle);
		WRAPPER_FUNC(version_dll, GetFileVersionInfoExW);
		WRAPPER_FUNC(version_dll, GetFileVersionInfoSizeA);
		WRAPPER_FUNC(version_dll, GetFileVersionInfoSizeExW);
		WRAPPER_FUNC(version_dll, GetFileVersionInfoSizeW);
		WRAPPER_FUNC(version_dll, GetFileVersionInfoW);
		WRAPPER_FUNC(version_dll, VerFindFileA);
		WRAPPER_FUNC(version_dll, VerFindFileW);
		WRAPPER_FUNC(version_dll, VerInstallFileA);
		WRAPPER_FUNC(version_dll, VerInstallFileW);
		WRAPPER_FUNC(version_dll, VerLanguageNameA);
		WRAPPER_FUNC(version_dll, VerLanguageNameW);
		WRAPPER_FUNC(version_dll, VerQueryValueA);
		WRAPPER_FUNC(version_dll, VerQueryValueW);
		
		return 1;
	}
	OutputDebugStringW(L"[ADBLOCK] Can't load \"version.dll\".");
	return 0;
}