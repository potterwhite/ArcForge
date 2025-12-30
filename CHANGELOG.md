# Changelog

All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

---

## v0.5.2 (2025-12-30)

### 🐛 Fixes
- **Docs**: Corrected broken relative hyperlinks in `README.md` and `docs/zh-cn/README_ZH_CN.md` that were pointing to incorrect file paths after the directory restructuring.

## v0.5.2 (2025-12-30)

### 📚 Documentation
- **Structure**: Major refactor of the `docs/` folder. Content is now split into `en/` and `zh-cn/` subdirectories to support better internationalization.
- **New Guide**: Added **"How to Add Support for a New SoC"** (`ADD_NEW_SOC.md`). This guide details the 5-step process for porting ArcForge to new embedded platforms (e.g., RK3588), covering toolchains, `.env` configuration, and dependency management.
- **Deep Dive**: Added **"ArcForge Build System Explained"** (`BUILD_SYSTEM.md`), featuring Mermaid diagrams to visualize the Modern CMake architecture, dependency topology, and DSL logic.
- **README**: Updated the main README with high-contrast architecture diagrams, improved Quick Start instructions, and correct links to the new documentation paths.

### 🧹 Refactor
- Removed legacy documentation files in the root `docs/` folder in favor of the new localized structure.


## [v0.5.1] - 2025-12-27

### 🧹 Refactor & Style (重构与规范)
* **Global Internationalization**: Completed a full pass over the codebase to translate all non-English comments (Chinese) into English. This covers:
    * `apps/`
    * `libs/asr_engine`
    * `libs/network`
    * `libs/utils`
    * `third_party/`
* **Code Cleanup**: Removed deprecated legacy logger files (`logger-old.c`, `logger-old.h`) from `libs/utils` to reduce technical debt.

### 👥 Contributors
* @PotterWhite

## [v0.5.0] - 2025-12-27

**RK3588s Platform Support & NPU Integration**

This release expands the framework's capabilities to edge computing by adding official support for the Rockchip RK3588s platform, featuring native NPU acceleration through `librknnrt` integration.

### Added
- **Platform**: Added `cmake/toolchains/rk3588s.cmake` for cross-compiling to RK3588s (aarch64 Buildroot SDK).
- **Dependency**: Added `third_party/librknnrt` module to manage Rockchip NPU runtime libraries.
- **Presets**: Added new dependency mixins in `CMakePresets.json`:
    - `mixin-deps-librknnrt-2.2.0-linux-aarch64` (Default for current Sherpa-Onnx).
    - `mixin-deps-librknnrt-2.3.2-linux-aarch64`.
    - GCC-specific mixins (`gcc11_3_0`, `gcc13_3_0`, `gcc11_4_0`) to ensure ABI compatibility for prebuilt binaries.
- **Presets**: Added complete build presets for RK3588s (`rk3588s-release`, `rk3588s-debug`, and static variants).

### Changed
- **Build**: Refactored `third_party/sherpa-onnx/CMakeLists.txt` to depend on and link against `LibRKNNRT` when required.
- **Build**: Updated `libs/asr_engine` to link `ThirdParty::LibRKNNRT` into the main library interface.
- **Apps**: Updated default model paths in `apps/asr/server` to point to `sherpa-onnx-rk3588-streaming-zipformer-bilingual-zh-en-2023-02-20` for NPU inference demonstrations.

## [v0.4.0] - 2025-12-26

**Documentation & Build System Overhaul**

### Added
- **Docs**: Added Simplified Chinese documentation (`docs/README_ZH_CN.md`).
- **Docs**: Added light/dark mode banners for better project presentation.
- **Build**: Added `cmake/ArcFunctions.cmake` to support custom CMake utility functions.
- **Build**: Implemented `arc_extract_version_from_changelog` to strictly synchronize project version with CHANGELOG.md (Single Source of Truth).
- **Presets**: Added dedicated dependency mixins (`mixin-deps-sherpa-linux-aarch64` and `mixin-deps-sherpa-linux-x86_64`) for cross-platform consistency.

