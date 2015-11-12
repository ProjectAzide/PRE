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

// GearScene.h
// mike weiblen 2014-12-16
// Scene of the spinning GL gears, circa 1999.

#ifndef GEARSCENE_H
#define GEARSCENE_H

#include "Scene.h"              // our baseclass


class GearScene : public Scene
{
public:
    friend Scene* Scene::factory( Scene::Type type );
    virtual ~GearScene();

    virtual Type getType() const { return ST_GEAR; }

    virtual void update( const App& app, ConfigFile& config );

    virtual void reset();
    virtual void animate( const App& app, const ConfigFile& config );

    virtual void preFrame();

    virtual void draw();

protected:
    GearScene();                // must use Scene::factory()

private:
    double _gearRate;           // [degrees per second]?

private:        // disallowed
    GearScene( const GearScene& );
    GearScene& operator=( const GearScene& );
};

#endif

// vim: sw=4 ts=8 et ic ai nows ru:
