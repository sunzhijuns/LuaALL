#include"..\lua.hpp"

#include <iostream>
#include <string>
#include <windows.h>
using namespace std;

#define LUA_SZJ_WINDOWS_FILES "Files"

//������ʾ����Windows�²����ļ�����
//����:string·����
//���:userdata���Handle(���û�ҵ�������nil), string�ļ���
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

//����:userdata:findfirst���ص�Handle
//���:string���ļ��������û�ҵ����򷵻�nil
int findnext(lua_State *L)
{
	WIN32_FIND_DATAA FindFileData;
	if (::FindNextFileA(lua_touserdata(L, 1), &FindFileData))
		lua_pushstring(L, FindFileData.cFileName);
	else
		lua_pushnil(L);
	return 1;
}

//����:userdata:findfirst���ص�Handle
//û�����
int findclose(lua_State *L)
{
	::FindClose(lua_touserdata(L, 1));
	return 0;
}


//ע�ắ����
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

һ��Lua��ʵ������һ��������һϵ��Lua�����Ĵ���飬������Щ�����������ʵ��ĵط���ͨ����Ϊtable���������档Lua��C���������ʵ�ֵġ�
��Ϊһ�������Ŀ⣬���ǻ���Ҫдһ������������ѿ��е����й��������ŵ�table�Ȼ��ע�ᵽLuaȫ�ֱ��������luaopen_*����һ���� LuaΪ���������ṩ�˸�������luaL_register��������һ��C�������б�����Ƕ�Ӧ�ĺ�������������Ϊһ������һ��table��ע��������Щ ������
������ע����һ����ΪFiles�Ŀ⣬�����������⺯����FindFirst,FindNext,FindClose��
*/