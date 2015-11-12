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

// TeapotScene.h
// mike weiblen 2015-02-03
// A scene consisting of the GLUT teapot (solid or wireframe)

#ifndef TEAPOTSCENE_H
#define TEAPOTSCENE_H

#include "Scene.h"              // our baseclass


class TeapotScene : public Scene
{
public:
    friend Scene* Scene::factory( Scene::Type type );
    virtual ~TeapotScene();

    virtual Type getType() const { return ST_TEAPOT; }

    virtual void update( const App& app, ConfigFile& config );

    virtual void reset();
    virtual void animate( const App& app, const ConfigFile& config );

    virtual bool keyFunc( unsigned char key, int x, int y );

    virtual void preFrame();

    virtual void draw();

protected:
    TeapotScene();              // must use Scene::factory()

private:        // data
    bool _isWireframe;

private:        // disallowed
    TeapotScene( const TeapotScene& );
    TeapotScene& operator=( const TeapotScene& );
};

#endif

// vim: sw=4 ts=8 et ic ai nows ru:
