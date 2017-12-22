#include"call_va.h"
#include<stdarg.h>
#include<string.h>

// 调用lua的函数
static void call_va(lua_State*L, const char *func, const char*sig, ...) {
	va_list vl;
	int narg, nres;
	va_start(vl, sig);
	lua_getglobal(L, func);
	
	narg = 0;
	while (*sig)
	{
		switch (*sig++)
		{
		case 'd':
			lua_pushnumber(L, va_arg(vl, double));
			break;
		case 'i':
			lua_pushnumber(L, va_arg(vl, int));
			break;
		case 's':
			lua_pushstring(L, va_arg(vl, char*));
			break;
		case '>':
			goto endwhile;
		default:
			luaL_error(L, "invalid option (%c)", *(sig - 1));
			break;
		}
		narg++;
		luaL_checkstack(L, 1, "too mang arguments");
	}endwhile:

	nres = strlen(sig);
	if (lua_pcall(L, narg, nres, 0) != 0)
	{
		luaL_error(L, "error running function '%s':%s", func, lua_tostring(L, -1));
	}
	nres = -nres;
	while (*sig)
	{
		switch (*sig++)
		{
		case 'd':
			if (!lua_isnumber(L, nres))
			{
				luaL_error(L, "wrong result type d");
			}
			*va_arg(vl, double *) = lua_tonumber(L, nres);
			break;
		case 'i':
			if (!lua_isnumber(L, nres))
			{
				luaL_error(L, "wrong result type i");
			}
			*va_arg(vl, int *) = (int)lua_tonumber(L, nres);
			break;
		case 's':
			if (!lua_isstring(L, nres))
			{
				luaL_error(L, "wrong result type s");
			}
			*va_arg(vl, const char **) = lua_tostring(L, nres);
			break;
		default:
			luaL_error(L, "invalid option (%c)", *(sig - 1));
			break;
		}
		nres++;
	}
	va_end(vl);
	
}

//加载配置文件
static void load(lua_State* L, char * filename, int *width, int * height)
{

	if (luaL_loadfile(L, filename) || lua_pcall(L, 0, 0, 0)) {
		luaL_error(L, "cannot run configuration file:%s.", lua_tostring(L, -1));
	}
	lua_getglobal(L, "width");
	lua_getglobal(L, "height");
	if (!lua_isnumber(L, -2))
	{
		luaL_error(L, "'width' should be a number!\n");
	}
	if (!lua_isnumber(L, -1))
	{
		luaL_error(L, "'height' should be a number!\n");
	}
	*width = (int)lua_tonumber(L, -2);
	*height = (int)lua_tonumber(L, -1);

}

double f(lua_State* L, double x, double y)
{
	double z;
	lua_getglobal(L, "f");
	lua_pushnumber(L, x);
	lua_pushnumber(L, y);

	if (lua_pcall(L, 2, 1, 0) != 0)
	{
		luaL_error(L, "error running function 'f': %s", lua_tostring(L, -1));
	}
	if (!lua_isnumber(L, -1))
	{
		luaL_error(L, "function 'f' must return a number");
	}
	z = lua_tonumber(L, -1);
	lua_pop(L, 1);
	return z;
}


void jiazaipeizhiwenjian()
{	
	//加载配置文件
	lua_State *L = luaL_newstate();
	luaL_openlibs(L);
	int width, height;
	load(L, "lua_res1\\config_w_h.lua", &width, &height);
	double z;
	double x = 1;
	double y = 91;
	call_va(L, "f", "dd>d",x, y, &z);
	printf("width = %d, height = %d, f = %f", width, height,z);

	
	
	lua_close(L);
}