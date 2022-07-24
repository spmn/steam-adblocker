#pragma once

#include <string>
#include <vector>

bool get_executable_path(wchar_t* exe_path, size_t size);
bool read_file(const std::wstring& file, std::vector<char>& content);
void cef_urlparts_free(cef_urlparts_t* cef_urlparts);

template <typename T>
void cef_string_free(T* cef_string)
{
    if (cef_string && cef_string->str && cef_string->dtor)
    {
        cef_string->dtor(cef_string->str);
    }
}
