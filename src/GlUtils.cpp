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

// GlUtils.cpp
// mike weiblen 2015-06-16
// Helpers for cross-platform OpenGL

#include <cmath>                // std::tan
#include <stdio.h>              // printf
#include <assert.h>

#include "GlUtils.h"            // our header
#include "Utils.h"

#ifndef GL_INVALID_FRAMEBUFFER_OPERATION
#define GL_INVALID_FRAMEBUFFER_OPERATION        0x0506
#endif


/////////////////////////////////////////////////////////////////////////////

int checkGlError( const char* file, int line, const char* msg )
{
    GLenum err = GL_NO_ERROR;
    int errCount = 0;

    while( (err = glGetError()) != GL_NO_ERROR )
    {
        const char* str = "UNKNOWN";
        switch( err )
        {
#define CASE(x)  case x : str = #x; break;
            CASE(GL_INVALID_ENUM);
            CASE(GL_INVALID_VALUE);
            CASE(GL_INVALID_OPERATION);
            CASE(GL_INVALID_FRAMEBUFFER_OPERATION);
            CASE(GL_OUT_OF_MEMORY);
            CASE(GL_STACK_UNDERFLOW);
            CASE(GL_STACK_OVERFLOW);
#undef CASE
        }

        printErr( "%s : %s %s@%d\n", str, msg, file, line );
        ++errCount;

#if 0
        GLint mvDepth, projDepth;
        glGetIntegerv( GL_MODELVIEW_STACK_DEPTH, &mvDepth );
        glGetIntegerv( GL_PROJECTION_STACK_DEPTH, &projDepth );
        printErr( "\tmvDepth %d projDepth %d\n", mvDepth, projDepth );
#endif
    }
    assert( errCount == 0 );
    return errCount;
}

/////////////////////////////////////////////////////////////////////////////

GlStackCheck::GlStackCheck() : _top(depth())
{
}


GlStackCheck::~GlStackCheck()
{
    assert( ok() );
}


bool GlStackCheck::ok() const
{
    const int d = delta();
    if( d == 0 )  return true;
    printErr( "GlStackCheck top %d delta %d\n", _top, d );
    return false;
}


void GlStackCheck::check( const char* file, int line ) const
{
    printf( "GlStackCheck delta %d : %s@%d\n", delta(), file, line );
}


int GlStackCheck::depth() const
{
    GLint depth = 0;
    glGetIntegerv( GL_MODELVIEW_STACK_DEPTH, &depth );
    return depth;
}

/////////////////////////////////////////////////////////////////////////////

void Rect::glViewport() const
{
    ::glViewport( _x, _y, _width, _height );
}


void Rect::glScissor() const
{
    ::glScissor( _x, _y, _width, _height );
}

/////////////////////////////////////////////////////////////////////////////

void Matrix44::identity()
{
    for( int i = 0; i < 16; ++i ) { _m[i] = 0.0f; }
    _m[0] = _m[5] = _m[10] = _m[15] = 1.0f;
}


void Matrix44::perspective( double fovX, double fovY, double zNear, double zFar )
{
    const double zDelta = (zFar - zNear);

    float q = 0;
    if( (zDelta > 0) && (zNear > 0) )
    {
        q = 1;          // regular orthoscopic "forward perspective"
    }
    else if ( (zDelta > 0) && (zFar < 0) )
    {
        q = -1;         // pseudoscopic "reverse perspective"
    }
    else
    {
        printErr( "Matrix44::perspective zNear %f zFar %d\n", zNear, zFar );
    }
    assert( q != 0 );

    identity();
    _m[0]  = q * static_cast<float>( 1 / std::tan( DEG2RAD(fovX/2) ) );
    _m[5]  = q * static_cast<float>( 1 / std::tan( DEG2RAD(fovY/2) ) );
    _m[10] = q * static_cast<float>( -(zFar + zNear) / zDelta );
    _m[11] = q * -1;
    _m[14] = q * static_cast<float>( -2 * zFar * zNear / zDelta );
    _m[15] = 0;
}


void Matrix44::ortho( double left, double right, double bottom, double top, double zNear, double zFar )
{
    const double xDelta = right - left;
    const double yDelta = top - bottom;
    const double zDelta = zFar - zNear;

    identity();
    _m[0]  = static_cast<float>( 2 / xDelta );
    _m[5]  = static_cast<float>( 2 / yDelta );
    _m[10] = static_cast<float>( -2 / zDelta );
    _m[12] = static_cast<float>( -(right + left) / xDelta );
    _m[13] = static_cast<float>( -(top + bottom) / yDelta );
    _m[14] = static_cast<float>( -(zFar + zNear) / zDelta );
}


void Matrix44::pose( double x, double y, double z )
{
    identity();
    _m[12] = -static_cast<float>(x);
    _m[13] = -static_cast<float>(y);
    _m[14] = -static_cast<float>(z);
}


void Matrix44::pose( double x, double y, double z, const Quat& orientation )
{
    pose( x, y, z );
    assert(0);                  // FUTURE: not yet supported
    // TODO do something w Quat
}


void Matrix44::glLoadMatrix() const
{
    ::glLoadMatrixf( _m );
}

/////////////////////////////////////////////////////////////////////////////

void Projection::perspective( double fov, double zNear, double zFar )
{
    _fov = fov;
    _zNear = zNear;
    _zFar = zFar;

    _matrix.perspective( _fov, _fov, _zNear, _zFar );
}


void Projection::ortho( double diameter, double zNear, double zFar )
{
    _fov = 0.0;
    _zNear = zNear;
    _zFar = zFar;

    const double radius = diameter / 2.0;
    _matrix.ortho( -radius, radius, -radius, radius, _zNear, _zFar );
}


void Projection::depthRange( double dNear, double dFar )
{
    _dNear = dNear;
    _dFar = dFar;
}


void Projection::apply() const
{
    glMatrixMode( GL_PROJECTION );
    _matrix.glLoadMatrix();
    glMatrixMode( GL_MODELVIEW );

    glDepthRange( _dNear, _dFar );
}


// vim: sw=4 ts=8 et ic ai nows ru:
