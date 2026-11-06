# A03 构建规则类图确认

来源：本聊天；归档日期：2026-11-05（Asia/Shanghai）。

## 用户原文

> 是

## 确认语境与范围

上一轮提交 A03 构建规则类图及 CMake 目标关系，待确认项为两个 TargetRules 类、一个 ModuleRules 类、默认游戏模块入口、项目模块编译和依赖策略、独立核心库/检查程序/CTest 及目录隔离。用户本次确认该方案。

确认 UEgameTarget、UEgameEditorTarget、UEgame 构建类与 FDefaultGameModuleImpl 注册入口；核心会话生命周期仍归已确认的 Subsystem/Session。项目模块使用 C++20、NoPCHs、关闭 Unity，不主动请求异常或 RTTI；Editor 全局参数可能不同，Linux GCC 强检查核心不依赖异常/RTTI。两个 Target 固定既定 UE/SDK/编译器约束；MSVC14.38 完整补丁版本仍需环境准备后读取，不虚构已安装版本。

确认根 CMake 入口、experiment_core 静态库、experiment_core_checks 检查程序和 CTest 场景；Tests 与构建输出放 UE Source 之外。同一份核心源码分别由 Linux GCC 与 Windows UBT/MSVC 编译，Build.cs 不启动 CMake、Docker 或安装任务。

后续进入 A04「球体世界、局部活动区域与球体小地图」总体方案；A04 总体、类图均需分别确认。本次不预先接受 A04 的具体实现选择，也不授权工程实现、环境安装、容器构建发布或正式冻结。
