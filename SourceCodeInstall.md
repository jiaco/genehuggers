# Introduction #

This page will eventually detail all the requirements to build GeneHuggers from source.


# Details #

Step 1: (All platforms) Install open-source Qt (v4.7+).

Linux Mint 13/14: sudo aptitude install libqt4-dev

Step 2: download the GeneHuggers source package (clone with hg)
Step 3: Define environment variables:
  * Mac: (in progress)
  * Windows: GH\_PATH needs to be set (example: C:\Gh) and all headers and dlls will get put in this location.
  * Linux:
    1. GH\_HOME points to the genehuggers source directory
    1. GH\_BIN is where executables get installed (make install)
    1. GH\_LIB is where the shared libraries get installed
    1. GH\_INC is where the header files get installed

Step 4: run a script ./scripts/build.sh linux|mac|win

the build script should be able to compile GhCore GhBio GhGui and GhSalsa. On linux ghmake even gets made properly. For windows:
  * cd ghamke
  * qmake
  * make
  * cp release/ghmake.exe C:\Gh\.

On windows, you need to put GH\_PATH into your PATH.
On linux, you need to put GH\_LIB into LD\_LIBRARY\_PATH and GH\_BIN into your PATH.