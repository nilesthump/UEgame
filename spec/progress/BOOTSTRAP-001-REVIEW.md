# Bootstrap 独立只读审查与 B06 承接演练

审查日期：2026-11-06。独立代理：/root/bootstrap_independent_review。范围：G:/UEgame 的目录规划、B00–B15 任务职责/依赖/写范围、验证契约与实际检查入口；只读模拟承接 B06。此记录不是正式冻结审查，也不是全链治理行为验收。

## 结论

未发现 B00–B15 在当前规定下存在具体 P1/P2 写入范围冲突。共享构建、Session/RunState、Director/Subsystem、Config、主地图均由集成角色串行维护；B01 共享测试移交 B14 有明确时点；B00/B13/B15 的 Validation 子目录与 B14 的范围分离。业务目录没有被误写为多个 Runtime 模块。任务依赖以已集成契约里程碑解锁，未将口头承诺或接口完成当作全领域验收完成。

发现一项 P2 检查缺陷，已交协调者修正，待针对性复核：Scripts/Validation/Invoke-RepositoryChecks.ps1 第 77 行仅收集同时具备 path、sha256、quote 的字典；缺少其中一项的确认来源被静默跳过。第 301 行仅要求收集总数非零，因此其余合法来源存在时，损坏的单条记录不会失败。与 spec/governance/VALIDATION.md 第 17 行“每条确认来源”均校验的契约不符。最小修正应先识别确认来源容器/记录并验证必填字段，再检验摘要与引文，不能靠三字段完整才进入检查。

实际内存演示（tool chunk 313a71）：从已审阅脚本 AST 提取真实 Get-ConfirmationSourceReferences 函数，在内存输入完整来源、缺 sha256、缺 quote 共 3 条，输出仅收集 1 条完整记录。未更改任何 WORKING 或来源文件。完整入口对缺字段隔离副本的负例由协调者执行，本代理未冒称已运行该负例。

## 实际读序及工具行为

以下为真实工具调用记录，均通过 exec_command 读取已授权仓库。按主任务说明使用 require_escalated；没有安装工具、启动游戏开发、修改仓库或尝试越界写入。

| 顺序 | 实际读取内容 | 工具返回 chunk |
| --- | --- | --- |
| 0 | 根 AGENTS.md | 8785aa |
| 1 | spec/progress/CURRENT.md | b08a6b |
| 2 | architecture/README.md，然后 ARCHITECTURE-SUMMARY.md | 6c2d1e |
| 3 | WRITE_SCOPE.md，然后 WORKFLOW.md | df8661 |
| 4 | TASKS.md 全文，先取得 B06 依赖、范围、验收及详细架构入口 | d2b143 |
| 4a | A06-combat-and-damage.md、A06-class-design.md、A06-latitude-trajectory-derivation.md | 3bbb89 |
| 4b | 上次合并输出被截断，因此在进入下一规范层之前补读 A06-class-design.md 全文及 combat 总体后半部；两项独立读取并行 | 7b3bfc、3c9208 |
| 5 | REQUIREMENTS.md 全文 | fb9598 |
| 5a | VALIDATION.md，然后性能契约 001-input-to-render.md | 426092 |

完成根规定读序之后，才读取目录规范、实际验证脚本、validation.json、GitHub 工作流、忽略/换行规则、README/CLAUDE、本轮原文，并抽核 A01/A12 类归属。实际 rg 文件清单确认 Source/Content/Tests 等目前是 .gitkeep 占位，没有游戏源码、uproject、CMake 或真实构建入口。

## B06 只读接手演练

已读取的依据为 TASKS.md 第 55、114–131 行，WORKFLOW.md 第 7 行及根规则。

B06 的精确可写范围仅为：

