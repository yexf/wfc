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
	static wfcClientBrowser *CreateBrowser(wfcBrowser::BrowerType bType, HWND hWnd, LPCSTR lpszMainPage, void *pUserData);
	~wfcClientBrowser();
	wfcClientBrowser(HWND hWnd, LPCSTR lpszMainPage, CefRefPtr<CefClientHandler> pHandle);

	void Stop();
	
	bool IsSame(wfcBrowser *pBrowser);

	virtual void *GetData(){return m_pUserData;}

protected:
	CefRefPtr<CefClientHandler> m_pHandle;
	CefString m_strMainPage;
	HWND m_hWnd;
	void *m_pUserData;
	WFC_CLASSNAME(wfcClientBrowser)
};

#endif  // WFC_CLIENT_BROWSER_H_
