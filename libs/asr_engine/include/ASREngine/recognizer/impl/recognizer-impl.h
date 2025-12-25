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

// 文件: recognizer/impl/recognizer-impl.h
#pragma once

#include "ASREngine/pch.h"
// #include <memory>  // For std::unique_ptr
// #include <string>
// #include <vector>

// 向前声明你自己的配置类，因为 Impl 需要它
#include "ASREngine/recognizer/recognizer-config.h"  // << 注意路径，相对于 impl 目录调整
    // 或者更常见的做法是在 CMake 中配置 include 路径
    // 使得可以直接 #include "recognizer-config.h"

// 向前声明 Sherpa-ONNX 的内部类型
namespace sherpa_onnx {
namespace cxx {
class OnlineRecognizer;
class OnlineStream;
}  // namespace cxx
}  // namespace sherpa_onnx

namespace arcforge {
namespace embedded {
namespace ai_asr {

class RecognizerImpl {
   public:
	RecognizerImpl();
	~RecognizerImpl();  // 确保在 .cpp 中定义

	bool Initialize(const SherpaConfig& user_config);
	void ProcessAudioChunk(const std::vector<float>& audio_chunk);
	void InputFinished();
	std::string GetCurrentText() ;
	bool IsEndpoint() const;
	void ResetStream();
	int GetExpectedSampleRate() const;

	// 禁止拷贝和赋值
	RecognizerImpl(const RecognizerImpl&) = delete;
	RecognizerImpl& operator=(const RecognizerImpl&) = delete;
	// 允许移动 (如果需要)
	RecognizerImpl(RecognizerImpl&&) noexcept;
	RecognizerImpl& operator=(RecognizerImpl&&) noexcept;

   private:
	// PIMPL for Sherpa-ONNX types, even within Impl's header for maximum cleanliness if needed
	// For unique_ptr members, we'll need to include their headers in .cpp
	std::unique_ptr<sherpa_onnx::cxx::OnlineRecognizer> recognizer_ptr_;
	std::unique_ptr<sherpa_onnx::cxx::OnlineStream> stream_ptr_;

	// 其他成员变量
	std::string last_displayed_text_;
	int expected_sample_rate_ = 16000;  // 默认值或从配置初始化
};

}  // namespace ai_asr
}  // namespace embedded
}  // namespace arcforge