- Source/UEgame/Private/Combat/**
- Source/UEgame/Private/ExperimentCore/include/experiment/Combat/**
- Source/UEgame/Private/ExperimentCore/src/Combat/**
- Tests/Shared/Combat/**
- Source/UEgame/Private/Tests/Combat/**
- Content/UEgame/Combat/**

Config 没有直接写权；根构建文件、Session、Director/Subsystem、共享 UE Contracts、玩家 Controller/Character/Movement、主地图及其他领域文件只读。B06 持有 Weapon 文件及 FWeaponCutProgress；B07 的 UUnarmedAttackComponent 仍归 B07，只由 B06 唯一 CombatWorld 推进入口调用，不能因此扩大 B06 写范围。

开工依赖：B02、B03，以及已审查且已集成的 B05 输入/瞄准/落地接口。B04 共用移动通过 B05 前置链约束。当前没有这些实现交付，因此保持 Assigned，报告缺失里程碑，不编造可用接口、测试假回执或游戏通过结果。只有具体外部条件缺失且有证据时用 Blocked，不能将一般依赖尚未交付混为外部阻塞。

如果战斗需要扩展玩家 Controller，先提出精确文件/接口/理由/失败及取消行为/验证需求，交 integration-owner 协调，由 B05 玩家控制 owner 修改其所属类。涉及共享事实/接线的部分由 B02/集成串行更新；B06 不直接改 Controller。已经授权的例行修正由相关 owner 继续，不再次要求相同许可。

后续真实实施须从 canonical/main 已集成基线创建独立任务分支/worktree，并记录基线、执行者和精确写范围。本次只是只读演练，没有创建任务 worktree、编辑实现、提交流程、合并或清理他人数据。

## 实际检查结果与范围

独立运行：pwsh -NoProfile -File Scripts/Validation/Invoke-RepositoryChecks.ps1。tool chunk d1dcc8，退出码 0：

- RequiredDocuments.Checked=8
- Markdown.DocumentsChecked=172
- Markdown.LocalLinksChecked=594
- Markdown.EnvironmentReferences.NotValidated=0
- ConfirmationSources.Checked=528
- RepositoryBaseline=Passed
- UE=NotRun；Linux=NotRun；Performance=NotRun
- GameTests.Executed=0；GameTests.Status=NotRun；GameAcceptance=NotEvaluated

上述成功只证明当前资料基线，不证明缺字段负例防护完整。脚本未将零游戏用例当作游戏通过，VALIDATION、README、WORKFLOW 均区分本地脚本、远端工作流配置与实际远端执行。git symbolic-ref 返回 main；git remote -v 无输出；git worktree list 仅 canonical。审查发生于首次提交准备期间，未提交状态不是本次缺陷。

性能原文、需求、任务及性能契约一致：同一有效输入从客户端接收，经核心及真实世界响应，到首个对应视觉反馈实际呈现，严格小于 50 ms。它不是帧预算或核心单步预算；均值/P99 不能掩盖超标样本。未实测、仅内部代理时间戳或误差跨越阈值不能记通过。未发现本次 bootstrap 把目标冒称为实测结果。

未执行：UE/Linux 构建、真实游戏测试、独立包启动、实际 IO/媒体/全场恢复、真实设备、性能和本人体验；未实测 worktree 合并、PR/托管 CI/保护规则、第二独立代理接续或正式冻结。没有对不存在的能力作通过推定。本报告不是 governance-verify 全链通过证明。

## 受审文件摘要

首次检查时 SHA-256：

| 文件 | SHA-256 |
| --- | --- |
| AGENTS.md | 4CBF32D861981FFECB23C53096457420FFDC45F1F4285201BD22E3A7F8565B1C |
| spec/tasks/001-causal-prototype/TASKS.md | 6E27B01A32C311F8541FD05A0B49F4C17CCE34E8A18B9A622573FC968A1F32F7 |
| spec/governance/VALIDATION.md | 934DE83228E8C43B9363BDDD0D5130408C1E456454F9BE4836519D6695F66CDC |
| spec/performance/001-input-to-render.md | 86EF5CC4B8E47CC4C7CC32B3580301B3EA9B0F9A1C3BFFC4EBB275C84CF059D8 |
| Scripts/Validation/Invoke-RepositoryChecks.ps1 | 0785D010CB424C15E08C33151094544F756AB6FDCA4454EAA79951C02F511E4F |

本代理唯一写入为此 scratch 审查记录；仓库文件、WORKING 和原始来源未修改。

## P2 针对性复核与关闭（2026-11-06）

结论：初次发现的确认来源缺字段漏检 P2 已关闭；本次所查 bootstrap 范围内没有剩余具体 P1/P2 问题。保留以上首次问题、原摘要和演示作为历史证据，以下结论绑定修正后脚本。

修后脚本 SHA-256：A762C6BEB22DA111DB876C440FC9C47B5E173CC2B9AA907505BA9E35C0EB5E75。

实际复核行为：

1. 只读重读完整脚本（chunk 3569aa）：按 source 属性位置或摘要/引文字段发现来源；缺失/非字典 source 以及带 kind 却无 source 的 evidence 生成无效记录；主循环显式验证 path、sha256、quote 三项必填字段。递归输出使用逗号保护字典对象，未重现前次过滤遗漏。
2. 从修正脚本 AST 提取真实发现函数，仅在内存检查 7 条：1 条完整记录、分别缺 path/sha256/quote 的 3 条记录、缺 source 对象、null source、错误类型 source。真实结果（chunk 526ee4）：InputRecords=7、CollectedRecords=7、MalformedRecords=6、ParserErrors=0、AllAreDictionaries=true。坏记录全部进入失败判据，没有静默丢弃。
3. 只读读取协调者的 Validation/Runs/bootstrap-20261106/negative-checks.json（chunk b78967）：10 项完整入口运行的实际/预期退出码一致。baseline=0；断链、错误摘要、错误引文、缺 path、缺 sha256、缺 quote、缺 source 对象、零来源均=1；恢复=0。区分证据归属：这些隔离副本入口由协调者执行，本代理审阅记录，没有冒称亲自重建/执行副本。
4. 进一步读取缺 path、缺 sha256、缺 quote、缺 source 对象的原始 .log，以及恢复 .log（chunk 17d263）。四个负例均保留 528 条被检查记录，并输出 missing path/sha256/quote fields 与 RepositoryBaseline=Failed; Failures=1；恢复输出 Passed。原始日志与摘要一致。
5. 独立在 canonical 重新运行完整只读入口（chunk 8cc0ad），退出码 0：8 必需文档、172 Markdown、594 本地链接、528 来源、RepositoryBaseline=Passed。UE/Linux/Performance 仍 NotRun，游戏执行仍 0 且明确 NotRun，验收仍 NotEvaluated。

此次仅追加本 scratch 报告；未修改 canonical、WORKING 或来源。前述 B06 范围/依赖/转交结论和未实测范围不变。此关闭只覆盖已发现的漏检缺陷及相关回归，不将检查脚本正确性或仓库基线成功扩大为游戏、性能、正式冻结或治理全链验收成功。
