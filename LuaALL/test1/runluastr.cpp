#include"..\lua.hpp"
#include <iostream>
#include <string>

using namespace std;

//计算函数
int count(lua_State *L)
{
	//得到UpValue
	double m_ = lua_tonumber(L, lua_upvalueindex(1));
	//更改UpValue
	lua_pushnumber(L, ++m_);
	lua_replace(L, lua_upvalueindex(1));
	//返回结果（直接复制一份UpValue作为结果）
	lua_pushvalue(L, lua_upvalueindex(1));
	return 1;
}
//工厂函数，把一个数字和count函数关联打包后返回闭包。
int newCount(lua_State *L)
{
	//计数器初值（即UpValue）
	lua_pushnumber(L, 0);
	//放入计算函数，告诉它与这个函数相关联的数据个数
	lua_pushcclosure(L, count, 1);
	return 1;//一个结果,即函数体
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

	//放入C函数
	lua_register(L, "NewCount", newCount);

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

闭包(closure)

在编写用于Lua的C函数时，我们可能需要一些类似于面向对象的能力，比如我们想在Lua中使用象这样的一个计数器类:
struct CCounter{
CCounter()
	:m_(0){}
int count(){
	return ++i;
}
private:
	int m_;
};
这里如果我们仅仅使用lua_pushcfunction提供一个count函数已经不能满足要求(使用static? 不行，这样就不能同时使用多个计数器，并且如果程序中有多个Lua环境的话它也不能工作)。
这时我们就需要一种机制让数据与某个函数关联，形成一个整体，这就是Lua中的闭包，而闭包里与函数关联的数据称为UpValue。
使用Lua闭包的方法是定义一个工厂函数，由它来指定UpValue的初值和对应的函数，如：
...
//代码

--------

可以发现这两个计算器之间没有干扰，我们成功地在Lua中生成了两个“计数器类”。

这里的关键函数是lua_pushcclosure，她的第二个参数是一个基本函数（例子中是count），第三个参数是UpValue的个数（例子中为 1）。在创建新的闭包之前，我们必须将关联数据的初始值入栈，在上面的例子中，我们将数字0作为初始值入栈。如预期的一样， lua_pushcclosure将新的闭包放到栈内，因此闭包作为newCounter的结果被返回。

实际上，我们之前使用的lua_pushcfunction只是lua_pushcclosure的一个特例：没有UpValue的闭包。查看它的声明可 以知道它只是一个宏而已：

#define lua_pushcfunction(L,f)    lua_pushcclosure(L, (f), 0)

在count函数中，通过lua_upvalueindex(i)得到当前闭包的UpValue所在的索引位置，查看它的定义可以发现它只是一个简单的 宏：

#define lua_upvalueindex(i)    (LUA_GLOBALSINDEX-(i))

宏里的LUA_GLOBALSINDEX是一个伪索引，关于伪索引的知识请看下节





*/