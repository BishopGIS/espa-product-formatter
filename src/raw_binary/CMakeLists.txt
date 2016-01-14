################################################################################
# Project:  ESPA product formatter
# Purpose:  CMake build scripts
# Author:   Dmitry Baryshnikov, dmitry.baryshnikov@nexgis.com
# Author:   Alexander Lisovenko, alexander.lisovenko@nextgis.com
################################################################################
# Copyright (C) 2015, NextGIS <info@nextgis.com>
#
# Permission is hereby granted, free of charge, to any person obtaining a
# copy of this software and associated documentation files (the "Software"),
# to deal in the Software without restriction, including without limitation
# the rights to use, copy, modify, merge, publish, distribute, sublicense,
# and/or sell copies of the Software, and to permit persons to whom the
# Software is furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included
# in all copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
# OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
# THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
# FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
# DEALINGS IN THE SOFTWARE.
################################################################################

if(WIN32)
    add_definitions(-DWIN32)
    list(APPEND TARGET_LINK_LIB_GLOB ws2_32.lib Secur32.lib)
    if (MSVC)
        add_definitions (-D_CRT_NONSTDC_NO_DEPRECATE -D_CRT_SECURE_NO_WARNINGS -D_CRT_NON_CONFORMING_SWPRINTFS -D_CRT_SECURE_NO_DEPRECATE)
    endif()
endif()       

set(LIBDIRS	
    common 
    io_libs 
    format_conversion_libs 
    per_pixel_angles_libs/ias_lib 
    per_pixel_angles_libs 
    land_water_mask_libs/GCTP3 
    land_water_mask_libs
)

set(RAW_BINSOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR})
include_directories (${CMAKE_CURRENT_SOURCE_DIR})
include_directories (${CMAKE_SOURCE_DIR}/port)

if(UNIX)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g -O -W -Wall -Wno-sign-compare -Wno-unused-parameter") #-pedantic 
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -O -W -Wall -Wno-sign-compare -Wno-unused-parameter") 
endif()

include(find_anyproject)

set(TARGET_LINK_LIB ${TARGET_LINK_LIB_GLOB})

if(UNIX)
  set(TARGET_LINK_LIB ${TARGET_LINK_LIB} m)
endif()


find_anyproject(ZLIB REQUIRED)
find_anyproject(LibXml2 REQUIRED)
find_anyproject(JPEG REQUIRED)
find_anyproject(JBIG REQUIRED)
find_anyproject(TIFF REQUIRED)
find_anyproject(GeoTIFF REQUIRED)
find_anyproject(HDF4 REQUIRED)
find_anyproject(HDFEOS2 REQUIRED)

# debug stuff
#get_property(dirs DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY INCLUDE_DIRECTORIES)
#foreach(dir ${dirs})
#  message(STATUS "dir='${dir}'")
#endforeach()

# libs
foreach(LIB ${LIBDIRS})
    add_subdirectory(${LIB})
endforeach()

# executables
add_subdirectory(tools)

message(STATUS "TARGET_LINK_LIB ${TARGET_LINK_LIB}")
