// Copyright (c) 2013 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

#include <string>
#include "cef_app_handler.h"

void OnContextInit();

void CefAppHandler::OnBeforeCommandLineProcessing(
	const CefString& process_type,
	CefRefPtr<CefCommandLine> command_line)
{
	OutputDebugString(L"OnBeforeCommandLineProcessing");
	command_line->AppendSwitch(L"no-proxy-server");
	command_line->AppendSwitchWithValue(L"--renderer-process-limit", L"10");
	
	command_line->AppendSwitch(L"--disable-web-security");
	command_line->AppendSwitchWithValue(L"ppapi-flash-version", L"21.0.0.197");
	command_line->AppendSwitchWithValue(L"ppapi-flash-path", L"PepperFlase\\pepflashplayer.dll");
}

void CefAppHandler::OnContextCreated(CefRefPtr<CefBrowser> browser,
	CefRefPtr<CefFrame> frame,
	CefRefPtr<CefV8Context> context)
{
	
}

void CefAppHandler::OnContextInitialized()
{
	OnContextInit();
}
