# Locate SDL2 library
# This module defines
# SDL2_ttf_LIBRARY, the name of the library to link against
# SDL2_ttf_FOUND, if false, do not try to link to SDL2_ttf
# SDL2_ttf_INCLUDE_DIR, where to find SDL_ttf.h
# SDL2_ttf_LIBRARY_DIR, where to find SDL2_ttf.lib
#
# This module responds to the the flag:
# SDL2_BUILDING_LIBRARY
# If this is defined, then no SDL2main will be linked in because
# only applications need main().
# Otherwise, it is assumed you are building an application and this
# module will attempt to locate and set the the proper link flags
# as part of the returned SDL2_LIBRARY variable.
#
# Don't forget to include SDLmain.h and SDLmain.m your project for the
# OS X framework based version. (Other versions link to -lSDL2main which
# this module will try to find on your behalf.) Also for OS X, this
# module will automatically add the -framework Cocoa on your behalf.
#
#
# Additional Note: If you see an empty SDL2_LIBRARY_TEMP in your configuration
# and no SDL2_LIBRARY, it means CMake did not find your SDL2 library
# (SDL2.dll, libsdl2.so, SDL2.framework, etc).
# Set SDL2_LIBRARY_TEMP to point to your SDL2 library, and configure again.
# Similarly, if you see an empty SDL2MAIN_LIBRARY, you should set this value
# as appropriate. These values are used to generate the final SDL2_LIBRARY
# variable, but when these values are unset, SDL2_LIBRARY does not get created.
#
#
# $SDL2DIR is an environment variable that would
# correspond to the ./configure --prefix=$SDL2DIR
# used in building SDL2.
# l.e.galup  9-20-02
#
# Modified by Eric Wing.
# Added code to assist with automated building by using environmental variables
# and providing a more controlled/consistent search behavior.
# Added new modifications to recognize OS X frameworks and
# additional Unix paths (FreeBSD, etc).
# Also corrected the header search path to follow "proper" SDL guidelines.
# Added a search for SDL2main which is needed by some platforms.
# Added a search for threads which is needed by some platforms.
# Added needed compile switches for MinGW.
#
# On OSX, this will prefer the Framework version (if found) over others.
# People will have to manually change the cache values of
# SDL2_LIBRARY to override this selection or set the CMake environment
# CMAKE_INCLUDE_PATH to modify the search paths.
#
# Note that the header path has changed from SDL2/SDL.h to just SDL.h
# This needed to change because "proper" SDL convention
# is #include "SDL.h", not <SDL2/SDL.h>. This is done for portability
# reasons because not all systems place things in SDL2/ (see FreeBSD).

#=============================================================================
# Copyright 2003-2009 Kitware, Inc.
#
# Distributed under the OSI-approved BSD License (the "License");
# see accompanying file Copyright.txt for details.
#
# This software is distributed WITHOUT ANY WARRANTY; without even the
# implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
# See the License for more information.
#=============================================================================
# (To distribute this file outside of CMake, substitute the full
#  License text for the above reference.)

if(CMAKE_SIZEOF_VOID_P EQUAL 8)
  set(SDL2_ARCH_64 TRUE)
  set(SDL2_PROCESSOR_ARCH "x64")
else()
  set(SDL2_ARCH_64 FALSE)
  set(SDL2_PROCESSOR_ARCH "x86")
endif(CMAKE_SIZEOF_VOID_P EQUAL 8)

SET(SDL2_TTF_SEARCH_PATHS
	~/Library/Frameworks
	/Library/Frameworks
	/usr/local
	/usr
	/sw # Fink
	/opt/local # DarwinPorts
	/opt/csw # Blastwave
	/opt
	${SDL2_ttf_ROOT}
)

FIND_PATH(SDL2_TTF_INCLUDE_DIR SDL_ttf.h
	HINTS
	$ENV{SDL2DIR}
	PATH_SUFFIXES include/SDL2 include
	PATHS ${SDL2_TTF_SEARCH_PATHS}
)

FIND_LIBRARY(SDL2_TTF_LIBRARY_TEMP
	NAMES SDL2_ttf
	HINTS
	$ENV{SDL2DIR}
	PATH_SUFFIXES lib64 lib lib/${SDL2_PROCESSOR_ARCH}
	PATHS ${SDL2_TTF_SEARCH_PATHS}
)

IF(SDL2_TTF_LIBRARY_TEMP)
	SET(SDL2_TTF_ORIGINAL_LIBRARY ${SDL2_TTF_LIBRARY_TEMP})
	get_filename_component(SDL2_TTF_LIBRARY_DIR ${SDL2_TTF_ORIGINAL_LIBRARY} PATH)

	# Set the final string here so the GUI reflects the final state.
	SET(SDL2_TTF_LIBRARY ${SDL2_TTF_ORIGINAL_LIBRARY} CACHE STRING "Where the SDL2 Library can be found")
	SET(SDL2_TTF_LIBRARIES ${SDL2_TTF_LIBRARY_TEMP} CACHE STRING "All libraries to link against when using SDL2")
	# Set the temp variable to INTERNAL so it is not seen in the CMake GUI
	SET(SDL2_TTF_LIBRARY_TEMP "${SDL2_TTF_LIBRARY_TEMP}" CACHE INTERNAL "")
	SET(SDL2_TTF_LIBRARY_TEMP "${SDL2_TTF_ORIGINAL_LIBRARY}" CACHE INTERNAL "")
	SET(SDL2_TTF_FOUND "YES")
ENDIF(SDL2_TTF_LIBRARY_TEMP)

INCLUDE(FindPackageHandleStandardArgs)

FIND_PACKAGE_HANDLE_STANDARD_ARGS(SDL2_TTF REQUIRED_VARS SDL2_TTF_LIBRARY SDL2_TTF_INCLUDE_DIR)
