# ops — 当前工作

- 目标版本：`0.1.0`
- 实际线上版本：未发布（`null`，无 tag）
- 状态：InProgress（治理集成；无产品发布候选）
- 分支/工作区：`pipeline/ops` / `.worktrees/pipelines/ops`

## 当前工作项

`OPS-0.1.0-01` 建立发布流程、规则集与 `main` 集成入口；接收 test 的治理验收结论后发起并组织 `ops → main` 审核。运营一次只接收一个发布候选。

## 发布边界

- 当前没有已验产品候选，不发布游戏包、不创建正式 tag。
- 本轮治理合并沿用实际产品发布状态：`onlineVersion=null`，`main` 不作为“已上线 0.1.0”记录。
- 发布成功后按 [releases/README.md](../../releases/README.md) 更新 `main` 和线上 `x.y.z`；失败保持原线上版本。

## MR

上游：`test`、`maintenance`；下游：`main`（由授权接收者合并）。
