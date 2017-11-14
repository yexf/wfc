// Copyright (c) 2013 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.
#include <tchar.h>
#include <io.h>
#include "libwfc.h"
#include "cef_app_handler.h"
#include "cef_client_handler.h"
#include "wfc_client_browser.h"
const TCHAR *psztSubProcPath = _T("wfcweb");
#ifndef F_OK
#define F_OK 0
#endif

BOOL CALLBACK EnumWindowsProc(HWND hwnd, LPARAM lParam)
{
	DWORD dwCurProcessId = *((DWORD*)lParam);
	DWORD dwProcessId = 0;

	GetWindowThreadProcessId(hwnd, &dwProcessId);
	if (dwProcessId == dwCurProcessId && GetParent(hwnd) == NULL)
	{
		*((HWND *)lParam) = hwnd;
		return FALSE;
	}
	return TRUE;
}


HWND GetMainWindow()
{
	DWORD dwCurrentProcessId = GetCurrentProcessId();
	if (!EnumWindows(EnumWindowsProc, (LPARAM)&dwCurrentProcessId))
	{
		return (HWND)dwCurrentProcessId;
	}
	return NULL;
}

static void ConfigSetting(unsigned int uFlag, CefSettings &settings)
{
	settings.single_process = !(uFlag & SINGLE_PROCESS);
	settings.multi_threaded_message_loop = !(uFlag & SINGLE_THREAD_MSGLOOP);
	settings.windowless_rendering_enabled = !(uFlag & WINDOW_LESS);

	TCHAR path[MAX_PATH]			= {0};
	TCHAR drive[MAX_PATH]			= {0};
	TCHAR dir[MAX_PATH]				= {0};
	TCHAR fname[MAX_PATH]			= {0};
	TCHAR ext[MAX_PATH]				= {0};

	::GetModuleFileName(NULL, path, MAX_PATH);
	_tsplitpath(path, drive, dir, fname, ext);

	wsprintf(path, _T("%s%s%s"), drive, dir, _T("cache"));
	if (_taccess(path, F_OK) == 0)
	{
		CefString(&settings.cache_path).FromWString(path);
	}

	wsprintf(path, _T("%s%s%s"), drive, dir, psztSubProcPath);
	if (_taccess(path, F_OK) == 0)
	{
		CefString(&settings.browser_subprocess_path).FromWString(path);
	}

	wsprintf(path, _T("%s%s%s"), drive, dir, _T("Resources"));
	if (_taccess(path, F_OK) == 0)
	{
		CefString(&settings.resources_dir_path).FromWString(path);
		wsprintf(path, _T("%s%s%s"), drive, dir, _T("Resources\\debug.log"));
		CefString(&settings.log_file).FromWString(path);
	}

	wsprintf(path, _T("%s%s%s"), drive, dir, _T("Resources\\locales"));
	if (_taccess(path, F_OK) == 0)
	{
		CefString(&settings.locales_dir_path).FromWString(path);
	}
	CefString(&settings.locale).FromWString(_T("zh-CN"));
}

static void ConfigPlugins(unsigned int uFlag)
{
	TCHAR path[MAX_PATH]			= {0};
	TCHAR drive[MAX_PATH]			= {0};
	TCHAR dir[MAX_PATH]				= {0};
	TCHAR fname[MAX_PATH]			= {0};
	TCHAR ext[MAX_PATH]				= {0};

	::GetModuleFileName(NULL, path, MAX_PATH);
	_tsplitpath(path, drive, dir, fname, ext);

	wsprintf(path, _T("%s%s%s"), drive, dir, _T("plugin\\NPSWF32.dll"));
	CefRegisterWebPluginCrash(path);
	CefRefreshWebPlugins();
}

static void ConfigMsgLoop(unsigned int uFlag)
{
	if (uFlag & SINGLE_THREAD_MSGLOOP)
	{
		// Run the CEF message loop. This will block until CefQuitMessageLoop() is
		// called.
		CefRunMessageLoop();
		// Shut down CEF.
		CefShutdown();
	}
}

int InitCef(unsigned int uFlag)
{
	// Enable High-DPI support on Windows 7 or newer.
	CefEnableHighDPISupport();

	void* sandbox_info = NULL;

#if defined(CEF_USE_SANDBOX)
	// Manage the life span of the sandbox information object. This is necessary
	// for sandbox support on Windows. See cef_sandbox_win.h for complete details.
	CefScopedSandboxInfo scoped_sandbox;
	sandbox_info = scoped_sandbox.sandbox_info();
#endif

	// Provide CEF with command-line arguments.
	CefMainArgs main_args(NULL);

	// SimpleApp implements application-level callbacks for the browser process.
	// It will create the first browser instance in OnContextInitialized() after
	// CEF has initialized.
	CefRefPtr<CefAppHandler> app(new CefAppHandler);

	// CEF applications have multiple sub-processes (render, plugin, GPU, etc)
	// that share the same executable. This function checks the command-line and,
	// if this is a sub-process, executes the appropriate logic.
	int exit_code = CefExecuteProcess(main_args, app, sandbox_info);
	if (exit_code >= 0) {
		// The sub-process has completed so return here.
		return exit_code;
	}

	// Specify CEF global settings here.
	CefSettings settings;

#if !defined(CEF_USE_SANDBOX)
	settings.no_sandbox = true;
#endif
	ConfigSetting(uFlag, settings);

	// Initialize CEF.
	CefInitialize(main_args, settings, app.get(), sandbox_info);

	ConfigPlugins(uFlag);

	ConfigMsgLoop(uFlag);

	return 0;
}

void CoInitializeCef()
{
	InitCef(NORMAL);
}
void CoUnInitializeCef()
{
	CefShutdown();
}
void RunMsgLoopCef()
{
	CefRunMessageLoop();
}
void QuitMsgLoopCef()
{
	CefQuitMessageLoop();
}

wfcBrowser *CreateTopBrowser(const char *pstrUrl, void *pData)
{
	return wfcClientBrowser::CreateBrowser(wfcBrowser::TopBrowser, NULL, pstrUrl, pData);

}
wfcBrowser *CreateChildBrowser(void *hWnd, const char *pstrUrl, void *pData)
{
	return wfcClientBrowser::CreateBrowser(wfcBrowser::ChildBrowser, (HWND)hWnd, pstrUrl, pData);
}

wfcBrowser *CreatePopupBrowser(const char *pstrUrl, void *pData)
{
	return wfcClientBrowser::CreateBrowser(wfcBrowser::PopupBrowser, NULL, pstrUrl, pData);
}
