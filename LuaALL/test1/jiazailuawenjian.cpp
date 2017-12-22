
/* 调用lua代码

*/
#include "lua.hpp"
void jiazailuawenjia()
{
	lua_State *l = luaL_newstate();
	luaL_openlibs(l);
	luaL_dofile(l, "lua_res1\\main.lua");
	lua_close(l);
}