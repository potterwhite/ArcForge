<div align="center">
  <h1>ArcForge ASR SDK</h1>
  <p><i>High-Performance C++ Automatic Speech Recognition Inference Software</i></p>
</div>

<p align="center">
  <!-- Place your generated Banner image here -->
  <img src="https://github.com/potterwhite/ArcForge/blob/8afe0dc3f26d82b3643dba5d9dc1fd3070b95b2d/docs/asset/banner-light.jpeg" alt="Project Banner" width="100%"/>
</p>

<p align="center">
  <a href="#"><img src="https://img.shields.io/badge/standard-C++17-blue" alt="C++ Standard"></a>
  <a href="#"><img src="https://img.shields.io/badge/platform-Linux%20%7C%20Embedded-orange" alt="Platform"></a>
  <a href="#"><img src="https://img.shields.io/badge/design-Convention%20over%20Configuration-success" alt="Design Philosophy"></a>
  <a href="#"><img src="https://img.shields.io/badge/build-Automated%20CMake-blueviolet" alt="Build System"></a>
</p>

<p align="center">
  <strong>English</strong> | <a href="./docs/README_ZH_CN.md">简体中文</a>
</p>

<p align="center">
  <a href="#-quick-start">🚀 Quick Start</a> •
  <a href="#-command-cheatsheet">⌨️ Command Cheatsheet</a> •
  <a href="#-output-structure">📂 Output Structure</a>
</p>

---

> ### ⚠️ Cross-Compilation Notice
> **Env Configuration is Critical.** This project relies heavily on the `.env` file to locate the cross-compilation toolchain. If you encounter `gcc not found` or similar errors, please check if your `.env` configuration is loaded correctly.

### ✨ Core Features

-   **⚡ High-Performance Inference**: Currently uses the `sherpa-zipformer` model for ASR inference (tested and verified); designed as an integrated ASR inference engine for edge computing.
-   **🛠️ Modern CMake Architecture**: Adopts namespace isolation (`Namespace::Lib`, e.g., `Arcforge::Utils`). Automatically renames artifacts during the build process (e.g., `libarcforge_utils.so`) to ensure symbol safety.
-   **📦 Dependency Isolation**: Decouples the local environment from the source code via `.env`. Supports offline caching and automatic downloading of third-party dependencies (e.g., sherpa-onnx).
-   **🔌 Client/Server Ready**: Built-in complete C/S architecture examples. The Server includes NPU binding logic, while the Client provides standard calling interfaces.

---

### 🚀 Quick Start

This project uses Modern CMake for build management and isolates local environment differences via the `.env` file.

#### Step 1: Configuration

Before compiling, you must create a local configuration file. This file is git-ignored and used to adapt to different developers' local paths.

```bash
# 1. Copy config file from template
cp .env.example .env

# 2. Edit .env file, fill in local SDK and cache paths
vim .env
```

**Key Configuration Items:**

*   `ARC_RV1126BP_SDK_ROOT`: **(Required)** The absolute path to the cross-compilation toolchain (Buildroot SDK) on the host machine.
*   `ARC_DEP_CACHE_DIR`: **(Optional)** Local cache directory for third-party dependencies (e.g., `sherpa-onnx`).
    *   If this path is configured and the corresponding tarball exists, the build system will prefer the local file (offline mode).
    *   If not configured or the file is missing, it will automatically download from GitHub Releases.

#### Step 2: Build

The project provides a unified build script `build.sh`, which encapsulates the CMake Presets logic.

**Basic Syntax:**
```bash
./build.sh <Action> [Platform] [BuildType]
```

**Common Workflow Examples:**

1.  **Clean Build**: Suitable for first-time builds or after environment changes.
    ```bash
    ./build.sh cb rv1126bp
    ```
2.  **Incremental Build**: For debugging after code modifications.
    ```bash
    ./build.sh build rv1126bp
    ```
3.  **Native Test**: Verify business logic on an x86 host.
    ```bash
    ./build.sh cb native
    ```

---

### ⌨️ Command Cheatsheet

| Action | Command | Description |
| :--- | :--- | :--- |
| **Release Build** | | |
| Clean Build (RV1126) | `./build.sh cb rv1126bp` | Clean and compile Release version, includes SDK linking |
| Incremental Build | `./build.sh build rv1126bp` | Compile only modified files; fastest speed |
| **Debug Build** | | |
| Debug Mode (RV1126) | `./build.sh cb rv1126bp debug` | Enable debug symbols, unoptimized version |
| Native Debug (x86) | `./build.sh cb native debug` | Compile using host GCC/Clang, used for logic verification |
| **Maintenance** | | |
| Clean All | `./build.sh cleanall` | Delete all build directories and install artifacts |

---

### 📂 Output Structure

Upon build completion, all artifacts will be installed to the `install/` directory, following the renaming rules (`libarcforge_*.so`):

```text
install/
└── rv1126bp/
    └── debug/
        ├── bin/
        │   ├── ArcForge_ASR_Client   # Client Application
        │   └── ArcForge_ASR_Server   # Server Application (with NPU binding)
        ├── lib/
        │   ├── libarcforge_core.so   # Core Business Lib (was Arcforge::Core)
        │   ├── libarcforge_utils.so  # Utils Lib (was Arcforge::Utils)
        │   └── libsherpa-onnx*.so    # 3rd-party Dependency Lib
        └── include/                  # SDK Headers
```

### ⚠️ Limitations & Roadmap

-   **Network & Caching**:
    -   The build system defaults to attempting to pull third-party dependencies (e.g., `sherpa-onnx`) from GitHub.
    -   Given potential network instability, it is **recommended** to configure `ARC_DEP_CACHE_DIR` in `.env` and pre-populate it with dependency packages to ensure build stability ("Offline-First" mode). Please refer to `CMakePresets.json` for exact package URLs.
