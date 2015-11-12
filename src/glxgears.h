/*
 * Copyright (C) 1999-2001  Brian Paul   All Rights Reserved.
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
 * BRIAN PAUL BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN
 * AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

/* $XFree86: xc/programs/glxgears/glxgears.c,v 1.3tsi Exp $ */

/*
 * This is a port of the infamous "gears" demo to straight GLX (i.e. no GLUT)
 * Port by Brian Paul  23 March 2001
 *
 * Command line options:
 *    -info      print GL implementation information
 *
 */


#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <GL/gl.h>

#ifndef M_PI
#define M_PI 3.14159265
#endif


static GLfloat view_rotx = 20.0, view_roty = 30.0, view_rotz = 0.0;
static GLint gear1, gear2, gear3;
static GLfloat angle = 0.0;


/*
 *
 *  Draw a gear wheel.  You'll probably want to call this function when
 *  building a display list since we do a lot of trig here.
 * 
 *  Input:  inner_radius - radius of hole at center
 *          outer_radius - radius at center of teeth
 *          width - width of gear
 *          teeth - number of teeth
 *          tooth_depth - depth of tooth
 */
static void
gear(GLfloat inner_radius, GLfloat outer_radius, GLfloat width,
     GLint teeth, GLfloat tooth_depth)
{
   GLint i;
   GLfloat r0, r1, r2;
   GLfloat angle, da;
   GLfloat u, v, len;

   r0 = inner_radius;
   r1 = outer_radius - tooth_depth / 2.0;
   r2 = outer_radius + tooth_depth / 2.0;

   da = 2.0 * M_PI / teeth / 4.0;

   glShadeModel(GL_FLAT);

   glNormal3f(0.0, 0.0, 1.0);

   /* draw front face */
   glBegin(GL_QUAD_STRIP);
   for (i = 0; i <= teeth; i++) {
      angle = i * 2.0 * M_PI / teeth;
      glVertex3f(r0 * cosf(angle), r0 * sinf(angle), width * 0.5);
      glVertex3f(r1 * cosf(angle), r1 * sinf(angle), width * 0.5);
      if (i < teeth) {
	 glVertex3f(r0 * cosf(angle), r0 * sinf(angle), width * 0.5);
	 glVertex3f(r1 * cosf(angle + 3 * da), r1 * sinf(angle + 3 * da),
		    width * 0.5);
      }
   }
   glEnd();

   /* draw front sides of teeth */
   glBegin(GL_QUADS);
   da = 2.0 * M_PI / teeth / 4.0;
   for (i = 0; i < teeth; i++) {
      angle = i * 2.0 * M_PI / teeth;

      glVertex3f(r1 * cosf(angle), r1 * sinf(angle), width * 0.5);
      glVertex3f(r2 * cosf(angle + da), r2 * sinf(angle + da), width * 0.5);
      glVertex3f(r2 * cosf(angle + 2 * da), r2 * sinf(angle + 2 * da),
		 width * 0.5);
      glVertex3f(r1 * cosf(angle + 3 * da), r1 * sinf(angle + 3 * da),
		 width * 0.5);
   }
   glEnd();

   glNormal3f(0.0, 0.0, -1.0);

   /* draw back face */
   glBegin(GL_QUAD_STRIP);
   for (i = 0; i <= teeth; i++) {
      angle = i * 2.0 * M_PI / teeth;
      glVertex3f(r1 * cosf(angle), r1 * sinf(angle), -width * 0.5);
      glVertex3f(r0 * cosf(angle), r0 * sinf(angle), -width * 0.5);
      if (i < teeth) {
	 glVertex3f(r1 * cosf(angle + 3 * da), r1 * sinf(angle + 3 * da),
		    -width * 0.5);
	 glVertex3f(r0 * cosf(angle), r0 * sinf(angle), -width * 0.5);
      }
   }
   glEnd();

   /* draw back sides of teeth */
   glBegin(GL_QUADS);
   da = 2.0 * M_PI / teeth / 4.0;
   for (i = 0; i < teeth; i++) {
      angle = i * 2.0 * M_PI / teeth;

      glVertex3f(r1 * cosf(angle + 3 * da), r1 * sinf(angle + 3 * da),
		 -width * 0.5);
      glVertex3f(r2 * cosf(angle + 2 * da), r2 * sinf(angle + 2 * da),
		 -width * 0.5);
      glVertex3f(r2 * cosf(angle + da), r2 * sinf(angle + da), -width * 0.5);
      glVertex3f(r1 * cosf(angle), r1 * sinf(angle), -width * 0.5);
   }
   glEnd();

   /* draw outward faces of teeth */
   glBegin(GL_QUAD_STRIP);
   for (i = 0; i < teeth; i++) {
      angle = i * 2.0 * M_PI / teeth;

      glVertex3f(r1 * cosf(angle), r1 * sinf(angle), width * 0.5);
      glVertex3f(r1 * cosf(angle), r1 * sinf(angle), -width * 0.5);
      u = r2 * cosf(angle + da) - r1 * cosf(angle);
      v = r2 * sinf(angle + da) - r1 * sinf(angle);
      len = sqrt(u * u + v * v);
      u /= len;
      v /= len;
      glNormal3f(v, -u, 0.0);
      glVertex3f(r2 * cosf(angle + da), r2 * sinf(angle + da), width * 0.5);
      glVertex3f(r2 * cosf(angle + da), r2 * sinf(angle + da), -width * 0.5);
      glNormal3f(cosf(angle), sinf(angle), 0.0);
      glVertex3f(r2 * cosf(angle + 2 * da), r2 * sinf(angle + 2 * da),
		 width * 0.5);
      glVertex3f(r2 * cosf(angle + 2 * da), r2 * sinf(angle + 2 * da),
		 -width * 0.5);
      u = r1 * cosf(angle + 3 * da) - r2 * cosf(angle + 2 * da);
      v = r1 * sinf(angle + 3 * da) - r2 * sinf(angle + 2 * da);
      glNormal3f(v, -u, 0.0);
      glVertex3f(r1 * cosf(angle + 3 * da), r1 * sinf(angle + 3 * da),
		 width * 0.5);
      glVertex3f(r1 * cosf(angle + 3 * da), r1 * sinf(angle + 3 * da),
		 -width * 0.5);
      glNormal3f(cosf(angle), sinf(angle), 0.0);
   }

   glVertex3f(r1 * cosf(0), r1 * sinf(0), width * 0.5);
   glVertex3f(r1 * cosf(0), r1 * sinf(0), -width * 0.5);

   glEnd();

   glShadeModel(GL_SMOOTH);

   /* draw inside radius cylinder */
   glBegin(GL_QUAD_STRIP);
   for (i = 0; i <= teeth; i++) {
      angle = i * 2.0 * M_PI / teeth;
      glNormal3f(-cosf(angle), -sinf(angle), 0.0);
      glVertex3f(r0 * cosf(angle), r0 * sinf(angle), -width * 0.5);
      glVertex3f(r0 * cosf(angle), r0 * sinf(angle), width * 0.5);
   }
   glEnd();
}


