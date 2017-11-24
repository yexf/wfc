/*
 * wfc_cef_lock.h
 *
 *  Created on: 2017Äê11ÔÂ24ÈÕ
 *      Author: viruser
 */

#ifndef LIBWFC_WFC_CEF_LOCK_H_
#define LIBWFC_WFC_CEF_LOCK_H_

#pragma once
#include <include/base/internal/cef_lock_impl.h>

typedef base::cef_internal::LockImpl CefLock;
class CefAutoLock {
public:
	explicit CefAutoLock(CefLock& lock) : lock_(lock) {
		lock_.Lock();
	}

	~CefAutoLock() {
		lock_.Unlock();
	}

private:
	CefLock& lock_;
	DISALLOW_COPY_AND_ASSIGN(CefAutoLock);
};

#endif /* LIBWFC_WFC_CEF_LOCK_H_ */
