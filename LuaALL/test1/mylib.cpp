#include "lua.hpp"
#include <string.h>
#include <math.h>

#define LUA_SUNZHIJUN_LIB "utils"

//演示栈操作
static int stackDump(lua_State *L) {
	int i;
	int top = lua_gettop(L);
	for (i = 1; i <= top; i++)
	{
		int t = lua_type(L, i);
		switch (t)
		{
		case LUA_TSTRING:
			printf("'%s'", lua_tostring(L, i));
			break;
		case LUA_TBOOLEAN:
			printf(lua_toboolean(L, i) ? "true" : "false");
			break;
		case LUA_TNUMBER:
			printf("%g", lua_tonumber(L, i));
			break;
		default:

			printf("%s", lua_typename(L, t));
			break;
		}
		printf(" ");

	}
	printf("\n");
	return 1;

}

static int l_sin(lua_State *L)
{
	double d = lua_tonumber(L, 1);
	lua_pushnumber(L, sin(d));
	return 1;
}

static int l_dir(lua_State *L)
{
	lua_newtable(L);
	int ii = 1;
	for (size_t i = 0; i < 10; i++)
	{
		lua_pushnumber(L, ii++);
		lua_pushstring(L, "ii");
		lua_settable(L, -3);
	}
	return 1;
}

static const struct luaL_Reg mylib[] = {
	{ "dir", l_dir },
	{"sin", l_sin},
	{"print",stackDump},
	{ NULL, NULL },
};

int lua_open_mylib(lua_State *L)
{
	luaL_newlib(L, mylib);
	return 1;
}

static const luaL_Reg loadedlibs[]{
	{ LUA_SUNZHIJUN_LIB , lua_open_mylib},
	{NULL, NULL}
};

LUALIB_API void luaL_open_mylibs(lua_State *L)
{
	const luaL_Reg *lib;
	for (lib = loadedlibs; lib->func; lib++)
	{
		luaL_requiref(L, lib->name, lib->func, 1);
		lua_pop(L, 1);
	}
}