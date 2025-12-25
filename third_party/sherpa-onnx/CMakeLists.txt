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

# third_party/sherpa-onnx/CMakeLists.txt
cmake_minimum_required(VERSION 3.14)
project(sherpa-onnx-download NONE)          # NONE means current project does not need to be compiled

# ---------------------------------
# I. Import "FetchContent" Module
# ---------------------------------
include(FetchContent)

message(STATUS "[Configuration][ThirdParty] Preparing Sherpa-Onnx...")

# ---------------------------------
# II. Declare Download Detail声明下载详情
# ---------------------------------
# These variables come from CMakePresets.json
if(NOT DEFINED SHERPA_ONNX_URL OR NOT DEFINED SHERPA_ONNX_HASH)
    message(Warning "[Configuration][ThirdParty] SHERPA_ONNX_URL or HASH not defined. Check CMakePresets.json.")
endif()

set(CHOSEN_SHERPA_URL "${SHERPA_ONNX_URL}")

if(DEFINED ENV{ARC_DEP_CACHE_DIR})
    # Obtain the filename from the URL (e.g. sherpa-onnx-v1.12.20-rv1126bp.tar.xz)
    get_filename_component(DEP_FILENAME "${SHERPA_ONNX_URL}" NAME)

    set(LOCAL_FILE_PATH "$ENV{ARC_DEP_CACHE_DIR}/${DEP_FILENAME}")

    # If file exist then it will be added as first place of candidates
    # Caution here: We will not check Hashcode, FetchContent will do instead
    #               If Hashcode is not matched, FetchContent will take warning
    if(EXISTS "${LOCAL_FILE_PATH}")
        message(STATUS "[Configuration][ThirdParty] Found local cache candidate: ${LOCAL_FILE_PATH}")
        set(CHOSEN_SHERPA_URL "file://${LOCAL_FILE_PATH}")
    else()
        message(STATUS "[Configuration][ThirdParty] Local cache not found at: ${LOCAL_FILE_PATH}, using remote URL.")
    endif()
endif()

message(STATUS "[Configuration][ThirdParty] Final URL used: ${CHOSEN_SHERPA_URL}")

FetchContent_Declare(
    sherpa_onnx_pkg
    URL      ${CHOSEN_SHERPA_URL}
    URL_HASH ${SHERPA_ONNX_HASH}
    # 下载并解压到 build/_deps/sherpa_onnx_pkg-src
    # DOWNLOAD_NO_EXTRACT FALSE (默认就是自动解压)
)

# ---------------------------------
# III. Exec Downloading and Decompressing (Configure Stage)
# ---------------------------------
FetchContent_MakeAvailable(sherpa_onnx_pkg)

# 获取解压后的根目录路径
set(SHERPA_ROOT "${sherpa_onnx_pkg_SOURCE_DIR}")
message(STATUS "[Configuration][ThirdParty] Sherpa-Onnx mounted at: ${SHERPA_ROOT}")

# ---------------------------------
# IV. Define "IMPORTED Targets" (Point to the Dir After Decompressing)
# ---------------------------------

# --- a. Core Lib: C++ API ---
add_library(sherpa_onnx_lib
    SHARED
    IMPORTED
    GLOBAL
)
set_target_properties(sherpa_onnx_lib
    PROPERTIES
        IMPORTED_LOCATION
            "${SHERPA_ROOT}/lib/libsherpa-onnx-cxx-api.so"
        INTERFACE_INCLUDE_DIRECTORIES
            "${SHERPA_ROOT}/include"
        # 3. 自动链接依赖
        INTERFACE_LINK_LIBRARIES
            "SherpaDeps::C_API;SherpaDeps::OnnxRuntime;SherpaDeps::CArgs"
)

# --- b. Dep Lib: OnnxRuntime ---
add_library(onnxruntime_lib
    SHARED
    IMPORTED
    GLOBAL
)
set_target_properties(onnxruntime_lib
    PROPERTIES
        IMPORTED_LOCATION
            "${SHERPA_ROOT}/lib/libonnxruntime.so"
)

# --- c. Dep Lib: C API ---
add_library(sherpa_c_api_lib
    SHARED
    IMPORTED
    GLOBAL
)
set_target_properties(sherpa_c_api_lib
    PROPERTIES
        IMPORTED_LOCATION
            "${SHERPA_ROOT}/lib/libsherpa-onnx-c-api.so"
)

# --- d. Dep Lib: CArgs ---
add_library(cargs_lib
    SHARED
    IMPORTED
    GLOBAL
)
set_target_properties(cargs_lib
    PROPERTIES
        IMPORTED_LOCATION
            "${SHERPA_ROOT}/lib/libcargs.so"
)

# --- e. Setup Dependent Chain ---
set_target_properties(sherpa_onnx_lib
    PROPERTIES
        INTERFACE_LINK_LIBRARIES
            "sherpa_c_api_lib;onnxruntime_lib;cargs_lib"
)

# --- f. Create Alias (External Interface) ---
add_library(${PROJECT_NAMESPACE}::ThirdParty::SherpaOnnx
    ALIAS
        sherpa_onnx_lib
)

# ---------------------------------
# V. Install Rule (lib)
#    Will Copy from "build/_deps/..." to "install/lib"
# ---------------------------------
file(
    GLOB
        ALL_SO_FILES
            "${SHERPA_ROOT}/lib/*.so*"
)
install(
    FILES
        ${ALL_SO_FILES}
    DESTINATION
        lib
    COMPONENT
        runtime
)

# ---------------------------------
# VI. Install Rule (include)
#    Will Copy from "build/_deps/..." to "install/include"
# ---------------------------------
if(EXISTS "${SHERPA_ROOT}/include")
    install(
        DIRECTORY "${SHERPA_ROOT}/include/"
        DESTINATION include
        COMPONENT headers
    )
endif()

# ---------------------------------
# VII. Install Rule (bin)
#    Will Copy from "build/_deps/..." to "install/bin"
# ---------------------------------
if(EXISTS "${SHERPA_ROOT}/bin")
    file(
        GLOB
            ALL_BIN_FILES
                "${SHERPA_ROOT}/bin/*"
    )

    # Default to OFF (base on .env file on the top directory of repository)
    string(TOUPPER "${ARC_INSTALL_SHERPA_TOOLS}" _upper_val)
    if(_upper_val STREQUAL "ON")
        message(STATUS "ARC_INSTALL_SHERPA_TOOLS=ON SherpaOnnx bin will be installed")
        install(
            PROGRAMS
                ${ALL_BIN_FILES}
            DESTINATION
                bin/tools
            COMPONENT
                tools
        )
    endif()
endif()

# ---------------------------------
#               End
# ---------------------------------
