# test — 当前工作

- 目标版本：`0.1.0`；线上未发布，`onlineVersion=null`。
- 状态：InProgress（PR #33 受限候选已接收；完整 B01 未验收）。
- 分支/工作区：`pipeline/test` / `.worktrees/pipelines/test`。

## 当前工作项

`TEST-0.1.0-01`：接收 tech 的 B01 默认启动、后果灰盒与中文 draft 探针。

- 唯一当前结论：[evidence.md](../../evidence/evidence.md)。
- 上游 [MR #33](https://github.com/nilesthump/UEgame/pull/33)：`a5251c6`，产品构建 `fb2861e`；已按 merge commit `b848a0d` 合并，当前远端状态以原 PR 为准。
- 本机同一包三个用例已独立运行，各 1/1、零失败，日志及进程退出码 0；新截图已目视。报告为 `.build/b01-receive-20261113-163037/`，身份、截图与限制见唯一证据页。
- 接收条件及复现命令见原 PR 和 tech 的 `TEST-HANDOFF-0.1.0.md`（可用 `git show pipeline/tech:spec/pipelines/tech/TEST-HANDOFF-0.1.0.md` 读取）。交接文档为发送前快照，远端状态以 PR 为准。
- 新增或变化内容须独立审查；问题留在原 PR，由 tech 修复后正向交付；满足接收条件后使用 merge commit。

## 未满足项与下一步

- 本批结论只覆盖受限灰盒骨架与 NAR-0004 draft 显示；B01 InProgress，B02–B15 不启动。
- 真实玩法/成功路线、正式字幕与媒体送达、其余文本显示、端到端 <50ms、本人体验、真实手柄未完成。全部 NAR 仍 draft。
- 本批独立审查及 MR 接收已完成；下一步按 test → ops 正向交付验收证据；不得把受限检查通过扩展为发布批准。
- 只写 test 范围；不改业务代码、文本真源、资产或发布状态，不发布或创建 tag。

## MR

上游：`tech`；下游：`ops`。
