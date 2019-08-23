#include <Windows.h>

bool initialize_wrapper();
bool initialize_hooks();
bool initialize_adblocker();

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
	if (ul_reason_for_call == DLL_PROCESS_ATTACH)
	{
		if (!initialize_wrapper())
		{
			OutputDebugStringW(L"[ADBLOCK] Can't initialize wrapper.");
			return FALSE;
		}
		if (wcsstr(GetCommandLineW(), L"-steampid") != nullptr)
		{
			if (!initialize_hooks())
			{
				OutputDebugStringW(L"[ADBLOCK] Can't initialize hooks.");
				return TRUE;
			}
			if (!initialize_adblocker())
			{
				OutputDebugStringW(L"[ADBLOCK] Can't initialize adblocker.");
				return TRUE;
			}
			OutputDebugStringW(L"[ADBLOCK] Fully loaded.");
		}
		else if (wcsstr(GetCommandLineW(), L"--type=crashpad-handler") != nullptr)
		{
			OutputDebugStringW(L"[ADBLOCK] Killing crashpad handler process.");
			ExitProcess(0);
			return FALSE;
		}
		else
		{
			OutputDebugStringW(L"[ADBLOCK] Wrapper only.");
		}
	}
	return TRUE;
}

