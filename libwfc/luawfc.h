#ifndef _LUA_WFC_H_
#define _LUA_WFC_H_
#pragma once
#include "lua.hpp"
#include "libwfc.h"


#define PAI(L, n) (long) (lua_isnumber(L,n) ? luaL_checknumber((L), (n)) : 0)

#define PAS(L, n) (lua_isstring(L,n) ? luaL_checkstring((L), (n)) : "")

#define PASL(L, n) (lua_isstring(L,n) ? luaL_strlen((L), (n)) : 0)

#define PAD(L, n) (double) (lua_isnumber(L,n) ? luaL_checknumber((L), (n)) : 0.0f)

WFC_API int luaopen_libwfc(lua_State* L);

int scheme_call_lua(const char *url, const char *req, lua_Integer conf, std::string &err);

void scheme_get_data(int func_ref, std::string &data_, std::string &mime_, std::string &err);
#endif //_LUA_WFC_H_