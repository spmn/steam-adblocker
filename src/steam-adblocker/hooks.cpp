#include <Windows.h>

#include "subhook/subhook.h"
#include "include/capi/cef_client_capi.h"
#include "include/capi/cef_parser_capi.h"
#include "include/capi/cef_app_capi.h"

#include "utils.h"

cef_return_value_t(CEF_CALLBACK* on_before_resource_load__original)(struct _cef_resource_request_handler_t* self, struct _cef_browser_t* browser, struct _cef_frame_t* frame, struct _cef_request_t* request, struct _cef_request_callback_t* callback);
cef_return_value_t CEF_CALLBACK on_before_resource_load__hook(struct _cef_resource_request_handler_t* self, struct _cef_browser_t* browser, struct _cef_frame_t* frame, struct _cef_request_t* request, struct _cef_request_callback_t* callback)
{
    static constexpr const char kHeader[] = "User-Agent";
    static constexpr const char kValue[] = "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/105.0.0.0 Safari/537.36";
    static constexpr const char kScript[] = R"((function() {
    try {
        const userAgent = 'Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/105.0.0.0 Safari/537.36';
        const appVersion = '5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/105.0.0.0 Safari/537.36';
        const platform = 'Win32';
        const vendor = 'Google Inc.';

        navigator.__defineGetter__('userAgent', () => userAgent);
        navigator.__defineGetter__('appVersion', () => appVersion);
        navigator.__defineGetter__('platform', () => platform);
        navigator.__defineGetter__('vendor', () => vendor);

        console.log('new ua: ' + window.navigator.userAgent);
    }
    catch (e) {
        console.log('exception: ' + e.toString());
    }
    console.log('executed');
})();)";

    static cef_string_userfree_t ua_header = cef_string_userfree_alloc();
    static cef_string_userfree_t ua_value = cef_string_userfree_alloc();
    static cef_string_userfree_t script_set_ua = cef_string_userfree_alloc();
    static auto init = []() {
        cef_string_from_ascii(kHeader, strlen(kHeader), ua_header);
        cef_string_from_ascii(kValue, strlen(kValue), ua_value);
        cef_string_from_ascii(kScript, strlen(kScript), script_set_ua);
        return 1;
    }();

    cef_return_value_t ret;

    if (on_before_resource_load__original != nullptr)
    {
        ret = on_before_resource_load__original(self, browser, frame, request, callback);
    }
    else
    {
        ret = RV_CONTINUE;
    }

    request->set_header_by_name(request, ua_header, ua_value, true);
    frame->execute_java_script(frame, script_set_ua, nullptr, 0);

    // cef_string_userfree_free(ua_header);
    // cef_string_userfree_free(ua_value);
    return ret;
}

struct _cef_resource_request_handler_t*(CEF_CALLBACK* get_resource_request_handler__original)(struct _cef_request_handler_t* self, struct _cef_browser_t* browser, struct _cef_frame_t* frame, struct _cef_request_t* request, int is_navigation, int is_download, const cef_string_t* request_initiator, int* disable_default_handling);
struct _cef_resource_request_handler_t* CEF_CALLBACK get_resource_request_handler__hook(struct _cef_request_handler_t* self, struct _cef_browser_t* browser, struct _cef_frame_t* frame, struct _cef_request_t* request, int is_navigation, int is_download, const cef_string_t* request_initiator, int* disable_default_handling)
{
    _cef_resource_request_handler_t* ret;

    if (get_resource_request_handler__original)
    {
        ret = get_resource_request_handler__original(self, browser, frame, request, is_navigation, is_download, request_initiator, disable_default_handling);

        if (on_before_resource_load__original != nullptr && on_before_resource_load__original != ret->on_before_resource_load)
            OutputDebugStringW(L"[ADBLOCK] \"on_before_resource_load__original\" not equal to \"ret->on_before_resource_load\".");
        
        on_before_resource_load__original = ret->on_before_resource_load;
        ret->on_before_resource_load = on_before_resource_load__hook;
    }
    else
    {
        OutputDebugStringW(L"[ADBLOCK] \"get_resource_request_handler__original\" is null.");
        ret = nullptr;
    }

