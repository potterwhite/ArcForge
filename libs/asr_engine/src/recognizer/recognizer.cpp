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

// 文件: recognizer/recognizer.cpp
#include "ASREngine/recognizer/impl/recognizer-impl.h"  // 包含PIMPL实现类的头文件 (注意路径)
#include "ASREngine/recognizer/recognizer.h"  // 包含你更新后的公共头文件 (recognizer.h)
#include "Utils/logger/logger.h"

// !! 不再需要在这里包含 <sherpa-onnx/...> 的头文件
// !! 也不再需要 using namespace sherpa_onnx::cxx;

namespace arcforge {
namespace embedded {
namespace ai_asr {

// 构造函数: 创建 PIMPL 对象
Recognizer::Recognizer() : impl_(std::make_unique<RecognizerImpl>()) {
	// 原来的 cout "Recognizer object constructed..." 应该移到 RecognizerImpl 的构造函数中
	// 或者如果你希望接口层也有日志，可以保留，但通常 Impl 构造时打日志更合适
}

// 析构函数: 默认实现即可，std::unique_ptr 会自动处理 impl_ 的销毁
// 重要的是这个定义要放在 .cpp 文件中，这样编译器在销毁 impl_ 时能看到 RecognizerImpl 的完整定义
Recognizer::~Recognizer() {
	// 原来的 cout "Recognizer cleaned up..." 应该移到 RecognizerImpl 的析构函数中
}

// 移动构造函数 (通常对于 PIMPL 是需要的)
Recognizer::Recognizer(Recognizer&& other) noexcept : impl_(std::move(other.impl_)) {}

// 移动赋值运算符 (通常对于 PIMPL 是需要的)
Recognizer& Recognizer::operator=(Recognizer&& other) noexcept {
	if (this != &other) {
		impl_ = std::move(other.impl_);
	}
	return *this;
}

// 所有公共方法都委托给 impl_ 对象
bool Recognizer::Initialize(const SherpaConfig& config) {
	// 在调用 Initialize 之前，impl_ 应该已经被构造函数创建好了。
	// 如果构造函数可能失败并使 impl_ 为空，你需要错误处理。
	// 但 std::make_unique 如果失败会抛 bad_alloc，所以通常 impl_ 不会是空的，除非移动后。
	if (!impl_) {
		// 这是一个异常情况，可能源于一个被移动的 Recognizer 对象
		// 或者在未来如果构造函数变得更复杂。
		// 对于基本情况，构造函数会确保 impl_ 有效。
		// 可以考虑重新创建一个，或者抛出异常，或者返回false。
		// 为简单起见，这里假设 impl_ 有效，或者 Initialize 会处理。
		// 如果之前的 Recognizer 对象被移动了，impl_ 就会是 nullptr。
		// 调用 Initialize 的前提是对象是有效的。
		// 如果一个 Recognizer 对象被移动了，它的状态就是空的，不应该再调用方法。
		// 或者，如果允许在空 impl_ 上调用 Initialize，那么就需要在这里创建：
		// impl_ = std::make_unique<RecognizerImpl>(); // 如果允许 Initialize “重新激活”

		arcforge::embedded::utils::Logger::GetInstance().Error(
		    "Recognizer::Initialize called on a moved-from or uninitialized object's PIMPL.",
		    kcurrent_lib_name);
		return false;
	}
	return impl_->Initialize(config);
}

void Recognizer::ProcessAudioChunk(const std::vector<float>& audio_chunk) {
	if (impl_) {  // 总是在访问指针前检查是个好习惯
		impl_->ProcessAudioChunk(audio_chunk);
	} else {
		arcforge::embedded::utils::Logger::GetInstance().Error(
		    "Recognizer::ProcessAudioChunk called on a null PIMPL.", kcurrent_lib_name);
		// 或者抛出异常 std::runtime_error("Recognizer not initialized or moved-from");
	}
}

void Recognizer::InputFinished() {
	if (impl_) {
		impl_->InputFinished();
	} else {
		arcforge::embedded::utils::Logger::GetInstance().Error(
		    "Recognizer::InputFinished called on a null PIMPL.", kcurrent_lib_name);
	}
}

std::string Recognizer::GetCurrentText() const {
	if (impl_) {
		return impl_->GetCurrentText();
	}

	arcforge::embedded::utils::Logger::GetInstance().Error(
	    "Recognizer::GetCurrentText called on a null PIMPL.", kcurrent_lib_name);
	return "";  // 返回空字符串或抛异常
}

bool Recognizer::IsEndpoint() const {
	if (impl_) {
		return impl_->IsEndpoint();
	}

	arcforge::embedded::utils::Logger::GetInstance().Error(
	    "Recognizer::IsEndpoint called on a null PIMPL.", kcurrent_lib_name);
	return false;  // 返回 false 或抛异常
}

void Recognizer::ResetStream() {
	if (impl_) {
		impl_->ResetStream();
	} else {

		arcforge::embedded::utils::Logger::GetInstance().Error(
		    "Recognizer::ResetStream called on a null PIMPL.", kcurrent_lib_name);
	}
}

int Recognizer::GetExpectedSampleRate() const {
	if (impl_) {
		return impl_->GetExpectedSampleRate();
	}

	arcforge::embedded::utils::Logger::GetInstance().Error(
	    "Recognizer::GetExpectedSampleRate called on a null PIMPL.", kcurrent_lib_name);
	// 返回一个表示错误/未初始化的值，或者默认值
	// 这个默认值应该与 RecognizerImpl 中未初始化时的返回值一致，或根据业务逻辑决定
	return kdefault_sample_rate;  // 例如，返回一个通用的默认采样率
}

}  // namespace ai_asr
}  // namespace embedded
}  // namespace arcforge