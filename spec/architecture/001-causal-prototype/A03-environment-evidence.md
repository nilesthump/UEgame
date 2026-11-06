# A03 本机工具链只读核对

核对日期：2026-11-05（Asia/Shanghai）。此文件记录工具返回的本机事实，不代表版本选型已确认，也不代表编译或打包已通过。

## 已观察事实

| 项目 | 结果 / 证据 |
| --- | --- |
| 工作区 | G:/UEgame 根目录当前只有 .agents 与 spec；未找到游戏 .uproject、.uplugin、Build.cs、Target.cs，尚无游戏工程或游戏构建/CI基线 |
| UE 安装 | G:/Epic Games/UE_5.6；Build.version 为 5.6.1，Changelist 44394996，IsPromotedBuild=1；InstalledBuild.txt 存在 |
| VS2022 | vswhere 返回 Community 17.14.37，安装目录 C:/Program Files/Microsoft Visual Studio/2022/Community；检测到 VC x86/x64 工具组件 |
| VS2022 工具集 | VC/Tools/MSVC 下仅发现 14.44.35207；默认工具集文件同值；cl.exe 文件版本 19.44.35228.0，link.exe 为 14.44.35228.0 |
| VS2026 | 同时安装 Community 18.10；VC/Tools/MSVC 下发现 14.51.36231；不据此宣称 UE5.6 已验证支持 |
| MSVC14.38 | 在已发现的 VS2022/2026 安装的工具集目录中未找到；推荐选型若接受，需要另行补齐 |
| Windows SDK | 发现 Include/10.0.22621.0 与 10.0.26100.0；22621 的 Windows.h、x64 kernel32.lib 与 rc.exe 均存在 |
| UE 自带 .NET | Engine/Binaries/ThirdParty/DotNet/8.0.300/win-x64/dotnet.exe 存在；未运行 .NET/UBT 构建 |
| 构建入口 | Engine/Build/BatchFiles/Build.bat、RunUAT.bat 与 Engine/Binaries/DotNET/UnrealBuildTool/UnrealBuildTool.dll 存在 |
| 模板资源 | 本机 Templates 下有 TP_Blank、TP_FirstPerson、TP_ThirdPerson 等目录；没有复制、迁移或选定具体模板玩法 |
| 覆盖配置核对 | Engine/Saved/UnrealBuildTool/BuildConfiguration.xml 和用户 Documents 对应位置未找到。用户 AppData/Roaming 对应文件读取被拒绝，无法判断该处覆盖配置；不能把该错误当作文件不存在或无配置覆盖 |

## 安装版本自己的配置与源码

- Engine/Config/Windows/Windows_SDK.json：推荐 MSVC 工具集族 14.38.33130–14.38.99999，最低 14.38.33130；SDK MainVersion=10.0.22621.0，最低 10.0.19041.0；VS 最低 17.8。
- 上述配置禁止 MSVC 14.39.33519–14.39.99999、14.40.33807–14.40.99999。14.44 未列在此禁止范围，但也不在推荐族；“未禁止”不代表已通过本项目验证。
- Engine/Source/Programs/UnrealBuildTool/System/CppCompileEnvironment.cs：Default 与 EngineDefault 均为 Cpp20。
- Engine/Source/Programs/UnrealBuildTool/Platform/Windows/MicrosoftPlatformSDK.cs 的检测逻辑对较新 VS 安装也可能返回 VisualStudio2022 分类；后续必须核对实际使用的工具集目录，不能只看 IDE 标签。

| 原始文件 | SHA-256 |
| --- | --- |
| Engine/Build/Build.version | b6a16a262d0136d1812f134ebd83e81138546dcc705f5f088a96702c970d24a0 |
| Engine/Config/Windows/Windows_SDK.json | 13f7627178166f191d2404b4f2494eebb1b90d2aeafca447763d7057f1f1a6e3 |
| Engine/Source/Programs/UnrealBuildTool/System/CppCompileEnvironment.cs | 088e0cb78bff78c622585757d3310b6fbd3329aad85be54d991d82ee8a791ab7 |

以上相对路径均相对于 G:/Epic Games/UE_5.6。没有修改引擎、VS、SDK、用户级配置或项目治理，也没有执行游戏工程生成、编译、Cook、打包或运行验证。

## Linux/GCC 与 Docker 方向补充核对

同日，在用户新增 Linux/GCC 要求后，只读核对如下。没有启动发行版、安装工具或构建镜像。

| 项目 | 观察结果及限制 |
| --- | --- |
| WSL 入口 | `C:/Windows/system32/wsl.exe` 存在；`--list --verbose` 返回“指定的登录会话不存在。可能已被终止。”，无法确认已安装发行版及其可用性 |
| Docker CLI | `docker --version` 返回 29.5.2，build 79eb04c；同时提示读取用户 `.docker/config.json` 被拒绝。仅确认 CLI 文件/版本，未验证 Docker Engine、Linux 容器或镜像运行 |
| Windows PATH 的 GCC | TDM-GCC 的 `g++ -dumpmachine` 返回 `x86_64-w64-mingw32`，是 Windows 目标，不是 Linux/GCC 验证环境 |
| CMake | 当前 Windows PATH 未找到 cmake.exe；没有据此判断 VS 内置 CMake 或 Linux 环境是否存在 |
| UE Linux 工具链配置 | 本机 `Engine/Config/Linux/Linux_SDK.json` 的 MainVersion/MinVersion/MaxVersion 均为 `v25_clang-18.1.0-rockylinux8`，AutoSDKPlatform 为 Linux_x64；仅读取配置，不代表工具链已安装或服务器已构建 |

核心 Linux/GCC 独立构建与未来 UE Linux 服务器构建是不同验证路径。当前没有可声称通过的 Linux 核心编译、规则执行、容器构建或服务端运行结果。
