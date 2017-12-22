#include"..\lua.hpp"
#include <iostream>
#include <string>
using namespace std;

int runluastr()
{
	//Luaʾ�����룬cʹ��lua����
	char *szLua_code =
		"function gsub(Str, Mode, Tag)\n"
		"    a,b = string.gsub(Str, Mode, Tag)\n "
		"    c = string.upper(a) \n"
		"    return a,b,c --�������ֵ \n"
		"end\n";
	//Lua���ַ���ģʽ
	char *szMode = "(%w+)%s*=%s*(%w+)";
	//Ҫ������ַ���
	char *szStr = "key1 = value1 key2 = value2";
	//Ŀ���ַ���ģʽ
	char *szTag = "<%1>%2</%1>";

	lua_State *L = luaL_newstate();
	luaL_openlibs(L);

	//ִ��
	bool err = luaL_loadbuffer(L, szLua_code, strlen(szLua_code),
		"demo") || lua_pcall(L, 0, 0, 0);
	if (err)
	{
		cerr << lua_tostring(L, -1);
		lua_pop(L, 1);
	}
	else
	{
		//Luaִ�к�ȡ��ȫ�ֱ�����ֵ
		lua_getglobal(L, "gsub");
		if (lua_isfunction(L, -1))    //ȷ��һ���Ǹ�����
		{
			//���η�����������
			lua_pushstring(L, szStr);
			lua_pushstring(L, szMode);
			lua_pushstring(L, szTag);
			//����,������3��������Ҫ�õ�2�����
			//�����ע�⵽gsub����������3������������ֻҪ2������û������
			//û��ʹ�ô�����ص�������lua_pcall���һ��������0
			if (0 != lua_pcall(L, 3, 2, 0))
			{
				//���������ʾ
				cerr << lua_tostring(L, -1);
				lua_pop(L, 1);
			}
			else
			{
				//��ȷ���õ����������ע����ջ���˳��
				cout << "a = " << lua_tostring(L, -2) << endl;
				cout << "b = " << lua_tostring(L, -1) << endl;
				//�������������
				lua_pop(L, 2);
			}
		}
		else
		{
			lua_pop(L, 1);
		}
	}
	lua_close(L);
	return 0;
}
/*
http://blog.csdn.net/jiafu1115/article/details/8957488

����Lua�Ӻ���ʹ�õ���lua_pcall���������ǵ����������ж����������������˵������:
lua_pcall (lua_State *L, int nargs, int nresults, int errfunc);
���ã��Ա���ģʽ����һ��������
Ҫ����һ����������ѭ����Э�飺���ȣ�Ҫ���õĺ���Ӧ�ñ�ѹ���ջ�����ţ�����Ҫ���ݸ���������Ĳ���������ѹջ������ָ��һ����������ѹջ��������lua_pcall��
nargs ����ѹ���ջ�Ĳ���������������������Ϻ����еĲ����Լ������������ջ���������ķ���ֵ��ʱ��ѹ���ջ������ֵ�ĸ�����������Ϊ nresults �������� nresults �����ó� LUA_MULTRET������������£����еķ���ֵ����ѹ���ջ�С� Lua �ᱣ֤����ֵ������ջ�ռ��С���������ֵ��������ѹջ����һ������ֵ����ѹջ��������ڵ��ý��������һ������ֵ��������ջ����
����д������Ļ��� lua_pcall �Ჶ������Ȼ��ѵ�һ��ֵ��������Ϣ��ѹ���ջ��Ȼ�󷵻ش����롣lua_pcall ���ǰѺ�����������Ĳ�����ջ���Ƴ���
��� errfunc �� 0 ��������ջ���Ĵ�����Ϣ�ͺ�ԭʼ������Ϣ��ȫһ�¡�������������ᱻ���ö��������Ǵ�����Ϣ�����������ķ���ֵ���� lua_pcall ��Ϊ������Ϣ�����ڶ�ջ�ϡ�
*/