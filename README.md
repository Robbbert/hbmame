<<<<<<< HEAD
What is HBMAME?
=======

# **MAME** #

[![Join the chat at https://gitter.im/mamedev/mame](https://badges.gitter.im/Join%20Chat.svg)](https://gitter.im/mamedev/mame?utm_source=badge&utm_medium=badge&utm_campaign=pr-badge&utm_content=badge)

Build status for tiny build only, containing just core parts of project

| OS/Compiler        | Status           | 
| ------------- |:-------------:| 
|Linux GCC / OSX Clang| [![Build Status](https://travis-ci.org/mamedev/mame.svg?branch=master)](https://travis-ci.org/mamedev/mame) |
|Windows MinGW | [![Build Status](https://tea-ci.org/api/badges/mamedev/mame/status.svg)](https://tea-ci.org/mamedev/mame) |


What is MAME?
=============

MAME originally stood for Multiple Arcade Machine Emulator.

MAME's purpose is to preserve decades of software history. As electronic technology continues to rush forward, MAME prevents this important "vintage" software from being lost and forgotten. This is achieved by documenting the hardware and how it functions. The source code to MAME serves as this documentation. The fact that the software is usable serves primarily to validate the accuracy of the documentation (how else can you prove that you have recreated the hardware faithfully?). Over time, MAME absorbed the sister-project MESS (Multi Emulator Super System), so MAME now documents a wide variety of (mostly vintage) computers, video game consoles and calculators, in addition to the arcade video games that were its initial focus.

How to compile?
>>>>>>> upstream/master
===============

HBMAME stands for Home Brew Multiple Arcade Machine Emulator.

HBMAME's purpose is to preserve homebrew games written for arcade machines or arcade emulators. All the documentation of the hardware can be found in the MAME source.

How to compile
==============

```
make TARGET=hbmame
```

or

```
make TARGET=hbmame OSD=winui
```

Please read compile instructions at docs/compile.doc
=======


For Windows users, we provide a ready-made [build environment](http://mamedev.org/tools/) based on MinGW-w64. [Visual Studio builds](http://wiki.mamedev.org/index.php?title=Building_MAME_using_Microsoft_Visual_Studio_compilers) are also possible.


Where can I find out more?
==========================

* [Official MAME Development Team Site](http://mamedev.org/) (includes binary downloads for MAME and MESS, wiki, forums, and more)
* [Official HBMAME site](http://hbmame.1emulation.com/)
* [Forum](http://www.mameworld.info/ubbthreads/postlist.php?Cat=&Board=misfitmame) (bug reports go here too)


Licensing Information
=====================

Information about the MAME content can be found at https://github.com/mamedev/mame/blob/master/README.md

Information about the MAME license particulars and disclaimers can be found at https://github.com/mamedev/mame/blob/master/LICENSE.md

Information about the WINUI portion can be found at https://github.com/Robbbert/mameui/blob/master/docs/winui_license.txt

