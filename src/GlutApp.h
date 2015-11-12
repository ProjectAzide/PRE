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

// GlutApp.h
// mike weiblen 2015-01-13
// An application implementation based on GLUT (the OpenGL Utility Toolkit).

#ifndef GLUTAPP_H
#define GLUTAPP_H

#include "App.h"                // our baseclass


class GlutApp : public App
{
public:
    friend App* App::factory();
    virtual ~GlutApp();

    virtual void doMainLoopOnce();

    // for TeapotScene
    static void wireTeapot( double scale );
    static void solidTeapot( double scale );

protected:
    GlutApp();                  // must use App::factory()

    virtual void printInfo();
    virtual void registerLuaFunctions();
    virtual bool createGlSurface();
    virtual double getElapsedTime() const;      // [seconds]

private:
    void toggleCursor();

    int _prevCursor;

    // static wrappers for GLUT callbacks
    static GlutApp* s_instance;
    static void s_displayFunc();
    static void s_reshapeFunc( int width, int height );
    static void s_keyFunc( unsigned char key, int x, int y );

private:        // disallowed
    GlutApp( const GlutApp& );
    GlutApp& operator=( const GlutApp& );
};

#endif

// vim: sw=4 ts=8 et ic ai nows ru:
