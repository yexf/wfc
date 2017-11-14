// Copyright (c) 2013 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

#include <include/cef_browser.h>
#include "include/views/cef_browser_view.h"
#include "include/views/cef_window.h"
#include "include/wrapper/cef_helpers.h"
#include <tchar.h>
#include "wfc_client_browser.h"
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


static wfcClientBrowser *CreateTopBrowser(LPCSTR lpszMainPage)
{
	// SimpleHandler implements browser-level callbacks.
	CefRefPtr<CefClientHandler> handler(new CefClientHandler(true));
	wfcClientBrowser *pBrowser = new wfcClientBrowser(NULL, lpszMainPage, handler);
	// Specify CEF browser settings here.
	CefBrowserSettings browser_settings;

	const std::string &url = lpszMainPage;
	// Create the BrowserView.
	CefRefPtr<CefBrowserView> browser_view = CefBrowserView::CreateBrowserView(
		handler, url, browser_settings, NULL, NULL);

	// Create the Window. It will show itself after creation.
	CefWindow::CreateTopLevelWindow(new wfcWindowDelegate(browser_view));
	return pBrowser;
}
static wfcClientBrowser *CreateChildBrowser(HWND hWnd, LPCSTR lpszMainPage)
{
	CefRefPtr<CefClientHandler> handler(new CefClientHandler(false));
	wfcClientBrowser *pBrowser = new wfcClientBrowser(hWnd, lpszMainPage, handler);
	RECT rect;
	CefWindowInfo info;
	CefBrowserSettings settings;

	GetClientRect(hWnd, &rect);
	info.SetAsChild(hWnd, rect);
	const std::string &url = lpszMainPage;
	CefBrowserHost::CreateBrowser(info, handler.get(), url, settings, NULL);
	return pBrowser;
}
static wfcClientBrowser *CreatePopupBrowser(LPCSTR lpszMainPage)
{
	CefRefPtr<CefClientHandler> handler(new CefClientHandler(false));
	wfcClientBrowser *pBrowser = new wfcClientBrowser(NULL, lpszMainPage, handler);
	CefWindowInfo window_info;
#if defined(OS_WIN)
    // On Windows we need to specify certain flags that will be passed to
    // CreateWindowEx().
    window_info.SetAsPopup(NULL, "popWnd");
#endif
    CefBrowserSettings settings;
    CefRefPtr<CefRequestContext> request_context;
    const std::string &url = lpszMainPage;
    CefBrowserHost::CreateBrowser(window_info, handler, url, settings, request_context);
    return pBrowser;
}

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

wfcClientBrowser *wfcClientBrowser::CreateBrowser(wfcBrowser::BrowerType bType, HWND hWnd, LPCSTR lpszMainPage, void *pUserData)
{
	wfcClientBrowser *pBrowser = NULL;
	switch(bType) {
	case wfcBrowser::TopBrowser:
			pBrowser = CreateTopBrowser(lpszMainPage);
			break;
	case wfcBrowser::ChildBrowser:
			pBrowser = CreateChildBrowser(hWnd, lpszMainPage);
			break;
	case wfcBrowser::PopupBrowser:
			pBrowser = CreatePopupBrowser(lpszMainPage);
			break;
	}
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
