# Makefile for linaro (linux/gcc)

#
# Copyright (c) 2015, Phil J. Bostley III 
# All rights reserved.
# 
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions are met:
#     * Redistributions of source code must retain the above copyright
#       notice, this list of conditions and the following disclaimer.
#     * Redistributions in binary form must reproduce the above copyright
#       notice, this list of conditions and the following disclaimer in the
#       documentation and/or other materials provided with the distribution.
#     * Neither the name of Phil J Bostley III nor the
#       names of its contributors may be used to endorse or promote products
#       derived from this software without specific prior written permission.
# 
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
# ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
# WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
# DISCLAIMED. IN NO EVENT SHALL <COPYRIGHT HOLDER> BE LIABLE FOR ANY
# DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
# (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
# LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
# ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
# (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
# SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
# 

SRC_DIR = ../src
VPATH = ${SRC_DIR}

EXE_FILE = lefdy
VER_FILE = ${SRC_DIR}/GIT_DESCRIBE.h

LUA = lua5.2

INC_DIRS = -I/usr/include/${LUA}
LDLIBS = -l${LUA} -lglut -lGL

CFLAGS = -DLFD_PLATFORM_LINUX=1 ${INC_DIRS}

# release settings
CFLAGS += -O2
LDFLAGS += -s

# debug settings
#CFLAGS += -g
#LDFLAGS += -g

CXXFLAGS = ${CFLAGS}

OBJS =                          \
	App.o                   \
	Camera.o                \
	ConfigFile.o            \
	CubeScene.o             \
	GearScene.o             \
	GlutApp.o               \
	GlUtils.o               \
	LfplatepViewer.o        \
	LuaInterp.o             \
	Scene.o                 \
	SingleViewer.o          \
	StencilScene.o          \
	TableViewer.o           \
	TeapotScene.o           \
	Utils.o                 \
	Viewer.o                \
        main.o


all: ${EXE_FILE}

.PHONY: ${VER_FILE} clean clobber

${VER_FILE}:
	printf "#define GIT_DESCRIBE \"%s\"\n" "$(shell git describe --tags --long)" > $@

main.o: ${VER_FILE}

${EXE_FILE}: ${OBJS}
	$(LINK.cpp) $^ $(LDLIBS) -o $@

clean:
	-rm -f ${OBJS}
	> ${VER_FILE}

clobber: clean
	-rm -f ${EXE_FILE}

