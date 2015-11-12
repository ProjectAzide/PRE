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

// App.h
// mike weiblen 2015-03-26
// abstract base class for all Apps; provides a factory().

#ifndef APP_H
#define APP_H

#include <string>

#include "ConfigFile.h"

class Viewer;
class Scene;


#define APP_NAME "lefdy"        // the name of our App


class App
{
public:
    static App* factory();
    virtual ~App();

    virtual bool create( int argc, char** argv );
    virtual void destroy();
    virtual void doMainLoopOnce() = 0;

    double getDeltaT() const;

    void exitMainLoop();
    bool isMainLoopActive() const;

protected:      // methods
    App();

    virtual void printInfo();
    virtual void registerLuaFunctions();
    virtual bool createGlSurface() = 0;
    virtual double getElapsedTime() const = 0;  // [seconds]

    void draw();

    Viewer& getViewer() const { return *_viewer; }
    Scene& getScene() const { return *_scene; }
    ConfigFile& getConfig() { return _config; }
    const char* getInstanceName() const { return _instanceName.c_str(); }

    void setNeedUpdate() { _needUpdate = true; }

    void setTime( double seconds );

    void resetStats();
    void updateStats();
    bool isStatsIntervalEnabled() const;
    double getStatsInterval() const;
    void printStats() const;

private:
    void update();

private:        // data
    Viewer* _viewer;
    Scene* _scene;
    ConfigFile _config;
    bool _needUpdate;
    std::string _instanceName;
    bool _mainLoopIsActive;

    double _prevSecs;           // [seconds]
    double _currSecs;           // [seconds]

    double _statsStartTime;
    double _statsPrintInterval;
    int _statsNumFrames;
    int _statsNumViews;

    // static wrappers for Lua function callbacks
    static App* s_instance;
    static int App::s_instanceName( lua_State* L );
    static int App::s_statsPrintInterval( lua_State* L );

private:        // disallowed
    App( const App& );
    App& operator=( const App& );
};

#endif

// vim: sw=4 ts=8 et ic ai nows ru:
