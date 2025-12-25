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

// 文件: base/base.cpp
#include "Network/base/base.h"            // 包含你更新后的公共头文件 (base.h)
#include "Network/base/impl/base-impl.h"  // 包含PIMPL实现类的头文件 (注意路径)

// !! 不再需要在这里包含 <sherpa-onnx/...> 的头文件
// !! 也不再需要 using namespace sherpa_onnx::cxx;

namespace arcforge {
namespace embedded {
namespace network_socket {

// 构造函数: 创建 PIMPL 对象
Base::Base() : impl_(std::make_unique<BaseImpl>()) {
	// 原来的 cout "Base object constructed..." 应该移到 BaseImpl 的构造函数中
	// 或者如果你希望接口层也有日志，可以保留，但通常 Impl 构造时打日志更合适
}

Base::Base(std::unique_ptr<BaseImpl> param_impl) : impl_(std::move(param_impl)) {}

// 析构函数: 默认实现即可，std::unique_ptr 会自动处理 impl_ 的销毁
// 重要的是这个定义要放在 .cpp 文件中，这样编译器在销毁 impl_ 时能看到 BaseImpl 的完整定义
Base::~Base() {
	// 原来的 cout "Base cleaned up..." 应该移到 BaseImpl 的析构函数中
}

// 移动构造函数 (通常对于 PIMPL 是需要的)
// Base::Base(Base&& other) noexcept : impl_(std::move(other.impl_)) {
// 	// 移动构造函数应该保证noexcept，去掉抛出异常的代码
// 	// impl_的移动已经在初始化列表中完成
// }
Base::Base(Base&& other) noexcept = default;

// 移动赋值运算符 (通常对于 PIMPL 是需要的)
//     Base& Base::operator=(Base&& other) noexcept {
// 	if (this != &other) {
// 		impl_ = std::move(other.impl_);
// 		// std::cout << "Base move assigned." << std::endl;
// 	}
// 	return *this;
// }
Base& Base::operator=(Base&& other) noexcept = default;

SocketStatus Base::isSocketFDValid() const {
	if (impl_ != nullptr) {
		return impl_->isSocketFDValid_safe();
	}

	return SocketStatus::kunknowerror;
}

void Base::closeSocket() {
	if (impl_ != nullptr) {
		return impl_->closeSocket_safe();
	}
}

int Base::getFD() const {
	if (impl_ != nullptr) {
		return impl_->getFD_safe();
	}

	return -1;
}

void Base::setFD(int fd) {
	if (impl_ != nullptr) {
		impl_->setFD_safe(fd);
	}
}

const std::string& Base::getSocketPath() {
	if (impl_ != nullptr) {
		return impl_->getSocketPath_safe();
	}

	return k_empty_socket_path_sentinel;
}

void Base::setSocketPath(const std::string& path) {
	if (impl_ != nullptr) {
		impl_->setSocketPath_safe(path);
	}
}

SocketReturnValue Base::sendFloat(const std::vector<float>& data) {
	if (impl_) {  // Always check if impl_ is valid
		return impl_->sendFloat_safe(data);
	}
	// Handle cases where impl_ might be null (e.g., after a move-from operation)
	// You might log an error or return a specific status
	return SocketReturnValue::kimpl_nullptr_error;  // Or throw, or return an error status
}

SocketReturnValue Base::receiveFloat(std::vector<float>& data) {
	if (impl_) {
		return impl_->receiveFloat_safe(data);
	}
	return SocketReturnValue::kimpl_nullptr_error;
}

SocketReturnValue Base::sendString(const std::string& message) {
	if (impl_) {
		return impl_->sendString_safe(message);
	}
	return SocketReturnValue::kimpl_nullptr_error;
}

SocketReturnValue Base::receiveString(std::string& message) {
	if (impl_) {
		return impl_->receiveString_safe(message);
	}
	return SocketReturnValue::kimpl_nullptr_error;
}

// void Base::log(const std::string& msg) {
// 	if (impl_) {
// 		impl_->log_alert_safe(msg);
// 	}
// }

// void Base::log_warning(const std::string& msg) {
// 	if (impl_) {
// 		impl_->log_warning_safe(msg);
// 	}
// }

// void Base::log_error(const std::string& msg) {
// 	if (impl_) {
// 		impl_->log_error_safe(msg);
// 	}
// }

// void Base::log_alert(const std::string& msg) {
// 	if (impl_) {
// 		impl_->log_alert_safe(msg);
// 	}
// }

SocketReturnValue Base::connectToServer() {
	if (impl_) {
		return impl_->connectToServer();
	}
	return SocketReturnValue::kimpl_nullptr_error;
}

SocketReturnValue Base::startServer(const size_t& timeout) {
	if (impl_) {
		return impl_->startServer(timeout);
	}
	return SocketReturnValue::kimpl_nullptr_error;
}

SocketAcceptReturn Base::acceptClient() {
	if (impl_ == nullptr) {
		return {SocketReturnValue::kimpl_nullptr_error, nullptr};
	}

	SocketAcceptImplReturn impl_return;
	impl_return = impl_->acceptClient();
	if (SocketReturnValueIsSuccess(impl_return.return_value_impl) == false) {
		return {impl_return.return_value_impl, nullptr};
	}

	// how to package baseimpl into base object???
	auto client_connection = std::unique_ptr<Base>(new Base(std::move(impl_return.client_impl)));

	return {impl_return.return_value_impl, std::move(client_connection)};
}

SocketReturnValue Base::unlinkSocketPath() {
	if (impl_) {
		return impl_->unlinkSocketPath();
	}
	return SocketReturnValue::kimpl_nullptr_error;
}

}  // namespace network_socket
}  // namespace embedded
}  // namespace arcforge