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

// StencilScene.cpp
// mike weiblen 2015-02-03
// Renders the stencil mask around each Camera

#include <cmath>                // std::sin, std::cos

#include "StencilScene.h"       // our header


StencilScene::StencilScene() : _dispList(0)
{
    const float numSteps = 6.0f;                // TODO CONFIG
    const float phase = (30.0f / 360.0f);       // TODO CONFIG

    _dispList = glGenLists(1);
    glNewList( _dispList, GL_COMPILE );

    glBegin( GL_TRIANGLE_FAN );
    glVertex3f( 0, 0, 0 );
    for( float i = 0; i <= numSteps; ++i )
    {
        float a = i / numSteps;
        a += phase;
        a *= (2.0f * 3.1415927f);               // convert revolution to radian
        glVertex3f( std::cos(a), std::sin(a), 0 );
    }
    glEnd();

    // a white spot at the center
    glColor3f( 1, 1, 1 );                       // TODO CONFIG
    glPointSize( 1.0f );                        // TODO CONFIG
    glBegin( GL_POINTS );
    glVertex3f( 0, 0, 0 );
    glEnd();

    glEndList();

    reset();
}


StencilScene::~StencilScene()
{
    if( _dispList != 0 )
    {
        glDeleteLists( _dispList, 1 );
        _dispList = 0;
    }
}


bool StencilScene::isVisible( Camera::ProjVolume pv ) const
{
    // Stencil scene is 2D at Z=0, so only need to draw in 1 ProjVolume;
    // doesn't matter which, as draw() overrides the projection.
    return (pv == Camera::PV_EPSILON);
}


void StencilScene::preFrame()
{
    glClearColor( 1, 0, 0, 1 );
    glClearDepth( 1.0f );
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

    glDisable( GL_DEPTH_TEST );
}


void StencilScene::draw()
{
    // override the projection with orthographic in NDC space
    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();
    glMatrixMode( GL_MODELVIEW );

    // set color based on current reference value
    GLint stencilRef = 0;
    glGetIntegerv( GL_STENCIL_REF, &stencilRef );
    switch( stencilRef )
    {
        case STID_0: glColor3f( 0.2f, 0.0f, 0.0f ); break;
        case STID_1: glColor3f( 0.0f, 0.2f, 0.0f ); break;
        case STID_2: glColor3f( 0.0f, 0.0f, 0.2f ); break;
        default:     glColor3f( 0.0f, 1.0f, 0.0f ); break;
    }

    glPushMatrix();
    glLoadIdentity();
    glCallList( _dispList );
    glPopMatrix();
}


// vim: sw=4 ts=8 et ic ai nows ru:
