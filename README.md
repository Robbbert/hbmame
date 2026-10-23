# HBMAME for macOS

Unofficial native macOS builds of [HBMAME](https://github.com/Robbbert/hbmame), signed with a Developer ID and notarized by Apple.

- **Download:** [latest release](https://github.com/estefan3112/hbmame-macos/releases/latest)
- **Requirements:** Apple Silicon Mac, macOS 13 or later, SDL3 in `/Library/Frameworks` (see the release notes)
- **Source:** unmodified upstream release tags; this fork only adds macOS build and packaging

### Compile on macOS

MACOSX_DEPLOYMENT_TARGET=13.0 make REGENIE=1 USE_LIBSDL=0 -j$(sysctl -n hw.ncpu) TARGET=hbmame

`USE_LIBSDL=0` is required if Homebrew's SDL3 is installed; otherwise the build silently links against it.

Mac-specific issues belong in this repository, not upstream.

---

*Original HBMAME README:*

What is HBMAME?
===============

HBMAME stands for Home Brew MAME. It runs on Windows 10 build 1607, 64-bit or later.

HBMAME has 2 purposes:
Firstly is to preserve homebrew games written for arcade machines or arcade emulators.
Secondly is to include worthwhile hacks of arcade games.
All the documentation of the hardware can be found in the MAME source.

How to compile
==============

```
make TARGET=hbmame SYMBOLS=0 NO_SYMBOLS=1 DEPRECATED=0
```

or

```
make TARGET=hbmame OSD=winui SYMBOLS=0 NO_SYMBOLS=1 DEPRECATED=0
```



Where can I find out more?
==========================

* [Official HBMAME site] http://hbmame.1emulation.com/
* [Forum] https://www.1emulation.com/forums/forum/125-mameui/


Licensing Information
=====================

The primary license is GPL_2.0 : https://www.gnu.org/licenses/old-licenses/gpl-2.0.en.html

Information about the MAME content can be found at https://github.com/mamedev/mame/blob/master/README.md

Information about the license can be found in COPYING

Information about the WINUI portion can be found at https://github.com/Robbbert/mameui/blob/master/docs/winui_license.txt
