// Copyright (c) 2013 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

#ifndef WFC_CLIENT_BROWSER_H_
#define WFC_CLIENT_BROWSER_H_
#pragma once

#include <set>
#include "wfc_browser.h"
#include "cef_client_handler.h"

// When using the Views framework this object provides the delegate
// implementation for the CefWindow that hosts the Views-based browser.
class wfcWindowDelegate : public CefWindowDelegate {
public:
	explicit wfcWindowDelegate(CefRefPtr<CefBrowserView> browser_view)
	  : browser_view_(browser_view) {}

	void OnWindowCreated(CefRefPtr<CefWindow> window) OVERRIDE {
		// Add the browser view and show the window.
		window->AddChildView(browser_view_);
		window->Show();

		// Give keyboard focus to the browser view.
		browser_view_->RequestFocus();
	}

	void OnWindowDestroyed(CefRefPtr<CefWindow> window) OVERRIDE {
		browser_view_ = NULL;
	}

	bool CanClose(CefRefPtr<CefWindow> window) OVERRIDE {
		// Allow the window to close if the browser says it's OK.
		CefRefPtr<CefBrowser> browser = browser_view_->GetBrowser();
		if (browser)
		  return browser->GetHost()->TryCloseBrowser();
		return true;
	}

private:
	CefRefPtr<CefBrowserView> browser_view_;

	IMPLEMENT_REFCOUNTING(wfcWindowDelegate);
	DISALLOW_COPY_AND_ASSIGN(wfcWindowDelegate);
};

class wfcClientBrowser : public wfcBrowser
{
public:
	static wfcClientBrowser *CreateClientBrowser(HWND hWnd, LPCTSTR lpszMainPage);
	~wfcClientBrowser();
	
	void Stop();
	
	bool IsSame(wfcBrowser *pBrowser);
private:
	wfcClientBrowser(HWND hWnd, LPCTSTR lpszMainPage);
protected:
	CefRefPtr<CefClientHandler> m_pHandle;
	CefString m_strMainPage;
	HWND m_hWnd;
	WFC_CLASSNAME(wfcClientBrowser)
};

#endif  // WFC_CLIENT_BROWSER_H_
