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

// GlutApp.cpp
// mike weiblen 2015-01-13
// An application implementation based on GLUT (the OpenGL Utility Toolkit).

// FreeGLUT will #include <GL/gl.h>; on MSVC, will also #include <windows.h>
#define FREEGLUT_STATIC 1       // build for static linkage
#define FREEGLUT_LIB_PRAGMAS 0  // prevent hardcoded library names
#include "GL/freeglut.h"

#include <assert.h>
#include <stdio.h>

#include "GlutApp.h"            // our header
#include "ConfigFile.h"
#include "Viewer.h"
#include "Scene.h"


GlutApp::GlutApp()
{
    assert( s_instance == 0 );
    s_instance = this;

    _prevCursor = GLUT_CURSOR_NONE;
}


GlutApp::~GlutApp()
{
    s_instance = 0;
}


bool GlutApp::createGlSurface()
{
    bool fullScreen = true;
    bool hideCursor = true;
    std::string displayMode = "";
    std::string windowTitle = getInstanceName();

    getConfig().getBoolean( "glutWindow", "fullScreen", fullScreen, true );     // optional
    getConfig().getBoolean( "glutWindow", "hideCursor", hideCursor, true );     // optional
    getConfig().getString( "glutWindow", "displayMode", displayMode, true );    // optional
    getConfig().getString( "glutWindow", "title", windowTitle, true );          // optional

    printf( "glutWindow.displayMode\t\"%s\"\n", displayMode.c_str() );

    // trick GLUT into not consuming any args
    int fake_argc = 1;
    char argv0[] = "argv0";
    char* fake_argv[] = { argv0, 0 };
    glutInit( &fake_argc, fake_argv );

    glutInitDisplayString( displayMode.c_str() );
    //glutInitWindowPosition( 0, 0 );
    //glutInitWindowSize( 1200, 800 );

    glutCreateWindow( windowTitle.c_str() );

    glutDisplayFunc( s_displayFunc );
    glutReshapeFunc( s_reshapeFunc );
    glutKeyboardFunc( s_keyFunc );
    glutIdleFunc( glutPostRedisplay );

    if( fullScreen )  glutFullScreenToggle();
    if( hideCursor )  toggleCursor();
    return true;
}


void GlutApp::printInfo()
{
    App::printInfo();

#define PRINT(x) printf( "%s\t%d\n", #x, glutGet(x) );
    PRINT( GLUT_VERSION );
    PRINT( GLUT_WINDOW_X );
    PRINT( GLUT_WINDOW_Y );
    PRINT( GLUT_WINDOW_WIDTH );
    PRINT( GLUT_WINDOW_HEIGHT );
    PRINT( GLUT_WINDOW_CURSOR );
    PRINT( GLUT_SCREEN_WIDTH );
    PRINT( GLUT_SCREEN_HEIGHT );
    PRINT( GLUT_SCREEN_WIDTH_MM );
    PRINT( GLUT_SCREEN_HEIGHT_MM );
    PRINT( GLUT_INIT_DISPLAY_MODE );
    PRINT( GLUT_DISPLAY_MODE_POSSIBLE );
#undef PRINT
}


void GlutApp::registerLuaFunctions()
{
    App::registerLuaFunctions();
}


void GlutApp::doMainLoopOnce()
{
    // do one iteration of the main loop
    glutMainLoopEvent();
}


double GlutApp::getElapsedTime() const
{
    double msec = static_cast<double>( glutGet(GLUT_ELAPSED_TIME) );
    return ( msec / 1000.0 );           // [seconds]
}


void GlutApp::toggleCursor()
{
    const int currCursor = glutGet( GLUT_WINDOW_CURSOR );
    glutSetCursor( _prevCursor );
    _prevCursor = currCursor;
}


void GlutApp::wireTeapot( double scale )
{
    glutWireTeapot( scale );
}


void GlutApp::solidTeapot( double scale )
{
    glutSolidTeapot( scale );
}

/////////////////////////////////////////////////////////////////////////////
// static wrappers for GLUT callbacks

GlutApp* GlutApp::s_instance = 0;


void GlutApp::s_displayFunc()
{
    assert( s_instance != 0 );
    s_instance->draw();
    glutSwapBuffers();
    glutPostRedisplay();
}


void GlutApp::s_reshapeFunc( int width, int height )
{
    assert( s_instance != 0 );
    s_instance->getViewer().setWindowSize( width, height );
    s_instance->setNeedUpdate();
}


void GlutApp::s_keyFunc( unsigned char key, int x, int y )
{
    assert( s_instance != 0 );
    switch( key )
    {
        case 27:   s_instance->exitMainLoop(); return;
        case 'f':  glutFullScreenToggle(); return;
        case 'c':  s_instance->toggleCursor(); return;
        case 's':  s_instance->getViewer().toggleStencil(); return;
        case 'u':  s_instance->setNeedUpdate(); return;
        case 'p':
            s_instance->printStats();
            s_instance->resetStats();
            return;
    }

    //if( s_instance->getViewer().keyFunc( key, x, y ) )   return;
    if( s_instance->getScene().keyFunc( key, x, y ) )   return;
}

// vim: sw=4 ts=8 et ic ai nows ru:
