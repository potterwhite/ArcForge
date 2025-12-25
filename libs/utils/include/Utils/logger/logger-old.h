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

enum class LoggerLevel {
	kdebug = 0x10,
	kinfo = 0x11,
	kwarning = 0x12,
	kerror = 0x13,
	kcritical = 0x14
};

class Logger {
   public:
	static Logger& GetInstance();

	~Logger();

	// copy
	Logger(const Logger&) = delete;
	Logger& operator=(const Logger&) = delete;

	// std::move
	Logger(Logger&&) = delete;
	Logger& operator=(Logger&&) = delete;

	// ----------------------------------------------
	void setLevel(LoggerLevel lv);

	// ----------------------------------------------
	void BatchDebugLog(const std::vector<std::string>& messages);
	void MultiLineDebugLog(const std::string& multiline_message);
	void Log(const std::string& message);
	void Log(const std::string& message, const std::string& color);
	void Log(const std::string& message, const std::string& color, const std::string& background);
	void Log(const std::string& message, const std::string& color, const std::string& background,
	         const std::string& font);
	void Log(const std::string& message, const std::string& color, const std::string& background,
	         const std::string& font, const std::string& style);

	void DebugLog(const std::string& message);
	void DebugLog(const std::string& message, const std::string& color);
	void DebugLog(const std::string& message, const std::string& color,
	              const std::string& background);
	void DebugLog(const std::string& message, const std::string& color,
	              const std::string& background, const std::string& font);
	void DebugLog(const std::string& message, const std::string& color,
	              const std::string& background, const std::string& font, const std::string& style);

	void WarningLog(const std::string& message);

	void ErrorLog(const std::string& message);

	void CriticalLog(const std::string& message);

   private:
	Logger();

   private:
	// 互斥锁保护日志输出，确保线程安全
	std::mutex log_mutex_;
	std::queue<std::string> log_queue_;
	std::condition_variable log_cv_;
	LoggerLevel loggerlevel_;
};

}  // namespace utils
}  // namespace embedded
}  // namespace arcforge