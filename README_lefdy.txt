This is a rudimentary pinhole lightfield demo.

--- BUILD --- BUILD --- BUILD --- BUILD --- BUILD --- BUILD --- BUILD --- BUILD --- BUILD --- BUILD ---

Build freeglut-2.8.1 
4 Build lua-5.2.3 
5 Build vrpn-07.33 
6 
 
7 Run vs2012\MAKE_BUILD.txt 
8 
 
9 or 
10 
 
11 Load solution vs2012\000.sln into Visual Studio 2012 
12 Build Debug and Release configurations 

--- TODO --- TODO --- TODO --- TODO --- TODO --- TODO --- TODO --- TODO --- TODO --- TODO --- TODO ---

efdy user guide

vrpn : add internal server for standalone operation

update README.md

create PPT slides of:
- latest LFD projection diagram
- configuration of QPI demo array

The 1st demo is not to impress; it is to educate and inspire discussion.
Once educated, then the 2nd demo can impress.

TableViewer

rename Viewer to "Panel"?

EventHandler to be generic, not use GLUT

filesystem & renaming tweaks

frame out CalibrationScene

Pose location to implicitly define Viewport location
(Viewport size/orientation is separate)

make DodecaScene be more generic wireframe geometry

post screengrabs of hogels to website.

block diagrams:
- single compute node
- whole system
- proposed ORE
- double frustum v. slice-n-dice

fix CRLF in MAKE_GIT_DESCRIBE

look at pinouts for LCD backlight


--- NOTES --- NOTES ---  NOTES ---  NOTES ---  NOTES ---  NOTES ---  NOTES ---  NOTES ---  NOTES ---   

/////////////////////////////////////////////////////////////////////////////

"ORE" = Objective Rendering Engine
A hardware architecture, distinct from a GPU, optimized for rendering hogels.
- parallel rendering of an array of Cameras from a single dispatch.
- rendering each view using Pinhole Projection, rather than 3 separate draw
  passes (fwd perspect, ortho epsilon, rev perspect)
- ORE embedded in the photonics/optics package means Camera can be calibrated
  once at manufacturing, with those parameters flashed into ROM.

grep for the word "NUGGET" to find essential architectural concepts.

"DBU" = "Database Unit", the dimensionless units of OpenGL vertices.

"hogel" = holographic element (like pixel = picture element)
encodes color/intensity per direction.

/////////////////////////////////////////////////////////////////////////////
Per-platform compile-time defines:
-DLFD_PLATFORM_MSVC=1           // Microsoft Visual Studio
-DLFD_PLATFORM_LINUX=1          // GCC on Linux
-DLFD_PLATFORM_ANDROID=1

/////////////////////////////////////////////////////////////////////////////
// Simplifying assumptions regarding assignment of attributes between
// Viewer and Cameras.  (In the future, some attributes could migrate
// from shared values in the Viewer to per-Camera values):
//
// 1) Camera FOV in X and Y directions is equal.
// 2) Camera FOV for near and far volumes is equal.
// 3) Camera size in X and Y directions is equal (ie aspect=1).
// 4) FOV for all Cameras in a Viewer is the same.
// 5) Size for all Cameras in a Viewer is the same.
// 6) zNear and zFar for all Camera in a Viewer are the same.
// 7) Camera's image area (ie viewport) is circular.
// 8) Position of Camera is the center of its image area (ie viewport).
// 9) defn: zFar > zNear
// 10) defn: zFarEpsilon > 0
// 11) defn: zNearEpsilon == -zFarEpsilon
// 12) Rather than partition the glDepthRange, could instead
//     glClear(GL_DEPTH_BUFFER_BIT) between passes.
// 13) "Display Space" is the coordinate system of physical dimensions [mm]
// 14) defn: NearVolume is in -Z of Display Space, FarVolume is in +Z.
// 15) EpsilonVolume straddles -Z and +Z of Display Space, centered on 0.
// 16) defn: 0.0 <= _dNearEpsilon <= _dFarEpsilon <= 1.0
// 17) Volumes are defined in Display Space.
// 18) Viewer has ultimate ownership of SCISSOR_TEST and STENCIL_TEST

/////////////////////////////////////////////////////////////////////////////
From the SPIE Proceedings Abstracts, page 8
http://spie.org/Documents/ConferencesExhibitions/EI15-Abstracts.pdf

