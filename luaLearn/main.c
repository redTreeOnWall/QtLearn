

#include <stdio.h>

#include "./include/lauxlib.h"
#include "./include/lua.h"
#include "./include/lualib.h"

int DoFileAndString()
{
	printf("this is a test of lua \n");
	char buff[256] = {0};
	int error = 0;
	lua_State *L = luaL_newstate(); // 创建Lua状态机。
	luaL_openlibs(L);				// 打开Lua状态机"L"中的所有Lua标准库。
	int ret = luaL_dofile(L, "../lua.lua");
	printf("ret:%d \n", ret);

	//   lua_call(L,0,0);
	lua_getglobal(L, "txt");
	char *txt = lua_tostring(L, -1);
	printf("txt:%s\n", txt);

	lua_getglobal(L,"gval");
	double f = lua_tonumber(L,-1);
	printf("gval:%f\n",f);

	//   luaL_dostring(L, "print('do a string')");

	/*
  while (fgets(buff, sizeof(buff), stdin) != NULL) {  // 获取用户输入。
    // "luaL_loadbuffer()"，编译用户输入为Lua的"chunk"并将其入栈。
    // "line"是"chunk"的名字，用于调试信息和错误消息。
    // "lua_pcall()"，以保护模式运行"chunk"并将其从栈顶弹出。
    // 两个函数均是在成功时返回"LUA_OK"（实际的值是0），
    // 失败时返回错误码，并将错误信息入栈。
    //
    error =
        luaL_loadbuffer(L, buff, strlen(buff), "line") || lua_pcall(L, 0, 0, 0);
    if (error) {
      fprintf(stderr, "%s", lua_tostring(L, -1));  // 从栈顶取出错误信息打印。
      lua_pop(L, 1);  // 弹出栈顶的错误信息。
    }
  }
  */
	lua_close(L); // 关闭Lua状态机。
	return 0;
}

void LuaStack()
{
	// https://juejin.im/post/5dd62dca5188256ec8054944
	// 1.创建一个state
	lua_State *L = luaL_newstate();

	// 2.入栈操作
	lua_pushstring(L, "I am so cool~");
	lua_pushnumber(L, 20);

	// 3.取值操作
	if (lua_isstring(L, 1))
	{ //这里的index也可以是-2
		printf("[line:%d] lua_tostring(L, 1):%s\n", __LINE__, lua_tostring(L, 1));
	}
	if (lua_isnumber(L, 2))
	{ //这里的index也可以是-1
		printf("[line:%d] lua_tonumber(L, 2):%f\n", __LINE__, lua_tonumber(L, 2));
	}

	// 4.关闭state
	lua_close(L);
}

int main()
{
	DoFileAndString();
	//   LuaStack();
	return 0;
}
