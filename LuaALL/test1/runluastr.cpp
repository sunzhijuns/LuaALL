#include"..\lua.hpp"
#include <iostream>
#include <string>
using namespace std;

int runluastr()
{
	//Luaʾ�����룬ʹ��table
	char *szLua_code =
		"x = {} --���ڴ�Ž����table\n "
		"x[1],x[2] = string.gsub(c.Str, c.Mode, c.Tag) --x[1]���ǽ����x[2]�����滻����\n "
		"x.u = string.upper(x[1])\n";
	//Lua���ַ���ģʽ
	char *szMode = "(%w+)%s*=%s*(%w+)";
	//Ҫ������ַ���
	char *szStr = "key1 = value1 key2 = value2";
	//Ŀ���ַ���ģʽ
	char *szTag = "<%1>%2</%1>";

	lua_State *L = luaL_newstate();
	luaL_openlibs(L);

	//��һ��tabele�͸�Lua
	lua_newtable(L);    //�½�һ��table��ѹ��ջ��
	lua_pushstring(L, "Mode");// key
	lua_pushstring(L, szMode);// value
							  //����newtable[Mode]=szMode
							  //������������ѹջ������tableԪ������ջ��������������λ��
	lua_settable(L, -3);
	//lua_settable���Լ���������ѹ���key��value

	lua_pushstring(L, "Tag");// key
	lua_pushstring(L, szTag);// value
	lua_settable(L, -3);    //����newtable[Tag]=szTag

	lua_pushstring(L, "Str");// key
	lua_pushstring(L, szStr);// value
	lua_settable(L, -3);    //����newtable[Str]=szStr

	lua_setglobal(L, "c"); //��ջ��Ԫ�أ�newtable����ΪLua�е�ȫ�ֱ���c

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
		lua_getglobal(L, "x");

		//���xӦ���Ǹ�table
		if (lua_istable(L, -1))
		{
			//ȡ��x.u,��x["u"]
			lua_pushstring(L, "u");    //key
									   //�������ѹջ��x����ջ���ڶ�λ��
			lua_gettable(L, -2);
			//lua_gettable�ᵯ������ѹ���key��Ȼ��Ѷ�Ӧ��valueѹ��
			//ȡ�����ݣ�Ȼ���ջ�е������value
			cout << "x.u = " << lua_tostring(L, -1) << endl;
			lua_pop(L, 1);

			//ȡ��x[1]��x[2]
			for (int i = 1; i <= 2; i++)
			{
				//����key�������⣬�������ûʲô����
				lua_pushnumber(L, i);
				lua_gettable(L, -2);
				cout << "x[" << i << "] = " << lua_tostring(L, -1) << endl;
				lua_pop(L, 1);
			}
		}

		//����ջ����x
		lua_pop(L, 1);
	}
	lua_close(L);
	return 0;
}
/*
�������õ�����Lua C API�ǣ�
void lua_newtable (lua_State *L);
�½�һ���յ�table��ѹ��ջ����
void lua_settable (lua_State *L, int idx);
lua_settable��table��ջ�е�������Ϊ����������ջ����key��value��ջ����������ֵ�޸�table��
void lua_gettable (lua_State *L, int idx);
lua_gettable��table��ջ�е�������Ϊ����������ջ����Ԫ����Ϊkey��������key��Ӧ��value��ѹ��ջ����
���Lua������table�ṩ�˴�ȡ����
void lua_rawgeti (lua_State *L, int idx, int n)
ȡ��table[n]���ŵ�ջ����������69-70�е�lua_pushnumber(L,i);lua_gettable(L,-2);������lua_rawgeti(L,-1)���档
lua_getfield (lua_State *L, int idx, const char *k)
ȡ��table.k���ŵ�ջ����������57-59�е�lua_pushstring(L,"u");lua_gettable(L,-2);�����滻��lua_getfield(L,-1,"u")��
void lua_setfield (lua_State *L, int idx, const char *k)
��ջ����������Ϊvalue����table.k�У������е�����lua_pushstring(L, "key");lua_pushstring(L, value);lua_settable(L, -3);���Ըĳ�lua_pushstring(L, value);lua_setfield(L,-2,"key");����ʽ��
void lua_rawseti (lua_State *L, int idx, int n)
��ջ����������Ϊvalue����table[n]��
*/