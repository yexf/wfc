// Copyright (c) 2013 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

#ifndef WFC_CLIENT_BROWSER_H_
#define WFC_CLIENT_BROWSER_H_
#pragma once

#include <set>
#include "cef_client_handler.h"

class wfcClientBrowser
{
public:
	static wfcClientBrowser *CreateClientBrowser(HWND hWnd, LPCTSTR lpszMainPage);
	~wfcClientBrowser();
	
	void Stop();
	
	bool hasClient(CefRefPtr<CefClient> pClient);
private:
	wfcClientBrowser(HWND hWnd, LPCTSTR lpszMainPage);
protected:
	CefRefPtr<CefClientHandler> m_pHandle;
	CefString m_strMainPage;
	HWND m_hWnd;
};

#endif  // WFC_CLIENT_BROWSER_H_
