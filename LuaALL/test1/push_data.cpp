#include"push_data.h"
#include<iostream>
using namespace std;
#include "lua.hpp"

//演示栈操作
static void stackDump(lua_State *L) {
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

}



void push_data()
{
	///*
	//演示栈操作

	lua_State *L = luaL_newstate();
	lua_pushboolean(L, 1); lua_pushnumber(L, 10);
	lua_pushnil(L); lua_pushstring(L, "hello");
	lua_pushstring(L, "123dddd");
	lua_pushlstring(L,"dddddd",6);

	const char *s = lua_tostring(L, -1);
	printf("%s\n",s);
	lua_settop(L,10);

	stackDump(L);

	lua_pushvalue(L, 2); stackDump(L);

	lua_replace(L, 3); stackDump(L);

	lua_settop(L, 6); stackDump(L);

	lua_remove(L, -3); stackDump(L);

	lua_settop(L, -5); stackDump(L);

	lua_close(L);
	//*/
}