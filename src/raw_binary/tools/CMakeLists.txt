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
    ${RAW_BINSOURCE_DIR}/format_conversion_libs
    ${RAW_BINSOURCE_DIR}/common
    ${RAW_BINSOURCE_DIR}/io_libs
    ${RAW_BINSOURCE_DIR}/per_pixel_angles_libs    
    ${RAW_BINSOURCE_DIR}/per_pixel_angles_libs/ias_lib
    ${RAW_BINSOURCE_DIR}/land_water_mask_libs
    ${RAW_BINSOURCE_DIR}/land_water_mask_libs/GCTP3
)

set(UTILS
    convert_lpgs_to_espa 
    convert_espa_to_hdf
    convert_espa_to_gtif
    convert_espa_to_bip 
    espa_band_subset 
    espa_product_subset
    convert_modis_to_espa 
    create_l8_angle_bands 
    create_land_water_mask
    create_julian_date_bands
)

foreach(UTIL ${UTILS})
    add_executable(${UTIL} ${UTIL}.c)
    if(DEPENDENCY_LIB)
        add_dependencies(${UTIL} ${DEPENDENCY_LIB})  
    endif()    
    if(TARGET_LINK_LIB)
        list(REMOVE_DUPLICATES TARGET_LINK_LIB)
        target_link_libraries(${UTIL} PRIVATE ${TARGET_LINK_LIB})
    endif() 
endforeach()

install (TARGETS ${UTILS}
    LIBRARY DESTINATION ${INSTALL_LIB_DIR}
    ARCHIVE DESTINATION ${INSTALL_LIB_DIR}
    RUNTIME DESTINATION ${INSTALL_BIN_DIR}
)

