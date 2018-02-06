#include <include/cef_version.h>
#include "luawfc.h"
#include "libwfc.h"
#include "cef_client_scheme.h"

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

const luaL_Reg l_lib[] = {
	{"get_version", lua_get_version},
	{0, 0}
};

int luaopen_libwfc(lua_State* L)
{
	CoInitializeCef();
	RegisterSchemeHandlers();
	luaL_register(L, "libwfc", l_lib);
}