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

// GlUtils.h
// mike weiblen 2015-06-16
// Helpers for cross-platform OpenGL

#ifndef GLUTILS_H
#define GLUTILS_H

#if LFD_PLATFORM_MSVC
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN     // exclude some header bloat
#endif
#include <windows.h>            // required before GL/gl.h on Windows
#endif

#include <GL/gl.h>

#define  DEG2RAD(deg) ((deg) * 3.1415927 / 180.0)

/////////////////////////////////////////////////////////////////////////////

class Quat;             // TODO

/////////////////////////////////////////////////////////////////////////////

enum StencilId
{
    STID_MIN_BITS = 2,          // required minimum number of stencil buffer bits
    STID_CLEAR    = 0x00,       // glClearStencil value
    STID_0        = 0x01,
    STID_1        = 0x02,
    STID_2        = 0x03,
    STID_MASK     = (STID_0 | STID_1 | STID_2)
};

/////////////////////////////////////////////////////////////////////////////

int checkGlError( const char* file, int line, const char* msg );

#define CHECK_GLERROR(msg) checkGlError(__FILE__,__LINE__,msg)
//#define CHECK_GLERROR(msg)

/////////////////////////////////////////////////////////////////////////////

class GlStackCheck
{
public:
    GlStackCheck();
    ~GlStackCheck();

    int delta() const { return (depth() - _top); }

    bool ok() const;
    void check( const char* file, int line ) const;

private:
    int depth() const;

private:
    int _top;
};

//#define CHECK_GLSTACK(x) (x).check(__FILE__,__LINE__)
#define CHECK_GLSTACK(x)

/////////////////////////////////////////////////////////////////////////////

class Rect
{
public:
    Rect() {}
    ~Rect() {}

    void glViewport() const;
    void glScissor() const;

public:
    int _x, _y, _width, _height;        // [pixels]
};

/////////////////////////////////////////////////////////////////////////////

class Matrix44
{
public:
    Matrix44() { identity(); }
    ~Matrix44() {}

    void identity();

    void perspective( double fovX, double fovY, double zNear, double zFar );
    void ortho( double left, double right, double bottom, double top, double zNear, double zFar );

    // Pose is defined as being in Display (physical) space [mm].
    void pose( double x, double y, double z );
    void pose( double x, double y, double z, const Quat& orientation );

    void glLoadMatrix() const;

    // TEMPORARY HACK for Viewer::getDisplayBounds
    void getXY( double& x, double& y ) const { x = -_m[12]; y = -_m[13]; }

private:
    float _m[16];
};

/////////////////////////////////////////////////////////////////////////////

class Projection
{
public:
    Projection() {}
    ~Projection() {}

    void perspective( double fov, double zNear, double zFar );
    void ortho( double diameter, double zNear, double zFar );

    void depthRange( double dNear, double dFar );

    void apply() const;

private:
    double _fov;        // [degrees]  0 == ortho

    double _zNear;      // [mm]
    double _zFar;       // [mm]

    double _dNear;      // [0,1] for glDepthRange()
    double _dFar;       // [0,1]

    Matrix44 _matrix;

private:        // disallowed
    Projection( const Projection& );
    Projection& operator=( const Projection& );
};

#endif

// vim: sw=4 ts=8 et ic ai nows ru:
