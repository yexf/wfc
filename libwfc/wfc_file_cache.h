// Copyright (c) 2013 yexf_fyf@163.com Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.
#ifndef _WFC_FILE_CACHE_H_
#define _WFC_FILE_CACHE_H_
#pragma once

#include <windows.h>
#include "aux-cvt.h"
#include "wfc_cef_lock.h"

///////////////////////////////////////////////////////////////////////////////////////
DECLARE_HANDLE(HZIP);	// An HZIP identifies a zip file that has been opened
typedef DWORD ZRESULT;
typedef struct
{ 
    int index;                 // index of this file within the zip
    char name[MAX_PATH];       // filename within the zip
    DWORD attr;                // attributes, as in GetFileAttributes.
    FILETIME atime,ctime,mtime;// access, create, modify filetimes
    long comp_size;            // sizes of item, compressed and uncompressed. These
    long unc_size;             // may be -1 if not yet known (e.g. being streamed in)
} ZIPENTRY;
typedef struct
{ 
    int index;                 // index of this file within the zip
    TCHAR name[MAX_PATH];      // filename within the zip
    DWORD attr;                // attributes, as in GetFileAttributes.
    FILETIME atime,ctime,mtime;// access, create, modify filetimes
    long comp_size;            // sizes of item, compressed and uncompressed. These
    long unc_size;             // may be -1 if not yet known (e.g. being streamed in)
} ZIPENTRYW;
#define OpenZip OpenZipU
#define CloseZip(hz) CloseZipU(hz)
extern HZIP OpenZipU(void *z,unsigned int len,DWORD flags);
extern ZRESULT CloseZipU(HZIP hz);
#ifdef _UNICODE
#define ZIPENTRY ZIPENTRYW
#define GetZipItem GetZipItemW
#define FindZipItem FindZipItemW
#else
#define GetZipItem GetZipItemA
#define FindZipItem FindZipItemA
#endif
extern ZRESULT GetZipItemA(HZIP hz, int index, ZIPENTRY *ze);
extern ZRESULT GetZipItemW(HZIP hz, int index, ZIPENTRYW *ze);
extern ZRESULT FindZipItemA(HZIP hz, const TCHAR *name, bool ic, int *index, ZIPENTRY *ze);
extern ZRESULT FindZipItemW(HZIP hz, const TCHAR *name, bool ic, int *index, ZIPENTRYW *ze);
extern ZRESULT UnzipItem(HZIP hz, int index, void *dst, unsigned int len, DWORD flags);
///////////////////////////////////////////////////////////////////////////////////////

inline bool ReadZipItemRaw(void *file_zip,
	int zip_size,
	const TCHAR *pkgName,
	std::string &data_,
	std::string &err_)
{
	ZIPENTRY ze;
	int i;
	HZIP hz = OpenZip(file_zip, zip_size, zip_size == 0 ? 2 : 3);

	if (hz == NULL)
	{
		err_ = "Could not find file";
		return false;
	}
	
	if (FindZipItem(hz, pkgName, true, &i, &ze) != 0)
	{
		err_ = "Could not find ziped file";
		CloseZip(hz);
		return false;
	}
	
	DWORD dwSize = ze.unc_size;
	if (dwSize == 0) 
	{
		err_ = "File is empty";
		CloseZip(hz);
		return false;
	}
	
	if (dwSize > 4096*1024) 
	{
		err_ = "File is too large";
		CloseZip(hz);
		return false;
	}
	
	data_.resize(dwSize);
	int res = UnzipItem(hz, i, &data_[0], dwSize, 3);
	if (res != 0x0000000 && res != 0x00000600)
	{
		err_ = "Could not unzip file";
		CloseZip(hz);
		return false;
	}
	CloseZip(hz);
	return true;
}

inline bool ReadZipItemFromMem(const std::string &file_zip,
	const std::string &pkgName,
	std::string &data_,
	std::string &err_)
{
	void *pFile = (void*)file_zip.c_str();
	int zip_size = file_zip.size();
#ifdef _UNICODE	
	return ReadZipItemRaw(pFile, zip_size, aux::a2w(pkgName.c_str()), data_, err_);
#else
	return ReadZipItemRaw(pFile, zip_size, pkgName.c_str(), data_, err_);
#endif
}

