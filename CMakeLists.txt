# Copyright (c) 2025 PotterWhite
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in
# all copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.

cmake_minimum_required(VERSION 3.20)

# ---------------------------------
# I. Project Definitions
# ---------------------------------
set(PROJECT_NAMESPACE "ArcForge")
project(${PROJECT_NAMESPACE}
    VERSION
        1.2.3
    LANGUAGES
        CXX
)

# ---------------------------------
# II.
#    Injecting project-local cmake dir to "CMake Module Search Path"-CMAKE_MODULE_PATH
#    add ArcFunctions module
# ---------------------------------
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
include(ArcFunctions)

# ---------------------------------
# III. Batch Mode Configure
#      a. C++ Standard
#      b. PIC
#      c. Compile Policy
#      d. Build Type
# ---------------------------------
arc_init_global_settings()

# ---------------------------------
# IV. Bath Mode Configure
#     a. Global Project Version
#     b. Author (Name & Email)
#     c. Build Timestamp
# ---------------------------------
arc_init_project_metadata()

# ---------------------------------
# IX. Install Directory (Refactored)
# ---------------------------------
# [CHANGE LOG]:
# 1. Removed the logic that appended "/${BUILD_TYPE_LOWER}".
#    Reason: Directory structure should be defined by the caller (CMakePresets.json),
#    not hardcoded in the script.
# 2. Logic Simplified: If CMAKE_INSTALL_PREFIX is not set, default to local "install" folder.
#    Otherwise, trust the path provided by CMakePresets.
if(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
    set(CMAKE_INSTALL_PREFIX "${CMAKE_CURRENT_SOURCE_DIR}/install"
        CACHE PATH "Default install prefix" FORCE)
    message(STATUS "[Configuration] Install Path Defaulted to: ${CMAKE_INSTALL_PREFIX}")
else()
    message(STATUS "[Configuration] Install Path Specified: ${CMAKE_INSTALL_PREFIX}")
endif()

# ---------------------------------
# X. Main Executes
# ---------------------------------
# 1. 先处理第三方库 (因为 libs 可能会依赖它)
#    注意顺序：必须在 add_subdirectory(libs) 之前！
add_subdirectory(third_party)

add_subdirectory(libs)
add_subdirectory(apps)

# ---------------------------------
# VII. [Optional] Test
# ---------------------------------
if(BUILD_TEST STREQUAL "TRUE")
    add_subdirectory(test)
endif()

# ---------------------------------
#               End
# ---------------------------------
