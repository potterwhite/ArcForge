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

#include "ASREngine/wav-reader/wav-reader.h"
#include "Network/client/client.h"
#include "Utils/logger/logger.h"  // 引入日志库
#include "Utils/logger/worker/consolesink.h"  // 仅在配置时需要
#include "Utils/logger/worker/filesink.h"

#include <chrono>
#include <csignal>  // For signal handling
#include <iostream>
#include <sstream>  // For std::ostringstream
#include <string>
#include <thread>
#include <vector>

using namespace arcforge::embedded;
// using namespace arcforge::embedded::network_socket;

// 假设 kcurrent_lib_name 在此编译单元中可用
// 这里我们定义一个临时的，实际项目中可能来自别处
const std::string_view kcurrent_app_name = "test-client";

const std::string ksocket_path = "/tmp/soCket.paTh";
const int ksample_rate = 16000;
const int CHUNK_DURATION_MS = 800;  // 毫秒, 每次处理和sleep的时长

// --- 全局用于信号处理 ---
// static bool g_stop_signal_received = false;
static std::atomic<bool> g_stop_signal_received(false);  // 修改后的代码

void SignalHandler(int signal_num) {
	g_stop_signal_received = true;
	std::ostringstream oss;
	oss << "\nInterrupt signal (" << signal_num << ") received. Shutting down...";
	// 使用 Logger 而不是 cerr
	arcforge::embedded::utils::Logger::GetInstance().Warning(oss.str(), kcurrent_app_name);
}

bool isDebug() {
	constexpr std::string_view build_type = BUILD_TYPE;
	return build_type == "Debug";
}

bool isRelease() {
	constexpr std::string_view build_type = BUILD_TYPE;
	return build_type == "Release";
}