"Small form factor full parallax tiled light field display", Alpaslan et al.
Ostendo's QPI technology is an emissive display technology with
- Each tile
        - 10 m pixels = 0.01mm
        - 1000x800 pixels = 0.8 Mpix
        - 20x16 = 320 hogels/tile
        - 50x50 = 2500 pixels/hogel @ 0.5mm
- Tiled display
        - 4x2 array, 6.4 Mpix
        - approx 48x17x2mm w/ small gaps.
        - 80x32 = 2560 hogels
- 120 mm depth of field, 30 degree @ 60 Hz refresh rate.
/////////////////////////////////////////////////////////////////////////////
LCD panel

Adafruit 1931
https://www.adafruit.com/products/1931
7" Display 1280x800 IPS - HDMI/VGA/NTSC/PAL

    Resolution: 1280 x 800
    Dimensions of Screen: 105mm x 160mm x 3mm / 4.1" x 6.3" x 0.1"
    Visible area: 150mm x 95mm (16:10)
    Display dimensions: 162mm x 104mm x 4mm (6.4" x 4.1" x 0.2")
    Uses an HSD070PWW1 display

from HSD070PWW1-B00.pdf:
    Outline Dimension 161.2(Typ) x105.5 (Typ) mm
    Display area 150.72 (H) x 94.2(V) mm
    Number of Pixel 1280 RGB (H) x 800(V) pixels
    Pixel pitch 0.11775(H) x 0.11775(V) mm

    Mechanical dims:    Min     Typ     Max
    Horizontal (H)      160.9   161.2   161.5 mm
    Vertical (V)        105.2   105.5   105.8 mm
    Depth (D) w/o PCB   -         2.35    2.65 mm
    Depth (D) w/ PCB    -         4.2     4.5 mm
/////////////////////////////////////////////////////////////////////////////
Inforce 6410
http://www.inforcelive.com/index.php?route=product/product&product_id=53

    Snapdragon 600
    Krait CPU, 4-core, 1.7 GHz, 2MB L2 cache
    Adreno A320 GPU
/////////////////////////////////////////////////////////////////////////////
Looking ahead to something like OVR_multiview...

1) render each Camera to a layer of a texture array.
   OVR_multiview makes this efficient.
   but can it handle 500-1000 views and texture layers??
2) each Camera uses entire texture; no scissor while rendering.
   texture can leverage multisampling if desired.
3) composite the Camera textures into a framebuffer using textured quads.
   this is essentially point sprites (need quad size and pose)
4) apply stencil while compositing?
5) various hogel corrections can be done during sprite composition:
   spatial offsets: translation in sprite location.
   aspect!=1: nonuniform scale of quad.
   chromatic: render each color layer w/ different sprite scale.
6) Camera textures dont use viewport coords to be composited;
   all compositing done natively in Display space.

/////////////////////////////////////////////////////////////////////////////

A "Pose" is defined as being in Display space.
It represents the position and orientation (ie XYZHPR) of an entity in
physical dimensions.

Viewers and Cameras in physical space are specified by a pose.

Viewers have a pose for specific limited reasons:
1) to determine which Camera belong to them.
2) to position the Camera within the Viewer.

In the current implementation, we assume all entities are coplanar,
and so their pose can be fully expressed using only X and Y 2D translation.
Non-coplanar (ie Z != 0, 3D translation) and non-parallel (rotation, differing
orientation) will come in the future if needed.

/////////////////////////////////////////////////////////////////////////////

There are different mechanisms possible for a Viewer implementation.

The current lefdy Viewer (circa Apr 2015) is a "direct viewport" renderer:
each Camera is rendered directly into the output framebuffer was separate
OpenGL viewports.  Limitations: discrete resolution defined by viewport rect,
requires scissor and stencil.

Another implementation is a "texture compositing" renderer: render each
Camera to a target texture, then composite the textures into the output
framebuffer using a textured quad or point sprite per Camera.  The texture
dimensions are decoupled from the viewport pixel coordinates, and can support
additional manipulations such as scaling, oblique orientation, chromatic
compensation, shader-computation during composition, stenciling via non-quad
geometry, etc.
This approach also maps well to the OVR_multiview extension.

/////////////////////////////////////////////////////////////////////////////

