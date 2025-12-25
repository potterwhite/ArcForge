/*
 * Copyright (c) 2025 PotterWhite
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#pragma once

#include "Utils/pch.h"

namespace arcforge {
namespace embedded {
namespace utils {

// 前向声明 LoggerLevel，避免与 logger.h 循环引用
enum class LoggerLevel;

// 日志条目结构体，封装一条日志的所有原始信息
struct LogEntry {
	std::chrono::system_clock::time_point timestamp;
	LoggerLevel level;
	std::string message;
	std::string_view tag;  // <--- 2. 添加成员
	                       // 未来可扩展：
	                       // std::thread::id thread_id;
	                       // const char* file;
	                       // int line;
};

}  // namespace utils
}  // namespace embedded
}  // namespace arcforge