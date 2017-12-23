#include"..\lua.hpp"

#include <iostream>
#include <string>
#include <windows.h>
using namespace std;

#define LUA_SZJ_WINDOWS_FILES "Files"

//函数库示例，Windows下查找文件功能
//输入:string路径名
//输出:userdata存放Handle(如果没找到，则是nil), string文件名
int findfirst(lua_State *L)
{
	WIN32_FIND_DATAA FindFileData;
	HANDLE hFind = ::FindFirstFileA(luaL_checkstring(L, 1), &FindFileData);

	if (INVALID_HANDLE_VALUE == hFind)
		lua_pushnil(L);
	else
		lua_pushlightuserdata(L, hFind);

	lua_pushstring(L, FindFileData.cFileName);

	return 2;
}

//输入:userdata:findfirst返回的Handle
//输出:string：文件名，如果没找到，则返回nil
int findnext(lua_State *L)
{
	WIN32_FIND_DATAA FindFileData;
	if (::FindNextFileA(lua_touserdata(L, 1), &FindFileData))
		lua_pushstring(L, FindFileData.cFileName);
	else
		lua_pushnil(L);
	return 1;
}

//输入:userdata:findfirst返回的Handle
//没有输出
int findclose(lua_State *L)
{
	::FindClose(lua_touserdata(L, 1));
	return 0;
}


//注册函数库
static const struct luaL_Reg lrFiles[] = {
	{ "FindFirst", findfirst },
	{ "FindNext", findnext },
	{ "FindClose", findclose },
	{ NULL, NULL }    /* sentinel */
};

int luaopen_Files(lua_State *L)
{
	luaL_newlib(L, lrFiles);
	return 1;
}



static const luaL_Reg loadedlibs[]{
	{ LUA_SZJ_WINDOWS_FILES , luaopen_Files },
	{ NULL, NULL }
};

LUALIB_API void luaL_open_Files(lua_State *L)
{
	luaL_requiref(L, LUA_SZJ_WINDOWS_FILES, luaopen_Files, 1);
	lua_pop(L, 1);
}

LUALIB_API void luaL_open_mylibs(lua_State *L)
{
	const luaL_Reg *lib;
	for (lib = loadedlibs; lib->func; lib++)
	{
		luaL_requiref(L, lib->name, lib->func, 1);
		lua_pop(L, 1);
	}
}

using namespace std;



int runluastr()
{
	char* szLua_code =
		"hFind,sFile = Files.FindFirst('c:////*.*'); "
		"if hFind then "
		"    repeat "
		"        print(sFile) "
		"        sFile = Files.FindNext(hFind) "
		"    until sFile==nil; "
		"    Files.FindClose(hFind) "
		"end";
	lua_State *L = luaL_newstate();
	luaL_openlibs(L);
	//luaL_open_mylibs(L);
	luaL_open_Files(L);

	bool err = luaL_loadstring(L, szLua_code) || lua_pcall(L, 0, 0, 0);
	if (err)
	{
		cerr << lua_tostring(L, -1);
		lua_pop(L, 1);
	}
	lua_close(L);
	return 0;
}
/*
http://blog.csdn.net/jiafu1115/article/details/8957488

一个Lua库实际上是一个定义了一系列Lua函数的代码块，并将这些函数保存在适当的地方，通常作为table的域来保存。Lua的C库就是这样实现的。
作为一个完整的库，我们还需要写一个函数来负责把库中的所有公共函数放到table里，然后注册到Lua全局变量里，就像luaopen_*做的一样。 Lua为这种需求提供了辅助函数luaL_register，它接受一个C函数的列表和他们对应的函数名，并且作为一个库在一个table中注册所有这些 函数。
下例中注册了一个名为Files的库，定义了三个库函数：FindFirst,FindNext,FindClose。
*/