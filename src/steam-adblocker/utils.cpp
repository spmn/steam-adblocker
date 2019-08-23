#include <Windows.h>

#include <string>
#include <fstream>
#include <vector>

#include "include/capi/cef_base_capi.h"

bool get_executable_path(wchar_t* exe_path, size_t size)
{
	if (!GetModuleFileNameW(NULL, exe_path, (DWORD)size))
		return 0;

	wchar_t* sep = wcsrchr(exe_path, L'\\');
	if (!sep)
		return 0;

	sep[1] = L'\0';
	return 1;
}

bool read_file(const std::wstring& file, std::vector<char>& content)
{
	std::ifstream f(file, std::ios::binary);
	if (!f)
		return 0;

	std::vector<char> temp((std::istreambuf_iterator<char>(f)), std::istreambuf_iterator<char>());
	content = temp;
	return 1;
}

void cef_string_free(cef_string_t* cef_string)
{
	if (cef_string && cef_string->str && cef_string->dtor)
	{
		cef_string->dtor(cef_string->str);
	}
}

void cef_urlparts_free(cef_urlparts_t* cef_urlparts)
{
	if (cef_urlparts)
	{
		cef_string_free(&cef_urlparts->host);
		cef_string_free(&cef_urlparts->origin);
		cef_string_free(&cef_urlparts->password);
		cef_string_free(&cef_urlparts->path);
		cef_string_free(&cef_urlparts->port);
		cef_string_free(&cef_urlparts->query);
		cef_string_free(&cef_urlparts->scheme);
		cef_string_free(&cef_urlparts->spec);
		cef_string_free(&cef_urlparts->username);
	}
}
