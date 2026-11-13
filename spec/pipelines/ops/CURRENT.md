# ops — 当前工作

- 目标版本：`0.1.0`
- 实际线上版本：未发布（`onlineVersion=null`，无 tag）
- 状态：InProgress（已接收 B01 受限候选证据；完整 B01 与发布条件未满足）
- 分支/工作区：`pipeline/ops` / `.worktrees/pipelines/ops`

## 当前工作项

`OPS-0.1.0-01` 的治理集成已完成。当前跟进 `0.1.0 / B01`：通过 [MR #36](https://github.com/nilesthump/UEgame/pull/36) 接收 test 的生命周期受限 Win64 候选验收证据，合并提交 `324bf9fe96e1e8c41ade8479c91981c88b1a3cce`，源提交 `15d31034edffe1ad9045fd4e717d9a62a039033f`。该接收不是完整 B01 验收或发布批准。

- 唯一当前验收结论由 test 维护：[evidence.md](../../evidence/evidence.md)，重点查看 `PROD-B01`、`DISPLAY-01`、`PERF-01` 和 `FEEL-01`；ops 不另写验收结论。
- 候选、实际执行报告和截图身份见原 MR 与 evidence；本次独立 agent `/root/ops_receive_review` 重新审查完整批次并批准受限接收，无阻断项，记录见 [原 PR 接收评论](https://github.com/nilesthump/UEgame/pull/36#issuecomment-6853106352)。未重新核实历史审查原始轨迹；agent 审查不等于 GitHub 账号批准。
- 包、原生 JSON、日志与截图为本机忽略产物，不随 Git/MR 传递；跨机器接收须另行提供同一产物。
- 当前包 BUILD-IDENTITY 记录产品 `74772e2` 的干净源码构建；此前包的提交前工作树构建限制不套用于本包。四份日志仍有 HTML 报告模板缺失错误，不把原生 JSON 成功扩大为日志无错误。本次 ops 只复核已有报告、日志与截图，未重跑 UE、Editor、构建或 Linux。

## 未满足项与下一步

- B01 仍为 InProgress；B02–B15 不启动。真实玩法/成功路线、正式字幕与媒体送达、其余文本显示、输入→核心处理→对应画面实际呈现 <50ms、本人体验和真实手柄仍未完成，详见唯一证据页。
- 产品修复由 tech 正向交付 test 复测，再通过 test → ops 接收；问题与修正保留在原 PR 讨论/验收条目，不另建平行结论文件。
- NAR-0001–0007 全部为 draft；文本定稿由 narrative 维护。其他管线 CURRENT 与契约中的旧快照由各文件所有者后续更新，接收状态以原 PR、当前验收以 evidence 为准。

## 发布边界

- 当前没有满足完整发布条件的产品候选；运营仍一次只接收一个发布候选。
- 不因本次证据接收执行 ops → main、发布游戏包或创建正式 tag；`onlineVersion=null`，`main` 不作为“已上线 0.1.0”记录。
- 发布成功后按 [releases/README.md](../../releases/README.md) 更新 `main` 和线上 `x.y.z`；失败保持原线上版本。

## MR

上游：`test`、`maintenance`；下游：`main`（由授权接收者合并）。本批上游 [MR #36](https://github.com/nilesthump/UEgame/pull/36) 已合并，后续交付仍遵循固定管线协议。
