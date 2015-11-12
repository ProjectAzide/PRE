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

// Scene.cpp
// mike weiblen 2014-12-23
// abstract base class for all Scenes; provides a factory().

#include <assert.h>

#include "Scene.h"              // our header
#include "ConfigFile.h"
#include "Utils.h"

#include "GearScene.h"          // derived class for factory()
#include "TeapotScene.h"        // derived class for factory()
#include "StencilScene.h"       // derived class for factory()
#include "CubeScene.h"          // derived class for factory()


Scene::Scene()
{
    _displayScale = 1.0;
    _isCullFace = false;
    reset();
}


Scene::~Scene()
{
}


Scene* Scene::factory( Type type )
{
    Scene* scene = 0;
    switch( type )
    {
        case ST_GEAR:        scene = new GearScene;    break;
        case ST_TEAPOT:      scene = new TeapotScene;  break;
        case ST_CUBE:        scene = new CubeScene;    break;
        case ST_STENCIL:     scene = new StencilScene; break;
        default:             printErr( "invalid Scene::Type\n" ); break;
    }
    assert( scene != 0 );
    return scene;
}


Scene::Type Scene::getType( ConfigFile& config )
{
    int type = ST_INVALID;
    config.getInteger( NULL, "sceneType", type, false );        // mandatory
    return static_cast<Type>(type);
}


const char* Scene::getTypeName( Scene::Type type )
{
    switch( type )
    {
#define CASE(x)  case x : return #x;
        CASE(ST_INVALID);
        CASE(ST_GEAR);
        CASE(ST_TEAPOT);
        CASE(ST_CUBE);
        CASE(ST_STENCIL);
#undef CASE
    }
    return "UNKNOWN";
}


// TODO MANIPULATOR
void Scene::reset()
{
    _posX = _posY = _posZ = 0;
    _rotX = _rotY = _rotZ = 0;
}


// TODO MANIPULATOR
bool Scene::keyFunc( unsigned char key, int x, int y )
{
    switch( key )
    {
        case ' ': reset(); return true;
    }
    return false;
}


#if 0
// TODO MANIPULATOR
bool Scene::specialFunc( int key, int x, int y )
{
    switch( key )
    {
        case GLUT_KEY_UP:        _rotX += 5; return true;
        case GLUT_KEY_DOWN:      _rotX -= 5; return true;

        case GLUT_KEY_RIGHT:     _rotY += 5; return true;
        case GLUT_KEY_LEFT:      _rotY -= 5; return true;

        case GLUT_KEY_PAGE_UP:   _posZ += 1; return true;
        case GLUT_KEY_PAGE_DOWN: _posZ -= 1; return true;

    }
    return false;
}
#endif


void Scene::setGlCullFace() const
{
    if( _isCullFace )
        glEnable( GL_CULL_FACE );
    else
        glDisable( GL_CULL_FACE );
}



// vim: sw=4 ts=8 et ic ai nows ru:
