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
)

set(LIB_NAME gctp3)

set(HDRS
	gctp.h
)

set(HDRS_PRIVATE
	isin.h
)

set(SRCS
	alberfor.c
	alberinv.c
	alconfor.c
	alconinv.c
	azimfor.c
	aziminv.c
	cproj.c	
	eqconfor.c
	eqconinv.c
	equifor.c
	equiinv.c
	for_init.c
	gctp.c	
	gnomfor.c
	gnominv.c
	goodfor.c
	goodinv.c
	gvnspfor.c
	gvnspinv.c
	hamfor.c
	haminv.c
	imolwfor.c
	imolwinv.c
	inv_init.c
	isinfor.c
	isininv.c
	lamazfor.c
	lamazinv.c
	merfor.c
	merinv.c
	millfor.c
	millinv.c
	molwfor.c
	molwinv.c
	obleqfor.c
	obleqinv.c
	orthfor.c
	orthinv.c
	paksz.c	
	robfor.c
	robinv.c
	sinfor.c
	sininv.c
	sphdz.c	
	sterfor.c
	sterinv.c
	untfz.c	
	vandgfor.c
	vandginv.c
	wivfor.c
	wivinv.c
	wviifor.c
	wviiinv.c
    gctp_create_transformation.c
    gctp_dms2degrees.c
    gctp_transform.c
    gctp_print_message.c
    gctp_utility.c
    gctp_report.c
    geographic.c
    oblique_mercator.c
    polar_stereographic.c
    polyconic.c
    lambert_conformal_conic.c
    som.c
    state_plane.c
    tm.c
)

add_library (${LIB_NAME} ${LIB_TYPE} ${HDRS} ${HDRS_PRIVATE} ${SRCS})

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

set(SHARE_DATA_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/nad1927.dat
    ${CMAKE_CURRENT_SOURCE_DIR}/nad1983.dat
)

set(SPLOAD_LUT_GEN spload)
add_executable(${SPLOAD_LUT_GEN} spload.c)

foreach(SHARE_DATA_FILE ${SHARE_DATA_FILES})
  add_custom_command(TARGET ${SPLOAD_LUT_GEN} PRE_BUILD
                     COMMAND ${CMAKE_COMMAND} -E
                         copy ${SHARE_DATA_FILE} ${CMAKE_CURRENT_BINARY_DIR}) #$<TARGET_FILE_DIR:${SPLOAD_LUT_GEN}>
endforeach()

add_custom_command(TARGET ${SPLOAD_LUT_GEN} POST_BUILD
  COMMAND ${SPLOAD_LUT_GEN} > out.log
  DEPENDS ${SPLOAD_LUT_GEN}
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
  COMMENT "Generate LUT files"
  VERBATIM
)

# TODO: install somethere generated files
