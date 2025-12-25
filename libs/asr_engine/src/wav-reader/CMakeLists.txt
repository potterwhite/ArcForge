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
# sherpa-lib/src/wav-reader/CMakeLists.txt
#

set(WAV_READER_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/wav-reader.cpp")

#[[
set(WAV_READER_PUBLIC_HEADERS "${INCLUDE_DIR}/ASR/wav-reader/wav-reader.h")

# 上报给父级
set(LIB_SOURCES
	${LIB_SOURCES} ${WAV_READER_SOURCES}
	PARENT_SCOPE)
set(LIB_PUBLIC_HEADERS
	${LIB_PUBLIC_HEADERS} ${WAV_READER_PUBLIC_HEADERS}
	PARENT_SCOPE)

install(FILES ${WAV_READER_PUBLIC_HEADERS} DESTINATION include/${PROJECT_NAME}/wav-reader)
]]

target_sources(${PROJECT_NAME}
    PRIVATE
        ${WAV_READER_SOURCES}
)