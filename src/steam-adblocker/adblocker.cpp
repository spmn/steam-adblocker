#include <Windows.h>

#include <string>
#include <fstream>
#include <vector>

#include "ad-block/ad_block_client.h"

const wchar_t *ADBLOCKPLUS_FILTER = L"ABPFilterParserData.dat";

AdBlockClient ad_block;
std::vector<char> adblock_filter;


bool get_executable_path(wchar_t *exe_path, size_t size)
{
	if (!GetModuleFileNameW(NULL, exe_path, size))
		return 0;

	wchar_t *sep = wcsrchr(exe_path, L'\\');
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

	return ad_block.deserialize(&adblock_filter[0]);
}

bool is_request_blocked(const char *input, const char *context_domain)
{
	return ad_block.matches(input, FilterOption::FONoFilterOption, context_domain);
}