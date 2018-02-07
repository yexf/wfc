#include <string>
#include <include/cef_version.h>
#include "luawfc.h"
#include "cef_client_scheme.h"
#include "wfc_cef_lock.h"

lua_State* g_cefLua = NULL;
int g_nPostFunc = 0;
CefLock g_oLuaCall;
// scheme lua 入口函数，返回url对应的lua处理函数的引用
int scheme_call_lua(const char *url, const char *req, lua_Integer conf, std::string &err)
{
	if (g_cefLua == NULL)
	{
		err = "lua vm is null";
		return -1;
	}
	CefAutoLock lock_scope(g_oLuaCall);
	lua_rawgeti(g_cefLua, LUA_REGISTRYINDEX, g_nPostFunc);
	lua_pushstring(g_cefLua, "scheme");
	lua_pushstring(g_cefLua, url);
	lua_pushstring(g_cefLua, req);
	lua_pushinteger(g_cefLua, conf);
	if (lua_pcall(g_cefLua, 4, 1, 0) != 0)
	{
		err = lua_tostring(g_cefLua, -1);
		return -1;
	}
	if (!lua_isfunction(g_cefLua, -1))
	{
		err = "scheme call lua return error";
		lua_pop(g_cefLua, 1);
		return -1;
	}
	else
	{
		return lua_ref(g_cefLua, 1);
	}
}

void scheme_get_data(int func_ref, std::string &data_, std::string &mime_, std::string &err)
{
	if (g_cefLua == NULL)
	{
		err = "lua vm is null";
		return -1;
	}
	CefAutoLock lock_scope(g_oLuaCall);
	lua_rawgeti(g_cefLua, LUA_REGISTRYINDEX, func_ref);
	if (lua_pcall(g_cefLua, 0, 2, 0) != 0)
	{
		data_ = lua_tostring(g_cefLua, -1);
		mime_ = "text/plain";
	}
	else
	{
		data_ = lua_tostring(g_cefLua, -2);
		if (lua_isstring(g_cefLua, -1))
		{
			mime_ = lua_tostring(g_cefLua, -1);
		}
		lua_pop(g_cefLua, 2);
	}
	
	lua_unref(g_cefLua, func_ref);
}

void tactic_call_lua(const char *msg, std::string &err)
{
	if (g_cefLua == NULL)
	{
		err = "lua vm is null";
		return -1;
	}
	CefAutoLock lock_scope(g_oLuaCall);
	lua_rawgeti(g_cefLua, LUA_REGISTRYINDEX, g_nPostFunc);
	lua_pushstring(g_cefLua, "tactic");
	lua_pushstring(g_cefLua, msg);
	if (lua_pcall(g_cefLua, 2, 0, 0) != 0)
	{
		err = lua_tostring(g_cefLua, -1);
	}
}

static int lua_get_version(lua_State* L)
{
	char arChromeVer[256] = {0};
	sprintf(arChromeVer, "%d.%d.%d.%d", CHROME_VERSION_MAJOR, CHROME_VERSION_MINOR,\
		CHROME_VERSION_BUILD, CHROME_VERSION_PATCH);
	lua_pushstring(L, arChromeVer);
	lua_pushstring(L, CEF_VERSION);
	lua_pushstring(L, CEF_COMMIT_HASH);
	return 3;
}

static int lua_cef_init(lua_State* L)
{
	const char *errStr = NULL;
	do
	{
		CefAutoLock lock_scope(g_oLuaCall);
		if (g_cefLua == NULL)
		{
			g_cefLua = luaL_newstate();
			luaL_openlibs(g_cefLua);
		}
		
		const char *lua_str = PAS(L, 1);
		if (luaL_loadstring(g_cefLua, lua_str) != 0)
		{
			errStr = lua_tostring(g_cefLua, -1);
		}
		if (lua_pcall(g_cefLua, 0, LUA_MULTRET, 0) != 0)
		{
			errStr = lua_tostring(g_cefLua, -1);
		}
		g_nPostFunc = lua_ref(g_cefLua, 1);
	} while (false);
	
	if (errStr)
	{
		lua_pushnil(L);
		lua_pushstring(L, errStr);
		return 2;
	}
	
	lua_pushboolean(L, 1);
	return 1;
}

const luaL_Reg l_lib[] = {
	{"get_version", lua_get_version},
	{"cef_init", lua_cef_init},
	{0, 0}
};

int luaopen_libwfc(lua_State* L)
{
	CoInitializeCef();
	RegisterSchemeHandlers();
	luaL_register(L, "libwfc", l_lib);
}