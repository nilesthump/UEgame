# 0.1.0 治理集成候选

- 目标版本：`0.1.0`
- 候选类型：治理基线，不是产品发布候选
- 内容提交：`3516fc7`（ops 已接收 test 证据）
- 实际线上版本：未发布（`onlineVersion=null`，无 tag）
- 发布决定：不发布产品包，不创建 tag

## 候选范围

- 七条固定管线、独立 sparse 工作区、分支生命周期规则集；
- 文件所有权、五项 PR 协议、九条 MR 流向；
- `x.y.z` 目标/线上版本分离与发布流程；
- 剧情文本单一真源（7 条 `draft`）；
- 单一当前证据文件与精简后的 CI/required check；
- B01 代码/资产状态与未完成边界。

## 验收输入

- [evidence.md](../../evidence/evidence.md)：GOV-01～GOV-08 Passed；FLOW-01 8/9（本集成 PR 为第 9 条）；产品/显示/性能/体验 NotRun。
- PR #9–#16 的独立审查与 CI 结果保留在原 PR。
- `main` 当前 `05c308d`；本候选合并后只更新治理基线，不改变线上版本。

## 构建与发布身份

- 没有产品构建产物、Cook/Stage 包或可启动 UE 场景。
- CI 身份为 `.github/workflows/repository-checks.yml` 的 `repository-baseline`、`narrative-text` 与按改动执行的 `core-linux`。
- 不把文档/CI 候选写成产品构建候选；没有可发布的构建产物。

## 集成入口

- `ops → main` PR：[#17](https://github.com/nilesthump/UEgame/pull/17)。
- 授权接收者：`nilesthump`；使用既有 main owner bypass，独立审查和 CI 仍须完成。
- 合并后：`main` 记录治理基线；`state.json` 仍为 `unreleased`；维护分支再同步到新 `main`。

## 回退

- 合并前失败：保留 `main` 于 `05c308d`，不改变线上状态。
- 合并后发现治理回归：以新修复 PR 回退/修正，不改写已公开历史；不创建发布 tag。
