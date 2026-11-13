# ops — 当前工作

- 目标版本：`0.1.0`
- 实际线上版本：未发布（`onlineVersion=null`，无 tag）
- 状态：InProgress（已接收 B01 受限候选证据；完整 B01 与发布条件未满足）
- 分支/工作区：`pipeline/ops` / `.worktrees/pipelines/ops`

## 当前工作项

`OPS-0.1.0-01` 的治理集成已完成。当前跟进 `0.1.0 / B01`：通过 [MR #34](https://github.com/nilesthump/UEgame/pull/34) 接收 test 的受限 Win64 候选验收证据，合并提交 `6598a5105026004bce8e18647cf9c6ae7bdd0d64`，源提交 `00b467049486c07f228a1aa1642306dcf5c2b605`。该接收不是完整 B01 验收或发布批准。

- 唯一当前验收结论由 test 维护：[evidence.md](../../evidence/evidence.md)，重点查看 `PROD-B01`、`DISPLAY-01`、`PERF-01` 和 `FEEL-01`；ops 不另写验收结论。
- 候选、实际执行报告和截图身份以及历史独立审查见原 MR 与 evidence；agent 审查不等于 GitHub 账号批准。
- 包、原生 JSON、日志与截图为本机忽略产物，不随 Git/MR 传递；跨机器接收须另行提供同一产物。
- 保留 HTML 报告模板缺失及提交前工作树构建限制，不把原生 JSON 成功扩大为日志无错误或干净提交构建。

## 未满足项与下一步

- B01 仍为 InProgress；B02–B15 不启动。真实玩法/成功路线、正式字幕与媒体送达、其余文本显示、输入→核心处理→对应画面实际呈现 <50ms、本人体验和真实手柄仍未完成，详见唯一证据页。
- 产品修复由 tech 正向交付 test 复测，再通过 test → ops 接收；问题与修正保留在原 PR 讨论/验收条目，不另建平行结论文件。
- NAR-0001–0007 全部为 draft；文本定稿由 narrative 维护。其他管线 CURRENT 与契约中的旧快照由各文件所有者后续更新，接收状态以原 PR、当前验收以 evidence 为准。

## 发布边界

- 当前没有满足完整发布条件的产品候选；运营仍一次只接收一个发布候选。
- 不因本次证据接收执行 ops → main、发布游戏包或创建正式 tag；`onlineVersion=null`，`main` 不作为“已上线 0.1.0”记录。
- 发布成功后按 [releases/README.md](../../releases/README.md) 更新 `main` 和线上 `x.y.z`；失败保持原线上版本。

## MR

上游：`test`、`maintenance`；下游：`main`（由授权接收者合并）。本批上游 [MR #34](https://github.com/nilesthump/UEgame/pull/34) 已合并，后续交付仍遵循固定管线协议。
