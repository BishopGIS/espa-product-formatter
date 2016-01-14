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

include_directories (
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_BINARY_DIR} 
    ${RAW_BINSOURCE_DIR}/common
    ${RAW_BINSOURCE_DIR}/io_libs
    ${RAW_BINSOURCE_DIR}/format_conversion_libs
    ${RAW_BINSOURCE_DIR}/per_pixel_angles_libs/ias_lib
    ${CMAKE_CURRENT_SOURCE_DIR}/GCTP3
)

set(LIB_NAME espa_land_water_mask)

set(HDRS
	ias_lw_geo.h 
	generate_land_water_mask.h
)

set(SRCS
	generate_land_water_mask.c
	deg_to_dms.c
	ias_math_point_in_closed_polygon.c
	ias_geo_convert_dms2deg.c
	ias_geo_convert_deg2dms.c
	ias_geo_find_deg.c
	ias_geo_find_min.c
	ias_geo_find_sec.c
	ias_geo_handle_180.c
	ias_geo_projection_transformation.c
	ias_geo_shape_file.c
	ias_geo_shape_mask.c
)

add_library (${LIB_NAME} ${LIB_TYPE} ${HDRS} ${SRCS})

if(DEPENDENCY_LIB)
    add_dependencies(${LIB_NAME} ${DEPENDENCY_LIB})  
endif()    
if(TARGET_LINK_LIB)
    list(REMOVE_DUPLICATES TARGET_LINK_LIB)
    target_link_libraries(${LIB_NAME} ${TARGET_LINK_LIB})
endif()   

set(TARGET_LINK_LIB ${TARGET_LINK_LIB} ${LIB_NAME} PARENT_SCOPE)

if (BUILD_SHARED_LIBS)
    if (WIN32)
      set (LIB_VERSION ${PACKAGE_VERSION_MAJOR})
    else ()
      set (LIB_VERSION ${PACKAGE_VERSION})
    endif ()
    set_target_properties (${LIB_NAME} PROPERTIES VERSION ${LIB_VERSION})
    set_target_properties (${LIB_NAME} PROPERTIES SOVERSION ${LIB_VERSION})
endif ()

#-- Apple Specific install_name for libraries
if (APPLE)
    option (BUILD_WITH_INSTALL_NAME "Build with library install_name set to the installation path" OFF)
    if (BUILD_WITH_INSTALL_NAME)
      set_target_properties(${LIB_NAME} PROPERTIES
          LINK_FLAGS "-current_version ${PACKAGE_VERSION} -compatibility_version ${PACKAGE_VERSION}"
          INSTALL_NAME_DIR "${INSTALL_LIB_DIR}"
          BUILD_WITH_INSTALL_RPATH ${BUILD_WITH_INSTALL_NAME}
      )
    endif ()
endif ()

#-----------------------------------------------------------------------------
# Add file(s) to CMake Install
#-----------------------------------------------------------------------------
if (NOT INSTALL_NO_DEVELOPMENT)
  install (
      FILES
          ${HDRS}
      DESTINATION
          ${INSTALL_INC_DIR}
      COMPONENT
          headers
  )
endif ()

#-----------------------------------------------------------------------------
# Add Target(s) to CMake Install for import into other projects
#-----------------------------------------------------------------------------
install (TARGETS ${LIB_NAME}
    LIBRARY DESTINATION ${INSTALL_LIB_DIR} 
    ARCHIVE DESTINATION ${INSTALL_LIB_DIR}
    RUNTIME DESTINATION ${INSTALL_BIN_DIR}
)


