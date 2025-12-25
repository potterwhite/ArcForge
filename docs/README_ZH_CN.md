

<div align="center">
  <h1>ArcForge 自动语音识别（ASR）软件开发包</h1>
  <p><i>高性能C++ 自动语音识别模型推理</i></p>
</div>

<p align="center">
  <!-- 这里可以放你生成好的 Banner 图片 -->
  <img src="https://github.com/potterwhite/ArcForge/blob/8afe0dc3f26d82b3643dba5d9dc1fd3070b95b2d/docs/asset/banner-light.jpeg" alt="Project Banner" width="100%"/>
</p>

<p align="center">
  <a href="#"><img src="https://img.shields.io/badge/standard-C++17-blue" alt="C++ Standard"></a>
  <a href="#"><img src="https://img.shields.io/badge/platform-Linux%20%7C%20Embedded-orange" alt="Platform"></a>
  <a href="#"><img src="https://img.shields.io/badge/design-Convention%20over%20Configuration-success" alt="Design Philosophy"></a>
  <a href="#"><img src="https://img.shields.io/badge/build-Automated%20CMake-blueviolet" alt="Build System"></a>
</p>

<p align="center">
  <a href="../README.md">English</a> | <strong>简体中文</strong>
</p>

<p align="center">
  <a href="#-快速上手-quick-start">🚀 快速上手</a> •
  <a href="#-构建命令速查-command-cheatsheet">⌨️ 构建命令速查</a> •
  <a href="#-产物说明-output">📂 产物说明</a>
</p>

---

> ### ⚠️ 交叉编译注意事项（Cross-Compilation Notice）
> **Env Configuration is Critical.** 本项目强依赖 `.env` 文件来定位交叉编译工具链。如果遇到 `gcc not found` 或类似错误，请直接检查你的 `.env` 配置是否正确加载。

### ✨ 核心功能（Core Features）

-   **⚡ 高性能推理（High-Performance Inference）**: 目前代码使用 `sherpa-zipformer` 模型进行ASR推理，并测试通过，是专为边缘计算进行整合的 ASR 推理引擎。
-   **🛠️ 现代CMake架构（Modern CMake Architecture）**: 采用命名空间隔离（`Namespace::Lib` 如 `Arcforge::Utils`），构建时自动重命名产物（如 `libarcforge_utils.so`），确保符号安全。
-   **📦 依赖隔离（Dependency Isolation）**: 通过 `.env` 实现本地环境与源码解耦，支持第三方依赖（如 sherpa-onnx）的离线缓存与自动下载。
-   **🔌 CS应用程序范例（Client/Server Ready）**: 内置完整的 C/S 架构示例，服务端包含 NPU 绑定逻辑，客户端提供标准调用接口。

---

### 🚀 快速上手 (Quick Start)

本项目采用 Modern CMake 管理构建，并通过 `.env` 文件隔离本地环境差异。

#### Step 1: 环境配置 (Configuration)

在开始编译前，必须建立本地配置文件。该文件已被 git 忽略，用于适配不同开发者的本地路径。

```bash
# 1. 从模板复制配置文件
cp .env.example .env

# 2. 编辑 .env 文件，填入本地 SDK 和缓存路径
vim .env
```

**关键配置项说明：**

*   `ARC_RV1126BP_SDK_ROOT`: **(必填)** 交叉编译工具链（Buildroot SDK）在宿主机上的绝对路径。
*   `ARC_DEP_CACHE_DIR`: **(可选)** 第三方依赖包（如 `sherpa-onnx`）的本地缓存目录。
    *   若配置该路径且目录下存在对应 tarball，构建系统将优先使用本地文件（离线模式）。
    *   若未配置或文件不存在，将自动从 GitHub Release 下载。

#### Step 2: 编译构建 (Build)

项目提供统一构建脚本 `build.sh`，封装了 CMake Presets 逻辑。

**基础语法：**
```bash
./build.sh <Action> [Platform] [BuildType]
```

**常用场景流程：**

1.  **全量构建 (Clean Build)**: 适用于首次构建或环境变更后。
    ```bash
    ./build.sh cb rv1126bp
    ```
2.  **开发调试 (Incremental)**: 修改代码后的增量编译。
    ```bash
    ./build.sh build rv1126bp
    ```
3.  **本机测试 (Native)**: 在 x86 主机上验证业务逻辑。
    ```bash
    ./build.sh cb native
    ```

---

### ⌨️ 构建命令速查 (Command Cheatsheet)

| Action | Command | Description |
| :--- | :--- | :--- |
| **Release Build** | | |
| Clean Build (RV1126) | `./build.sh cb rv1126bp` | 清理并编译 Release 版本，包含 SDK 链接 |
| Incremental Build | `./build.sh build rv1126bp` | 仅编译修改过的文件，速度最快 |
| **Debug Build** | | |
| Debug Mode (RV1126) | `./build.sh cb rv1126bp debug` | 开启调试符号，未优化版本 |
| Native Debug (x86) | `./build.sh cb native debug` | 使用本机 GCC/Clang 编译，用于逻辑验证 |
| **Maintenance** | | |
| Clean All | `./build.sh cleanall` | 删除所有 build 目录和 install 产物 |

---

### 📂 产物说明 (Output)

构建完成后，所有产物将遵循重命名规则（`libarcforge_*.so`）安装至 `install/` 目录下：

```text
install/
└── rv1126bp/
    └── debug/
        ├── bin/
        │   ├── ArcForge_ASR_Client   # 客户端应用
        │   └── ArcForge_ASR_Server   # 服务端应用 (含 NPU 绑定)
        ├── lib/
        │   ├── libarcforge_core.so   # 核心业务库 (原 Arcforge::Core)
        │   ├── libarcforge_utils.so  # 工具库 (原 Arcforge::Utils)
        │   └── libsherpa-onnx*.so    # 第三方依赖库
        └── include/                  # SDK 头文件
```

### ⚠️ 限制与规划（Limitations & Roadmap）

-   **Network & Caching (网络环境)**:
    -   构建系统默认尝试从 GitHub 拉取第三方依赖（如 `sherpa-onnx`）。
    -   鉴于网络环境的不稳定性，**建议** 配置 `.env` 中的 `ARC_DEP_CACHE_DIR` 并预先放入依赖包，以确保构建的稳定性（即“离线优先”模式）。确切的包地址请查看CMakePreset.json。

