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

// main.cpp
// mike weiblen 2015-01-13
// This is it: the main() function.

#include <stdio.h>
#include <stdlib.h>             // exit()

#include "App.h"                // abstract App baseclass

#include "GIT_DESCRIBE.h"       // generated by MAKE_GIT_DESCRIBE.lua
#ifndef GIT_DESCRIBE
#define GIT_DESCRIBE ""
#endif


int main( int argc, char* argv[] )
{
    printf( "argc\t%d\n", argc );
    for( int i = 0; i < argc; ++i )
    {
        printf( "argv[%d]\t\"%s\"\n", i, argv[i] );
    }

    printf( "APP_NAME\t\"%s\"\n", APP_NAME );
    printf( "BUILD_TIMESTAMP\t\"%s %s\"\n", __DATE__, __TIME__ );
    printf( "GIT_DESCRIBE\t\"%s\"\n", GIT_DESCRIBE );

    int status = EXIT_FAILURE;

    {
        App* app = App::factory();
        if( app->create( argc, argv ) )
        {
            while( app->isMainLoopActive() )
            {
                app->doMainLoopOnce();
            }
            app->destroy();
            status = EXIT_SUCCESS;
        }
        delete app;
    }

    return status;
}

// vim: sw=4 ts=8 et ic ai nows ru:
