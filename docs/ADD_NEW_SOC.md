Here is the updated documentation. I have added **Step 3** specifically for managing the `sherpa-onnx` prebuilt binaries and hashes, while keeping the rest of the structure intact.

---

# How to Add Support for a New SoC

This guide outlines the steps to add support for a new System-on-Chip (SoC) or Cross-Compilation target in the ArcForge build system.

**Old Way:** Create a toolchain file and hardcode absolute paths.
**New Way:** Create a toolchain file (using Env Vars), update `CMakePresets.json`, and configure your environment.

---

## Step 1: Create the Toolchain File

We strictly follow the "99/100 Best Practice" template. Do not start from scratch; copy an existing configuration.

1.  **Copy** the template:
    ```bash
    cp cmake/toolchains/rv1126bp.cmake cmake/toolchains/<new_soc_name>.cmake
    # Example: cp cmake/toolchains/rv1126bp.cmake cmake/toolchains/rk3588.cmake
    ```

2.  **Edit** the new file (`cmake/toolchains/rk3588.cmake`):
    *   **Modify Processor**: Update `CMAKE_SYSTEM_PROCESSOR` (e.g., `aarch64`, `arm`, `riscv64`).
    *   **Define Env Var**: Change the environment variable name used to find the SDK.
        *   *Change*: `if(DEFINED ENV{RV1126BP_SDK_ROOT})`
        *   *To*: `if(DEFINED ENV{RK3588_SDK_ROOT})`
    *   **Update Paths**: Adjust the internal layout variables (like `TOOLCHAIN_BIN_DIR`) to match your specific SDK structure.
    *   **Update Compilers**: Update the filenames for `gcc`, `g++`, `strip`, etc.

**Key Rule**: Never hardcode absolute paths (like `/home/user/sdk`). Always rely on the `$ENV{...}` variable or a fallback default.

---

## Step 2: Configure Your Environment

Since the toolchain file now relies on environment variables, you must define where the SDK lives on your specific machine.

1.  **Temporary (Current Shell)**:
    ```bash
    export RK3588_SDK_ROOT=/opt/rk3588_sdk/host
    ```

2.  **Permanent (Recommended)**:
    Add the line above to your `~/.bashrc` or `~/.zshrc`.

---

## Step 3: Configure External Dependencies (Sherpa-Onnx)

You must provide the download URL and SHA256 Hash for the prebuilt `sherpa-onnx` binaries specific to your new SoC.

1.  **Prepare the Tarball**: Upload your prebuilt `sherpa-onnx` tarball (containing `lib/` and `include/`) to a hosted location (e.g., GitHub Releases).
2.  **Get the Hash**: Calculate the SHA256 hash of the file: `sha256sum filename.tar.xz`.
3.  **Create a Dependency Mixin**: In `CMakePresets.json`, add a new entry under `configurePresets` to define these variables.

```json
    {
      "name": "mixin-deps-rk3588",
      "hidden": true,
      "description": "External dependencies URLs and Hashes for RK3588",
      "cacheVariables": {
        "SHERPA_ONNX_URL": "https://github.com/.../sherpa-onnx-v1.12.20-rk3588.tar.xz",
        "SHERPA_ONNX_HASH": "SHA256=a1b2c3d4e5..."
      }
    },
```

---

## Step 4: Register in CMakePresets.json

Now define the platform configuration and the final build targets, inheriting the dependency mixin you created above.

Open `CMakePresets.json` and add entries to the `configurePresets` array.

### 4.1. Add the "Platform" Preset (Hidden)
This links the logical name to the toolchain file.

```json
    {
      "name": "plat-rk3588",
      "hidden": true,
      "description": "Rockchip RK3588 Target",
      "cacheVariables": {
        "CMAKE_TOOLCHAIN_FILE": "${sourceDir}/cmake/toolchains/rk3588.cmake"
      }
    },
```

### 4.2. Add Concrete Build Presets
Define the actual build targets.
**Important**: You must inherit the dependency mixin (`mixin-deps-rk3588`) here.

```json
    {
      "name": "rk3588-release",
      "inherits": ["base", "plat-rk3588", "mixin-deps-rk3588", "cfg-release", "cfg-shared"],
      "installDir": "${sourceDir}/install/rk3588/release",
      "cacheVariables": {
        "CMAKE_PREFIX_PATH": "${sourceDir}/install/rk3588/release"
      }
    },
    {
      "name": "rk3588-debug",
      "inherits": ["base", "plat-rk3588", "mixin-deps-rk3588", "cfg-debug", "cfg-shared"],
      "installDir": "${sourceDir}/install/rk3588/debug",
      "cacheVariables": {
        "CMAKE_PREFIX_PATH": "${sourceDir}/install/rk3588/debug"
      }
    }
```

---

## Step 5: Verify and Build

1.  **Check Visibility**:
    Run the interactive build script or list presets to see if your new SoC appears.
    ```bash
    ./build.sh list
    ```
    *You should see `rk3588-release` and `rk3588-debug` in the list.*

2.  **Run Build**:
    ```bash
    ./build.sh cb rk3588-release
    ```

---

## Summary Checklist

- [ ] **Toolchain**: Created `cmake/toolchains/xxx.cmake` (Copied from template).
- [ ] **Env Var**: Updated the specific `ENV{XXX_SDK_ROOT}` in the toolchain file & exported it in shell.
- [ ] **Deps**: Added `mixin-deps-xxx` in `CMakePresets.json` with valid `SHERPA_ONNX_URL` and `HASH`.
- [ ] **Presets**: Added `plat-xxx` and `xxx-release` (inheriting `mixin-deps-xxx`).
- [ ] **Sync**: Ensured `installDir` matches `CMAKE_PREFIX_PATH` in the preset.