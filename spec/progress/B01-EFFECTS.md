# B01 效果执行资格

本功能基于3755ff445c8424ca0658cc4ad9343278f1beb5d9，在G:/UEgame/.worktrees/B01的task/B01-completion实施；canonical未集成。

Session为每个效果分配本轮单调请求号，宿主须先检查绑定世界，再紧邻实际执行调用TryConsumeEffect。该入口核对轮次、阶段及完整请求，按发出顺序只允许消费一次；消费仅代表获得执行资格，绝不是操作完成回执。离开Rescue使未执行的旧快照请求失效；Ended仍可消费StopRescue与StopRun。宿主卸载/解绑须调用InvalidatePendingEffects；其后不复用请求号。新的Session须由宿主分配新RunId。

此次仅完成核心资格契约与共享用例，真实Subsystem/Director、世界绑定检查、失败回执与可见反馈尚未实现，不能声称真实世界隔离通过。没有修改WORKING或既有sources，共享检查尚未移交B14。

## 实际验证

证据在本任务工作树Validation/Runs/B01-effects/。Linux GCC13/CMake3.28同源编译及头文件自包含编译成功，CTest/JUnit为6/6通过、无跳过。Game/Editor构建均Succeeded，实际MSVC目录14.38.33130及SDK22621；UE原生index.json为6成功、0失败/未运行/进行中，日志退出0。NullRHI只验证核心，不证明可见反馈。生成配置归档generated-config，未纳入产品配置。

独立b01_effect_review检查实际diff、A03生命周期、共享用例与上述原始证据，无剩余具体P1/P2。任务PR、远端CI和canonical集成未执行。B01仍InProgress；下一步为真实适配和Win64独立包。性能、完整玩法、本人体验未验收；架构未冻结。
