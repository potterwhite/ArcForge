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


#include "Network/server/server.h"
#include "Utils/logger/logger.h"

namespace arcforge {
namespace embedded {
namespace network_socket {

ServerBase::ServerBase() {
	// 基类Base已经初始化了socket相关资源
}

ServerBase::~ServerBase() {
	// 基类Base的析构函数会处理socket关闭

	// 1. 调用 unlinkSocketPath() 来删除文件系统中的 socket 路径。
	//    这是 ServerBase 特有的职责。
	//    注意：unlinkSocketPath() 内部应该使用 getSocketPath() 来获取路径。
	//         或者，如果 ServerBase 自己也存了一份 path，就用自己的。
	//         从你的 server.cpp 实现来看，unlinkSocketPath 似乎使用了成员变量 path，
	//         而 startServer 使用了 getSocketPath()。需要统一。
	//         假设 getSocketPath() 是从 Base 继承的，并且返回正确的路径。
	std::ostringstream ss;
	ss << "[ServerBase PID:" << getpid()
	   << "] ServerBase destructor called. Attempting to unlink socket path.";
	arcforge::embedded::utils::Logger::GetInstance().Info(ss.str(), kcurrent_lib_name);

	unlinkSocketPath();  // 调用成员函数来执行 unlink

	// 2. 不需要显式调用 close(socketfd_) 或 this->closeSocket()。
	//    因为 ServerBase 继承自 Base (而 Base 可能使用 BaseImpl)，
	//    Base 的析构函数 (最终是 BaseImpl 的析构函数) 会被自动调用，
	//    并且它会负责关闭 socketfd_。
	//    重复关闭已经关闭的 fd (或者一个无效 fd) 通常是无害的，但没必要。
	ss.clear();
	ss << "[ServerBase PID:" << getpid()
	   << "] ServerBase destructor finished. Base destructor will handle fd closure.";
	arcforge::embedded::utils::Logger::GetInstance().Info(ss.str(), kcurrent_lib_name);
}

}  // namespace network_socket
}  // namespace embedded
}  // namespace arcforge