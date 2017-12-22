#include"..\lua.hpp"
#include <iostream>
#include <string>
using namespace std;

int runluastr()
{
	//Lua示例代码
	char *szLua_code =
		"r = string.gsub(c_Str, c_Mode, c_Tag) "
		"u = string.upper(r)";
	//Lua的字符串模式
	char *szMode = "(%w+)%s*=%s*(%w+)";
	//要处理的字符串
	char *szStr = "key1 = value1 key2 = value2";
	//目标字符串模式
	char *szTag = "<%1>%2</%1>";

	lua_State *L = luaL_newstate();
	luaL_openlibs(L);

	//把一个数据送给Lua
	lua_pushstring(L, szMode);
	lua_setglobal(L, "c_Mode");
	lua_pushstring(L, szTag);
	lua_setglobal(L, "c_Tag");
	lua_pushstring(L, szStr);
	lua_setglobal(L, "c_Str");

	//执行
	bool err = luaL_loadbuffer(L, szLua_code, strlen(szLua_code),
		"demo") || lua_pcall(L, 0, 0, 0);
	if (err)
	{
		//如果错误，显示
		cerr << lua_tostring(L, -1);
		//弹出栈顶的这个错误信息
		lua_pop(L, 1);
	}
	else
	{
		//Lua执行后取得全局变量的值
		lua_getglobal(L, "r");
		cout << "r = " << lua_tostring(L, -1) << endl;
		lua_pop(L, 1);

		lua_getglobal(L, "u");
		cout << "u = " << lua_tostring(L, -1) << endl;
		lua_pop(L, 1);
	}
	lua_close(L);
	return 0;
}