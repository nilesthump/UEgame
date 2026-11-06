# A03 总体确认与 Linux/GCC 核心要求

来源：本聊天；归档日期：2026-11-05（Asia/Shanghai）。

## 用户原文

> 确认，同时核心用Linux gcc方式写，便于后续打包docker运行

## 确认语境与范围

上一轮请求确认 A03 工具链、源码集成和验证方案，之后单独确认 ModuleRules / TargetRules / 模块入口的构建规则类图。

已展示并获确认的基线：Windows UE5.6.1 预编译安装版、VS2022 17.14、MSVC14.38 推荐族、Windows SDK22621、C++20、UBT/UHT/UAT 和引擎自带 .NET8；独立 ExperimentCore 源码编入一个 UEgame Runtime 模块，Game/Editor 双 Target；灰盒资源与依赖按需引入；核心独立编译、编辑器构建和独立 Windows Development 包验证。

用户同时新增明确约束：核心面向 Linux/GCC 编写，便于后续打包 Docker 运行。该约束补充核心独立构建目标，不撤销已确认的标准 C++20 / 无 UE 依赖，也没有要求将 Windows UE 工具链替换为 GCC。

Linux 发行版、GCC 具体版本、独立构建工具、可执行宿主协议及 Docker 基础镜像此前尚未展示，不视为本次已确认。后续补充技术栈/数据流/PUML 方案后，再按原流程确认构建规则类图。

本次未授权安装环境、创建游戏实现、实现联网、构建/发布镜像或正式冻结。后续 Docker 方向不自动等于当前规则库已具备完整世界模拟或服务器入口。
