#include"..\lua.hpp"
#include <iostream>
#include <string>
using namespace std;

int runluastr()
{
	//Lua示例代码，c使用lua函数
	char *szLua_code =
		"function gsub(Str, Mode, Tag)\n"
		"    a,b = string.gsub(Str, Mode, Tag)\n "
		"    c = string.upper(a) \n"
		"    return a,b,c --多个返回值 \n"
		"end\n";
	//Lua的字符串模式
	char *szMode = "(%w+)%s*=%s*(%w+)";
	//要处理的字符串
	char *szStr = "key1 = value1 key2 = value2";
	//目标字符串模式
	char *szTag = "<%1>%2</%1>";

	lua_State *L = luaL_newstate();
	luaL_openlibs(L);

	//执行
	bool err = luaL_loadbuffer(L, szLua_code, strlen(szLua_code),
		"demo") || lua_pcall(L, 0, 0, 0);
	if (err)
	{
		cerr << lua_tostring(L, -1);
		lua_pop(L, 1);
	}
	else
	{
		//Lua执行后取得全局变量的值
		lua_getglobal(L, "gsub");
		if (lua_isfunction(L, -1))    //确认一下是个函数
		{
			//依次放入三个参数
			lua_pushstring(L, szStr);
			lua_pushstring(L, szMode);
			lua_pushstring(L, szTag);
			//调用,我们有3个参数，要得到2个结果
			//你可能注意到gsub函数返回了3个，不过我们只要2个，这没有问题
			//没有使用错误处理回调，所以lua_pcall最后一个参数是0
			if (0 != lua_pcall(L, 3, 2, 0))
			{
				//如果错误，显示
				cerr << lua_tostring(L, -1);
				lua_pop(L, 1);
			}
			else
			{
				//正确，得到两个结果，注意在栈里的顺序
				cout << "a = " << lua_tostring(L, -2) << endl;
				cout << "b = " << lua_tostring(L, -1) << endl;
				//弹出这两个结果
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

调用Lua子函数使用的是lua_pcall函数，我们的所有例子中都有这个函数，它的说明如下:
lua_pcall (lua_State *L, int nargs, int nresults, int errfunc);
作用：以保护模式调用一个函数。
要调用一个函数请遵循以下协议：首先，要调用的函数应该被压入堆栈；接着，把需要传递给这个函数的参数按正序压栈；这是指第一个参数首先压栈。最后调用lua_pcall；
nargs 是你压入堆栈的参数个数。当函数调用完毕后，所有的参数以及函数本身都会出栈。而函数的返回值这时则被压入堆栈。返回值的个数将被调整为 nresults 个，除非 nresults 被设置成 LUA_MULTRET。在这种情况下，所有的返回值都被压入堆栈中。 Lua 会保证返回值都放入栈空间中。函数返回值将按正序压栈（第一个返回值首先压栈），因此在调用结束后，最后一个返回值将被放在栈顶。
如果有错误发生的话， lua_pcall 会捕获它，然后把单一的值（错误信息）压入堆栈，然后返回错误码。lua_pcall 总是把函数本身和它的参数从栈上移除。
如果 errfunc 是 0 ，返回在栈顶的错误信息就和原始错误信息完全一致。否则，这个函数会被调用而参数就是错误信息。错误处理函数的返回值将被 lua_pcall 作为出错信息返回在堆栈上。
*/