# B00 Windows MSVC补装 — 2026-11-06

执行者：本聊天协调agent。分支`task/B00-dependencies`，工作树`.worktrees/B00-dependencies`，基线`ea06a66`。授权见[用户原文](../../spec/requirements/001-causal-prototype/sources/2026-11-06-dependency-installation.md)；遵循[A03固定工具链](../../spec/architecture/001-causal-prototype/A03-toolchain-and-dependencies.md)及[构建规则](../../spec/architecture/001-causal-prototype/A03-build-class-design.md)。

**结果：MSVC14.38缺项已补齐；明确选择14.38与SDK22621的独立C++20编译、链接及运行自检成功。** 本记录不是UE或ExperimentCore构建通过证据。

## 安装与实际范围

使用既有微软签名有效的Visual Studio Installer，向既有VS2022 Community执行`modify`，只指定新增组件`Microsoft.VisualStudio.Component.VC.14.38.17.8.x86.x64`，使用`--quiet --norestart`。组件ID及安装方式依据微软[组件目录](https://learn.microsoft.com/en-us/visualstudio/install/workload-component-id-vs-build-tools?view=vs-2022)与[命令行说明](https://learn.microsoft.com/en-us/visualstudio/install/use-command-line-parameters-to-install-visual-studio?view=visualstudio)。没有请求update/updateall、移除组件、强制关闭程序或重启。

- 首次调用附带`--noUpdateInstaller`，21:57:22日志明确报`Please update Visual Studio Installer before proceeding`，进程退出1。原失败日志保留，没有把它记为成功。
- 第二次移除该开关，允许完成必要的安装器自身升级：4.10.30 → **4.11.24+081b4f81da**。旧启动进程退出0时组件安装仍未完成，继续跟踪安装器接续的两个进程和原生日志。
- 最终安装器UI与提权进程均于22:03:42（Asia/Shanghai）记录`Exit Code: 0`，随后两个进程均退出，工具集目录和文件存在。
- 实例`selectedPackages`按组件ID前后比较仅新增上述一个组件；既有ManagedDesktop工作负载的userSelectedState由Explicit变为Implicit，selectedState仍为GroupSelected，属于安装器选择元数据变化，未删除其包。实际包清单715→797，**新增82个包条目，全部为14.38系列，移除0**。原始包清单及差异保留。
- VS2022产品版本仍为17.14.37516.0（显示17.14.37）；vswhere返回isComplete=true、isRebootRequired=false。原有14.44.35207工具集仍存在。VS18和SSMS的产品版本也与安装前相同。
- SDK22621、UE安装和用户级UBT选择配置未因本任务改写；没有设置全局PATH或改变默认编译器。安装器正常维护的自身状态和缓存不属于项目源码。

## 固定的实测版本与验证

| 项目 | 实际值 |
| --- | --- |
| B01应固定的CompilerVersion / VCToolsVersion | **14.38.33130** |
| Hostx64/x64 cl.exe文件版本 | **19.38.33145.0** |
| Hostx64/x64 link.exe文件版本 | **14.38.33145.0** |
| Windows SDK选择 | **10.0.22621.0** |
| VS2022实例版本 | 17.14.37516.0 |

目录版本与编译器文件版本含义不同；B01不能把19.38文件版本误填入UBT的CompilerVersion，也不能选择Latest。项目Target/Build.cs仍需使用项目配置和实测值，不能写个人安装绝对路径。

在自身忽略目录创建独立`smoke.cpp`和调用脚本：通过`vcvarsall.bat x64 10.0.22621.0 -vcvars_ver=14.38`仅初始化本次进程环境；日志明确输出VCToolsVersion=14.38.33130、WindowsSDKVersion=10.0.22621.0以及实际cl路径。以`/std:c++20 /permissive- /EHs-c- /GR- /W4 /Bv`编译，实际包含标准库array/span和Windows.h，链接并运行调用GetCurrentProcessId、核对求和结果的小程序。

**编译、链接、程序运行和脚本总退出均成功（0）**，输出`B00 MSVC C++20 + SDK smoke passed`；/Bv确认全部编译器阶段与链接器来自14.38目录。此程序仅证明安装工具和所用SDK基本可用，不含项目生产源码，也不构成UE/游戏规则用例。

## 原始证据与限制

本任务worktree忽略目录`Validation/Runs/B00-install-20261106/windows/`保留：

- `install-request.json`/`install-result.json`与`install-request-02.json`/`install-result-02.json`：真实参数、起止时间、PID及启动进程退出码。
- `dd_installer_20261106215715.log`：首次失败原因；`dd_installer_20261106220008.log`、`dd_installer_elevated_20261106220009.log`与`dd_setup_20261106220023.log`：更新后的完整实际安装和最终退出；对应分包日志一并保留。
- `vs-before.json`/`vs-after.json`、`state-before.json`/`state-after.json`、`packages-before.json`/`packages-after.json`及`package-diff.json`：产品和组件前后证据。
- `smoke/smoke.cpp`、`smoke/run-smoke.cmd`、`smoke/smoke.log`、`smoke/smoke-result.json`：隔离自检输入、实际完整工具选择和执行结果。

原生日志中已有的superseded-package兼容信息警告保留；没有据此执行修复/卸载或宣称所有其他VS功能都已验证。个人绝对路径和安装器原始诊断只保存在本机忽略证据中。

当前同源构建路线与唯一项目检查入口仍沿[初次B00报告](B00-2026-11-06.md)；其“缺MSVC”是当时历史状态，由本次安装证据解决，不回写旧失败记录。**UE、ExperimentCore双路径构建、游戏用例、独立包、端到端<50ms性能与本人体验均NotRun。**
