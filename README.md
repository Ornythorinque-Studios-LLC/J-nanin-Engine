# J'nanin Engine
<img src="logo/J'nanin_HiRes.png" alt="J'nanin logo" width="80%" style="image-rendering: pixelated;"/> <br>
A RTS Game engine for MS-DOS.

*(as of now development has started, but aren't done. Please be patient. this repo currently contains information and plans for the engine)*

# What is the J'nanin Engine?

The J'nanin Engine is a custom, low level game engine, primarily written in C.  
It is designed to be compatible with MS-DOS, and compiled using legacy software such as Borland Turbo C.  
The goal of the J'nanin Engine is to be able to maintain a stable framerate (24 frames per second) on at least a 486/DX2@50 MHz.  
Another goal / challenge for myself is to not use more than 640k of RAM, which is the max amount that x86 Real Mode provides and can address without using DOS extenders to enter Protected Mode (32 bits) or using programs like HIMEM.SYS.  
The name "J'nanin" comes from the hub world age of the same name in the game Myst III: Exile. (Cyan Worlds is not associated with this project, however, and Myst is intellectual property of Cyan Worlds, Inc.)

# What would the requirements be?

As of now, there are no definitive requirements, but the probable requirements are as follows:

At least a 486/DX2 running at 50 MHz.  
640k of Conventional RAM.  
Any VGA capable video card and monitor (no SVGA needed).  
Any reasonably sized Winchester Disk (HDD).  
A floppy / CD drive.  
At least MS-DOS 3.3 installed.  

# What would the features be?

At the moment, we have a set of features planned that we think are reasonable to accomplish. They are as follows:

Semi-Fluid Animations for sprites.  
Streaming assets from disk with minimal CPU overhead.  
Full mouse support (PS/2 or Serial).  
(Potentially) simple multiplayer over COM ports.  
Running @ 320x200x8bpp (VGA Mode 13h) at 24 frames per second.  
2D tile and map rendering, with potential for Isometric rendering in the future.  
Simple Pathfinding and decision making for enemies and player units.  
Simple scripting for basic cutscenes.  
Up to 10 units on screen at once.  
Dialogue for all characters.  
Achievement system.  
AdLib support for audio.  

# Are there any other platforms?

At the moment, our only current target is DOS, but we do want to write it in a way where it is semi-abstracted for maximum platform portability.

Some platforms I would LIKE to see it run on in the future:  
- Windows 9x/NT  
- Mac OS 7 (68040/LC040)  
- Mac OS 9 (PPC, with Carbon support for OS X all the way through 10.14 \[recompilation on intel\])  
- PlayStation 4/5  
- Nintendo Switch (and switch 2)  
- Xbox One / Series  
- macOS (all modern versions)  
- Linux  
- Windows 10/11  
