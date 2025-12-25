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
# src/common/CMakeLists.txt
#

# 列出这个模块的源文件 (如果有 .cpp 文件)
set(COMMON_SOURCES
    "${CMAKE_CURRENT_SOURCE_DIR}/common-types.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/system-info.cpp"
)

# 将源文件列表设置到父作用域，以便顶层 CMakeLists.txt 可以使用它
#set(LIB_SOURCES ${LIB_SOURCES} ${COMMON_SOURCES} PARENT_SCOPE)

target_sources(${PROJECT_NAME}
    PRIVATE
        ${COMMON_SOURCES}
)

