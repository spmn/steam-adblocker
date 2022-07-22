#include <Windows.h>

#include <string>
#include <fstream>
#include <vector>

#include "include/internal/cef_types.h"
#include "adblock_rust_ffi/src/wrapper.h"

bool get_executable_path(wchar_t* exe_path, size_t size);
bool read_file(const std::wstring& file, std::vector<char>& content);

const wchar_t *ADBLOCKPLUS_FILTER = L"ABPFilterParserData.dat";

adblock::Engine ad_block;
std::vector<char> adblock_filter;


bool initialize_adblocker()
{
    wchar_t path[MAX_PATH];
    get_executable_path(path, _countof(path));
    wcscat_s(path, ADBLOCKPLUS_FILTER);

    if (!read_file(path, adblock_filter))
    {
        OutputDebugStringW(L"[ADBLOCK] Can't read filters file.");
        return 0;
    }

    return ad_block.deserialize(adblock_filter);
}

const char *get_resource_string_from_cef_type(cef_resource_type_t cef_type)
{
    switch (cef_type)
    {
    case RT_MAIN_FRAME:
        return "main_frame";
    case RT_SUB_FRAME:
        return "sub_frame";
    case RT_STYLESHEET:
        return "stylesheet";
    case RT_SCRIPT:
        return "script";
    case RT_IMAGE:
    case RT_FAVICON:
        return "image";
    case RT_FONT_RESOURCE:
        return "font";
    case RT_OBJECT:
        return "object";
    case RT_MEDIA:
        return "media";
    case RT_XHR:
        return "xhr";
    case RT_PING:
        return "ping";
    case RT_CSP_REPORT:
        return "csp_report";
    }

    return "other";
}

bool is_request_blocked(const char *url, const char *url_host, const char *referrer_host, cef_resource_type_t request_type)
{
    bool did_match_rule{}, did_match_exception{}, did_match_important{};
    std::string str_resource_type = get_resource_string_from_cef_type(request_type);

    ad_block.matches(url, url_host, referrer_host, str_resource_type, did_match_rule, did_match_exception, did_match_important);
    return did_match_rule;
}