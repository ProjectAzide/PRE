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

// Viewer.cpp
// mike weiblen 2014-12-23
// abstract base class for all Viewers; provides a factory().
// This implementation uses a "direct to framebuffer" rendering for camera
// (rather than a 2-stage "first to FBO, then composite" rendering)

#include <assert.h>

#include "Viewer.h"             // our header
#include "Scene.h"
#include "ConfigFile.h"
#include "Utils.h"

#include "SingleViewer.h"       // derived class for factory()
#include "TableViewer.h"        // derived class for factory()
#include "LfplatepViewer.h"     // derived class for factory()


Viewer::Viewer()
{
    _isStencilTest = true;
    _pixelSize = 0;
    _halfWidth = 0;
    _halfHeight = 0;
    _stencilFunc = GL_EQUAL;
}


Viewer::~Viewer()
{
}


Viewer* Viewer::factory( Type type )
{
    Viewer* viewer = 0;
    switch( type )
    {
        case VT_SINGLE:         viewer = new SingleViewer;   break;
        case VT_TABLE:          viewer = new TableViewer;    break;
        case VT_LFPLATEP:       viewer = new LfplatepViewer; break;
        default:                printErr( "invalid Viewer::Type\n" ); break;
    }
    assert( viewer != 0 );
    return viewer;
}


Viewer::Type Viewer::getType( ConfigFile& config )
{
    int type = VT_INVALID;
    config.getInteger( NULL, "viewerType", type, false );       // mandatory
    return static_cast<Type>(type);
}


const char* Viewer::getTypeName( Viewer::Type type )
{
    switch( type )
    {
#define CASE(x)  case x : return #x;
        CASE(VT_INVALID);
        CASE(VT_SINGLE);
        CASE(VT_TABLE);
        CASE(VT_LFPLATEP);
#undef CASE
    }
    return "UNKNOWN";
}


void Viewer::setWindowSize( int width, int height )
{
    _window._x = 0;
    _window._y = 0;
    _window._width = width;
    _window._height = height;
}


bool Viewer::update( const App& app, ConfigFile& config )
{
    config.getNumber( "viewer", "pixelSize", _pixelSize, false );       // mandatory
    assert( _pixelSize > 0 );

    // Bounds of Viewer in Display space [mm]
    _halfWidth  = _window._width  * _pixelSize / 2;
    _halfHeight = _window._height * _pixelSize / 2;

    Camera::update( app, config );

    generateStencil();

    return true;
}


void Viewer::frame( Scene& scene )
{
    // TODO move this out due to generateStencil(); put in App::draw()?
    if( _isStencilTest )
        glEnable( GL_STENCIL_TEST );
    else
        glDisable( GL_STENCIL_TEST );

    glDisable( GL_SCISSOR_TEST );
    scene.preFrame();

    // **********************************************************************
    // NUGGET: When we have actual ORE (Objective Rendering Engine) hardware,
    // we'll dispatch this entire CameraArray atomically to the hardware
    // for parallel rendering, rather than iterating over and dispatching
    // Cameras individually.  The ORE can leverage Adreno's tiled rendering,
    // and will manage per-Camera triangle visibility culling to minimize draws.
    // **********************************************************************

    const int numCameras = getNumCameras();
    for( int i = 0; i < numCameras; ++i )
    {
        const Camera& cam = getCamera(i);
        const StencilId stId = cam.getStencilId();
        glStencilFunc( _stencilFunc, stId, STID_MASK );
        cam.draw( scene );
    }

    scene.postFrame();
}


Camera& Viewer::addCamera()
{
    _cameras.push_back( Camera() );
    return _cameras.back();
}


void Viewer::getDisplayBounds( double& minX, double& minY, double& maxX, double& maxY ) const
{
    // FUTURE transform Viewer bounds (_half*) using entire Viewer pose matrix (_pose)
    double px, py;              // location of display origin (ie center)
    _pose.getXY( px, py );      // [mm] TEMPORARY HACK

    minX = px - _halfWidth;
    minY = py - _halfHeight;
    maxX = px + _halfWidth;
    maxY = py + _halfHeight;
}


void Viewer::setPose( double x, double y )
{
    _pose.pose( x, y, 0 );
}


void Viewer::setViewport( Camera& cam, double x, double y ) const
{
    // TODO query the Camera to get its pose

    const double diam = cam.getDiameter();      // [mm/Camera]

    x += _halfWidth;    // translate from Display space to Viewer origin
    y += _halfHeight;

    x -= diam/2;        // adjust for viewport origin at lower-left
    y -= diam/2;

    double px, py;              // location of display origin (ie center)
    _pose.getXY( px, py );      // [mm] TEMPORARY HACK
    x -= px;            // adjust for Viewer's pose
    y -= py;

    Rect vp;                                    // [pixels]
    vp._x = static_cast<int>(x / _pixelSize + 0.5);
    vp._y = static_cast<int>(y / _pixelSize + 0.5);
    vp._width  = static_cast<int>(diam / _pixelSize + 0.5);
    vp._height = static_cast<int>(diam / _pixelSize + 0.5);

    cam.setViewport( vp );
}


void Viewer::setViewportToFullWindow( Camera& cam ) const
{
    cam.setViewport( _window );
}


void Viewer::generateStencil()
{
    {   // sanity check the stencil buffer configuration
        GLint numBits = 0;
        glGetIntegerv( GL_STENCIL_BITS, &numBits );
        assert( numBits >= STID_MIN_BITS );
    }

    glEnable( GL_STENCIL_TEST );

    // prepare to _create_ the stencil buffer
    _stencilFunc = GL_ALWAYS;
    glStencilOp( GL_REPLACE, GL_REPLACE, GL_REPLACE );
    glStencilMask( STID_MASK );

    // render to stencil buffer
    glClearStencil( STID_CLEAR );
    glClear( GL_STENCIL_BUFFER_BIT );
    Scene* sten = Scene::factory( Scene::ST_STENCIL );
    frame( *sten );
    delete sten;

    // prepare to _use_ the stencil buffer
    _stencilFunc = GL_EQUAL;
    glStencilOp( GL_KEEP, GL_KEEP, GL_KEEP );
    glStencilMask( 0 );
}


void Viewer::toggleStencil()
{
    _isStencilTest = !_isStencilTest;
}

// vim: sw=4 ts=8 et ic ai nows ru:
