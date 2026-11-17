# test — 当前工作

- 目标版本：`0.1.0`；线上未发布，`onlineVersion=null`。
- 状态：InProgress（PR #37 同一包独立复测 5/5，独立审查与 merge commit 接收完成；完整 B01 未验收）。
- 分支/工作区：`pipeline/test` / `.worktrees/pipelines/test`。

## 当前工作项

`TEST-0.1.0-01`：接收 tech 的 B01 Rescue 销毁/GC 隔离候选，回归生命周期、默认启动、后果灰盒与中文 draft 探针。

- 唯一当前结论：[evidence.md](../../evidence/evidence.md)。
- 上游 [MR #37](https://github.com/nilesthump/UEgame/pull/37)：候选 `36b0529`，构建 `b4f94e6`；已按 merge commit `019a840c32f2a38585a1027786069ec077ee77cb` 接收。
- 2026-11-13 同一完整包五项各 1/1 Success，进程和日志测试退出码 0，新 00001 截图已目视；2026-11-17 完成接收。报告 `.build/b01-rescue-teardown-receive-20261113-205935/`；构建身份及 HTML 模板错误、ROOT 证书警告见唯一证据页。
- 接收条件与复现命令见原 PR 和 tech 的 `TEST-HANDOFF-0.1.0.md`；可通过 `git show pipeline/tech:spec/pipelines/tech/TEST-HANDOFF-0.1.0.md` 读取。
- 独立 agent `/root/receive37_review` 已批准受限接收。新增或变化内容须补审；问题回原 PR，由 tech 修复后正向交付。

## 未满足项与下一步

- 候选只覆盖同一 UWorld 的 Actor 销毁、跨帧 GC、弱发送者解析及旧 Run 隔离；不证明真实异步回调调度系统。
- 世界卸载、地图切换、Slate 残留、持久恢复、真实玩法/成功路线、正式媒体、其余文本、端到端 <50ms、本人体验及实物输入未验收。全部 NAR 仍 draft，B01 InProgress，B02–B15 不启动。
- 下一步按 test → ops 正向交付本批证据；受限检查通过不等于发布批准。
- 只写 test 范围；不改业务代码、文本真源、资产或发布状态，不发布或创建 tag。

## MR

上游：`tech`；下游：`ops`。