### Changed
- **Docs**: Refactored `README.md` structure and clarified "Convention over Configuration" philosophy.
- **Build**: Standardized Sherpa-Onnx dependency URLs and updated hash verification for both x86_64 and aarch64 (RV1126BP).
- **Test**: Fixed compilation issues in the test suite for x86_64 targets.
- **License**: Improved license comment blocks in test subdirectories.

## [v0.3.0] - 2025-12-25

**Initial Public Release & Framework Consolidation**

This release represents the consolidation of the entire ArcForge development history into a single, stable baseline. It integrates the ASR Engine, complete build infrastructure, application prototypes, and rigorous compliance standards.

### 🚀 Features & Capabilities (核心功能)

*   **ASR Engine Integration**:
    *   Integrated `libs/asr_engine` powered by **Sherpa-Onnx** (C++ API).
    *   Full support for Voice Activity Detection (VAD) and WavReader utilities.
    *   **Offline Support**: Smart detection mechanism for local `sherpa-onnx` tarballs to enable offline builds.
*   **Application Prototypes**:
    *   **ASR Client/Server**: Complete, compilable prototypes for both Client and Server applications (ported from RK3588 architecture).
    *   Unified application logic designed for cross-SoC compatibility.
*   **Environment Management**:
    *   **Dotenv Support**: `build.sh` automatically loads developer-specific variables (e.g., custom SDK paths) from a local `.env` file, isolating personal configs from git.

### 🛠 Build System Architecture (构建架构)

*   **CMake Infrastructure**:
    *   **Modern CMake**: strict adherence to `target_sources` and "Convention Over Configuration" (COC) policies.
    *   **Presets-Driven**: `CMakePresets.json` centralized all dependency search logic and build configurations (Debug/Release, Install Paths).
    *   **Centralized Functions**: `cmake/ArcFunctions.cmake` provides standard macros like `arc_install_library`, `arc_init_global_settings`.
*   **Toolchain Standardization**:
    *   Refactored `rv1126bp` and `aarch64` toolchains with `CMAKE_FIND_ROOT_PATH_MODE_*` set to `ONLY` to prevent host pollution.
*   **Dependency Management**:
    *   **Dual-State Linking**: Supports both Monorepo mode (direct target aliases `ArcForge::Utils`) and Standalone mode (fallback to `find_package`).
    *   **FetchContent**: Automated management of external binaries (ThirdParty Artifacts).
*   **Modular Scripts**:
    *   Refactored `build.sh` into a modular, function-based runner with interactive mode support.

### ✅ CI/CD & Compliance (规范与合规)

*   **License Automation**:
    *   Included `scripts/universal_copyright_v3.1.sh` for intelligent, automated license header injection (Apache 2.0/MIT/etc.).
    *   Supports SPDX identifier auto-detection for complex licenses (e.g., LGPL).
*   **Gatekeeping**:
    *   Added GitHub Actions workflow (`.github/workflows/license-check.yml`) to strictly enforce license header compliance on every PR/Push.

### ♻️ Architecture Refactoring (架构重构)

*   **Namespace Unification**:
    *   Global C++ namespace standardized to `arcforge::embedded`.
    *   CMake targets standardized to `ArcForge::<Module>` (e.g., `ArcForge::Network`, `ArcForge::Utils`).
*   **Directory Structure**:
    *   Strict separation of `include/` (Public Headers) and `src/` (Private Implementation).
    *   Cleaned up model paths and deprecated archive directories.

### 📚 Documentation (文档)

*   **Guides**: Added `docs/ADD_NEW_SOC.md` comprehensive guide for extending platform support.

---


[0.3.0]: https://github.com/potterwhite/ArcForge/tags/v0.3.0

[0.4.0]: https://github.com/potterwhite/ArcForge/tags/v0.4.0

[0.5.0]: https://github.com/potterwhite/ArcForge/tags/v0.5.0

[0.5.1]: https://github.com/potterwhite/ArcForge/tags/v0.5.1

