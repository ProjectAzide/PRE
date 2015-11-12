/*

Copyright (c) 2015, Phil J. Bostley III
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:
    * Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in the
      documentation and/or other materials provided with the distribution.
    * Neither the name of Phil J Bostley III nor the
      names of its contributors may be used to endorse or promote products
      derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL <COPYRIGHT HOLDER> BE LIABLE FOR ANY
DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

 */

// LuaInterp.cpp
// mike weiblen 2015-01-30
// Some utilities to simplify using Lua as our configuration language.
// This file was developed using the API of Lua 5.2.3

#include <assert.h>

#include "LuaInterp.h"          // our header
#include "Utils.h"              // printErr

#if LFD_PLATFORM_MSVC
#include "lua.hpp"
#else
#include "lua5.2/lua.hpp"
#endif

/////////////////////////////////////////////////////////////////////////////

class LuaStackCheck
{
public:
    LuaStackCheck( lua_State* L ) : _L(L), _top(lua_gettop(_L)) {}
    ~LuaStackCheck() { assert( ok() ); }

    int delta() const { return (lua_gettop(_L) - _top); }

    bool ok() const
    {
        const int d = delta();
        if( d == 0 )  return true;
        printErr( "LuaStackCheck top %d delta %d\n", _top, d );
        return false;
    }

    void check( const char* file, int line ) const
    {
        printf( "LuaStackCheck delta %d : %s@%d\n", delta(), file, line );
    }

private:
    lua_State* _L;
    int _top;
};

#define STACK_CHECK(x)     (x).check(__FILE__,__LINE__)
//#define STACK_CHECK(x)

/////////////////////////////////////////////////////////////////////////////

LuaInterp::LuaInterp()
{
}


LuaInterp::~LuaInterp()
{
    if( _luaState )
    {
        lua_close( _luaState );
        _luaState = 0;
    }
}


bool LuaInterp::create()
{
    printf( "LUA_RELEASE\t\"%s\"\n", LUA_RELEASE );

    _luaState = luaL_newstate();
    if( _luaState != NULL )
    {
        luaL_openlibs( _luaState );
    }

    return (_luaState != NULL);
}


void LuaInterp::registerCFunction( const char* name, lua_CFunction func )
{
    lua_pushcfunction( _luaState, func );
    lua_setglobal( _luaState, name );
}


bool LuaInterp::loadFile( const char* fileName )
{
    LuaStackCheck lsc(_luaState);
    int errCode = LUA_OK;

    errCode = luaL_loadfile( _luaState, fileName );
    if( errCode != LUA_OK )
    {
        printErr( "luaL_loadfile errCode %d \"%s\"\n", errCode, lua_tolstring( _luaState, -1, 0 ) );
        lua_pop( _luaState, 1 );
        return false;
    }

    int numArgs = 0;
    int numResults = 0;
    int ehi = 0;                // error handler index

    lua_getglobal( _luaState, "debug" );
    lua_getfield( _luaState, -1, "traceback" );
    lua_remove( _luaState, -2 );

    ehi = -2;
    lua_insert( _luaState, ehi );

    errCode = lua_pcall( _luaState, numArgs, numResults, ehi );
    if( errCode != LUA_OK )
    {
        printErr( "lua_pcall %d \"%s\"\n", errCode, lua_tolstring( _luaState, -1, 0 ) );
        lua_pop( _luaState, 2 );
        return false;
    }

    lua_pop( _luaState, 1 );
    return true;
}

/////////////////////////////////////////////////////////////////////////////

bool LuaInterp::getString( const char* table, const char* key, std::string& value, bool nilOK )
{
    LuaStackCheck lsc(_luaState);
    bool isSuccess = false;
    int popCount = 0;
    assert( key != NULL );

    if( table != NULL )
    {
        lua_getglobal( _luaState, table );
        popCount = 1;
        if( lua_istable( _luaState, -1 ) )
        {
            lua_getfield( _luaState, -1, key );
            popCount = 2;
        }
    }
    else
    {
        lua_getglobal( _luaState, key );
        popCount = 1;
    }

    if( lua_isstring( _luaState, -1 ) )
    {
        size_t len = 0;
        value = lua_tolstring( _luaState, -1, &len );
        isSuccess = true;
    }
    else if( nilOK && lua_isnil( _luaState, -1 ) )
    {
        isSuccess = true;
    }
    else if( table != NULL )
    {
        printErr( "config \"%s.%s\" is not a string\n", table, key );
    }
    else
    {
        printErr( "config \"%s\" is not a string\n", key );
    }

    lua_pop( _luaState, popCount );
    return isSuccess;
}

/////////////////////////////////////////////////////////////////////////////

bool LuaInterp::getNumber( const char* table, const char* key, double& value, bool nilOK )
{
    LuaStackCheck lsc(_luaState);
    bool isSuccess = false;
    int popCount = 0;
    assert( key != NULL );

    if( table != NULL )
    {
        lua_getglobal( _luaState, table );
        popCount = 1;
        if( lua_istable( _luaState, -1 ) )
        {
            lua_getfield( _luaState, -1, key );
            popCount = 2;
        }
    }
    else
    {
        lua_getglobal( _luaState, key );
        popCount = 1;
    }

    if( lua_isnumber( _luaState, -1 ) )
    {
        value = lua_tonumber( _luaState, -1 );
        isSuccess = true;
    }
    else if( nilOK && lua_isnil( _luaState, -1 ) )
    {
        isSuccess = true;
    }
    else if( table != NULL )
    {
        printErr( "config \"%s.%s\" is not a number\n", table, key );
    }
    else
    {
        printErr( "config \"%s\" is not a number\n", key );
    }

    lua_pop( _luaState, popCount );
    return isSuccess;
}

/////////////////////////////////////////////////////////////////////////////

bool LuaInterp::getInteger( const char* table, const char* key, int& value, bool nilOK )
{
    LuaStackCheck lsc(_luaState);
    bool isSuccess = false;
    int popCount = 0;
    assert( key != NULL );

    if( table != NULL )
    {
        lua_getglobal( _luaState, table );
        popCount = 1;
        if( lua_istable( _luaState, -1 ) )
        {
            lua_getfield( _luaState, -1, key );
            popCount = 2;
        }
    }
    else
    {
        lua_getglobal( _luaState, key );
        popCount = 1;
    }

    if( lua_isnumber( _luaState, -1 ) )
    {
        value = lua_tointeger( _luaState, -1 );
        isSuccess = true;
    }
    else if( nilOK && lua_isnil( _luaState, -1 ) )
    {
        isSuccess = true;
    }
    else if( table != NULL )
    {
        printErr( "config \"%s.%s\" is not an integer\n", table, key );
    }
    else
    {
        printErr( "config \"%s\" is not an integer\n", key );
    }

    lua_pop( _luaState, popCount );
    return isSuccess;
}

/////////////////////////////////////////////////////////////////////////////

bool LuaInterp::getBoolean( const char* table, const char* key, bool& value, bool nilOK )
{
    LuaStackCheck lsc(_luaState);
    bool isSuccess = false;
    int popCount = 0;
    assert( key != NULL );

    if( table != NULL )
    {
        lua_getglobal( _luaState, table );
        popCount = 1;
        if( lua_istable( _luaState, -1 ) )
        {
            lua_getfield( _luaState, -1, key );
            popCount = 2;
        }
    }
    else
    {
        lua_getglobal( _luaState, key );
        popCount = 1;
    }

    if( lua_isboolean( _luaState, -1 ) )
    {
        value = (lua_toboolean( _luaState, -1 ) != 0);
        isSuccess = true;
    }
    else if( nilOK && lua_isnil( _luaState, -1 ) )
    {
        isSuccess = true;
    }
    else if( table != NULL )
    {
        printErr( "config \"%s.%s\" is not a boolean\n", table, key );
    }
    else
    {
        printErr( "config \"%s\" is not a boolean\n", key );
    }

    lua_pop( _luaState, popCount );
    return isSuccess;
}

// vim: sw=4 ts=8 et ic ai nows ru:
