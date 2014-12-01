#include <lua.h>//Lua 5.1.5
#include <lauxlib.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/stat.h>  
 
#include "time.h"  
#include "unistd.h" 

static int file_create(lua_State* L)
{
	struct stat* pStat=NULL;
	if (lua_gettop(L) != 1)
		return 0;
	pStat=malloc(sizeof(struct stat));
	if(!pStat){
		return 0;
	}
	memset(pStat,0,sizeof(*pStat));
	int nRet=stat(lua_tostring(L,1),pStat);
	if(nRet<0){
		return 0;
	}
	lua_pushlightuserdata(L,pStat);
	return 1;
}
static int file_dev(lua_State* L)
{
	struct stat* pStat=NULL;
	if (lua_gettop(L) != 1)
		return 0;
	pStat=lua_touserdata(L,1);
	if(!pStat)
		return 0;
	lua_pushinteger(L,pStat->st_dev);
	return 1;
}

static int file_mode(lua_State* L)
{
	struct stat* pStat=NULL;
	if (lua_gettop(L) != 1)
		return 0;
	pStat=lua_touserdata(L,1);
	if(!pStat)
		return 0;
	lua_pushinteger(L,pStat->st_mode);
	return 1;
}

static int file_ino(lua_State*L)
{
	struct stat* pStat=NULL;
	if (lua_gettop(L) != 1)
		return 0;
	pStat=lua_touserdata(L,1);
	if(!pStat)
		return 0;
	lua_pushinteger(L,pStat->st_ino);
	return 1;
}

static int file_link(lua_State* L)
{
	struct stat* pStat=NULL;
	if (lua_gettop(L) != 1)
		return 0;
	pStat=lua_touserdata(L,1);
	if(!pStat)
		return 0;
	lua_pushinteger(L,pStat->st_nlink);
	return 1;
}

static int file_uid(lua_State* L)
{
	struct stat* pStat=NULL;
	if (lua_gettop(L) != 1)
		return 0;
	pStat=lua_touserdata(L,1);
	if(!pStat)
		return 0;
	lua_pushinteger(L,pStat->st_uid);
	return 1;
}

static int file_gid(lua_State* L)
{
	struct stat* pStat=NULL;
	if (lua_gettop(L) != 1)
		return 0;
	pStat=lua_touserdata(L,1);
	if(!pStat)
		return 0;
	lua_pushinteger(L,pStat->st_gid);
	return 1;
}

static int file_size(lua_State* L)
{
	struct stat* pStat=NULL;
	if (lua_gettop(L) != 1)
		return 0;
	pStat=lua_touserdata(L,1);
	if(!pStat)
		return 0;
	lua_pushinteger(L,pStat->st_size);
	return 1;
}
//block size
static int file_blksize(lua_State* L)
{
	struct stat* pStat=NULL;
	if (lua_gettop(L) != 1)
		return 0;
	pStat=lua_touserdata(L,1);
	if(!pStat)
		return 0;
	lua_pushinteger(L,pStat->st_blksize);
	return 1;
}

//number of allocated blocks
static int file_blocks(lua_State* L)
{
	struct stat* pStat=NULL;
	if (lua_gettop(L) != 1)
		return 0;
	pStat=lua_touserdata(L,1);
	if(!pStat)
		return 0;
	lua_pushinteger(L,pStat->st_blocks);
	return 1;
}

//文件最后被访问的时间
static int file_access(lua_State* L)
{
	struct stat* pStat=NULL;
	if (lua_gettop(L) != 1)
		return 0;
	pStat=lua_touserdata(L,1);
	if(!pStat)
		return 0;
	struct tm* paccess=localtime(&(pStat->st_atime)); 
	char aBuffer[64] = {0};
	strftime(aBuffer, 64, "%Y-%m-%d %H:%M:%S", paccess);
	lua_pushstring(L,aBuffer);
	return 1;
}

//文件内容最后被修改的时间
static int file_modify(lua_State* L)
{
	struct stat* pStat=NULL;
	if (lua_gettop(L) != 1)
		return 0;
	pStat=lua_touserdata(L,1);
	if(!pStat)
		return 0;
	struct tm* pmodify=localtime(&(pStat->st_mtime)); 
	char aBuffer[64] = {0};
	strftime(aBuffer, 64, "%Y-%m-%d %H:%M:%S", pmodify);
	lua_pushstring(L,aBuffer);
	return 1;
}

//文件状态改变时间  
static int file_change(lua_State* L)
{
	struct stat* pStat=NULL;
	if (lua_gettop(L) != 1)
		return 0;
	pStat=lua_touserdata(L,1);
	if(!pStat)
		return 0;
	struct tm* pchange=localtime(&(pStat->st_ctime)); 
	char aBuffer[64] = {0};
	strftime(aBuffer, 64, "%Y-%m-%d %H:%M:%S", pchange);
	lua_pushstring(L,aBuffer);
	return 1;
}

static int file_destory(lua_State* L)
{
	struct stat* pStat=NULL;
	if (lua_gettop(L) != 1)
		return 0;
	pStat=lua_touserdata(L,1);
	if(pStat)
		free(pStat);
	return 0;
}

static const struct luaL_Reg file_lib[] = {
	{ "create", file_create },
	{ "dev", file_dev },
	{ "mode", file_mode },
	{ "ino", file_ino },
	{ "link", file_link },
	{ "uid", file_uid },
	{ "gid", file_gid },
	{ "uid", file_uid },
	{ "size", file_size },
	{ "blksize" ,file_blksize},
	{ "blocks", file_blocks },
	{ "access", file_access },
	{ "modify", file_modify },
	{ "change", file_change },
	{ "destory", file_destory },
	{ NULL, NULL }
};

static int settablereadonly(lua_State *L)
{
	return luaL_error(L, "Must not update a read-only table");
}

#define LUA_MACLIBNAME "file"

LUALIB_API int luaopen_file(lua_State *L)
{
	luaL_register(L, LUA_MACLIBNAME, file_lib);

	return 1;
}
