
所有的libs都是以Namespace::Lib的格式命名，例如Arcforge::Utils
但输出的时候，我加长了最终文件的名称长度，例如：libarcforge_utils.so

如果出现找不到gcc（或类似）的情况，请直接去看.env，配置是由./build.sh加载.env，然后


---

## 快速上手 (Quick Start)

本项目采用 Modern CMake 管理构建，并通过 `.env` 文件隔离本地环境差异。

### 1. 环境配置 (Configuration)

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

### 2. 编译构建 (Build)

项目提供统一构建脚本 `build.sh`，封装了 CMake Presets 逻辑。

**基础语法：**
```bash
./build.sh <Action> [Platform] [BuildType]
```

**常用命令示例：**

*   **全量构建与安装 (Clean, Build & Install)**
    适用于首次构建或环境变更后。
    ```bash
    # 编译 RV1126BP 版本 (Release)
    ./build.sh cb rv1126bp

    # 编译 RV1126BP 版本 (Debug)
    ./build.sh cb rv1126bp debug
    ```

*   **增量编译 (Incremental Build)**
    适用于开发过程中的修改调试。
    ```bash
    ./build.sh build rv1126bp
    ```

*   **本机编译 (Native Host)**
    用于在 x86/Linux 主机上调试业务逻辑。
    ```bash
    ./build.sh cb native
    ```

*   **清理构建 (Clean)**
    ```bash
    ./build.sh cleanall
    ```

### 3. 产物说明 (Output)

构建完成后，所有产物将安装至 `install/` 目录下：

```text
install/
└── rv1126bp/
    └── debug/
        ├── bin/
        │   ├── ArcForge_ASR_Client   # 客户端应用
        │   └── ArcForge_ASR_Server   # 服务端应用 (含 NPU 绑定)
        ├── lib/
        │   ├── libArcForge_*.so      # 核心业务库
        │   └── libsherpa-onnx*.so    # 第三方依赖库
        └── include/                  # SDK 头文件
```

---


