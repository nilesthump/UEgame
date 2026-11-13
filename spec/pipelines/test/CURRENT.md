# test — 当前工作

- 目标版本：`0.1.0`；线上未发布，`onlineVersion=null`。
- 状态：InProgress（PR #35 同一包独立复测 4/4；独立审查与 merge commit 接收已完成；完整 B01 未验收）。
- 分支/工作区：`pipeline/test` / `.worktrees/pipelines/test`。

## 当前工作项

`TEST-0.1.0-01`：接收 tech 的 B01 生命周期、默认启动、后果灰盒与中文 draft 探针。

- 唯一当前结论：[evidence.md](../../evidence/evidence.md)。
- 上游 [MR #35](https://github.com/nilesthump/UEgame/pull/35)：候选 `d3274f8`，构建 `74772e2`；已按 merge commit `4ba150c` 接收，当前远端状态以原 PR 为准。
- 本机同一完整包四用例实际各 1/1、零失败，进程和日志测试退出码 0，新截图已目视。报告 `.build/b01-lifecycle-receive-20261113-181045/`，完整身份、截图及 HTML 模板缺失限制见唯一证据页。
- 接收条件及复现命令见原 PR 和 tech 的 `TEST-HANDOFF-0.1.0.md`（可用 `git show pipeline/tech:spec/pipelines/tech/TEST-HANDOFF-0.1.0.md` 读取）。
- 新增或变化内容须独立审查；问题留在原 PR，由 tech 修复后正向交付；满足接收条件后使用 merge commit。

## 未满足项与下一步

- 本批仅覆盖受限灰盒、同一 UWorld 内三轮终态 Actor 重建/旧轮隔离及 NAR-0004 draft 显示。B01 InProgress，B02–B15 不启动。
- 未结束 Rescue 卸载、GC 后回调、地图切换、Slate 残留、持久恢复、真实玩法/成功路线、正式字幕与媒体、其余文本显示、端到端 <50ms、本人体验、实物输入未完成。全部 NAR 仍 draft。
- 本批独立审查与 MR 接收完成；下一步按 test → ops 正向交付验收证据；受限检查通过不等于发布批准。
- 只写 test 范围；不改业务代码、文本真源、资产或发布状态，不发布或创建 tag。

## MR

上游：`tech`；下游：`ops`。
