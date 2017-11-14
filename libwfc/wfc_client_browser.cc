// Copyright (c) 2013 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

#include <include/cef_browser.h>
#include <tchar.h>
#include "wfc_client_browser.h"

wfcClientBrowser::wfcClientBrowser(HWND hWnd, LPCTSTR lpszMainPage)
	:m_hWnd(hWnd), m_strMainPage(L"about:blank")
{
	OutputDebugString(L"wfcClientBrowser");
	m_pHandle = new CefClientHandler(false);
	if (lpszMainPage && lpszMainPage[0] != _T('\0'))
	{
		m_strMainPage = lpszMainPage;
	}
}

wfcClientBrowser::~wfcClientBrowser()
{
	OutputDebugString(L"~wfcClientBrowser");
	Stop();
	m_pHandle = NULL;
}


wfcClientBrowser *wfcClientBrowser::CreateClientBrowser(HWND hWnd, LPCTSTR lpszMainPage)
{
	wfcClientBrowser *pBrowser = new wfcClientBrowser(hWnd, lpszMainPage);
	RECT rect;
	CefWindowInfo info;
	CefBrowserSettings settings;
	
	GetClientRect(hWnd, &rect);
	info.SetAsChild(hWnd, rect);
	CefBrowserHost::CreateBrowser(info, pBrowser->m_pHandle.get(), pBrowser->m_strMainPage, settings, NULL);
	return pBrowser;
}

void wfcClientBrowser::Stop()
{
	if (m_pHandle.get())
	{
		if (!m_pHandle->IsClosing())
		{
			CefRefPtr<CefBrowser> browser = m_pHandle->GetBrowser();
			if (browser.get())
			{
				browser->StopLoad();
			}
		}
	}
}
bool wfcClientBrowser::IsSame(wfcBrowser *pBrowser)
{
	return false;
}
