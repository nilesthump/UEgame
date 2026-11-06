# B01 稳定批次与死亡优先

执行者：本聊天核心/集成协调者。基线 main 05c308d69660ce8df6fbbe25abffe447ba856e3b。
工作树 G:/UEgame/.worktrees/B01；任务分支 task/B01-completion，保留旧分支和其他工作树。
用户补充已由 df858bb 落实：PR按task汇集，功能仍分别验证、独立审查后立即提交。

本功能仅实现核心稳定判定边界：宿主确认必需事实全已登记，且提供有效、同轮同批的瞬时遭遇快照后，按玩家死亡、小孩死亡、清怪成功的顺序判定。无效引用必须由宿主标记快照无效，不能当作死亡。单调BatchId表示稳定提交点，不用其规则丢弃较低Sequence的独立使用/观察事实。结算时刻不得早于已登记事实。

死亡后果观察可先于死亡快照到达，发出RequestStableSnapshot请求而不打开活体统计窗；没有未来伤害也能提交新的稳定判定点。成功方式为空仅返回MissingMethod开发诊断，保持真实状态，不虚构教师或正常失败。离开Rescue先关闭事实接收并释放阶段去重和动作缓存，再返回StopRescue及后续类型化请求；保留统计和起点，成功尚不生成整轮最终结果。StopRescue不得清空森林状态和雨计划。

写入仅B01 Session/共享Session检查、CMake及协调者验证/进度入口。快照完整性来自未来真实宿主契约，核心用例的合成事实不能证明真实世界已经完整结算。效果执行资格、Subsystem/Director真实回执、可见反馈、跳时就绪及Win64独立包仍未实现；共享检查未移交B14，性能/完整游戏/本人体验未验收，架构未冻结。

## 实际验证

本工作树Validation/Runs/B01-settlement/linux.txt：Linux GCC13、CMake3.28同源配置/编译与独立头文件编译成功；CTest 5/5通过，无跳过。JUnit位于.build/core-linux-gcc13/ctest-junit.xml。
同目录editor-build.log和game-build.log均为Succeeded；实际MSVC目录14.38.33130（工具14.38.33145）及SDK22621。ue-core/index.json为5成功、0失败/未运行/进行中，每项Success，UE进程退出0；NullRHI只验证核心，不证明渲染和世界行为。自动生成配置移入generated-config保留。RepositoryBaseline182文档621链接528来源通过，diff --check通过。独立b01_review已直接检查实际diff、用例、Linux JUnit、Game/Editor日志、UE原生报告与退出日志，无剩余具体P1/P2。PR/CI/canonical集成未执行。