int main([[maybe_unused]] int argc, [[maybe_unused]] char* argv[]) {
	// 在主程序中，通常我们会配置 Logger。
	// 如果没有特殊配置，它会使用默认的 ConsoleSink。
	// auto& logger = arcforge::embedded::utils::Logger::GetInstance();
	// logger.setLevel(arcforge::embedded::utils::LoggerLevel::kdebug);

	//*****************************************************
	// logger level configuration
	// 获取 Logger 实例
	auto& logger = arcforge::embedded::utils::Logger::GetInstance();

	// 1. 设置日志级别 (例如，只记录 Warning 及以上)
	if (isRelease() == true) {
		logger.setLevel(arcforge::embedded::utils::LoggerLevel::kinfo);
	} else {
		logger.setLevel(arcforge::embedded::utils::LoggerLevel::kdebug);
	}

	// 2. 配置输出目标 (Sink)
	logger.ClearSinks();  // 清空默认的控制台输出
	logger.AddSink(std::make_shared<arcforge::embedded::utils::FileSink>(
	    "/root/my_app_client.log"));  // 添加一个文件输出
	logger.AddSink(std::make_shared<arcforge::embedded::utils::ConsoleSink>());

	// // ... 你的其他应用程序代码 ...
	// logger.Warning("Application has started.");

	//----------------------------------

	if (argc < 2) {
		std::ostringstream oss;
		oss << "Usage: " << argv[0] << " <path_to_input_wav_file>"
		    << "\n"
		    << "  Example: " << argv[0] << " full_audio_stream.wav";
		arcforge::embedded::utils::Logger::GetInstance().Error(oss.str(), kcurrent_app_name);
		return 1;
	}

	std::string wav_filepath = argv[1];

	// setup signal handler
	signal(SIGINT, SignalHandler);
	// signal(SIGTERM, SignalHandler);

	// init client object
	network_socket::ClientBase client;
	client.setSocketPath(ksocket_path);

	// connect to server
	network_socket::SocketReturnValue retval_flag = client.connectToServer();
	if (retval_flag > network_socket::SocketReturnValue::ksuccess) {
		arcforge::embedded::utils::Logger::GetInstance().Error("Client failed to connect to server.",
		                                                     kcurrent_app_name);
		exit(1);
	}

	// --- 2. 打开 WAV 文件 ---
	ai_asr::WavReader reader;
	if (!reader.Open(wav_filepath, ksample_rate, 1 /*expected channels*/)) {
		std::ostringstream oss;
		oss << "Failed to open WAV file: " << wav_filepath;
		arcforge::embedded::utils::Logger::GetInstance().Error(oss.str(), kcurrent_app_name);
		return 1;
	}

	{
		std::ostringstream oss;
		oss << "\nStarting client wav reading for '" << wav_filepath << "'..."
		    << "\n"
		    << "Processing audio in " << CHUNK_DURATION_MS << "ms chunks."
		    << "\n"
		    << "Press Ctrl+C to stop.\n";
		arcforge::embedded::utils::Logger::GetInstance().Info(oss.str(), kcurrent_app_name);
	}

	const size_t samples_per_chunk = static_cast<size_t>((ksample_rate * CHUNK_DURATION_MS) / 1000);
	std::vector<float> audio_chunk;  // 用于存储从WAV读取的样本

	// --- 3. 循环处理 ---
	while ((g_stop_signal_received == false) && (reader.Eof() == false)) {
		// 在每次循环开始时，我们作为调用者，主动准备好一块足够大的内存。
		//    这确保了无论 ReadSamples 的实现如何，我们提供的缓冲区总是安全的。
		audio_chunk.resize(samples_per_chunk);

		size_t samples_read = reader.ReadSamples(audio_chunk, samples_per_chunk);

		if (samples_read > 0) {
			{
				std::ostringstream oss;
				oss << "main-client.cpp: size of audio_chunk=" << audio_chunk.size();
				arcforge::embedded::utils::Logger::GetInstance().Debug(oss.str(), kcurrent_app_name);
			}

			//【关键步骤】根据实际读取的样本数，精确地截断 vector。
			//    这保证了我们发送给服务器的数据不多不少，正好是 samples_read 个样本。
			//    即使 ReadSamples 内部没有 resize，这行代码也能保证正确性。
			//    即使 ReadSamples 内部已经 resize 了，这行代码也只是做一次无害的重复操作。
			//    这就是防御性编程：不依赖对方，自己保证正确。
			if (samples_read < audio_chunk.size()) {
				audio_chunk.resize(samples_read);
			}

			//send float to server
			network_socket::SocketReturnValue retval = client.sendFloat(audio_chunk);
			if (retval > network_socket::SocketReturnValue::ksuccess) {
				arcforge::embedded::utils::Logger::GetInstance().Error(
				    "Client failed to send float data.", kcurrent_app_name);
				continue;
			}

			//receive string from server
			std::string result;
			retval = client.receiveString(result);
			// if (result.empty() == false) {
			{
				std::ostringstream oss;
				oss << "receive:" << result;
				arcforge::embedded::utils::Logger::GetInstance().Info(oss.str(), kcurrent_app_name);
			}
			// }
		}

		// else if (!reader.Eof()) {
		// 	// ReadSamples返回0但不是EOF，可能只是暂时没数据或很小的读取
		// 	// 短暂等待，避免忙等
		// 	std::this_thread::sleep_for(std::chrono::milliseconds(5));  // 短暂等待
		// 	arcforge::embedded::utils::Logger::GetInstance().Debug("read more wav samples", kcurrent_app_name);
		// 	continue;  // 重新尝试读取
		// }

	}  // end of while()

	if (g_stop_signal_received) {
		arcforge::embedded::utils::Logger::GetInstance().Info("\nProcessing stopped by user.",
		                                                    kcurrent_app_name);
	} else {
		arcforge::embedded::utils::Logger::GetInstance().Info(
		    "\nEnd of WAV file reached in main loop.", kcurrent_app_name);
		//-----------------------------------------------------
		// 发送一个空的音频块作为EOF标记
		std::vector<float> empty_chunk;
		client.sendFloat(empty_chunk);
		arcforge::embedded::utils::Logger::GetInstance().Info(
		    "!!!!!!!!!!!!!!!!!!!!!Sent EOF marker (empty chunk)", kcurrent_app_name);
	}

	return 0;
}