    return ret;
}

struct _cef_request_handler_t*(CEF_CALLBACK* get_request_handler__original)(struct _cef_client_t* self);
struct _cef_request_handler_t* CEF_CALLBACK get_request_handler__hook(struct _cef_client_t* self)
{
    _cef_request_handler_t* ret;

    if (get_request_handler__original) 
    {
        ret = get_request_handler__original(self);

        if (get_resource_request_handler__original != nullptr && get_resource_request_handler__original != ret->get_resource_request_handler)
            OutputDebugStringW(L"[ADBLOCK] \"get_resource_request_handler__original\" not equal to \"ret->get_resource_request_handler\".");
        
        get_resource_request_handler__original = ret->get_resource_request_handler;
        ret->get_resource_request_handler = get_resource_request_handler__hook;
    }
    else 
    {
        OutputDebugStringW(L"[ADBLOCK] \"get_request_handler__original\" is null.");
        ret = nullptr;
    }

    return ret;
}

subhook_t cef_string_utf8_set_hook;
int (*cef_string_utf8_set__original)(const char* src, size_t src_len, cef_string_utf8_t* output, int copy);
int cef_string_utf8_set_func(const char* src, size_t src_len, cef_string_utf8_t* output, int copy)
{
    // OutputDebugStringW(L"[ADBLOCK] cef_string_utf8_set called");
    // OutputDebugStringA(src);

    if (src && strstr(src, "Valve Steam Client") && strstr(src, "AppleWebKit/537.36"))
    {
        src = _strdup("Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/105.0.0.0 Safari/537.36");
        src_len = strlen(src);
    }

    subhook_remove(cef_string_utf8_set_hook);
    auto ret = cef_string_utf8_set__original(src, src_len, output, copy);
    subhook_install(cef_string_utf8_set_hook);

    return ret;
}

subhook_t cef_initialize_hook;
int (*cef_initialize__original)(const struct _cef_main_args_t* args, const struct _cef_settings_t* settings, cef_app_t* application, void* windows_sandbox_info);
int cef_initialize_func(const struct _cef_main_args_t* args, const struct _cef_settings_t* settings, cef_app_t* application, void* windows_sandbox_info)
{
    OutputDebugStringW(L"[ADBLOCK] cef_initialize called");

    static constexpr const char kUserAgent[] = "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/105.0.0.0 Safari/537.36";

    // Mozilla/5.0 (Windows; U; Windows NT 10.0; en-US; Valve Steam Client/default/1661825435; ) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/84.0.4147.105 Safari/537.36

    const_cast<_cef_settings_t*>(settings)->product_version.str = nullptr;
    const_cast<_cef_settings_t*>(settings)->product_version.dtor = nullptr;
    const_cast<_cef_settings_t*>(settings)->product_version.length = 0;
    // const_cast<_cef_settings_t*>(settings)->remote_debugging_port = 8089;
    cef_string_set(kUserAgent, strlen(kUserAgent), &const_cast<_cef_settings_t*>(settings)->user_agent, true);

    // cef_string_utf8_set

    OutputDebugStringA(settings->user_agent.str);
    OutputDebugStringA(settings->product_version.str);

    subhook_remove(cef_initialize_hook);
    auto ret = cef_initialize__original(args, settings, application, windows_sandbox_info);
    subhook_install(cef_initialize_hook);

    return ret;
}

