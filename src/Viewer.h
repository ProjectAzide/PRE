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

// Viewer.h
// mike weiblen 2014-12-16
// abstract base class for all Viewers; provides a factory().

#ifndef VIEWER_H
#define VIEWER_H

#include <vector>

#include "Camera.h"
#include "GlUtils.h"

class App;
class Scene;
class ConfigFile;


class Viewer
{
public:
    enum Type
    {
        VT_INVALID  = 0,
        VT_SINGLE   = 1,
        VT_TABLE    = 3,
        VT_LFPLATEP = 5,
    };

    static Viewer* factory( Type type );
    virtual ~Viewer();

    static Type getType( ConfigFile& config );
    static const char* getTypeName( Type type );
    static bool isTypeValid( Type type ) { return type != VT_INVALID; }

    virtual Type getType() const { return VT_INVALID; }

    virtual bool update( const App& app, ConfigFile& config );

    void frame( Scene& scene );

    int getNumCameras() const { return _cameras.size(); }

    void toggleStencil();

    void setWindowSize( int width, int height );

protected:      // methods
    Viewer();

    void clearCameras() { _cameras.clear(); }
    Camera& addCamera();
    Camera& getCamera( int i ) { return _cameras[i]; }

    // get this display's bounds in Display space [mm]
    void getDisplayBounds( double& minX, double& minY, double& maxX, double& maxY ) const;

    // location/orientation of display's origin (ie center) in Display space [mm]
    void setPose( double x, double y );
    //void setPose( double x, double y, double z );
    //void setPose( double x, double y, double z, const Quat& orientation );

    // set Camera's Viewport in window, given its origin (ie center) in Display space [mm]
    void setViewport( Camera& cam, double x, double y ) const;

    // set Camera's Viewport to occupy the entire Window
    void setViewportToFullWindow( Camera& cam ) const;

private:        // methods
    void generateStencil();

private:        // data
    std::vector<Camera> _cameras;
    bool _isStencilTest;        // should views be masked by stencil?
    Rect _window;               // [pixels] renderable area of window
    double _pixelSize;          // [mm/pixel]
    double _halfWidth;          // [mm]
    double _halfHeight;         // [mm]
    Matrix44 _pose;             // position/orientation in Display space
    GLint _stencilFunc;         // glStencilFunc test function

private:        // disallowed
    Viewer( const Viewer& );
    Viewer& operator=( const Viewer& );
};

#endif

// vim: sw=4 ts=8 et ic ai nows ru:
