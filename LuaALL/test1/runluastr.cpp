#include"..\lua.hpp"
#include <iostream>
#include <string>
#include <complex> //复数

using namespace std;

//C函数，做复数计算，输入实部，虚部。输出绝对值和角度
int calcComplex(lua_State *L)
{
	//从栈中读入实部，虚部
	double r = luaL_checknumber(L, 1);
	double i = luaL_checknumber(L, 2);
	complex<double> c(r, i);
	//存入绝对值
	lua_pushnumber(L, abs(c));
	//存入角度
	lua_pushnumber(L, arg(c)*180.0 / 3.14159);
	return 2;//两个结果
}



int runluastr()
{
	char *szLua_code =
		"v,a = CalcComplex(3,4) "
		"print(v,a)";

	lua_State *L = luaL_newstate();
	luaL_openlibs(L);

	//放入C函数
	lua_pushcfunction(L, calcComplex);
	lua_setglobal(L, "CalcComplex");

	//执行
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

例四，在Lua代码中调用C++函数

能Lua代码中调用C函数对Lua来说至关重要，让Lua能真正站到C这个巨人的肩膀上。
要写一个能让Lua调用的C函数，就要符合lua_CFunction定义：typedef int (*lua_CFunction) (lua_State *L);
当Lua调用C函数的时候，同样使用栈来交互。C函数从栈中获取她的参数，调用结束后将结果放到栈中，并返回放到栈中的结果个数。
这儿有一个重要的概念：用来交互的栈不是全局栈，每一个函数都有他自己的私有栈。当Lua调用C函数的时候，第一个参数总是在这个私有栈的index=1的位置。

结果返回5 53.13...，和其它数据一样，给Lua代码提供C函数也是通过栈来操作的，因为lua_pushcfunction和lua_setglobal的 组合很常用，所以Lua提供了一个宏:
#define lua_register(L,n,f) (lua_pushcfunction(L, (f)), lua_setglobal(L, (n)))
这两句代码也就可写成lua_register(L,"CalcComplex",calcComplex);

*/