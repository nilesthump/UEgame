# B01 最小骨架实施记录

执行者：本聊天协调 agent（核心/集成）。基线：`3a058befd82cb4424393f1de664c06195cf47472`。
分支：`task/B01-bootstrap`；工作树：`G:/UEgame/.worktrees/B01`。既有工作树保留。

写入边界沿用[任务表](../tasks/001-causal-prototype/TASKS.md)B01：Session 核心、ExperimentAdapter、模块入口、Game/Editor/Build.cs/uproject/CMake、启动配置/地图，以及最小共享测试入口和 Bootstrap 检查；协调者串行维护本记录、进度、任务、验证契约/机器清单和 CI。共享检查尚未移交 B14。其他领域、原始 sources、WORKING 和引擎安装只读。

## 功能 1：同源编译与世界就绪边界

一个 Runtime 模块与 Game/Editor 两个 Target；固定 CompilerVersion 14.38.33130、SDK 10.0.22621.0。核心 Session 私有拥有状态，宿主确认世界就绪才记录起点；拒绝旧轮/重复/非法时刻，零秒有效。此功能尚不包含发现/攻击起点、完整阶段或世界适配。

Linux CMake 显式源清单与模块核心 src 的 cpp 集合配置时核对；独立编译每个核心头。共享 CaseId `B01.Session.WorldReady` 同时供 CTest 和 UE Automation 调用，无断言消除造成的假通过。

本机证据均在本工作树忽略目录，未覆盖旧任务输出：

- `.build/core-linux-gcc13/compile_commands.json`、`ctest-junit.xml`、`Testing/Temporary/LastTest.log`：GCC 13.3.0，C++20、无扩展/异常/RTTI，Release 编译成功；CTest 1 执行、1 通过、0 失败/跳过。
- `Validation/Runs/B01/editor-build.log`：初次 UBA 因内存压力持续等待，任务自行取消，不能记成功。
- `Validation/Runs/B01/editor-build-retry.log`：`-NoUBA -MaxParallelActions=1` 重试成功；实际目录14.38.33130，UBT显示工具版本14.38.33145，SDK22621；未修改引擎/用户配置。
- `Validation/Runs/B01/game-build.log`：Win64 Development Game 目标实际编译/链接成功，同一固定工具链和源码。尚未 Cook/Stage/独立启动，不能记为独立包通过。
- `Validation/Runs/B01/ue-core/index.json` 与 `ue-core.log`：UE5.6.1 Editor / NullRHI 宿主实际完成 `UEgame.B01.Session.WorldReady`，1 项成功，进程退出0；只证明核心，不证明渲染或世界场景。
- 独立 reviewer `b01_review` 直接核对代码、构建日志、Linux JUnit、UE 原生报告及实际编译参数，修正复核后无剩余具体 P1/P2。未重跑构建、不冒充 GitHub 账号批准。Editor 实际 `/EHsc` 与 Linux 禁异常检查分别记录；未声称 UE 禁异常。
- 审查发现的 Editor 自动生成配置已移到忽略的 `Validation/Runs/B01/generated-config/` 保留，不混入提交。新增 CMake3.28 版本限制后重新配置成功，见 `linux-reconfigure.txt`。RepositoryBaseline：178文档、615链接、528来源通过。

## 后续与限制

PR #5 首次远端检查 `42237099783`：RepositoryBaseline 成功，Linux配置失败（runner不存在 `/usr/bin/cmake`），未运行核心测试，缺JUnit导致上传步骤也失败。不得记通过。独立修复为在一次性 Ubuntu runner 安装已验证的发行版 CMake/CTest 3.28.3 包，再运行原有入口；不修改开发机环境。修复后的远端结果待实际验证。

B01 InProgress，尚未集成。后续须完成独立起点/统计、稳定快照/同批死亡优先、旧轮事实/效果隔离、实际世界回执与可见反馈，以及 Win64 独立包启动。当前没有完整游戏功能、性能或本人体验通过证据；架构未冻结。
