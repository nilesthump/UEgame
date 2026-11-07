# B01 真实世界绑定与灰盒装配回执

日期：2026-11-07（Asia/Shanghai）。基于da1af65，在G:/UEgame/.worktrees/B01的task/B01-completion实施，canonical/main未修改或集成。

UExperimentSubsystem唯一拥有Session；同一GameInstance当前游戏世界只接受一个Director。绑定分配新RunId，世界组件实际注册后Director才回报灰盒装配就绪，Subsystem采样真实世界/平台时钟交给Session。重复回执、旧轮身份、异GameInstance及非当前Director被拒绝；解绑/Deinitialize失效待执行效果。Director在BeginPlay接线、EndPlay解绑，文本组件根据Session投影更新。

这只是Bootstrap组件装配就绪，不代表角色、碰撞、导航、环境或媒体就绪。没有实现效果命令分发及其真实完成回执，没有场景启动配置；文本组件虽已更新，实际画面未观察。不能把核心请求资格或本功能测试记成完整世界效果隔离、可见反馈或独立包通过。

## 实际验证

原始证据：本工作树Validation/Runs/B01-binding/，保留旧证据及本次失败。

- editor-final.log：最终Editor构建Succeeded；game-final-resume.log：最终Game构建Succeeded。固定MSVC目录14.38.33130及SDK10.0.22621.0。game-final.log未完整取回，未单独计通过；恢复调用成功。
- ue-tests.log：首轮因测试重复初始化InitializeStandalone已经创建的世界而Fatal，退出1；已删除重复初始化，未覆盖失败记录。
- ue-tests-retry/index.json：修正后7/7；之后补充自动生命周期和异GameInstance用例，最终ue-final/index.json为7成功、0失败/未运行/进行中，ue-final.log记录退出状态0。
- 最终包含6个既有共享核心用例和1个实际UWorld绑定用例。验证组件未注册时拒绝、注册后接受、单Director、重复/旧轮回执、显式解绑、替换轮次、DispatchBeginPlay/DestroyActor路径以及异GameInstance拒绝。销毁对象拒绝断言不能独自证明所有异步效果排空；效果执行尚未接线。
- NullRHI没有实际渲染；Linux本次未重跑（未改核心或共享用例），历史6/6仍仅为原核心证据。
- 自动生成Config已移至generated-config归档，不纳入产品配置。

独立b01_binding_review最终直接核对代码、文档和原始报告，无剩余具体P1/P2。RepositoryBaseline实际通过：184文档、624本地链接、528来源；git diff --check通过。

## 后续

B01仍InProgress：下一步类型化效果的实际执行/失败回执、串行回调队列与当前世界资格联检、可见启动场景，然后Win64独立包实际启动。任务级PR、远端CI、评审、canonical集成复核仍待执行；共享检查未移交B14。性能/完整玩法/本人体验未验收，架构未冻结。WORKING及既有sources未改写。
