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

// Camera.h
// mike weiblen 2014-12-23
// Camera captures the pixels that constitute the displayed Hogel data.

#ifndef CAMERA_H
#define CAMERA_H

#include "GlUtils.h"            // Rect, Matrix44

class Scene;
class App;
class ConfigFile;


class Camera
{
public:
    enum ProjVolume             // volumes for the 3 projection transformations
    {
        PV_INVALID = 0,
        PV_FAR,                 // orthoscopic forward-perspective
        PV_EPSILON,             // orthographic, enclosing the singularity
        PV_NEAR,                // pseudoscopic reverse-perspective
    };

    Camera();
    ~Camera();

    static bool update( const App& app, ConfigFile& config );

    void draw( Scene& scene ) const;

    double getDiameter() const;                                 // [mm/Camera]

    void setPose( double x, double y );                         // [mm]
    //void setPose( double x, double y, double z );
    //void setPose( double x, double y, double z, const Quat& orientation );

    void setViewport( const Rect& rect ) { _viewport = rect; }  // [pixels]

    void setStencilId( StencilId stId ) { _stencilId = stId; }
    StencilId getStencilId() const { return _stencilId; }


private:        // static data
    Rect _viewport;             // viewport in window space [pixels]
    Matrix44 _pose;             // position/orientation in Display space
    StencilId _stencilId;       // stencil through which to draw
};

#endif

// vim: sw=4 ts=8 et ic ai nows ru:
