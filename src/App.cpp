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

// App.cpp
// mike weiblen 2015-03-26
// abstract base class for all Apps; provides a factory().


#include <stdio.h>              // printf etc
#include <sstream>              // stringstream
#include <assert.h>

#include "App.h"                // our header
#include "Viewer.h"
#include "Scene.h"

#ifndef GL_SHADING_LANGUAGE_VERSION
#define GL_SHADING_LANGUAGE_VERSION       0x8B8C
#endif


// per-platform configuration ///////////////////////////////////////////////

#if LFD_PLATFORM_MSVC
    #pragma message ("building LFD_PLATFORM_MSVC")
    #include <process.h>        // _getpid
    #include <direct.h>         // _getcwd
    #define getpid _getpid
    #define getcwd _getcwd
    #include "GlutApp.h"
    #define APP_CLASS GlutApp
#elif LFD_PLATFORM_LINUX
    #pragma message ("building LFD_PLATFORM_LINUX")
    #include <unistd.h>         // getpid, getcwd
    #include "GlutApp.h"
    #define APP_CLASS GlutApp
#elif LFD_PLATFORM_ANDROID
    #error "LFD_PLATFORM_ANDROID not yet implemented"
    #include "AndroidApp.h"
    #define APP_CLASS AndroidApp
#else
    #error "LFD_PLATFORM_* must be specified"
    #define APP_CLASS (void)
#endif

/////////////////////////////////////////////////////////////////////////////

App* App::factory()
{
    App* app = new APP_CLASS;
    assert( app != 0 );
    return app;
}


App::App() : _viewer(0), _scene(0),
        _statsPrintInterval(0),
        _mainLoopIsActive(true),
        _currSecs(0), _prevSecs(0)
{
    assert( s_instance == 0 );
    s_instance = this;

    setNeedUpdate();

    // create a default instanceName
    std::stringstream name;
    name << APP_NAME << "_" << getpid();
    _instanceName = name.str();
}


App::~App()
{
    delete _viewer;
    delete _scene;
    s_instance = 0;
}


bool App::create( int argc, char** argv )
{
    const char* configFilename = APP_NAME "_config.txt";

    switch( argc )
    {
        case 1:
            // use default configFilename
            break;

        case 2:
            configFilename = argv[1];
            break;

        default:        // print usage
            printf( "\nusage: %s [configFilename]\n\n", argv[0] );
            return false;
    }

    char cwd[512];
    if( getcwd( cwd, sizeof(cwd)-1 ) == NULL )  cwd[0] = '\0';

    printf( "CWD\t\"%s\"\n", cwd );
    printf( "configFilename\t\"%s\"\n", configFilename );

    _config.create( configFilename );
    registerLuaFunctions();

    if( ! _config.loadFile() )  return false;

    printf( "instanceName\t\"%s\"\n", _instanceName.c_str() );

    if( ! createGlSurface() )  return false;

    // now an OpenGL context exists

    _viewer = Viewer::factory( Viewer::getType( _config ) );
    _scene = Scene::factory( Scene::getType( _config ) );

    CHECK_GLERROR( "App::create" );

    printInfo();
    printf( "\n" );

    resetStats();

    return true;
}


void App::destroy()
{
    if( isStatsIntervalEnabled() )
    {
        printStats();
        resetStats();
    }
}


void App::exitMainLoop()
{
    _mainLoopIsActive = false;
}


bool App::isMainLoopActive() const
{
    return _mainLoopIsActive;
}


void App::printInfo()
{
#define PRINT(x) { printf( "%s\t\"%s\"\n", #x, (char*)glGetString(x) ); }
    PRINT( GL_RENDERER );
    PRINT( GL_VERSION );
    PRINT( GL_VENDOR );
    PRINT( GL_SHADING_LANGUAGE_VERSION );
    //PRINT( GL_EXTENSIONS );
#undef PRINT

#define PRINT(x) { GLint i=0; glGetIntegerv(x,&i); printf( "%s\t%d\n", #x, i ); }
    PRINT( GL_DEPTH_BITS );
    PRINT( GL_STENCIL_BITS );
    //PRINT( GL_MAX_MODELVIEW_STACK_DEPTH );
    //PRINT( GL_MAX_PROJECTION_STACK_DEPTH );
    PRINT( GL_MAX_TEXTURE_SIZE );
#undef PRINT

    CHECK_GLERROR( "App::printInfo" );

#define PRINT(x) printf( "%s\t%d\n", #x, static_cast<int>(x) );
    //PRINT( sizeof(Viewer) );
    //PRINT( sizeof(Camera) );
#undef PRINT
}


void App::draw()
{
    if( _needUpdate )  update();

    setTime( getElapsedTime() );                // [seconds]
    _scene->animate( *this, _config );

    _viewer->frame( *_scene );
    CHECK_GLERROR( "App::draw" );

    updateStats();
}


void App::setTime( double seconds )
{
    _prevSecs = _currSecs;
    _currSecs = seconds;
}


double App::getDeltaT() const
{
    return _currSecs - _prevSecs;
}


void App::resetStats()
{
    _statsStartTime = getElapsedTime();         // [seconds]
    _statsNumFrames = 0;
    _statsNumViews = 0;
}


void App::updateStats()
{
    ++_statsNumFrames;
    _statsNumViews += _viewer->getNumCameras();

    if( isStatsIntervalEnabled() && (getStatsInterval() >= _statsPrintInterval) )
    {
        printStats();
        resetStats();
    }
}


bool App::isStatsIntervalEnabled() const
{
    return (_statsPrintInterval != 0);
}


double App::getStatsInterval() const
{
    return getElapsedTime() - _statsStartTime;
}


void App::printStats() const
{
    printf( "stats %d frames %d views", _statsNumFrames, _statsNumViews );

    const double sec = getStatsInterval();
    if( sec > 0.001 )
    {
        printf( " %.3f sec %.2f fps %.1f vps", sec,
                static_cast<double>(_statsNumFrames)/sec,
                static_cast<double>(_statsNumViews)/sec );
    }
    printf( "\n" );
    fflush( stdout );
}


void App::update()
{
    _config.loadFile();

    const Viewer::Type vt = Viewer::getType( _config );
    if( Viewer::isTypeValid(vt) && (_viewer->getType() != vt) )
    {
        delete _viewer;
        _viewer = Viewer::factory( vt );
    }

    const Scene::Type st = Scene::getType( _config );
    if( Scene::isTypeValid(st) && (_scene->getType() != st) )
    {
        delete _scene;
        _scene = Scene::factory( st );
    }

    _scene->update( *this, _config );
    _viewer->update( *this, _config );

    resetStats();
    _needUpdate = false;
}


void App::registerLuaFunctions()
{
    _config.registerCFunction( "instanceName", App::s_instanceName );
    _config.registerCFunction( "statsPrintInterval", App::s_statsPrintInterval );
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
// static wrappers for Lua function callbacks

#if LFD_PLATFORM_MSVC
#include "lua.hpp"
#else
#include "lua5.2/lua.hpp"
#endif


App* App::s_instance = 0;

int App::s_instanceName( lua_State* L )
{
    s_instance->_instanceName = luaL_checkstring( L, 1 );
    return 0;
}

int App::s_statsPrintInterval( lua_State* L )
{
    s_instance->_statsPrintInterval = luaL_checknumber( L, 1 );
    return 0;
}

// vim: sw=4 ts=8 et ic ai nows ru:
