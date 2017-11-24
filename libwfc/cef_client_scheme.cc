/*
 * cef_client_scheme.cc
 *
 *  Created on: 2017Äê11ÔÂ24ÈÕ
 *      Author: yesheng
 */

#include <direct.h>
#include <fstream>
#include <sstream>
#include <io.h>

#include <include/cef_parser.h>
#include <include/base/cef_platform_thread.h>
#include "cef_app_handler.h"
#include "cef_client_handler.h"
#include "wfc_cef_lock.h"
#include "cef_client_scheme.h"

#define WFC_LOCAL_API					"wfc_local_api"
#define WFC_LOCAL_API_SCHEME			"http"
#define WFC_LOCAL_API_HOST				WFC_LOCAL_API_SCHEME##"://"##WFC_LOCAL_API


CefLock g_ResLock;
std::string g_ConfigHost;

std::string DumpMapJson(const std::map<std::string, std::string>& kvmap)
{
	return "";
}

std::string DumpRequestContents(const std::map<std::string, std::string>& kvmap)
{
	return "";
}

class CefLocalApiScheme : public CefResourceHandler
{

};

class CefWebApiSchemeHandlerFactory : public CefSchemeHandlerFactory {
public:
	virtual CefRefPtr<CefResourceHandler> Create(
	      CefRefPtr<CefBrowser> browser,
	      CefRefPtr<CefFrame> frame,
	      const CefString& scheme_name,
	      CefRefPtr<CefRequest> request) {
		return CefRefPtr<CefResourceHandler>();
	}

	IMPLEMENT_REFCOUNTING(CefWebApiSchemeHandlerFactory);
};

void RegisterSchemeHandlers() {
}

void RegisterLuaSchemeHandlers(const char* scheme_name,
		const char* domain_name) {
}
