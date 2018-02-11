/*
 * cef_client_scheme.h
 *
 *  Created on: 2017Äê11ÔÂ24ÈÕ
 *      Author: viruser
 */

#ifndef LIBWFC_CEF_CLIENT_SCHEME_H_
#define LIBWFC_CEF_CLIENT_SCHEME_H_

#pragma once
#include <set>
#include "include/cef_browser.h"
#include "include/cef_callback.h"
#include "include/cef_frame.h"
#include "include/cef_resource_handler.h"
#include "include/cef_response.h"
#include "include/cef_request.h"
#include "include/cef_scheme.h"
#include "include/wrapper/cef_helpers.h"

#include "luawfc.h"

class CefLocalApiScheme : public CefResourceHandler
{
public:
	CefLocalApiScheme(const std::string &strParam) :str_param_(strParam)
	{}
	~CefLocalApiScheme() {}
	
	virtual bool ProcessRequest(CefRefPtr<CefRequest> request,
		CefRefPtr<CefCallback> callback);

	/*--cef()--*/
	virtual void GetResponseHeaders(CefRefPtr<CefResponse> response,
		int64& response_length,
		CefString& redirectUrl);

	/*--cef()--*/
	virtual bool ReadResponse(void* data_out,
		int bytes_to_read,
		int& bytes_read,
		CefRefPtr<CefCallback> callback);

	/*--cef()--*/
	virtual void Cancel() {
		CEF_REQUIRE_IO_THREAD();
	}
	
private:
	std::string data_;
	std::string mime_type_;
	int status_;
	size_t offset_;
	const std::string &str_param_;
	IMPLEMENT_REFCOUNTING(CefLocalApiScheme);
	IMPLEMENT_LOCKING(CefLocalApiScheme);
	
public:		
	class CefLocalApiSchemeHandlerFactory : public CefSchemeHandlerFactory {
	public:
		virtual CefRefPtr<CefResourceHandler> Create(
			  CefRefPtr<CefBrowser> browser,
			  CefRefPtr<CefFrame> frame,
			  const CefString& scheme_name,
			  CefRefPtr<CefRequest> request) {
			CEF_REQUIRE_IO_THREAD();
			if (browser.get())
			{
				CefRefPtr<CefBrowserHost> pHost = browser->GetHost();
				
			}
			return new CefLocalApiScheme("{}");
		}

		IMPLEMENT_REFCOUNTING(CefLocalApiSchemeHandlerFactory);
	};
};

class CefLuaScheme : public CefResourceHandler
{
public:
	CefLuaScheme();
	~CefLuaScheme();
	
	bool Confirm();
	
	std::string LuaError();
	
	virtual bool ProcessRequest(CefRefPtr<CefRequest> request,
		CefRefPtr<CefCallback> callback);

	/*--cef()--*/
	virtual void GetResponseHeaders(CefRefPtr<CefResponse> response,
		int64& response_length,
		CefString& redirectUrl);

	/*--cef()--*/
	virtual bool ReadResponse(void* data_out,
		int bytes_to_read,
		int& bytes_read,
		CefRefPtr<CefCallback> callback);

	/*--cef()--*/
	virtual void Cancel() {
		CEF_REQUIRE_IO_THREAD();
	}
	
private:
	std::string data_;
	std::string mime_type_;
	int func_ref_;
	size_t offset_;
	CefRefPtr<CefCallback> call_back_;
	IMPLEMENT_REFCOUNTING(CefLuaScheme);
	IMPLEMENT_LOCKING(CefLuaScheme);
	
public:		
	class CefLuaSchemeHandlerFactory : public CefSchemeHandlerFactory {
	public:
		virtual CefRefPtr<CefResourceHandler> Create(
			  CefRefPtr<CefBrowser> browser,
			  CefRefPtr<CefFrame> frame,
			  const CefString& scheme_name,
			  CefRefPtr<CefRequest> request) {
			CEF_REQUIRE_IO_THREAD();
			return new CefLuaScheme();
		}

		IMPLEMENT_REFCOUNTING(CefLuaSchemeHandlerFactory);
	};
};

void RegisterSchemeHandlers();

void RegisterLuaSchemeHandlers(const char *scheme_name, const char *domain_name);

#endif /* LIBWFC_CEF_CLIENT_SCHEME_H_ */
