/*
 * cef_client_scheme.h
 *
 *  Created on: 2017Äê11ÔÂ24ÈÕ
 *      Author: viruser
 */

#ifndef LIBWFC_CEF_CLIENT_SCHEME_H_
#define LIBWFC_CEF_CLIENT_SCHEME_H_

#pragma once
#include <set>
#include "include/cef_browser.h"
#include "include/cef_callback.h"
#include "include/cef_frame.h"
#include "include/cef_resource_handler.h"
#include "include/cef_response.h"
#include "include/cef_request.h"
#include "include/cef_scheme.h"
#include "include/wrapper/cef_helpers.h"

#include "luawfc.h"
void RegisterSchemeHandlers();

void RegisterLuaSchemeHandlers(const char *scheme_name, const char *domain_name);

#endif /* LIBWFC_CEF_CLIENT_SCHEME_H_ */
