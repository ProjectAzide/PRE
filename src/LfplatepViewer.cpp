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

// LfplatepViewer.cpp
// mike weiblen 2015-06-09
// Loads an explicit list of Cameras from a configuration file.

#include <stdio.h>

#include "LfplatepViewer.h"     // our header
#include "Camera.h"


// hardcoded camera position table //////////////////////////////////////////
// TODO load this table from a config file

struct CamPos
{
    double x;
    double y;
    StencilId stencil;
};

const CamPos camPos[] =
{
    // Xpos     Ypos    StencilId
    { 8.34131, 12.6095, STID_0 },
    { 15.9733, 12.6095, STID_1 },
    { 23.6054, 12.6095, STID_2 },
    { 31.2374, 12.6095, STID_0 },
    { 38.8694, 12.6095, STID_1 },
    { 46.5014, 12.6095, STID_2 },
    { 54.1334, 12.6095, STID_0 },
    { 61.7655, 12.6095, STID_1 },
    { 69.3975, 12.6095, STID_2 },
    { 77.0295, 12.6095, STID_0 },
    { 84.6615, 12.6095, STID_1 },
    { 92.2936, 12.6095, STID_2 },
    { 99.9256, 12.6095, STID_0 },
    { 107.558, 12.6095, STID_1 },
    { 115.190, 12.6095, STID_2 },
    { 122.822, 12.6095, STID_0 },
    { 130.454, 12.6095, STID_1 },
    { 138.086, 12.6095, STID_2 },
    { 145.718, 12.6095, STID_0 },
    { 153.350, 12.6095, STID_1 },

    { 12.1573, 19.2191, STID_2 },
    { 19.7893, 19.2191, STID_0 },
    { 27.4214, 19.2191, STID_1 },
    { 35.0534, 19.2191, STID_2 },
    { 42.6854, 19.2191, STID_0 },
    { 50.3174, 19.2191, STID_1 },
    { 57.9495, 19.2191, STID_2 },
    { 65.5815, 19.2191, STID_0 },
    { 73.2135, 19.2191, STID_1 },
    { 80.8455, 19.2191, STID_2 },
    { 88.4776, 19.2191, STID_0 },
    { 96.1096, 19.2191, STID_1 },
    { 103.742, 19.2191, STID_2 },
    { 111.374, 19.2191, STID_0 },
    { 119.006, 19.2191, STID_1 },
    { 126.638, 19.2191, STID_2 },
    { 134.270, 19.2191, STID_0 },
    { 141.902, 19.2191, STID_1 },
    { 149.534, 19.2191, STID_2 },
    { 157.166, 19.2191, STID_0 },

    { 8.34131, 25.8286, STID_0 },
    { 15.9733, 25.8286, STID_1 },
    { 23.6054, 25.8286, STID_2 },
    { 31.2374, 25.8286, STID_0 },
    { 38.8694, 25.8286, STID_1 },
    { 46.5014, 25.8286, STID_2 },
    { 54.1334, 25.8286, STID_0 },
    { 61.7655, 25.8286, STID_1 },
    { 69.3975, 25.8286, STID_2 },
    { 77.0295, 25.8286, STID_0 },
    { 84.6615, 25.8286, STID_1 },
    { 92.2936, 25.8286, STID_2 },
    { 99.9256, 25.8286, STID_0 },
    { 107.558, 25.8286, STID_1 },
    { 115.190, 25.8286, STID_2 },
    { 122.822, 25.8286, STID_0 },
    { 130.454, 25.8286, STID_1 },
    { 138.086, 25.8286, STID_2 },
    { 145.718, 25.8286, STID_0 },
    { 153.350, 25.8286, STID_1 },

    { 12.1573, 32.4381, STID_2 },
    { 19.7893, 32.4381, STID_0 },
    { 27.4214, 32.4381, STID_1 },
    { 35.0534, 32.4381, STID_2 },
    { 42.6854, 32.4381, STID_0 },
    { 50.3174, 32.4381, STID_1 },
    { 57.9495, 32.4381, STID_2 },
    { 65.5815, 32.4381, STID_0 },
    { 73.2135, 32.4381, STID_1 },
    { 80.8455, 32.4381, STID_2 },
    { 88.4776, 32.4381, STID_0 },
    { 96.1096, 32.4381, STID_1 },
    { 103.742, 32.4381, STID_2 },
    { 111.374, 32.4381, STID_0 },
    { 119.006, 32.4381, STID_1 },
    { 126.638, 32.4381, STID_2 },
    { 134.270, 32.4381, STID_0 },
    { 141.902, 32.4381, STID_1 },
    { 149.534, 32.4381, STID_2 },
    { 157.166, 32.4381, STID_0 },

    { 8.34131, 39.0476, STID_0 },
    { 15.9733, 39.0476, STID_1 },
    { 23.6054, 39.0476, STID_2 },
    { 31.2374, 39.0476, STID_0 },
    { 38.8694, 39.0476, STID_1 },
    { 46.5014, 39.0476, STID_2 },
    { 54.1334, 39.0476, STID_0 },
    { 61.7655, 39.0476, STID_1 },
    { 69.3975, 39.0476, STID_2 },
    { 77.0295, 39.0476, STID_0 },
    { 84.6615, 39.0476, STID_1 },
    { 92.2936, 39.0476, STID_2 },
    { 99.9256, 39.0476, STID_0 },
    { 107.558, 39.0476, STID_1 },
    { 115.190, 39.0476, STID_2 },
    { 122.822, 39.0476, STID_0 },
    { 130.454, 39.0476, STID_1 },
    { 138.086, 39.0476, STID_2 },
    { 145.718, 39.0476, STID_0 },
    { 153.350, 39.0476, STID_1 },

    { 12.1573, 45.6572, STID_2 },
    { 19.7893, 45.6572, STID_0 },
    { 27.4214, 45.6572, STID_1 },
    { 35.0534, 45.6572, STID_2 },
    { 42.6854, 45.6572, STID_0 },
    { 50.3174, 45.6572, STID_1 },
    { 57.9495, 45.6572, STID_2 },
    { 65.5815, 45.6572, STID_0 },
    { 73.2135, 45.6572, STID_1 },
    { 80.8455, 45.6572, STID_2 },
    { 88.4776, 45.6572, STID_0 },
    { 96.1096, 45.6572, STID_1 },
    { 103.742, 45.6572, STID_2 },
    { 111.374, 45.6572, STID_0 },
    { 119.006, 45.6572, STID_1 },
    { 126.638, 45.6572, STID_2 },
    { 134.270, 45.6572, STID_0 },
    { 141.902, 45.6572, STID_1 },
    { 149.534, 45.6572, STID_2 },
    { 157.166, 45.6572, STID_0 },

    { 8.34131, 52.2667, STID_0 },
    { 15.9733, 52.2667, STID_1 },
    { 23.6054, 52.2667, STID_2 },
    { 31.2374, 52.2667, STID_0 },
    { 38.8694, 52.2667, STID_1 },
    { 46.5014, 52.2667, STID_2 },
    { 54.1334, 52.2667, STID_0 },
    { 61.7655, 52.2667, STID_1 },
    { 69.3975, 52.2667, STID_2 },
    { 77.0295, 52.2667, STID_0 },
    { 84.6615, 52.2667, STID_1 },
    { 92.2936, 52.2667, STID_2 },
    { 99.9256, 52.2667, STID_0 },
    { 107.558, 52.2667, STID_1 },
    { 115.190, 52.2667, STID_2 },
    { 122.822, 52.2667, STID_0 },
    { 130.454, 52.2667, STID_1 },
    { 138.086, 52.2667, STID_2 },
    { 145.718, 52.2667, STID_0 },
    { 153.350, 52.2667, STID_1 },

    { 12.1573, 58.8762, STID_2 },
    { 19.7893, 58.8762, STID_0 },
    { 27.4214, 58.8762, STID_1 },
    { 35.0534, 58.8762, STID_2 },
    { 42.6854, 58.8762, STID_0 },
    { 50.3174, 58.8762, STID_1 },
    { 57.9495, 58.8762, STID_2 },
    { 65.5815, 58.8762, STID_0 },
    { 73.2135, 58.8762, STID_1 },
    { 80.8455, 58.8762, STID_2 },
    { 88.4776, 58.8762, STID_0 },
    { 96.1096, 58.8762, STID_1 },
    { 103.742, 58.8762, STID_2 },
    { 111.374, 58.8762, STID_0 },
    { 119.006, 58.8762, STID_1 },
    { 126.638, 58.8762, STID_2 },
    { 134.270, 58.8762, STID_0 },
    { 141.902, 58.8762, STID_1 },
    { 149.534, 58.8762, STID_2 },
    { 157.166, 58.8762, STID_0 },

    { 8.34131, 65.4858, STID_0 },
    { 15.9733, 65.4858, STID_1 },
    { 23.6054, 65.4858, STID_2 },
    { 31.2374, 65.4858, STID_0 },
    { 38.8694, 65.4858, STID_1 },
    { 46.5014, 65.4858, STID_2 },
    { 54.1334, 65.4858, STID_0 },
    { 61.7655, 65.4858, STID_1 },
    { 69.3975, 65.4858, STID_2 },
    { 77.0295, 65.4858, STID_0 },
    { 84.6615, 65.4858, STID_1 },
    { 92.2936, 65.4858, STID_2 },
    { 99.9256, 65.4858, STID_0 },
    { 107.558, 65.4858, STID_1 },
    { 115.190, 65.4858, STID_2 },
    { 122.822, 65.4858, STID_0 },
    { 130.454, 65.4858, STID_1 },
    { 138.086, 65.4858, STID_2 },
    { 145.718, 65.4858, STID_0 },
    { 153.350, 65.4858, STID_1 },

    { 12.1573, 72.0953, STID_2 },
    { 19.7893, 72.0953, STID_0 },
    { 27.4214, 72.0953, STID_1 },
    { 35.0534, 72.0953, STID_2 },
    { 42.6854, 72.0953, STID_0 },
    { 50.3174, 72.0953, STID_1 },
    { 57.9495, 72.0953, STID_2 },
    { 65.5815, 72.0953, STID_0 },
    { 73.2135, 72.0953, STID_1 },
    { 80.8455, 72.0953, STID_2 },
    { 88.4776, 72.0953, STID_0 },
    { 96.1096, 72.0953, STID_1 },
    { 103.742, 72.0953, STID_2 },
    { 111.374, 72.0953, STID_0 },
    { 119.006, 72.0953, STID_1 },
    { 126.638, 72.0953, STID_2 },
    { 134.270, 72.0953, STID_0 },
    { 141.902, 72.0953, STID_1 },
    { 149.534, 72.0953, STID_2 },
    { 157.166, 72.0953, STID_0 },

    { 8.34131, 78.7048, STID_0 },
    { 15.9733, 78.7048, STID_1 },
    { 23.6054, 78.7048, STID_2 },
    { 31.2374, 78.7048, STID_0 },
    { 38.8694, 78.7048, STID_1 },
    { 46.5014, 78.7048, STID_2 },
    { 54.1334, 78.7048, STID_0 },
    { 61.7655, 78.7048, STID_1 },
    { 69.3975, 78.7048, STID_2 },
    { 77.0295, 78.7048, STID_0 },
    { 84.6615, 78.7048, STID_1 },
    { 92.2936, 78.7048, STID_2 },
    { 99.9256, 78.7048, STID_0 },
    { 107.558, 78.7048, STID_1 },
    { 115.190, 78.7048, STID_2 },
    { 122.822, 78.7048, STID_0 },
    { 130.454, 78.7048, STID_1 },
    { 138.086, 78.7048, STID_2 },
    { 145.718, 78.7048, STID_0 },
    { 153.350, 78.7048, STID_1 },

    { 12.1573, 85.3144, STID_2 },
    { 19.7893, 85.3144, STID_0 },
    { 27.4214, 85.3144, STID_1 },
    { 35.0534, 85.3144, STID_2 },
    { 42.6854, 85.3144, STID_0 },
    { 50.3174, 85.3144, STID_1 },
    { 57.9495, 85.3144, STID_2 },
    { 65.5815, 85.3144, STID_0 },
    { 73.2135, 85.3144, STID_1 },
    { 80.8455, 85.3144, STID_2 },
    { 88.4776, 85.3144, STID_0 },
    { 96.1096, 85.3144, STID_1 },
    { 103.742, 85.3144, STID_2 },
    { 111.374, 85.3144, STID_0 },
    { 119.006, 85.3144, STID_1 },
    { 126.638, 85.3144, STID_2 },
    { 134.270, 85.3144, STID_0 },
    { 141.902, 85.3144, STID_1 },
    { 149.534, 85.3144, STID_2 },
    { 157.166, 85.3144, STID_0 },

    { 8.34131, 91.9239, STID_0 },
    { 15.9733, 91.9239, STID_1 },
    { 23.6054, 91.9239, STID_2 },
    { 31.2374, 91.9239, STID_0 },
    { 38.8694, 91.9239, STID_1 },
    { 46.5014, 91.9239, STID_2 },
    { 54.1334, 91.9239, STID_0 },
    { 61.7655, 91.9239, STID_1 },
    { 69.3975, 91.9239, STID_2 },
    { 77.0295, 91.9239, STID_0 },
    { 84.6615, 91.9239, STID_1 },
    { 92.2936, 91.9239, STID_2 },
    { 99.9256, 91.9239, STID_0 },
    { 107.558, 91.9239, STID_1 },
    { 115.190, 91.9239, STID_2 },
    { 122.822, 91.9239, STID_0 },
    { 130.454, 91.9239, STID_1 },
    { 138.086, 91.9239, STID_2 },
    { 145.718, 91.9239, STID_0 },
    { 153.350, 91.9239, STID_1 },

    { 12.1573, 98.5334, STID_2 },
    { 19.7893, 98.5334, STID_0 },
    { 27.4214, 98.5334, STID_1 },
    { 35.0534, 98.5334, STID_2 },
    { 42.6854, 98.5334, STID_0 },
    { 50.3174, 98.5334, STID_1 },
    { 57.9495, 98.5334, STID_2 },
    { 65.5815, 98.5334, STID_0 },
    { 73.2135, 98.5334, STID_1 },
    { 80.8455, 98.5334, STID_2 },
    { 88.4776, 98.5334, STID_0 },
    { 96.1096, 98.5334, STID_1 },
    { 103.742, 98.5334, STID_2 },
    { 111.374, 98.5334, STID_0 },
    { 119.006, 98.5334, STID_1 },
    { 126.638, 98.5334, STID_2 },
    { 134.270, 98.5334, STID_0 },
    { 141.902, 98.5334, STID_1 },
    { 149.534, 98.5334, STID_2 },
    { 157.166, 98.5334, STID_0 },

    { 8.34131, 105.143, STID_0 },
    { 15.9733, 105.143, STID_1 },
    { 23.6054, 105.143, STID_2 },
    { 31.2374, 105.143, STID_0 },
    { 38.8694, 105.143, STID_1 },
    { 46.5014, 105.143, STID_2 },
    { 54.1334, 105.143, STID_0 },
    { 61.7655, 105.143, STID_1 },
    { 69.3975, 105.143, STID_2 },
    { 77.0295, 105.143, STID_0 },
    { 84.6615, 105.143, STID_1 },
    { 92.2936, 105.143, STID_2 },
    { 99.9256, 105.143, STID_0 },
    { 107.558, 105.143, STID_1 },
    { 115.190, 105.143, STID_2 },
    { 122.822, 105.143, STID_0 },
    { 130.454, 105.143, STID_1 },
    { 138.086, 105.143, STID_2 },
    { 145.718, 105.143, STID_0 },
    { 153.350, 105.143, STID_1 },
};

