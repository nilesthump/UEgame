# B00 Linux 依赖补装 — 2026-11-06

执行者：本聊天 `linux_dependencies` agent。分支：`task/B00-dependencies`；独立工作树：`.worktrees/B00-dependencies`；基线：`ea06a66`。授权：[用户“安装依赖工具”原文](../../spec/requirements/001-causal-prototype/sources/2026-11-06-dependency-installation.md)。依据：[B00](../../spec/tasks/001-causal-prototype/TASKS.md)、[已确认 Linux 工具链](../../spec/architecture/001-causal-prototype/A03-linux-core-build.md)与[构建规则](../../spec/architecture/001-causal-prototype/A03-build-class-design.md)。

**结果：Linux CMake 3.28 / CTest 缺项已补齐；独立工具链冒烟检查通过。** 本报告仅覆盖已有 WSL Ubuntu 的工具安装与验证。B00 总体状态及 Windows MSVC 缺项由协调者根据另一侧实际证据汇总；本结果不是 ExperimentCore、UE 或游戏构建验收。

## 安装范围和实际版本

已有环境为 Ubuntu 24.04.4 LTS / x86_64 / WSL2。安装前 `apt-cache policy` 实际显示 CMake 与 cmake-data 均未安装，候选来自 `archive.ubuntu.com/ubuntu noble/main`，版本均为 `3.28.3-1build7`；与 [Ubuntu 官方包页](https://packages.ubuntu.com/noble/cmake)一致。

先执行 `apt-get --simulate --no-install-recommends install cmake=3.28.3-1build7 cmake-data=3.28.3-1build7`，确认仅新增以下 4 包；随后以相同固定版本执行实际安装，加入 `--no-remove`。模拟与安装均报告 **0 升级、4 新增、0 移除**，未运行全量升级、autoremove、发行版升级或修改 apt 源与用户 shell 配置。

| 包 | 实际安装版本 | 说明 |
| --- | --- | --- |
| cmake | 3.28.3-1build7 | 包含 `/usr/bin/cmake` 和 `/usr/bin/ctest`，由 `dpkg-query -S` 核实 |
| cmake-data | 3.28.3-1build7 | CMake 配套模块与数据 |
| libjsoncpp25:amd64 | 1.9.5-6build1 | 本次新增的必要依赖 |
| librhash0:amd64 | 1.4.3-3build1 | 本次新增的必要依赖 |

安装实际下载 13.6 MB，安装器报告增加 49.1 MB 占用；本轮没有运行 `apt update`，使用现有官方仓库索引与签名校验的 apt 安装路径，没有新增第三方源。安装使用 WSL 的 root 身份；后续版本查询、配置、编译和测试使用默认 WSL 用户。

| 可执行工具 / 既有依赖 | 安装后实际结果 |
| --- | --- |
| cmake --version | 3.28.3 |
| ctest --version | 3.28.3 |
| g++-13 | 13.3.0，target=`x86_64-linux-gnu`；包版本`13.3.0-6ubuntu2~24.04.1`，本次未更改 |
| GNU Make | 4.3，已存在，本次未更改 |
| libstdc++6:amd64 | 14.2.0-4ubuntu2~24.04.1，已存在，本次未更改 |

实际执行安装命令：

```sh
DEBIAN_FRONTEND=noninteractive timeout --signal=TERM 600 apt-get \
  -o Acquire::http::Timeout=30 -o Acquire::Retries=2 \
  --no-install-recommends --no-remove -y install \
  cmake=3.28.3-1build7 cmake-data=3.28.3-1build7
```

安装回执：2026-11-06T14:00:27.9337006Z 开始，14:00:53.7908923Z 结束，退出码 0，未达到 600 秒上限。WSL 启动时的 localhost 代理提示保留在原始日志；它没有阻止本次官方下载和安装，不据此宣称其他网络路径均可用。

## 实际工具链验证

在本任务忽略输出目录创建独立临时 CMake 项目 `B00ToolchainSmoke`，仅编译一个使用 `std::array`、`std::span` 与 `std::ranges::sort` 的 C++20 小程序，显式比较排序结果后返回退出码。它不包含项目生产源码或任何实验规则，不是第二份 ExperimentCore 实现。

- CMake 配置显式选 `/usr/bin/g++-13`，GNU 13 系列检查通过；配置、编译均退出 0。
- `compile_commands.json` 及实际编译日志确认 `-std=c++20 -fno-exceptions -fno-rtti -pedantic-errors`；没有 UE include、PCH、Unity 或隐式强制包含。
- CTest 先 `--show-only=json-v1` 发现且核对唯一预期 `B00.Toolchain.Cpp20`；随后以 `--no-tests=error --output-on-failure --verbose --output-junit` 实际执行。
- JUnit 明确为 **1 执行、1 通过、0 失败、0 跳过、0 禁用**；用例状态 `run`，实际输出 `B00.Toolchain.Cpp20=Passed; Scope=toolchain-smoke-only; GameTests=NotRun`。没有将发现数量代替执行结果。
- 验证回执：2026-11-06T14:01:43.6279209Z 至 14:01:57.1519800Z，退出码 0；配置/编译各有 60 秒上限，CTest 有 30 秒外部上限和 10 秒用例上限。

编译器 ABI 探测后的 CMake 常规输出包含 `Check for working CXX compiler ... skipped`；这不是本次 CTest 用例被跳过，实际可执行程序随后确已完成编译、链接和测试。

## 本机证据和边界

所有原始脚本、日志、回执及临时产物保留于本任务 worktree 的忽略目录 `Validation/Runs/B00-install-20261106/linux/`：

- `01-plan.sh`、`01-plan.txt`、`01-plan-receipt.json`：版本候选与安装模拟。
- `02-install.sh`、`02-install.txt`、`02-install-receipt.json`：实际固定版本安装、下载及包变更记录。
- `03-verify.sh`、`03-verify.txt`、`03-verify-receipt.json`：版本、包归属、实际配置/编译/CTest 命令与退出结果。
- `smoke-source/`、`smoke-build/compile_commands.json`、`ctest-discovery.json`、`ctest-junit.xml`、`verification-summary.json`：独立工具链小程序、编译参数、发现/执行报告及文件 SHA-256。

个人绝对路径仅出现在本机日志和临时项目中，没有写入产品构建规则。没有变更既有验证入口、CI、WORKING、原始来源、UE 安装、Windows 编译器或项目生产文件，也没有关闭 WSL。工具包版本已在本报告记录；没有为本任务设置系统级 apt hold。

**ExperimentCore 构建/规则测试、UE 构建/场景、独立游戏包、输入→核心→对应画面实际呈现 <50ms 及本人体验均 NotRun。** 本次 1 个工具链用例不能计入游戏验收用例数。后续 B01 仍需建立真实同源双路径构建并验证实际工具选择。
