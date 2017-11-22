// Copyright (c) 2013 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.
#ifndef _LIB_WFC_H_
#define _LIB_WFC_H_
#pragma once
#include "wfc_browser.h"
#ifdef _MSC_VER
#define WFC_EXPORT extern "C" __declspec(dllexport)
#else
#define WFC_EXPORT extern "C" __attribute__((visibility("default")))
#endif

#ifdef WFC_DLL
#define WFC_API	WFC_EXPORT
#else
#define WFC_API extern "C" 
#endif

#define RIGHT_SHIFT(x)				(0x01 << x)
#define SINGLE_PROCESS				RIGHT_SHIFT(0)
#define SINGLE_THREAD_MSGLOOP		RIGHT_SHIFT(1)
#define WINDOW_LESS					RIGHT_SHIFT(2)

#define NORMAL						(0)

WFC_API void CoInitializeCef();
WFC_API void CoUnInitializeCef();
WFC_API int InitCef(unsigned int uFlag);
WFC_API void RunMsgLoopCef();
WFC_API void QuitMsgLoopCef();
WFC_API wfcBrowser *CreateBrowser(void *hWnd, const char *pstrUrl, void *pData);
#endif //_LIB_WFC_H_
