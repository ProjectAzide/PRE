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

// GearScene.cpp
// mike weiblen 2014-12-16
// Scene of the spinning GL gears, circa 1999.

#include <stdio.h>

#include "GearScene.h"          // our header
#include "App.h"

// import the guts of glxgears.c using #include
#if LFD_PLATFORM_MSVC
#pragma warning(disable:4244) // conversion from 'double' to 'GLfloat'
#pragma warning(disable:4305) // truncation from 'double' to 'GLfloat'
#endif
namespace glxgears {
#include "glxgears.h"
}


GearScene::GearScene()
{
    glxgears::gear1 = 0;
    glxgears::gear2 = 0;
    glxgears::gear3 = 0;

    glxgears::init();
    reset();
}


GearScene::~GearScene()
{
    glDeleteLists( glxgears::gear1, 1 );
    glxgears::gear1 = 0;

    glDeleteLists( glxgears::gear2, 1 );
    glxgears::gear2 = 0;

    glDeleteLists( glxgears::gear3, 1 );
    glxgears::gear3 = 0;
}


void GearScene::update( const App& app, ConfigFile& config )
{
    config.getNumber( "gearScene", "scale", _displayScale, false );     // mandatory
    config.getNumber( "gearScene", "spinRate", _gearRate, false );      // mandatory
}


void GearScene::reset()
{
    Scene::reset();
    glxgears::angle = 0.0f;
}


void GearScene::animate( const App& app, const ConfigFile& config )
{
    glxgears::angle += _gearRate * app.getDeltaT();
}


void GearScene::preFrame()
{
    glClearColor( 0, 0, 0, 1 );
    glClearDepth( 1.0f );
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

    glDepthFunc( GL_LESS );
    glEnable( GL_DEPTH_TEST );
}


void GearScene::draw()
{
    glxgears::view_rotx = _rotX + 20.0f;
    glxgears::view_roty = _rotY + 30.0f;
    glxgears::view_rotz = _rotZ + 0.0f;

    glPushMatrix();

    // TODO MANIPULATOR
    glTranslatef( _posX, _posY, _posZ-40.0f );
    glScalef( _displayScale, _displayScale, _displayScale );

    glxgears::draw();

    glPopMatrix();
}


// vim: sw=4 ts=8 et ic ai nows ru:
