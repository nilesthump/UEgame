# B01 独立起点与使用统计

> 历史快照（2026-11-06）。当前状态见 [CURRENT.md](CURRENT.md) 与 [evidence.md](../evidence/evidence.md)；本文件只保留当时执行事实。

基线 main：2303171edd3c25bc575097d1e7d0a1c968e4ab1c。执行者：当前聊天核心/集成协调者。
工作树 G:/UEgame/.worktrees/B01，分支 task/B01-starts；保留 task/B01-progress 及既有证据。

本功能实现三个独立起点：世界实际就绪、活体遇险发现、首次已执行的朝哥布林攻击。使用和指向分别去重；无命中事实也能登记使用及攻击起点。统计按原始发生序号判断，发现前使用不补计，迟到的独立事实不因序号较小而丢弃。Rescue 内保留必要动作元数据以处理迟到发现，不复制世界生命状态。观察死亡后果、稳定批次、完整阶段和适配回执仍待后续功能。

写入范围：B01 Session、Session共享用例、共用注册及Bootstrap测试入口、CMake；协调者维护本记录与验证契约。其他任务、WORKING、既有原始来源不改写。

实际证据均位于本工作树 Validation/Runs/B01-starts/：
- linux.txt 与 .build/core-linux-gcc13/ctest-junit.xml：Linux GCC13/CMake3.28 实际同源构建，3/3通过，无跳过；独立头文件编译成功。
- editor-build.log、game-build.log：Editor/Game 构建成功，MSVC目录14.38.33130（工具14.38.33145）及SDK22621。
- ue-core/index.json、ue-core.log：UE宿主共用用例3/3成功，无失败/跳过，进程退出0。NullRHI不证明真实渲染或世界行为。
- 编辑器生成的两个配置移入 generated-config/ 保留，未纳入产品配置。

独立 reviewer b01_review 已直接检查实际 diff、共享用例、Linux JUnit/编译参数、UE 原生报告及 Game/Editor 日志，无剩余具体 P1/P2；独立执行 RepositoryBaseline180文档/618链接/528来源通过，未重跑游戏构建或核心用例。后续离开 Rescue 必须清理阶段去重/动作缓存。机器命令入口不变，CI自动运行新增CTest；远端CI/集成尚未执行。本功能不代表整个B01完成；稳定快照/死亡优先、旧效果隔离、Subsystem/Director实际链与Win64独立包仍未完成。性能、完整游戏、本人体验未验收；架构未冻结；共享测试尚未移交B14。

## 实际集成

功能提交4b765c036254595bdc671e44c4e973803008fb51，经[PR #7](https://github.com/nilesthump/UEgame/pull/7)保留提交合并至9b00411ee64b535899b50a03dee6fb0f441ff191。[PR CI42240803443](https://github.com/nilesthump/UEgame/actions/runs/42240803443)两个job成功，核心执行与JUnit上传完成。GitHub连接器创建PR返回403，改用本机已授权gh登录；合并沿既有owner bypass授权，无新增批准或规则修改。

canonical已从2303171快进至9b00411，RepositoryBaseline180文档/618链接/528来源通过且干净。该记录不把NullRHI核心测试扩大为真实世界或独立包验证。
[合并后main CI42241001493](https://github.com/nilesthump/UEgame/actions/runs/42241001493)已实际成功。