const int numCamPos = sizeof(camPos) / sizeof(CamPos);

/////////////////////////////////////////////////////////////////////////////

bool LfplatepViewer::update( const App& app, ConfigFile& config )
{
    if( ! Viewer::update( app, config ) )  return false;

    setPose( 0, 0 );    // set the Viewer's position in Display space

    // get the Viewer's bounds in Display space (origin at center) [mm]
    double minX, minY, maxX, maxY;
    getDisplayBounds( minX, minY, maxX, maxY );

    clearCameras();
    for( int i = 0; i < numCamPos; ++i )
    {
        const CamPos& pos = camPos[i];
        double x = pos.x;
        double y = pos.y;



        // TODO tweak pinhole coords to fit LFPLATEP [mm]

        y = -y;         // invert Y axis
        x -= (157.166 - 8.34131) / 2.0;
        y += (105.143 - 12.6095) / 2.0;
        y += 10.0;


        // is this Camera contained in the Viewer?
        if( (x >= minX) && (x <= maxX) && (y >= minY) && (y <= maxY) )
        {
            Camera& cam = addCamera();
            cam.setPose( x, y );
            cam.setStencilId( pos.stencil );
            setViewport( cam, x, y );
        }
    }

    printf( "GridViewer::update numCams %d\n", getNumCameras() );

    return true;
}

// vim: sw=4 ts=8 et ic ai nows ru:
