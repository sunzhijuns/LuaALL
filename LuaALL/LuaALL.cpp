// LuaALL.cpp: 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include<iostream>
using namespace std;
#include "lua.hpp"
#include"test1\jiazailuawenjian.h"
#include "test1\runluastr.h"

int main()
{
	runluastr();
	//jiazailuawenjia();
	//char buff[255];
	//int error;
	//lua_State *L = luaL_newstate();
	//luaL_openlibs(L);
	//
	//while (fgets(buff, sizeof(buff), stdin) != NULL)
	//{
	//	error = luaL_loadbuffer(L, buff, strlen(buff), "line") || lua_pcall(L, 0, 0, 0);
	//	if (error)
	//	{
	//		fprintf(stderr, "%s", lua_tostring(L, -1));
	//		lua_pop(L, 1);
	//	}
	//}
	//lua_close(L);
    return 0;
}

