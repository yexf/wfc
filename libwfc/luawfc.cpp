#include "luawfc.h"
#include "libwfc.h"
#include "cef_client_scheme.h"

int luaopen_libwfc(lua_State* L)
{
	CoInitializeCef();
	RegisterSchemeHandlers();
}