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
	static wfcClientBrowser *CreateBrowser(HWND hWnd, LPCSTR lpszMainPage, const char *pStrParam = NULL, void *pUserData = NULL);

	
public:	
	~wfcClientBrowser();
	
	wfcClientBrowser(HWND hWnd, LPCSTR lpszMainPage, CefRefPtr<CefClientHandler> pHandle);

	void Stop();
	
	void AdjustCefLayout(int left, int top, int right, int bottom);
	
	void ReSize(int left, int top, int right, int bottom);
	
	bool notifyClose(HWND msgWnd);
	
	bool hasClient(CefRefPtr<CefClient> pClient);
	
	virtual void *GetData(){return m_pUserData;}
	
	virtual const char *GetStrParam(){return m_pStrParam;}

protected:
	CefRefPtr<CefClientHandler> m_pHandle;
	CefString m_strMainPage;
	HWND m_hWnd;
	void *m_pUserData;
	const char *m_pStrParam;
	WFC_CLASSNAME(wfcClientBrowser)
};

#endif  // WFC_CLIENT_BROWSER_H_
