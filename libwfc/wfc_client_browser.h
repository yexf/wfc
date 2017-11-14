// Copyright (c) 2013 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

#ifndef WFC_CLIENT_BROWSER_H_
#define WFC_CLIENT_BROWSER_H_
#pragma once

#include <set>
#include "wfc_browser.h"
#include "cef_client_handler.h"

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
