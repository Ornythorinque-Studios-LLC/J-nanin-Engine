# J'nanin Engine
A RTS Game engine for MS-DOS.

(as of now development has not started, be patient. this repo currently contains information and plans for the engine)

# What is the J'nanin Engine?

The J'nanin Engine is a custom, low level game engine, primarily written in C.  
It is designed to be compatible with MS-DOS, and compiled using legacy software such as Borland Turbo C.  
The goal of the J'nanin Engine is to be able to maintain a stable framerate (24 frames per second) on at least a 486/DX2@66 MHz.  
Another goal / challenge for myself is to not use more than 640k of RAM, which is the max amount that x86 Real Mode provides and can address without using DOS extenders to enter Protected Mode (32 bits) or using programs like HIMEM.SYS.  
The name "J'nanin" comes from the hub world age of the same name in the game Myst III: Exile. (Cyan Worlds is not associated with this project, however, and Myst is intellectual property of Cyan Worlds, Inc.)

# What would the requirements be?

As of now, there are no definitive requirements, but the probable requirements are as follows:

At least a 486/DX2 running at 66 MHz.  
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
(Maybe) SoundBlaster support for audio.  
(Maybe) Windows 3.1 / 95 support using Win16/32 API.   

Most of these do seem like basic things, but in practice, the vast majority of these features are much more complex than most people would think (due to using low-level C in ring 0, which contains minimal abstraction and built-in driver support).
