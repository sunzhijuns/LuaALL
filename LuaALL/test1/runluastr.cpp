#include"..\lua.hpp"
#include <iostream>
#include <string>
using namespace std;

int runluastr()
{
	//Luaʾ������
	char *szLua_code =
		"r = string.gsub(c_Str, c_Mode, c_Tag) "
		"u = string.upper(r)";
	//Lua���ַ���ģʽ
	char *szMode = "(%w+)%s*=%s*(%w+)";
	//Ҫ������ַ���
	char *szStr = "key1 = value1 key2 = value2";
	//Ŀ���ַ���ģʽ
	char *szTag = "<%1>%2</%1>";

	lua_State *L = luaL_newstate();
	luaL_openlibs(L);

	//��һ�������͸�Lua
	lua_pushstring(L, szMode);
	lua_setglobal(L, "c_Mode");
	lua_pushstring(L, szTag);
	lua_setglobal(L, "c_Tag");
	lua_pushstring(L, szStr);
	lua_setglobal(L, "c_Str");

	//ִ��
	bool err = luaL_loadbuffer(L, szLua_code, strlen(szLua_code),
		"demo") || lua_pcall(L, 0, 0, 0);
	if (err)
	{
		//���������ʾ
		cerr << lua_tostring(L, -1);
		//����ջ�������������Ϣ
		lua_pop(L, 1);
	}
	else
	{
		//Luaִ�к�ȡ��ȫ�ֱ�����ֵ
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