static void
draw(void)
{
   glPushMatrix();
   glRotatef(view_rotx, 1.0, 0.0, 0.0);
   glRotatef(view_roty, 0.0, 1.0, 0.0);
   glRotatef(view_rotz, 0.0, 0.0, 1.0);

   glPushMatrix();
   glTranslatef(-3.0, -2.0, 0.0);
   glRotatef(angle, 0.0, 0.0, 1.0);
   glCallList(gear1);
   glPopMatrix();

   glPushMatrix();
   glTranslatef(3.1, -2.0, 0.0);
   glRotatef(-2.0 * angle - 9.0, 0.0, 0.0, 1.0);
   glCallList(gear2);
   glPopMatrix();

   glPushMatrix();
   glTranslatef(-3.1, 4.2, 0.0);
   glRotatef(-2.0 * angle - 25.0, 0.0, 0.0, 1.0);
   glCallList(gear3);
   glPopMatrix();

   glPopMatrix();
}


static void
init(void)
{
   static GLfloat pos[4] = { 5.0, 5.0, 10.0, 0.0 };
   static GLfloat red[4] = { 0.8, 0.1, 0.0, 1.0 };
   static GLfloat green[4] = { 0.0, 0.8, 0.2, 1.0 };
   static GLfloat blue[4] = { 0.2, 0.2, 1.0, 1.0 };

   glLightfv(GL_LIGHT0, GL_POSITION, pos);
   glEnable(GL_CULL_FACE);
   glEnable(GL_LIGHTING);
   glEnable(GL_LIGHT0);
   glEnable(GL_DEPTH_TEST);

   /* make the gears */
   gear1 = glGenLists(1);
   glNewList(gear1, GL_COMPILE);
   glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, red);
   gear(1.0, 4.0, 1.0, 20, 0.7);
   glEndList();

   gear2 = glGenLists(1);
   glNewList(gear2, GL_COMPILE);
   glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, green);
   gear(0.5, 2.0, 2.0, 10, 0.7);
   glEndList();

   gear3 = glGenLists(1);
   glNewList(gear3, GL_COMPILE);
   glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, blue);
   gear(1.3, 2.0, 0.5, 10, 0.7);
   glEndList();

   glEnable(GL_NORMALIZE);
}
