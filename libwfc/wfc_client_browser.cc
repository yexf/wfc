// Copyright (c) 2013 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

#include <include/cef_browser.h>
#include "include/views/cef_browser_view.h"
#include "include/views/cef_window.h"
#include "include/wrapper/cef_helpers.h"
#include <tchar.h>
#include "wfc_client_browser.h"

wfcClientBrowser::wfcClientBrowser(HWND hWnd, LPCSTR lpszMainPage, CefRefPtr<CefClientHandler> pHandle)
	:m_hWnd(hWnd), m_strMainPage(L"about:blank"), m_pHandle(pHandle), m_pUserData(NULL)
{
	OutputDebugString(L"wfcClientBrowser");
	if (lpszMainPage && lpszMainPage[0] != _T('\0'))
	{
		m_strMainPage = lpszMainPage;
	}
}

wfcClientBrowser::~wfcClientBrowser()
{
	OutputDebugString(L"~wfcClientBrowser");
	Stop();
	m_pHandle->CloseAllBrowsers(false);
	m_pHandle = NULL;
}

wfcClientBrowser *wfcClientBrowser::CreateBrowser(HWND hWnd, LPCSTR lpszMainPage, void *pUserData)
{
	CefRefPtr<CefClientHandler> handler(new CefClientHandler(false));
	wfcClientBrowser *pBrowser = new wfcClientBrowser(hWnd, lpszMainPage, handler);
	
	CefWindowInfo info;
	if (hWnd == NULL) {
		info.SetAsPopup(NULL, "libwfc");
	}
	else {
		RECT rect;
		GetClientRect(hWnd, &rect);
		info.SetAsChild(hWnd, rect);
	}

	CefBrowserSettings settings;
	const std::string &url = lpszMainPage;
	CefBrowserHost::CreateBrowser(info, handler.get(), url, settings, NULL);
	if (pBrowser != NULL) {
		pBrowser->m_pUserData = pUserData;
	}
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

void wfcClientBrowser::AdjustCefLayout(int left, int top, int right, int bottom)
{	
	if (m_pHandle.get())
	{
		if (m_pHandle->IsClosing())
		{
			return;
		}
		CefRefPtr<CefBrowser> br = m_pHandle->GetBrowser();
		if (br.get()) 
		{
			CefRefPtr<CefBrowserHost> host = br->GetHost();
			if (host.get()) 
			{
				HDWP hdwp = BeginDeferWindowPos(1);
				hdwp = DeferWindowPos(hdwp, host->GetWindowHandle(), NULL, 
					left,top,right-left,bottom-top,SWP_NOZORDER);
				EndDeferWindowPos(hdwp);
			}
		}
	}
}
void wfcClientBrowser::ReSize(int left, int top, int right, int bottom)
{
	if (m_hWnd)
	{
		AdjustCefLayout(left, top, right, bottom);
	}
}
bool wfcClientBrowser::notifyClose(HWND msgWnd)
{
	if (m_pHandle.get())
	{
		m_pHandle->CloseAllBrowsers(false);
	}
}