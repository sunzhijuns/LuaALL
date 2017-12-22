
#include "lua.hpp"
#include <string.h>
#include <math.h>
#include"mylib.h"
#define MAX_COLOR 255

struct ColorTable
{
	char* name;
	unsigned char red, green, blue;
}colortable[] = {
	{ "WHITE",	MAX_COLOR,	MAX_COLOR,	MAX_COLOR },
	{ "RED",		MAX_COLOR,	0,			0 },
	{ "GREEN",	0,			MAX_COLOR,	0 },
	{ "BLUE",	0,			0,			MAX_COLOR },
	{ "BLACK",	0,			0,			0 },
	{ NULL,		0,			0,			0 }
};

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
void setfield(lua_State *L, const char* key, int value)
{
	lua_pushstring(L, key);
	lua_pushnumber(L, (double)value / MAX_COLOR);
	lua_settable(L, -3);
}

int getfield(lua_State *L, const char* key)
{
	int result;
	lua_pushstring(L, key);
	lua_gettable(L, -2);
	if (!lua_isnumber(L, -1))
	{
		luaL_error(L, "invalid component in background color");
	}
	stackDump(L);
	result = (int)lua_tonumber(L, -1) * MAX_COLOR;
	lua_pop(L, 1);
	return result;
}

void setcolor(lua_State *L, struct ColorTable *ct)
{
	lua_newtable(L);
	setfield(L, "r", ct->red);
	setfield(L, "g", ct->green);
	setfield(L, "b", ct->blue);
	lua_setglobal(L, ct->name);

}


//加载配置文件
static void load(lua_State* L, char * filename, double *r, double * g, double *b)
{

	if (luaL_loadfile(L, filename) || lua_pcall(L, 0, 0, 0)) {
		luaL_error(L, "cannot run configuration file:%s.", lua_tostring(L, -1));
	}
	lua_getglobal(L, "background");
	if (!lua_istable(L, -1))
	{
		luaL_error(L, "'background' is not a valid color table");
	}
	stackDump(L);
	*r = getfield(L, "r");
	*g = getfield(L, "g");
	*b = getfield(L, "b");
}

void kongzhitai()
{
	///* 控制台
	char buff[255];
	int error;
	lua_State *L = luaL_newstate();
	luaL_openlibs(L);
	luaL_open_mylibs(L);

	int i = 0;
	while (colortable[i].name != NULL)
	{
		setcolor(L, &colortable[i++]);
	}
	double r, g, b;
	load(L, "lua_res1\\config_background.lua", &r, &g, &b);
	printf("%lf, %lf, %lf\n", r, g, b);
	while (fgets(buff, sizeof(buff), stdin) != NULL) {
		error = luaL_loadbuffer(L, buff, strlen(buff), "line") || lua_pcall(L, 0, 0, 0);
		if (error) {
			fprintf(stderr, "%s", lua_tostring(L, -1));
			lua_pop(L, 1);
		}
	}
	lua_close(L);
	//*/
}