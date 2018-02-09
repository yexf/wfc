/*
 * cef_client_scheme.cc
 *
 *  Created on: 2017Äê11ÔÂ24ÈÕ
 *      Author: yesheng
 */

#include <direct.h>
#include <fstream>
#include <sstream>
#include <set>
#include <map>
#include <string>
#include <io.h>

#include <include/cef_parser.h>
#include <include/base/cef_platform_thread.h>
#include "cef_app_handler.h"
#include "cef_client_handler.h"
#include "wfc_cef_lock.h"
#include "cef_client_scheme.h"

#define WFC_LOCAL_API					"wfc_local_api"
#define WFC_LOCAL_API_SCHEME			"http"
#define WFC_LOCAL_API_HOST				WFC_LOCAL_API_SCHEME##"://"##WFC_LOCAL_API


CefLock g_ResLock;
std::set<CefLuaScheme *> g_setLuaScheme;
std::map<std::string, std::string> g_HostMap;
std::map<std::string, std::string> g_RefMap;

void AddHost(const char *api_name, const char *host)
{
	CefAutoLock lock_scope(g_ResLock);
	g_HostMap[api_name] = host;
}

void AddRef(const char *ref_url, const char *url)
{
	CefAutoLock lock_scope(g_ResLock);
	g_RefMap[ref_url] = url;
}

void InsertLuaScheme(CefLuaScheme *pLuaScheme)
{
	CefAutoLock lock_scope(g_ResLock);
	g_setLuaScheme.insert(pLuaScheme);
}

void DeleteLuaScheme(CefLuaScheme *pLuaScheme)
{
	CefAutoLock lock_scope(g_ResLock);
	g_setLuaScheme.erase(pLuaScheme);
}

std::string DumpMapJson(const std::map<std::string, std::string>& kvmap)
{
	std::stringstream ss;
	std::string str_splite = "";
	ss << "{";
	std::map<std::string, std::string>::const_iterator iter = kvmap.begin();
	while (iter != kvmap.end())
	{
		ss << str_splite;
		ss << "\"" << iter->first << "\"" << ":";
		ss << "\"" << iter->second << "\"";
		str_splite = ",";
		++iter;
	}
	ss << "}";
	return ss.str();
}

std::string DumpRequestContents(CefRefPtr<CefRequest> request)
{
	std::stringstream ss;
	ss << "URL: " << std::string(request->GetURL());
	ss << "\nMethod: " << std::string(request->GetMethod());
	
	CefRequest::HeaderMap headerMap;
	request->GetHeaderMap(headerMap);
	
	if (headerMap.size() > 0)
	{
		ss << "\nHeaders:";
		CefRequest::HeaderMap::const_iterator it = headerMap.begin();
		for (; it != headerMap.end(); ++it)
		{
			ss << "\n\t" << std::string(it->first) << ": " << std::string(it->second);
		}
	}
	
	CefRefPtr<CefPostData> postData = request->GetPostData();
	if (postData.get())
	{
		CefPostData::ElementVector elements;
		postData->GetElements(elements);
		if (elements.size() > 0)
		{
			ss << "\nPost Data:";
			CefRefPtr<CefPostDataElement> element;
			CefPostData::ElementVector::const_iterator it = elements.begin();
			for (; it != elements.end(); ++it)
			{
				element = (*it);
				if (element->GetType() == PDE_TYPE_BYTES) 
				{
					ss << "\n\tBytes: ";
					size_t size = element->GetBytesCount();
					if (size == 0)
					{
						ss << "(empty)";
					}
					else
					{
						char *bytes = new char[size];
						element->GetBytes(size, bytes);
						ss << std::string(bytes, size);
						delete[] bytes;
					}
				}
				else if (element->GetType() == PDE_TYPE_FILE)
				{
					ss << "\n\tFile: " << std::string(element->GetFile());
				}
			}
		}
	}
	return ss.str();
}

void RegisterSchemeHandlers() {
}

void RegisterLuaSchemeHandlers(const char* scheme_name,
		const char* domain_name) {
}
