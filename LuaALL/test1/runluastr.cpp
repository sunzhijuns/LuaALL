#include"..\lua.hpp"
#include <iostream>
#include <string>
#include <complex> //����

using namespace std;

//C���������������㣬����ʵ�����鲿���������ֵ�ͽǶ�
int calcComplex(lua_State *L)
{
	//��ջ�ж���ʵ�����鲿
	double r = luaL_checknumber(L, 1);
	double i = luaL_checknumber(L, 2);
	complex<double> c(r, i);
	//�������ֵ
	lua_pushnumber(L, abs(c));
	//����Ƕ�
	lua_pushnumber(L, arg(c)*180.0 / 3.14159);
	return 2;//�������
}



int runluastr()
{
	char *szLua_code =
		"v,a = CalcComplex(3,4) "
		"print(v,a)";

	lua_State *L = luaL_newstate();
	luaL_openlibs(L);

	//����C����
	lua_pushcfunction(L, calcComplex);
	lua_setglobal(L, "CalcComplex");

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

���ģ���Lua�����е���C++����

��Lua�����е���C������Lua��˵������Ҫ����Lua������վ��C������˵ļ���ϡ�
Ҫдһ������Lua���õ�C��������Ҫ����lua_CFunction���壺typedef int (*lua_CFunction) (lua_State *L);
��Lua����C������ʱ��ͬ��ʹ��ջ��������C������ջ�л�ȡ���Ĳ��������ý����󽫽���ŵ�ջ�У������طŵ�ջ�еĽ��������
�����һ����Ҫ�ĸ������������ջ����ȫ��ջ��ÿһ�������������Լ���˽��ջ����Lua����C������ʱ�򣬵�һ���������������˽��ջ��index=1��λ�á�

�������5 53.13...������������һ������Lua�����ṩC����Ҳ��ͨ��ջ�������ģ���Ϊlua_pushcfunction��lua_setglobal�� ��Ϻܳ��ã�����Lua�ṩ��һ����:
#define lua_register(L,n,f) (lua_pushcfunction(L, (f)), lua_setglobal(L, (n)))
���������Ҳ�Ϳ�д��lua_register(L,"CalcComplex",calcComplex);

*/