#include <Windows.h>

#include "subhook/subhook.h"
#include "include/capi/cef_client_capi.h"
#include "include/capi/cef_parser_capi.h"

bool is_request_blocked(const char *input, const char *context_domain);
void cef_string_free(cef_string_t* cef_string);
void cef_urlparts_free(cef_urlparts_t* cef_urlparts);

cef_return_value_t(CEF_CALLBACK* on_before_resource_load__original)(struct _cef_resource_request_handler_t* self, struct _cef_browser_t* browser, struct _cef_frame_t* frame, struct _cef_request_t* request, struct _cef_request_callback_t* callback);
cef_return_value_t CEF_CALLBACK on_before_resource_load__hook(struct _cef_resource_request_handler_t* self, struct _cef_browser_t* browser, struct _cef_frame_t* frame, struct _cef_request_t* request, struct _cef_request_callback_t* callback)
{
	bool block_request = false;
	cef_string_userfree_t url = request->get_url(request);
	if (url) 
	{
		cef_string_userfree_t real_referrer_url = request->get_referrer_url(request);
		
		cef_string_t *referrer_url;
		cef_urlparts_t referrer_parts = {};

		if (real_referrer_url)
			referrer_url = real_referrer_url;
		else 
			referrer_url = url;

		if (cef_parse_url(referrer_url, &referrer_parts))
		{
			cef_string_utf8_t url_utf8 = {};
			cef_string_utf8_t referrer_host_utf8 = {};

			cef_string_to_utf8(url->str, url->length, &url_utf8);
			cef_string_to_utf8(referrer_parts.host.str, referrer_parts.host.length, &referrer_host_utf8);

			block_request = is_request_blocked(url_utf8.str, referrer_host_utf8.str);

			cef_string_free(&url_utf8);
			cef_string_free(&referrer_host_utf8);
			cef_urlparts_free(&referrer_parts);
		}

		if(real_referrer_url)
			cef_string_userfree_free(real_referrer_url);

		cef_string_userfree_free(url);

		if (block_request) 
			return RV_CANCEL;
	}

	if (on_before_resource_load__original != nullptr)
		return on_before_resource_load__original(self, browser, frame, request, callback);

	return RV_CONTINUE;
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

	return 1;
}