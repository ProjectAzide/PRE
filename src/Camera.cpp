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

// Camera.cpp
// mike weiblen 2014-12-23
// Camera captures the pixels that constitute the displayed Hogel data.

#include <assert.h>
#include <cmath>                // std::tan

#include "Camera.h"             // our header
#include "ConfigFile.h"
#include "Scene.h"


// TODO these static params may evolve to be per-Camera, rather than shared:
static double s_diameter;               // [mm/Camera]
static double s_fov;                    // [degrees]

// TODO these static params should be moved to Viewer:
static double s_zNear = -1000.0;        // [mm]
static double s_zFar  =  1000.0;        // [mm]

static Projection s_farProj;
static Projection s_epsilonProj;
static Projection s_nearProj;

static bool s_farIsVisible;
static bool s_epsilonIsVisible;
static bool s_nearIsVisible;


Camera::Camera() : _stencilId(STID_0)
{
}


Camera::~Camera()
{
}


bool Camera::update( const App& app, ConfigFile& config )
{
    config.getNumber( "camera", "diameter", s_diameter, false );        // mandatory
    assert( s_diameter > 0 );

    config.getNumber( "camera", "fov", s_fov, false );                  // mandatory
    assert( s_fov > 0 );

    // TODO move zNear/zFar to Viewer
    //config.getNumber( "camera", "zNear", s_zNear, true );               // optional
    //config.getNumber( "camera", "zFar", s_zFar, true );                 // optional
    assert( s_zNear < s_zFar );


    const double zFarEpsilon = (s_diameter/2.0) / std::tan( DEG2RAD(s_fov/2.0) );
    const double zNearEpsilon = -zFarEpsilon;

    const double DNEAR = 0.0;
    const double DFAR = 1.0;
    const double dNearEpsilon = 0.5 - 0.05;   // TODO compute from zNear & zFar
    const double dFarEpsilon = 0.5 + 0.05;

    s_farProj.perspective( s_fov, zFarEpsilon, s_zFar );
    s_farProj.depthRange( dFarEpsilon, DFAR );
    s_farIsVisible = (zFarEpsilon < s_zFar);

    s_nearProj.perspective( s_fov, s_zNear, zNearEpsilon );
    s_nearProj.depthRange( DNEAR, dNearEpsilon );
    s_nearIsVisible = (s_zNear < zNearEpsilon);

    s_epsilonProj.ortho( s_diameter, zNearEpsilon, zFarEpsilon );
    s_epsilonProj.depthRange( dNearEpsilon, dFarEpsilon );
    s_epsilonIsVisible = ((s_zNear < zFarEpsilon) && (zNearEpsilon < s_zFar));

    return true;
}


void Camera::draw( Scene& scene ) const
{
    //GlStackCheck glsc;

    // ***********************************************************************
    // NUGGET: If ORE is embedded within the photonics/optics package, each
    // pose and viewport can be calibrated once at manufacturing time, then
    // be permanently flashed into ROM as a static lookup table.
    // ***********************************************************************

    _viewport.glViewport();
    _viewport.glScissor();
    glEnable( GL_SCISSOR_TEST );

    glMatrixMode( GL_MODELVIEW );
    glPushMatrix();
    _pose.glLoadMatrix();       // Camera's pose is the 1st transformation

    // TODO put Manipulator's transformations here

    // we want to minimize the number of draw() required for a single view,
    // so only invoke a ProjVolume's draw() if there's unanimous agreement.

    // ***********************************************************************
    // NUGGET: ORE (Objective Rendering Engine) hardware could implement
    // the Pinhole Projection, which would allow these three draw passes to
    // be condensed into one.
    // ***********************************************************************

    if( s_farIsVisible && scene.isVisible(PV_FAR) )
    {
        s_farProj.apply();
        scene.setGlCullFace();
        scene.draw();
    }

    if( s_epsilonIsVisible && scene.isVisible(PV_EPSILON) )
    {
        s_epsilonProj.apply();
        glDisable( GL_CULL_FACE );
        scene.draw();
    }

    if( s_nearIsVisible && scene.isVisible(PV_NEAR) )
    {
        s_nearProj.apply();
        scene.setGlCullFace();
        scene.draw();
    }

    glPopMatrix();
}


double Camera::getDiameter() const
{
    return s_diameter;
}


void Camera::setPose( double x, double y )
{
    _pose.pose( x, y, 0 );
}

// vim: sw=4 ts=8 et ic ai nows ru:
