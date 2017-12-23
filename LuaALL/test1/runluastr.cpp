#include"..\lua.hpp"
#include <iostream>
#include <string>

using namespace std;

//���㺯��
int count(lua_State *L)
{
	//�õ�UpValue
	double m_ = lua_tonumber(L, lua_upvalueindex(1));
	//����UpValue
	lua_pushnumber(L, ++m_);
	lua_replace(L, lua_upvalueindex(1));
	//���ؽ����ֱ�Ӹ���һ��UpValue��Ϊ�����
	lua_pushvalue(L, lua_upvalueindex(1));
	return 1;
}
//������������һ�����ֺ�count������������󷵻رհ���
int newCount(lua_State *L)
{
	//��������ֵ����UpValue��
	lua_pushnumber(L, 0);
	//������㺯�����������������������������ݸ���
	lua_pushcclosure(L, count, 1);
	return 1;//һ�����,��������
}



int runluastr()
{
	char *szLua_code =
		"c1 = NewCount() "
		"c2 = NewCount() "
		"for i=1,5 do print(c1()) end "
		"for i=1,5 do print(c2()) end";

	lua_State *L = luaL_newstate();
	luaL_openlibs(L);

	//����C����
	lua_register(L, "NewCount", newCount);

	//ִ��
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

�հ�(closure)

�ڱ�д����Lua��C����ʱ�����ǿ�����ҪһЩ��������������������������������Lua��ʹ����������һ����������:
struct CCounter{
CCounter()
	:m_(0){}
int count(){
	return ++i;
}
private:
	int m_;
};
����������ǽ���ʹ��lua_pushcfunction�ṩһ��count�����Ѿ���������Ҫ��(ʹ��static? ���У������Ͳ���ͬʱʹ�ö������������������������ж��Lua�����Ļ���Ҳ���ܹ���)��
��ʱ���Ǿ���Ҫһ�ֻ�����������ĳ�������������γ�һ�����壬�����Lua�еıհ������հ����뺯�����������ݳ�ΪUpValue��
ʹ��Lua�հ��ķ����Ƕ���һ������������������ָ��UpValue�ĳ�ֵ�Ͷ�Ӧ�ĺ������磺
...
//����

--------

���Է���������������֮��û�и��ţ����ǳɹ�����Lua���������������������ࡱ��

����Ĺؼ�������lua_pushcclosure�����ĵڶ���������һ��������������������count����������������UpValue�ĸ�����������Ϊ 1�����ڴ����µıհ�֮ǰ�����Ǳ��뽫�������ݵĳ�ʼֵ��ջ��������������У����ǽ�����0��Ϊ��ʼֵ��ջ����Ԥ�ڵ�һ���� lua_pushcclosure���µıհ��ŵ�ջ�ڣ���˱հ���ΪnewCounter�Ľ�������ء�

ʵ���ϣ�����֮ǰʹ�õ�lua_pushcfunctionֻ��lua_pushcclosure��һ��������û��UpValue�ıհ����鿴���������� ��֪����ֻ��һ������ѣ�

#define lua_pushcfunction(L,f)    lua_pushcclosure(L, (f), 0)

��count�����У�ͨ��lua_upvalueindex(i)�õ���ǰ�հ���UpValue���ڵ�����λ�ã��鿴���Ķ�����Է�����ֻ��һ���򵥵� �꣺

#define lua_upvalueindex(i)    (LUA_GLOBALSINDEX-(i))

�����LUA_GLOBALSINDEX��һ��α����������α������֪ʶ�뿴�½�





*/