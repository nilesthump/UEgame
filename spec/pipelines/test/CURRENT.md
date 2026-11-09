# test — 当前工作

- 目标版本：`0.1.0`
- 状态：InProgress（治理/CI 验收；产品功能 NotRun）
- 分支/工作区：`pipeline/test` / `.worktrees/pipelines/test`

## 当前工作项

`TEST-0.1.0-01` 收敛验收证据并验证精简后的 CI/PR 门禁。

- 唯一当前结论：[evidence.md](../../evidence/evidence.md)。
- 只维护按版本和验收项的当前状态；修复通过原地更新，结果失效原地改为待复测。
- 禁止新增 `evidence-latest.md`、`evidence-final.md`。
- 验收结论引用实际执行链接、PR/提交或 CI；未执行、零用例、跳过、缺报告不记通过。

## 本轮验证范围

- 七管线文档/分支/工作区一致性；
- `repository-baseline`、`narrative-text`、受影响 `core-linux` 的实际 CI；
- 全部规定 MR 流向和 merge commit；
- 不把治理检查扩大为游戏功能、性能或本人体验通过。

## 未满足项

- 少量文本的 UE 显示/字幕接入、声音绑定、完整 B01、Win64 独立包、50ms 和本人体验均未验收。

## MR

上游：`tech`；下游：`ops`。
