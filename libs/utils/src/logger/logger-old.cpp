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

#include "Utils/logger/logger.h"

namespace arcforge {
namespace embedded {
namespace utils {

// 初始化静态互斥锁
// std::mutex Logger::log_mutex_;
// std::queue<std::string> Logger::log_queue_;
// std::condition_variable Logger::log_cv_;

Logger& Logger::GetInstance() {
	static Logger instance;

	return instance;
}

Logger::Logger() {
	std::cout << "Logger initialized in constructor." << std::endl;
}

Logger::~Logger() {
	std::cout << "Logger uninitialized in deconstructor." << std::endl;
}

void Logger::setLevel(LoggerLevel lv) {
	std::lock_guard<std::mutex> lock(log_mutex_);

	loggerlevel_ = lv;
}

/****************************************************************
*                           Log
****************************************************************/
void Logger::Log(const std::string& message) {
	if (loggerlevel_ <= LoggerLevel::kinfo) {
		std::lock_guard<std::mutex> lock(log_mutex_);
		std::cout << "\033[0;37m"
		          << "qtlog: " << message << "\033[0m" << std::endl;  // White color
	}
}

void Logger::Log(const std::string& message, const std::string& color) {
	if (loggerlevel_ <= LoggerLevel::kinfo) {
		std::lock_guard<std::mutex> lock(log_mutex_);
		std::cout << "\033[" << color << "m"
		          << "qtlog: " << message << "\033[0m" << std::endl;
	}
}

void Logger::Log(const std::string& message, const std::string& color,
                 const std::string& background) {
	if (loggerlevel_ <= LoggerLevel::kinfo) {
		std::lock_guard<std::mutex> lock(log_mutex_);
		std::cout << "\033[" << color << ";" << background << "m"
		          << "qtlog: " << message << "\033[0m" << std::endl;
	}
}

void Logger::Log(const std::string& message, const std::string& color,
                 const std::string& background, const std::string& font) {
	if (loggerlevel_ <= LoggerLevel::kinfo) {
		std::lock_guard<std::mutex> lock(log_mutex_);
		std::cout << "\033[" << color << ";" << background << ";" << font << "m"
		          << "qtlog: " << message << "\033[0m" << std::endl;
	}
}

void Logger::Log(const std::string& message, const std::string& color,
                 const std::string& background, const std::string& font, const std::string& style) {
	if (loggerlevel_ <= LoggerLevel::kinfo) {
		std::lock_guard<std::mutex> lock(log_mutex_);
		std::cout << "\033[" << color << ";" << background << ";" << font << ";" << style << "m"
		          << "qtlog: " << message << "\033[0m" << std::endl;
	}
}

/****************************************************************
*                          Debug Log
****************************************************************/

void Logger::BatchDebugLog([[maybe_unused]] const std::vector<std::string>& messages) {
	if (loggerlevel_ <= LoggerLevel::kdebug) {
		std::lock_guard<std::mutex> lock(log_mutex_);
		for (const auto& msg : messages) {
			std::cout << "\033[0;36m"
			          << "...qtdebug: " << msg << "\033[0m" << std::endl;
		}
	}
}

void Logger::MultiLineDebugLog([[maybe_unused]] const std::string& multiline_message) {
	if (loggerlevel_ <= LoggerLevel::kdebug) {
		{
			std::lock_guard<std::mutex> lock(log_mutex_);

			std::istringstream stream(multiline_message);
			std::string line;
			while (std::getline(stream, line)) {
				std::cout << "\033[0;36m"
				          << "...qtdebug: " << line << "\033[0m" << std::endl;
			}
		}
		// {
		// 	std::lock_guard<std::mutex> lock(log_mutex_);
		// 	log_queue_.push(multiline_message);

		// 	if (!log_queue_.empty()) {
		// 		std::string& msg = log_queue_.front();
		// 		std::istringstream stream(msg);
		// 		std::string line;
		// 		while (std::getline(stream, line)) {
		// 			std::cout << "\033[0;36m"
		// 			          << "...qtdebug: " << line << "\033[0m" << std::endl;
		// 		}
		// 	}

		// 	log_cv_.notify_one();
		// }
	}
}

void Logger::DebugLog([[maybe_unused]] const std::string& message) {
	if (loggerlevel_ <= LoggerLevel::kdebug) {
		std::lock_guard<std::mutex> lock(log_mutex_);
		std::cout << "\033[0;36m"
		          << "...qtdebug: " << message << "\033[0m" << std::endl;  // Cyan color
	}
}

void Logger::DebugLog([[maybe_unused]] const std::string& message,
                      [[maybe_unused]] const std::string& color) {
	if (loggerlevel_ <= LoggerLevel::kdebug) {
		std::lock_guard<std::mutex> lock(log_mutex_);
		std::cout << "\033[" << color << "m"
		          << "...qtdebug: " << message << "\033[0m" << std::endl;
	}
}

void Logger::DebugLog([[maybe_unused]] const std::string& message,
                      [[maybe_unused]] const std::string& color,
                      [[maybe_unused]] const std::string& background) {
	if (loggerlevel_ <= LoggerLevel::kdebug) {
		std::lock_guard<std::mutex> lock(log_mutex_);
		std::cout << "\033[" << color << ";" << background << "m"
		          << "...qtdebug: " << message << "\033[0m" << std::endl;
	}
}

void Logger::DebugLog([[maybe_unused]] const std::string& message,
                      [[maybe_unused]] const std::string& color,
                      [[maybe_unused]] const std::string& background,
                      [[maybe_unused]] const std::string& font) {
	if (loggerlevel_ <= LoggerLevel::kdebug) {
		std::lock_guard<std::mutex> lock(log_mutex_);
		std::cout << "\033[" << color << ";" << background << ";" << font << "m"
		          << "...qtdebug: " << message << "\033[0m" << std::endl;
	}
}

void Logger::DebugLog([[maybe_unused]] const std::string& message,
                      [[maybe_unused]] const std::string& color,
                      [[maybe_unused]] const std::string& background,
                      [[maybe_unused]] const std::string& font,
                      [[maybe_unused]] const std::string& style) {
	if (loggerlevel_ <= LoggerLevel::kdebug) {
		std::lock_guard<std::mutex> lock(log_mutex_);
		std::cout << "\033[" << color << ";" << background << ";" << font << ";" << style << "m"
		          << "...qtdebug: " << message << "\033[0m" << std::endl;
	}
}

/****************************************************************
*                          Warning Log
****************************************************************/
void Logger::WarningLog(const std::string& message) {
	if (loggerlevel_ <= LoggerLevel::kwarning) {
		std::lock_guard<std::mutex> lock(log_mutex_);
		std::cout << "\033[1;33m"
		          << "...qt warning: " << message << "\033[0m" << std::endl;  // Bold yellow color
	}
}

/****************************************************************
*                          Error Log
****************************************************************/
void Logger::ErrorLog(const std::string& message) {
	if (loggerlevel_ <= LoggerLevel::kerror) {
		std::lock_guard<std::mutex> lock(log_mutex_);
		std::cout << "\033[1;31m"
		          << "...qt error: " << message << "\033[0m" << std::endl;  // Bold red color
	}
}

/****************************************************************
*                          Critical Log
****************************************************************/
void Logger::CriticalLog(const std::string& message) {
	if (loggerlevel_ <= LoggerLevel::kcritical) {
		std::lock_guard<std::mutex> lock(log_mutex_);
		std::cout << "\033[1;37;41m"
		          << "...qt critical: " << message << "\033[0m"
		          << std::endl;  // Bold white text on red background
	}
}

}  // namespace utils
}  // namespace embedded
}  // namespace arcforge