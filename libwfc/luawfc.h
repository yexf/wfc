#ifndef _LUA_WFC_H_
#define _LUA_WFC_H_
#pragma once
#include "lua.hpp"
#include "libwfc.h"
#define PAI(L, n) (long) (lua_isnumber(L,n) ? luaL_checknumber((L), (n)) : 0)

WFC_API int luaopen_libwfc(lua_State* L);

#endif //_LUA_WFC_H_