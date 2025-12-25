
# ArcForge 构建系统架构与集成指南

本文档记录了 ArcForge 项目的构建系统设计原则、模块化规范以及集成开发指南。

## 1. 设计哲学

本项目采用 **Mono-repo** 结构，但构建逻辑高度解耦。核心设计原则如下：

*   **单一事实来源 (Single Source of Truth)**：`build.sh` 仅作为环境配置与启动器，所有构建依赖、顺序、产物规则完全由 CMake 内部逻辑（DAG）控制。
*   **源码外构建 (Out-of-Source Build)**：构建目录结构严格镜像源码目录，互不干扰。临时文件隔离在 `build/` 目录下，禁止污染源码树。
*   **统一交付区 (Staging Area)**：所有模块（Lib/Bin/Headers）编译后统一安装至 `build/install`，模拟最终文件系统布局。
*   **标准导出 (Standard Export)**：库文件通过 CMake `EXPORT` 机制生成标准配置文件，支持 `find_package` 索引与带命名空间的引用。

## 2. 构建流程详解

### 2.1 启动器 (`build.sh`)
脚本职责被严格限制为“点火”：
1.  解析参数（Platform, Debug/Release）。
2.  设置工具链路径 (`CMAKE_TOOLCHAIN_FILE`)。
3.  定义构建根目录 (`-B build`) 与源码入口 (`-S .`)。
4.  触发 `cmake` 配置与 `make` 构建。

### 2.2 依赖管理与头文件路径
为解决开发阶段与安装阶段路径不一致的问题，所有模块的 `CMakeLists.txt` 均采用生成器表达式定义头文件路径：

```cmake
target_include_directories(TargetName PUBLIC
    # 编译阶段：直接读取源码目录下的 include
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    # 安装阶段/外部引用：读取安装目录下的 include
    $<INSTALL_INTERFACE:include>
)
```
此机制确保了模块间（如 Network 依赖 Utils）在构建时无需先执行 `make install` 即可正确找到头文件。

## 3. 模块开发规范

新增模块需遵循以下 CMake 编写规范，以保证自动化集成。

### 3.1 库命名与输出
*   **逻辑目标 (Logical Target)**：CMake 内部使用的名称，如 `Utils`。保持简洁，便于内部 `target_link_libraries`。
*   **物理文件名 (Physical Output)**：通过 `OUTPUT_NAME` 属性增加前缀，避免系统级命名冲突。

**示例 (`libs/utils/CMakeLists.txt`)：**
```cmake
add_library(Utils SHARED ...)

# 生成 libArcForge_Utils.so，但内部引用依然叫 Utils
set_target_properties(Utils
    PROPERTIES
        OUTPUT_NAME
            "ArcForge_Utils"
)
```

### 3.2 安装与导出 (Export)
必须显式定义安装规则，并关联到 `EXPORT` 集合。禁止在 `src/` 子目录下编写 `install` 规则，所有安装逻辑收敛至模块顶层。

```cmake
install(TARGETS Utils
    EXPORT UtilsTargets          # 注册到导出集
    LIBRARY DESTINATION lib      # .so 路径
    ARCHIVE DESTINATION lib      # .a 路径
    RUNTIME DESTINATION bin      # 可执行文件路径
)

# 头文件统一安装
install(DIRECTORY include/ DESTINATION include)
```

### 3.3 命名空间 (Namespace)
为防止符号冲突，所有导出的库在安装阶段自动附加命名空间 `ArcForge::`。

```cmake
install(EXPORT UtilsTargets
    FILE "ArcForge_UtilsTargets.cmake"
    NAMESPACE "ArcForge::"       # 关键设计
    DESTINATION "lib/cmake/Utils"
)
```

## 4. 集成指南 (如何使用库)

### 4.1 内部模块依赖
在同一 Mono-repo 下开发其他模块（如 `libs/network`）时，直接链接逻辑目标名称：

```cmake
# Network 依赖 Utils
target_link_libraries(Socket PUBLIC Utils)
```

### 4.2 外部项目集成 (或 SDK 使用)
当 `build/install` 目录被打包作为 SDK 发布给客户或第三方应用使用时，需使用标准的 CMake 导入方式：

1.  **查找包**：
    ```cmake
    # CMake 会在 lib/cmake/Utils 下查找 ArcForge_UtilsTargets.cmake
    find_package(Utils REQUIRED)
    ```

2.  **链接** (必须带命名空间)：
    ```cmake
    # 使用 ArcForge:: 前缀，明确这是来自 ArcForge 的库
    target_link_libraries(MyApp PRIVATE ArcForge::Utils)
    ```

## 5. 常用维护命令

### 清理构建环境
若需彻底清理构建产物（推荐 git 提交前执行）：
```bash
# 方式一：Git 清理 (推荐，需小心未跟踪文件)
git clean -fdx -e .vscode

# 方式二：Find 清理 (仅删除 CMake 临时文件，保留 build 目录结构)
find . -name "CMakeCache.txt" -o -name "CMakeFiles" -exec rm -rf {} +
```

### 切换构建类型
```bash
./build.sh cb rv1126bp debug    # 交叉编译 Debug 版 (带符号表，无优化)
./build.sh cb rv1126bp release  # 交叉编译 Release 版 (O3 优化)
```