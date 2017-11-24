/*
 * cef_client_scheme.h
 *
 *  Created on: 2017Äê11ÔÂ24ÈÕ
 *      Author: viruser
 */

#ifndef LIBWFC_CEF_CLIENT_SCHEME_H_
#define LIBWFC_CEF_CLIENT_SCHEME_H_

#pragma once

void RegisterSchemeHandlers();

void RegisterLuaSchemeHandlers(const char *scheme_name, const char *domain_name);

#endif /* LIBWFC_CEF_CLIENT_SCHEME_H_ */
