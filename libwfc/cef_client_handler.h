// Copyright (c) 2011 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

#ifndef CEF_CLIENT_HANDLER_H_
#define CEF_CLIENT_HANDLER_H_
#pragma once

#include <set>
#include <map>
#include <list>
#include <string>

#include "include/cef_client.h"
#include "include/base/cef_lock.h"

class CefClientHandler : public CefClient,
	public CefDisplayHandler,
	public CefLifeSpanHandler,
	public CefLoadHandler {

public:
	CefClientHandler();
	~CefClientHandler() OVERRIDE;
	
	// CefClient methods:
	virtual CefRefPtr<CefDisplayHandler> GetDisplayHandler() OVERRIDE {
	return this;
	}
	virtual CefRefPtr<CefLifeSpanHandler> GetLifeSpanHandler() OVERRIDE {
	return this;
	}
	virtual CefRefPtr<CefLoadHandler> GetLoadHandler() OVERRIDE { return this; }

	// CefDisplayHandler methods:
	virtual void OnTitleChange(CefRefPtr<CefBrowser> browser,
							 const CefString& title) OVERRIDE;

	// CefLifeSpanHandler methods:
	virtual void OnAfterCreated(CefRefPtr<CefBrowser> browser) OVERRIDE;
	virtual bool DoClose(CefRefPtr<CefBrowser> browser) OVERRIDE;
	virtual void OnBeforeClose(CefRefPtr<CefBrowser> browser) OVERRIDE;

	// CefLoadHandler methods:
	virtual void OnLoadError(CefRefPtr<CefBrowser> browser,
						   CefRefPtr<CefFrame> frame,
						   ErrorCode errorCode,
						   const CefString& errorText,
						   const CefString& failedUrl) OVERRIDE;
						   
public:
	// Request that all existing browser windows close.
	void CloseAllBrowsers(bool force_close);

	bool IsClosing() const { return is_closing_; }
	
	CefRefPtr<CefBrowser> GetBrowser();
	
private:

	// List of existing browser windows. Only accessed on the CEF UI thread.
	typedef std::list<CefRefPtr<CefBrowser>> BrowserList;
	BrowserList browser_list_;

	bool is_closing_;

	// Include the default reference counting implementation.
	IMPLEMENT_REFCOUNTING(CefClientHandler);	
	IMPLEMENT_LOCKING(CefClientHandler);	
};
#endif  // CEF_CLIENT_HANDLER_H_