inline bool ReadZipItemFromFile(const std::string &file_zip,
	const std::string &pkgName,
	std::string &data_,
	std::string &err_)
{
#ifdef _UNICODE		
	aux::a2w file_name(file_zip.c_str());
	const wchar_t *pfile_name = file_name;
	return ReadZipItemRaw((void *)pfile_name, 0, aux::a2w(pkgName.c_str()), data_, err_);
#else
	return ReadZipItemRaw((void *)file_zip.c_str(), 0, pkgName.c_str(), data_, err_);
#endif
}

const char *CheckZipExt(const std::string &strZip)
{
	std::string strFile;
#define CHECK_FILE_EXT(ext) strFile = strZip + ext; 	\
	if (access(strFile.c_str(), 0) == 0) 				\
	{													\
		return ext;										\
	}
	
	CHECK_FILE_EXT(".ui")
	CHECK_FILE_EXT(".dat")
	CHECK_FILE_EXT(".pkg")
	CHECK_FILE_EXT(".pak")
	return NULL;
}

bool SpileFile(const std::string &strFile, std::string &strZip, std::string &strRef)
{
	if (strFile.empty()) return false;
	const char *pStr = strFile.c_str();
	while (*++pStr != '\0')
	{
		if (*pStr == '/')
		{
			strZip.assign(strFile.c_str(), pStr - strFile.c_str());
			strRef = ++pStr;
			return true;
		}
	}
	return false;
}

class WfcFileCache
{
public:
	WfcFileCache():m_iCacheCap(0x01000000), m_iCacheSize(0)	{}
	~WfcFileCache()	{ m_mapData.clear(); }
	
	bool GetZipData(const std::string &strPath, const std::string &strFile, std::string &data)
	{
		std::string strZip;
		std::string strRef;
		if (SpileFile(strFile, strZip, strRef))
		{
			const char *pExt = CheckZipExt(strPath + strZip);
			if (pExt != NULL)
			{
				return GetZipFile(strPath+strZip+pExt, strRef, data);
			}
		}
		return false;
	}
	
	bool GetZipFile(const std::string &strZip, const std::string &strFile, std::string &data)
	{
		std::string strID = strZip + strFile;
		{
			CefAutoLock scope_lock(m_oLock);
			if (m_mapData.find(strFile) != m_mapData.end())
			{
				data = m_mapData[strFile];
				return true;
			}
		}
		std::string strTemp;
		if (!GetFile(strZip, strTemp))
		{
			return false;
		}
		
		{
			CefAutoLock scope_lock(m_oLock);
			if (m_mapData.find(strZip) == m_mapData.end())
			{
				return false;
			}
		}
		
		if (ReadZipItemFromMem(m_mapData[strZip], strFile, data, data))
		{
			CefAutoLock scope_lock(m_oLock);
			AddData(strID, data);
			return true;
		}
		return false;
	}
	
	bool GetFile(const std::string &strFile, std::string &data, bool isCache = true)
	{
		if (isCache)
		{
			CefAutoLock scope_lock(m_oLock);
			if (m_mapData.find(strFile) != m_mapData.end())
			{
				data = m_mapData[strFile];
				return true;
			}
		}
		
		if (!ReadFile(strFile, data))
		{
			return false;
		}
		
		if (isCache)
		{
			CefAutoLock scope_lock(m_oLock);
			AddData(strFile, data);
		}
		return true;
	}
	
protected:
	bool ReadFile(const std::string &strFile, std::string &data)
	{
		FILE *pFile = fopen(strFile.c_str(), "rb");
		if (pFile == NULL)
		{
			return false;
		}
		else
		{
			int iSize = filelength(fileno(pFile));
			data.resize(iSize);
			fread(&data[0], 1, iSize, pFile);
			fclose(pFile);
			return true;
		}
	}
	
	bool AddData(const std::string &strID, const std::string &data)
	{
		if (m_mapData.find(strID) != m_mapData.end())
		{
			return true;
		}
		int iSize = data.size();
		if (iSize > m_iCacheCap)
		{
			return false;
		}
		if (m_iCacheSize + iSize > m_iCacheCap)
		{
			m_mapData.clear();
			m_iCacheSize = 0;
		}
		m_mapData[strID] = data;
		m_iCacheSize += iSize;
		return true;
	}
	
private:
	CefLock m_oLock;
	int m_iCacheSize;
	int m_iCacheCap;
	std::map<std::string, std::string> m_mapData;
};
#endif //_WFC_FILE_CACHE_H_
