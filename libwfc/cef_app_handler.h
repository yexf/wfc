// Copyright (c) 2011 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

#ifndef CEF_APP_HANDLER_H_
#define CEF_APP_HANDLER_H_
#pragma once

#include "include/cef_app.h"

class CefAppHandler : public CefApp,
					  public CefBrowserProcessHandler,
					  public CefRenderProcessHandler
{
public:	
	// CefApp methods:
	virtual CefRefPtr<CefBrowserProcessHandler> GetBrowserProcessHandler()
	  OVERRIDE {
		return this;
	}
	virtual CefRefPtr<CefRenderProcessHandler> GetRenderProcessHandler()
	  OVERRIDE {
		return this;
	}
	virtual void OnBeforeCommandLineProcessing(
        CefRefPtr<ClientAppBrowser> app,
        CefRefPtr<CefCommandLine> command_line) OVERRIDE;

	// CefRenderProcessHandler methods:
	virtual void OnContextCreated() OVERRIDE;
	
	// CefBrowserProcessHandler methods:
	virtual void OnContextInitialized() OVERRIDE;

	private:
	// Include the default reference counting implementation.
	IMPLEMENT_REFCOUNTING(CefAppHandler);
}
					  

#endif  // CEF_APP_HANDLER_H_