subhook_t cef_browser_host_create_browser_hook;
int (*cef_browser_host_create_browser__original)(const cef_window_info_t* windowInfo, struct _cef_client_t* client, const cef_string_t* url, const struct _cef_browser_settings_t* settings, struct _cef_dictionary_value_t* extra_info, struct _cef_request_context_t* request_context);
int cef_browser_host_create_browser_func(const cef_window_info_t* windowInfo, struct _cef_client_t* client, const cef_string_t* url, const struct _cef_browser_settings_t* settings, struct _cef_dictionary_value_t* extra_info, struct _cef_request_context_t* request_context)
{
    if (get_request_handler__original != nullptr && get_request_handler__original != client->get_request_handler)
        OutputDebugStringW(L"[ADBLOCK] \"get_request_handler__original\" not equal to \"client->get_request_handler\".");

    get_request_handler__original = client->get_request_handler;
    client->get_request_handler = get_request_handler__hook;

    subhook_remove(cef_browser_host_create_browser_hook);
    auto ret = cef_browser_host_create_browser__original(windowInfo, client, url, settings, extra_info, request_context);
    subhook_install(cef_browser_host_create_browser_hook);

    return ret;
}

bool initialize_hooks()
{
    HMODULE mod = GetModuleHandleW(L"libcef.dll");
    if (!mod) 
    {
        OutputDebugStringW(L"[ADBLOCK] Can't find \"libcef.dll\" module.");
        return 0;
    }

#if 0
    FARPROC func = GetProcAddress(mod, "cef_browser_host_create_browser");
    if (!func) 
    {
        OutputDebugStringW(L"[ADBLOCK] Can't find \"cef_browser_host_create_browser\" function.");
        return 0;
    }

#ifdef _WIN64
    cef_browser_host_create_browser_hook = subhook_new(func, cef_browser_host_create_browser_func, SUBHOOK_64BIT_OFFSET);
#else
    cef_browser_host_create_browser_hook = subhook_new(func, cef_browser_host_create_browser_func, (subhook_flags)0);
#endif
    cef_browser_host_create_browser__original = (decltype(cef_browser_host_create_browser__original))func;
    if (cef_browser_host_create_browser_hook == nullptr)
    {
        OutputDebugStringW(L"[ADBLOCK] Can't create \"cef_browser_host_create_browser\" hook.");
        return 0;
    }
    if (subhook_install(cef_browser_host_create_browser_hook) != 0)
    {
        OutputDebugStringW(L"[ADBLOCK] Can't install \"cef_browser_host_create_browser\" hook.");
        return 0;
    }


    FARPROC func2 = GetProcAddress(mod, "cef_initialize");
    if (!func2)
    {
        OutputDebugStringW(L"[ADBLOCK] Can't find \"cef_initialize\" function.");
        return 0;
    }

#ifdef _WIN64
    cef_initialize_hook = subhook_new(func2, cef_initialize_func, SUBHOOK_64BIT_OFFSET);
#else
    cef_initialize_hook = subhook_new(func2, cef_initialize_func, (subhook_flags)0);
#endif
    cef_initialize__original = (decltype(cef_initialize__original))func2;
    if (cef_initialize_hook == nullptr)
    {
        OutputDebugStringW(L"[ADBLOCK] Can't create \"cef_initialize\" hook.");
        return 0;
    }
    if (subhook_install(cef_initialize_hook) != 0)
    {
        OutputDebugStringW(L"[ADBLOCK] Can't install \"cef_initialize\" hook.");
        return 0;
    }
#endif

    FARPROC func3 = GetProcAddress(mod, "cef_string_utf8_set");
    if (!func3)
    {
        OutputDebugStringW(L"[ADBLOCK] Can't find \"cef_string_utf8_set\" function.");
        return 0;
    }

#ifdef _WIN64
    cef_string_utf8_set_hook = subhook_new(func3, cef_string_utf8_set_func, SUBHOOK_64BIT_OFFSET);
#else
    cef_string_utf8_set_hook = subhook_new(func3, cef_string_utf8_set_func, (subhook_flags)0);
#endif
    cef_string_utf8_set__original = (decltype(cef_string_utf8_set__original))func3;
    if (cef_string_utf8_set_hook == nullptr)
    {
        OutputDebugStringW(L"[ADBLOCK] Can't create \"cef_string_utf8_set\" hook.");
        return 0;
    }
    if (subhook_install(cef_string_utf8_set_hook) != 0)
    {
        OutputDebugStringW(L"[ADBLOCK] Can't install \"cef_string_utf8_set\" hook.");
        return 0;
    }

    return 1;
}