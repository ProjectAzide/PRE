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

// CubeScene.cpp
// mike weiblen 2015-03-30
// A scene consisting of a wireframe cube
// INCOMPLETE - WORK IN PROGRESS

#include <assert.h>

#include "CubeScene.h"          // our header
#include "App.h"


CubeScene::CubeScene() : _spinRate(30), _rotation(0), _lineWidth(1), _model(0)
{
    reset();
}


CubeScene::~CubeScene()
{
}


void CubeScene::update( const App& app, ConfigFile& config )
{
    config.getNumber( "cubeScene", "scale", _displayScale, false );
    config.getNumber( "cubeScene", "spinRate", _spinRate, true );
    config.getNumber( "cubeScene", "lineWidth", _lineWidth, true );
    config.getInteger( "cubeScene", "model", _model, true );

}


void CubeScene::reset()
{
    Scene::reset();
    _rotation = 0;
}


void CubeScene::animate( const App& app, const ConfigFile& config )
{
    // TODO
    _rotation += _spinRate * app.getDeltaT();
}


void CubeScene::preFrame()
{
    glClearColor( 0, 0, 0, 1 );
    glClearDepth( 1.0f );
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

    glDepthFunc( GL_LESS );
    glEnable( GL_DEPTH_TEST );
    glDisable( GL_LIGHTING );

    glDisable( GL_CULL_FACE );

    glLineWidth( static_cast<GLfloat>(_lineWidth) );
}


void CubeScene::draw()
{
    glPushMatrix();

    // TODO MANIPULATOR
    glTranslatef( _posX, _posY, _posZ-10.0f );
    glRotatef( static_cast<GLfloat>(_rotation), 1, 0, 0 );
    glRotatef( _rotZ, 0, 0, 1 );

    const GLfloat s = static_cast<GLfloat>( _displayScale );
    glScalef( s, s, s );

    const GLenum mode = (_model == 0) ? GL_LINE_LOOP : GL_QUADS;

    const GLfloat radius = 0.5f;
#define V(a,b,c) glVertex3f( a radius, b radius, c radius );
    glColor3f( 1, 0, 0 ); glBegin( mode );  V(+,+,+); V(-,+,+); V(-,-,+); V(+,-,+); glEnd();
    glColor3f( 0, 1, 0 ); glBegin( mode );  V(-,-,-); V(-,+,-); V(+,+,-); V(+,-,-); glEnd();
    glColor3f( 0, 0, 1 ); glBegin( mode );  V(+,+,+); V(+,+,-); V(-,+,-); V(-,+,+); glEnd();
    glColor3f( 1, 1, 1 ); glBegin( mode );  V(-,-,+); V(-,-,-); V(+,-,-); V(+,-,+); glEnd();
#undef V

    glPopMatrix();
}


// vim: sw=4 ts=8 et ic ai nows ru:
