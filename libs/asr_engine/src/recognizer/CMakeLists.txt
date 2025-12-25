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

#
# recognizer subdirectory CMakeLists.txt
#

set(RECOGNIZER_SOURCES
	"${CMAKE_CURRENT_SOURCE_DIR}/recognizer.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/recognizer-config.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/impl/recognizer-impl.cpp")

#[[
set(RECOGNIZER_PUBLIC_HEADERS "${INCLUDE_DIR}/ASR/recognizer/recognizer.h"
							  "${INCLUDE_DIR}/ASR/recognizer/recognizer-config.h")

# 上报给父级
set(LIB_SOURCES
	${LIB_SOURCES} ${RECOGNIZER_SOURCES}
	PARENT_SCOPE)
set(LIB_PUBLIC_HEADERS
	${LIB_PUBLIC_HEADERS} ${RECOGNIZER_PUBLIC_HEADERS}
	PARENT_SCOPE)

install(FILES ${RECOGNIZER_PUBLIC_HEADERS} DESTINATION include/${PROJECT_NAME}/recognizer)
# 如果 该 模块有特定的包含目录或编译定义，可以在这里设置给一个目标属性变量 target_include_directories( ${PARENT_LIBRARY_TARGET} PRIVATE
# some_vad_internal_include ) 但通常是父级聚合后统一设置给最终的库目标。 或者更简单地，父级通过
# $<TARGET_PROPERTY:tgt,INTERFACE_INCLUDE_DIRECTORIES> 来收集。 对于简单的源文件聚合，直接上报列表更常见。
]]

target_sources(${PROJECT_NAME}
    PRIVATE
        ${RECOGNIZER_SOURCES}
)
