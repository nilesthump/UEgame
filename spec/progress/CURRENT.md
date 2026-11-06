# 当前进度

## B01 最新集成：独立起点与使用统计

功能提交 `4b765c036254595bdc671e44c4e973803008fb51` 经 [PR #7](https://github.com/nilesthump/UEgame/pull/7) 合并至 `9b00411ee64b535899b50a03dee6fb0f441ff191`。Linux与UE共用用例各3/3，Game/Editor固定MSVC14.38.33130及SDK22621构建成功；独立 b01_review 无剩余具体P1/P2。[PR CI](https://github.com/nilesthump/UEgame/actions/runs/42240803443)成功；canonical快进后RepositoryBaseline180文档/618链接/528来源通过且干净。详细范围和证据见[B01独立起点记录](B01-STARTS.md)。

B01仍InProgress。下一功能为稳定快照与同批死亡优先，包括死亡后果观察触发判定、成功方式为空的诊断，以及离开Rescue后清理阶段缓存；之后仍需旧效果隔离、Subsystem/Director真实回执与可见反馈、Win64独立包启动。旧轮事实已在本功能共用用例覆盖，不能扩大为旧效果或真实世界集成通过。共享检查未移交B14；性能、完整游戏和本人体验未验收；架构未冻结。

本次沿用 G:/UEgame/.worktrees/B01，功能分支 task/B01-starts；集成记录分支 task/B01-starts-progress，基于9b00411。原 task/B01-progress 和其他工作树、既有证据均保留；最终分支/提交/工作区以Git复核为准。


## B01 当前实施（首个功能已集成，整体未完成）

首个功能及CI修复已分别提交 `c3dbb96`、`88de166`，Linux/UE共享用例各1/1、Game/Editor构建成功，独立审查无剩余具体P1/P2，PR #5修复后CI `42237486987`成功。实测main已启用ruleset `26700391`，要求1次GitHub账号批准；用户明确[授权owner bypass](../requirements/001-causal-prototype/sources/2026-11-06-owner-bypass.md)后，PR #5已合并至 `01a22f4c3d7d672b784239ed1c247211859366b0`。canonical已快进复核且干净，RepositoryBaseline178文档615链接528来源通过，合并后main CI `42237939016`成功。整个B01仍InProgress，具体证据/剩余项见下方实施记录。后文“未配置保护”为B00时点记录，以本次ruleset实测为准。

本聊天已按接手指示在基线 `3a058befd82cb4424393f1de664c06195cf47472` 创建 `task/B01-bootstrap` / `G:/UEgame/.worktrees/B01`，由协调 agent 担任核心/集成 owner。精确范围、实际验证、未完成项见[B01实施记录](B01-BOOTSTRAP.md)。以下 B00 交付段落中的“B01尚未启动”是该次交付时点记录，以本段及任务表新状态为准。仅首个功能已集成；共享检查尚未移交 B14。

日期：2026-11-06。canonical：`G:/UEgame`；本地集成分支：`main`。当前提交以`git log -1`和远端记录为准，避免自引用哈希。

用户已授权接手001、先公开发布仓库并采用MIT、每实现/修复一个功能点即提交，随后从B00工具链核对开始。依据见[本轮原文](../requirements/001-causal-prototype/sources/2026-11-06-public-repository-and-commits.md)。A01–A13总体及类图保持已确认；WORKING仍为discussion，未正式冻结。性能目标仍为输入→核心处理→对应画面实际呈现端到端<50ms。

## 已交付

- 规范目录、B00–B15职责和仓库验证入口已建立；bootstrap提交`c858048`，原[独立审查与B06接手演练](BOOTSTRAP-001-REVIEW.md)保持有效，其验证范围没有扩大为游戏能力。
- [公开仓库](https://github.com/nilesthump/UEgame)已创建，origin已配置，GitHub确认PUBLIC/MIT/main。`963cc48`添加MIT，`c834714`落实一个功能/修复一个提交；均从独立发布工作树经独立审查后集成和推送。[发布审查及证据](PUBLICATION-001-REVIEW.md)。
- 首轮[GitHub Actions](https://github.com/nilesthump/UEgame/actions/runs/42226638076)在`c834714`实际通过RepositoryBaseline。本地和云端仍使用原有同一个脚本；它只检查文档/结构/来源，UE/Linux游戏测试数为0、状态NotRun。
- main分支保护查询返回`protected=false`；没有强制PR、账号批准或管理员保护。接入后的任务按分支→PR→相同CI→独立agent审查→合并执行；agent审查不冒充GitHub账号批准。
- WORKING及原始sources未改写，讨论revision118/contentRevision119不变。没有新增游戏工程、素材或冻结产物。

## B00工具链环境准备

B00状态：**Integrated（环境缺项已补齐，安装与验证证据已集成）**。B01为Ready，尚未启动；B02–B15仍为Assigned。

- 初次核对记录保持原样：[B00初次核对](../../Validation/Toolchain/B00-2026-11-06.md)。当时缺MSVC14.38与Linux CMake/CTest，已通过PR #1集成核对结果；这不代表当时环境已通过。
- 本轮用户明确[授权安装依赖](../requirements/001-causal-prototype/sources/2026-11-06-dependency-installation.md)。分支`task/B00-dependencies`、工作树`.worktrees/B00-dependencies`，基线`ea06a66`。Windows由协调agent负责；Linux由`linux_dependencies` agent负责；各自仅写专属Validation/Toolchain报告及自身忽略输出，治理文件由协调者维护。
- Windows缺项已解决：[安装记录](../../Validation/Toolchain/B00-WINDOWS-INSTALL-2026-11-06.md)。MSVC目录/CompilerVersion为**14.38.33130**，cl文件**19.38.33145.0**、link文件14.38.33145.0。显式选择SDK**10.0.22621.0**，隔离C++20程序实际编译、链接、运行退出0。VS2022保持17.14.37，原14.44保留；安装器因必要前置升级为4.11.24，未重启。
- Linux缺项已解决：[安装记录](../../Validation/Toolchain/B00-LINUX-INSTALL-2026-11-06.md)。Ubuntu24.04.4/x86_64、GCC13.3.0保持；CMake与CTest均**3.28.3**（Ubuntu包3.28.3-1build7）。仅新增4个必要包，未升级/移除其他包；隔离C++20程序真实配置、编译、CTest **1/1通过**。
- UE5.6.1/CL44394996、SDK22621、PowerShell7.6.9沿用实测基线；B01须在真实UBT/CMake构建输出中进一步核对选择。工具链smoke不是ExperimentCore/UE构建、游戏用例或性能验收；这些仍NotRun。
- 原始安装失败、重试/接续、包清单、版本和smoke报告保留在本任务忽略目录`Validation/Runs/B00-install-20261106/{windows,linux}/`。初次Windows尝试因安装器需要自更新而失败，未覆盖成成功。
- 独立审查由`dependency_review`对照原始安装/版本/执行证据与实际diff进行，无剩余具体P1/P2。Linux安装、handoff治理、Windows安装和环境进度分别提交为`10ddd03`、`bb84137`、`5964967`、`6ff2720`，由[PR #3](https://github.com/nilesthump/UEgame/pull/3)保留功能提交合并至`29a2e4c`。[PR检查](https://github.com/nilesthump/UEgame/actions/runs/42232422269)与[合并后main检查](https://github.com/nilesthump/UEgame/actions/runs/42232598960)均实际成功。
- canonical已复核`29a2e4c`，工作区干净，RepositoryBaseline通过（177文档、611本地链接、528来源）；WORKING哈希保持`8E4350992F0A37B0449F11B0AEC687675310F355B79E1FE50F47E54BB30F1CDA`。常规Git传输连接失败后，通过GitHub API逐提交核对树与SHA，并恢复哈希精确匹配的远端签名合并对象；没有改写提交历史。本次集成状态由`task/B00-completion` / `.worktrees/B00-completion`基于`29a2e4c`记录；最终提交以Git和PR记录为准。
- 已依[新增用户指示](../requirements/001-causal-prototype/sources/2026-11-06-agent-completion-handoff.md)在AGENTS要求每个agent任务完成后于对话最终回复提供可复制handoff prompt；此治理独立提交`bb84137`。
- B01–B15均未启动。安装及验证没有创建游戏工程或改变已确认架构；各既有工作树保留，不清理其他任务数据。

## 下一步

B00环境证据已通过PR集成与canonical复核，下一入口为B01同源核心双编译与Win64最小独立启动，随后B02跨域契约。B01须固定本次实测CompilerVersion=14.38.33130和SDK22621，Linux使用GCC13/CMake3.28，并核对同份核心源清单；本次未启动B01实现。B01最小共享检查集成后再显式移交B14。

单UEgame Runtime模块、无UE依赖的同源C++20核心保持不变。端到端性能、UE/Linux游戏构建、独立包与本人试玩均NotRun；实际呈现是延迟终点，不能以核心耗时或平均帧率替代。详细范围和验收见[任务表](../tasks/001-causal-prototype/TASKS.md)、[验证契约](../governance/VALIDATION.md)与[性能契约](../performance/001-input-to-render.md)。
