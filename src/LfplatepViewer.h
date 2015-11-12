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

// LfplatepViewer.h
// mike weiblen 2015-06-09
// Loads an explicit list of Cameras from a configuration file.

#ifndef LFPLATEPVIEWER_H
#define LFPLATEPVIEWER_H

#include "Viewer.h"             // our baseclass


class LfplatepViewer : public Viewer
{
public:
    friend Viewer* Viewer::factory( Viewer::Type type );
    virtual ~LfplatepViewer() {}

    virtual Type getType() const { return VT_LFPLATEP; }

    virtual bool update( const App& app, ConfigFile& config );

protected:
    LfplatepViewer() {}         // must use Viewer::factory()

private:        // disallowed
    LfplatepViewer( const LfplatepViewer& );
    LfplatepViewer& operator=( const LfplatepViewer& );
};

#endif

// vim: sw=4 ts=8 et ic ai nows ru:
