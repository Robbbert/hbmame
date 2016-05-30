What is HBMAME?
===============

HBMAME stands for Home Brew Multiple Arcade Machine Emulator.

HBMAME's purpose is to preserve homebrew games written for arcade machines or arcade emulators. All the documentation of the hardware can be found in the MAME source.

How to compile
==============

```
make TARGET=hbmame OSD=newui
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

