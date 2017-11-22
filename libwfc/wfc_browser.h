// Copyright (c) 2013 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

#ifndef _WFC_BROWSER_H_
#define _WFC_BROWSER_H_
#pragma once

#define WFC_CLASSNAME(c)	protected:virtual const char *GetClassName() {return #c;}

class wfcBrowser
{
public:
	virtual ~wfcBrowser() {};
	virtual void Stop() = 0;	
	virtual bool IsSame(wfcBrowser *pBrowser) = 0;
	virtual void ReSize(int left, int top, int right, int bottom) = 0;
	virtual void *GetData() {return NULL;};
	WFC_CLASSNAME(wfcBrowser)
};


#endif //_WFC_BROWSER_H_
