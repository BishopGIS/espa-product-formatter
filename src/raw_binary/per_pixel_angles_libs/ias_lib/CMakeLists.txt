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
)

set(LIB_NAME espa_l8_ang)

set(HDRS
	ias_angle_gen_distro.h
	ias_angle_gen_private.h
	ias_angle_gen_includes.h
	ias_math.h
	ias_const.h
	ias_geo.h
	ias_types.h
	ias_structures.h
	ias_logging.h
	ias_odl.h
	ias_satellite_attributes.h
	ias_miscellaneous.h
	ias_parm.h
	ias_parm_private.h
	local_defines.h
	lablib3.h
	toolbox.h
)

set(SRCS
	ias_angle_gen_calculate_angles_rpc.c
	ias_angle_gen_read_ang.c
	ias_angle_gen_utilities.c
	ias_angle_gen_initialize.c
	ias_angle_gen_write_image.c
	ias_angle_gen_find_scas.c
	ias_geo_convert_dms2deg.c
	ias_math_find_line_segment_intersection.c
	ias_logging.c
	ias_misc_create_output_image_trim_lut.c
	ias_misc_convert_to_uppercase.c
	ias_misc_write_envi_header.c
	ias_odl_free_tree.c
	ias_odl_get_field.c
	ias_odl_read_tree.c
	ias_parm_provide_help.c
	ias_parm_read.c
	ias_parm_map_odl_type.c
	ias_parm_check_ranges.c
	ias_satellite_attributes.c
	landsat8.c
	lablib3.c
)

add_library (${LIB_NAME} ${LIB_TYPE} ${HDRS} ${SRCS})

if(DEPENDENCY_LIB)
    add_dependencies(${LIB_NAME} ${DEPENDENCY_LIB})  
endif()    
if(TARGET_LINK_LIB)
    list(REMOVE_DUPLICATES TARGET_LINK_LIB)
    target_link_libraries(${LIB_NAME} PRIVATE ${TARGET_LINK_LIB})
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


