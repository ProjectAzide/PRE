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

// TeapotScene.cpp
// mike weiblen 2015-02-03
// A scene consisting of the GLUT teapot (solid or wireframe)
// teapot bounding box: glOrtho( -1.50, 1.72, -0.75, 0.83, -1.0, 1.0 )

#include "TeapotScene.h"        // our header
#include "ConfigFile.h"
#include "GlUtils.h"
#include "GlutApp.h"            // *Teapot()


TeapotScene::TeapotScene()
{
    // create displaylist?
    reset();
}


TeapotScene::~TeapotScene()
{
}


void TeapotScene::update( const App& app, ConfigFile& config )
{
    config.getNumber( "teapotScene", "scale", _displayScale, false );   // mandatory
}


void TeapotScene::reset()
{
    Scene::reset();
    _isWireframe = false;
}


void TeapotScene::animate( const App& app, const ConfigFile& config )
{
    // TODO
}


bool TeapotScene::keyFunc( unsigned char key, int x, int y )
{
    switch( key )
    {
        case 'w': _isWireframe = !_isWireframe; return true;
    }
    return Scene::keyFunc( key, x, y );
}


void TeapotScene::preFrame()
{
    glClearColor( 0, 0, 0, 1 );
    glClearDepth( 1.0f );
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

    glDepthFunc( GL_LESS );
    glEnable( GL_DEPTH_TEST );

    // a directional light source
    glMatrixMode( GL_MODELVIEW );
    glPushMatrix();
    glLoadIdentity();
    static const GLfloat pos[] = { 5.0f, 5.0f, 10.0f, 0.0f };
    glLightfv( GL_LIGHT0, GL_POSITION, pos );
    glPopMatrix();
    glEnable( GL_LIGHT0 );
    glEnable( GL_LIGHTING );

    // make it a green teapot
    static const GLfloat color[] = { 0.4f, 1.0f, 0.4f, 1.0f };
    glMaterialfv( GL_FRONT, GL_AMBIENT_AND_DIFFUSE, color );
}


void TeapotScene::draw()
{
    glPushMatrix();

    // TODO MANIPULATOR
    glTranslatef( _posX, _posY, _posZ-10.0f );
    glRotatef( _rotX, 1, 0, 0 );
    glRotatef( _rotY, 0, 1, 0 );
    glRotatef( _rotZ, 0, 0, 1 );

    glFrontFace( GL_CW );       // teapot has "backwards" polygon winding

    if( _isWireframe )
    {
        GlutApp::wireTeapot( _displayScale );
    }
    else
    {
        GlutApp::solidTeapot( _displayScale );
    }

    glFrontFace( GL_CCW );      // restore conventional winding

    glPopMatrix();
}


// vim: sw=4 ts=8 et ic ai nows ru:
