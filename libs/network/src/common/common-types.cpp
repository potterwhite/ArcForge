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

#include "Network/base/base.h"
#include "Network/common/common-types.h"

namespace arcforge {
namespace embedded {
namespace network_socket {

const std::string_view kcurrent_lib_name = PROJECT_NAME;

// 1. 每一个 member to string 的函数
std::string SocketReturnValueToString(SocketReturnValue value) {
	switch (value) {
		case SocketReturnValue::ksuccess:
			return "ksuccess (0x49)";
		// --- recv opts errors ---
		case SocketReturnValue::kreceived_null:
			return "kreceived_null (0x50)";
		case SocketReturnValue::kreceived_illegal:
			return "kreceived_illegal (0x51)";
		case SocketReturnValue::kreceivelength_failed:
			return "kreceivelength_failed (0x52)";
		// --- send opts errors ---
		case SocketReturnValue::ksendcount_failed:
			return "ksendcount_failed (0x60)";
		case SocketReturnValue::ksenddata_failed:
			return "ksenddata_failed (0x61)";
		case SocketReturnValue::ksendlength_failed:
			return "ksendlength_failed (0x62)";
		// --- configuration errors ---
		case SocketReturnValue::kcount_too_large:
			return "kcount_too_large (0x70)";
		case SocketReturnValue::kempty_string:
			return "kempty_string (0x71)";
		case SocketReturnValue::kfd_illegal:  // 注意: 这个成员的值在新的 enum 中是 0x72
			return "kfd_illegal (0x72)";
		case SocketReturnValue::keof:
			return "keof (0x73)";
		case SocketReturnValue::ksocketpath_empty:
			return "ksocketpath_empty (0x74)";
		// --- posix api errors ---
		case SocketReturnValue::kconnect_server_failed:
			return "kconnect_server_failed (0x80)";
		case SocketReturnValue::klisten_error:
			return "klisten_error (0x81)";
		case SocketReturnValue::kbind_error:
			return "kbind_error (0x82)";
		case SocketReturnValue::kpeer_abnormally_closed:
			return "kpeer_abnormally_closed (0x83)";
		case SocketReturnValue::kaccept_timeout:  // 新增成员
			return "kaccept_timeout (0x84)";
		case SocketReturnValue::ksetsocketopt_error:  // 新增成员
			return "ksetsocketopt_error (0x85)";
		// --- impl layer errors ---
		case SocketReturnValue::kimpl_nullptr_error:
			return "kimpl_nullptr_error (0x90)";
		// ***************************
		case SocketReturnValue::kinit_state:
			return "kinit_state (0xff)";
		case SocketReturnValue::kunknownerror:
			return "kunknownerror (0x100)";
		// case SocketReturnValue::ksuccess_with_warning: return "ksuccess_with_warning (0xYY)"; // 示例未来扩展
		default:
			// 将int值转换为十六进制字符串，以便调试未知值
			char hex_string[20];
			// 使用 std::snprintf 需要 <cstdio>
			std::snprintf(hex_string, sizeof(hex_string), "Unknown (0x%x)",
			              static_cast<int>(value));
			return hex_string;
	}
}

// 2. 判断是否 success 的函数
// 未来有多个 options 表示 success，可以在这里添加
bool SocketReturnValueIsSuccess(SocketReturnValue value) {
	switch (value) {
		case SocketReturnValue::ksuccess:
			// 示例：如果未来有其他成功状态
			// case SocketReturnValue::ksuccess_with_warning:
			return true;
		// --- recv opts errors ---
		case SocketReturnValue::kreceived_null:
		case SocketReturnValue::kreceived_illegal:
		case SocketReturnValue::kreceivelength_failed:
		// --- send opts errors ---
		case SocketReturnValue::ksendcount_failed:
		case SocketReturnValue::ksenddata_failed:
		case SocketReturnValue::ksendlength_failed:
		// --- configuration errors ---
		case SocketReturnValue::kcount_too_large:
		case SocketReturnValue::kempty_string:
		case SocketReturnValue::kfd_illegal:
		case SocketReturnValue::keof:
		case SocketReturnValue::ksocketpath_empty:
		// --- posix api errors ---
		case SocketReturnValue::kconnect_server_failed:
		case SocketReturnValue::klisten_error:
		case SocketReturnValue::kbind_error:
		case SocketReturnValue::kpeer_abnormally_closed:
		case SocketReturnValue::kaccept_timeout:
		case SocketReturnValue::ksetsocketopt_error:
			// --- impl layer errors ---
		case SocketReturnValue::kimpl_nullptr_error:
		// ***************************
		case SocketReturnValue::kinit_state:
		case SocketReturnValue::kunknownerror:
		default:  // 所有未明确指定为 success 的情况（包括上面列出的错误和任何未定义的值）都返回 false
			return false;
	}
}

SocketAcceptReturn::SocketAcceptReturn()
    : return_value(SocketReturnValue::kinit_state), client(nullptr){};

SocketAcceptReturn::SocketAcceptReturn(SocketReturnValue value, std::unique_ptr<Base> client_ptr)
    : return_value(value), client(std::move(client_ptr)){};

}  // namespace network_socket
}  // namespace embedded
}  // namespace arcforge