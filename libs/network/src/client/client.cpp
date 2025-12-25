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

#include "Network/client/client.h"

// #include <sys/socket.h>
// #include <sys/un.h>
// #include <unistd.h>

namespace arcforge {
namespace embedded {
namespace network_socket {

ClientBase::ClientBase() {
	// Base类构造函数已经处理了基础初始化
}

ClientBase::~ClientBase() {
	// Base类析构函数会处理socket关闭
}

// SocketReturnValue ClientBase::connectToServer() {
// 	// 创建Unix域socket
// 	int sock_fd = socket(AF_UNIX, SOCK_STREAM, 0);
// 	if (sock_fd < 0) {
// 		return SocketReturnValue::kfd_illegal;
// 	}

// 	// socket path verification
// 	if (getSocketPath().empty() == true) {
// 		std::cerr << "Socket path is empty, pls set it before you connect to server." << std::endl;
// 		return SocketReturnValue::ksocketpath_empty;
// 	}

// 	// 设置服务器地址
// 	struct sockaddr_un server_addr;
// 	memset(&server_addr, 0, sizeof(server_addr));
// 	server_addr.sun_family = AF_UNIX;
// 	strncpy(server_addr.sun_path, getSocketPath().c_str(), sizeof(server_addr.sun_path) - 1);

// 	// 连接到服务器
// 	if (connect(sock_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
// 		perror("Client connect failed");
// 		close(sock_fd);
// 		return SocketReturnValue::kconnect_server_failed;
// 	}

// 	this->setFD(sock_fd);  // <--- 关键: ClientBase (作为Base的子类) 设置其连接FD
// 	// TODO: 需要设置sock_fd到Base类
// 	return SocketReturnValue::ksuccess;
// }

}  // namespace network_socket
}  // namespace embedded
}  // namespace arcforge
