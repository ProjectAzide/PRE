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

// Scene.h
// mike weiblen 2014-12-16
// abstract base class for all Scenes; provides a factory().

#ifndef SCENE_H
#define SCENE_H

#include "Camera.h"             // enum Camera::ProjVolume

class ConfigFile;
class App;


class Scene
{
public:
    enum Type
    {
        ST_INVALID = 0,
        ST_GEAR    = 1,
        ST_TEAPOT  = 2,
        ST_CUBE    = 3,
        ST_STENCIL = 9999
    };

    static Scene* factory( Type type );
    virtual ~Scene();

    static Type getType( ConfigFile& config );
    static const char* getTypeName( Type type );
    static bool isTypeValid( Type type ) { return type != ST_INVALID; }

    virtual Type getType() const { return ST_INVALID; }

    virtual void update( const App& app, ConfigFile& config ) {}

    virtual void reset();
    virtual void animate( const App& app, const ConfigFile& config ) {}

    virtual bool keyFunc( unsigned char key, int x, int y );

    virtual bool isVisible( Camera::ProjVolume pv ) const { return true; }

    virtual void preFrame() = 0;
    virtual void postFrame() {}

    virtual void draw() = 0;

    void setGlCullFace() const;

protected:      // methods
    Scene();

protected:      // data
    // TODO MANIPULATOR
    float _posX, _posY, _posZ;  // [DBU] position in Viewer space
    float _rotX, _rotY, _rotZ;  // [degrees] orientation in Viewer space

    double _displayScale;       // [mm/DBU]

    bool _isCullFace;           // does the scene want CULL_FACE?

private:        // disallowed
    Scene( const Scene& );
    Scene& operator=( const Scene& );
};

#endif

// vim: sw=4 ts=8 et ic ai nows ru:
