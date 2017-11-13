// Copyright (c) 2013 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

#include "cef_client_handler.h"

#include <sstream>
#include <string>

#include "include/base/cef_bind.h"
#include "include/cef_app.h"
#include "include/views/cef_browser_view.h"
#include "include/views/cef_window.h"
#include "include/wrapper/cef_closure_task.h"
#include "include/wrapper/cef_helpers.h"


CefClientHandler::CefClientHandler()
    : is_closing_(false) {
}

CefClientHandler::~CefClientHandler() {

}

void CefClientHandler::OnTitleChange(CefRefPtr<CefBrowser> browser,
                                  const CefString& title) {
	CEF_REQUIRE_UI_THREAD();
	if (IsClosing()) {
		return;
	}
	CefWindowHandle hwnd = browser->GetHost()->GetWindowHandle();
	SetWindowText(hwnd, std::wstring(title).c_str());
  
}

void CefClientHandler::OnAfterCreated(CefRefPtr<CefBrowser> browser) {
	OutputDebugString(L"OnAfterCreated");
	CEF_REQUIRE_UI_THREAD();

	// Add to the list of existing browsers.
	browser_list_.push_back(browser);
}

bool CefClientHandler::DoClose(CefRefPtr<CefBrowser> browser) {
	OutputDebugString(L"DoClose");
	CEF_REQUIRE_UI_THREAD();

	// Closing the main window requires special handling. See the DoClose()
	// documentation in the CEF header for a detailed destription of this
	// process.
	if (browser_list_.size() == 1) {
	// Set a flag to indicate that the window close should be allowed.
		is_closing_ = true;
	}

	// Allow the close. For windowed browsers this will result in the OS close
	// event being sent.
	return false;
}

void CefClientHandler::OnBeforeClose(CefRefPtr<CefBrowser> browser) {
	OutputDebugString(L"OnBeforeClose");
	CEF_REQUIRE_UI_THREAD();

	// Remove from the list of existing browsers.
	BrowserList::iterator bit = browser_list_.begin();
	for (; bit != browser_list_.end(); ++bit) {
		if ((*bit)->IsSame(browser)) {
		  browser_list_.erase(bit);
		  break;
		}
	}

	if (browser_list_.empty()) {
		// All browser windows have closed. Quit the application message loop.
		CefQuitMessageLoop();
	}
}

void CefClientHandler::OnLoadError(CefRefPtr<CefBrowser> browser,
                                CefRefPtr<CefFrame> frame,
                                ErrorCode errorCode,
                                const CefString& errorText,
                                const CefString& failedUrl) {
	CEF_REQUIRE_UI_THREAD();

	// Don't display an error for downloaded files.
	if (errorCode == ERR_ABORTED)
		return;

	// Display a load error message.
	std::stringstream ss;
	ss << "<html><body bgcolor=\"white\">"
		"<h2>Failed to load URL "
	 << std::string(failedUrl) << " with error " << std::string(errorText)
	 << " (" << errorCode << ").</h2></body></html>";
	frame->LoadString(ss.str(), failedUrl);
}
CefRefPtr<CefBrowser> CefClientHandler::GetBrowser()
{
	AutoLock lock_scope(this);
	if (browser_list_.empty())
	{
		return CefRefPtr<CefBrowser>();
	}
	return browser_list_.back();
}
void CefClientHandler::CloseAllBrowsers(bool force_close) {
	if (!CefCurrentlyOn(TID_UI)) {
		// Execute on the UI thread.
		CefPostTask(TID_UI, base::Bind(&CefClientHandler::CloseAllBrowsers, this,
									   force_close));
		return;
	}

	if (browser_list_.empty())
		return;

	BrowserList::const_iterator it = browser_list_.begin();
	for (; it != browser_list_.end(); ++it)
		(*it)->GetHost()->CloseBrowser(force_close);
}
