// Copyright (c) 2025 PotterWhite
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#include "Utils/common/system-info.h"

// 关键：包含由 CMake 在第 3 步生成的头文件
#include "system-info.h"

namespace arcforge {
namespace embedded {
namespace utils {

// 实现公共接口函数
std::string get_utils_library_version() {
	// 使用 CMake 生成的宏返回版本字符串
	return VERSION_STRING;
}

std::string get_utils_library_author() {
	// 使用 CMake 生成的宏返回版本字符串
	return AUTHOR_NAME;
}

std::string get_utils_library_author_email() {
	// 使用 CMake 生成的宏返回版本字符串
	return AUTHOR_EMAIL;
}

std::string get_utils_library_build_timestamp() {
	// 使用 CMake 生成的宏返回版本字符串
	return BUILD_TIMESTAMP;
}

// // 定义一个全局常量，以确保版本字符串被编译进二进制文件，
// // 这样 'strings' 命令就能找到它。
// const char* g_internal_version_string_for_binary_scan = VERSION_STRING;
// const char* g_internal_author_string_for_binary_scan = AUTHOR_NAME;
// const char* g_internal_author_email_string_for_binary_scan = AUTHOR_EMAIL;
// const char* g_internal_build_timestamp_string_for_binary_scan = BUILD_TIMESTAMP;

}  // namespace utils
}  // namespace embedded
}  // namespace arcforge