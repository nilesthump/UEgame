# 当前进度

- 目标版本：`0.1.0`
- 实际线上版本：未发布（`spec/releases/state.json` 的 `onlineVersion=null`，无 tag）
- 当前阶段：0.1.0 七管线治理已集成 `main`；B01 核心已并入，真实 UE/独立包/体验仍未完成
- 更新：2026-11-09

## 本轮治理

`GOV-0.1.0-01` 已完成并进入 `main`：

- `pipeline/growth|narrative|tech|art|test|ops|maintenance` 及各自 sparse-checkout 工作区；
- GitHub Rulesets 禁止新增/删除/强推，关闭合并后自动删支，`main` owner bypass 与生命周期保护分开；
- 唯一文件所有权、五项 PR 正文、完整 MR 流向、`ops → main` 发布入口；
- `x.y.z` 目标/线上版本分离、发布成功/失败处理；
- 剧情文本单一真源、当前证据文件收敛、去除来源 hash/精确引文门禁；
- 按改动执行必要 CI，保留 `repository-baseline` 为 `main` required check。

当前结论和实际执行链接见 [evidence.md](../evidence/evidence.md)；`Governance=Passed`、`ProductRelease=NotRun`。治理合并不改变产品发布状态。

## 产品状态

B01 仍为 InProgress。世界就绪、独立起点/统计、稳定批次/死亡优先、效果资格和真实 bootstrap 世界绑定已并入 `pipeline/tech`/`pipeline/test`/`pipeline/ops`；Linux 核心 6 个用例实际 6/6 通过。真实 UE 世界回执/可见反馈、Win64 独立包、端到端 <50ms 和本人因果体验仍未验收。B02–B15 未启动。

旧 7 个远端 task 分支、9 个本地 task 分支和 5 个 task 工作区已删除；B01 功能提交保留在新管线历史中。之后不再有 task 分支。

未验收：少量文本 UE 显示/字幕接入、真实 UE 世界回执、Win64 独立包、端到端 <50ms、本人因果体验。架构 A01–A13 已确认但未正式冻结。

## 保留

- 既有 `main` 提交、PR #1–#8 历史和合并提交保留。
- 旧任务分支与工作树已删除；迁移前归档 bundle 保留在忽略的 `.build/archives/task-branches-20261109.bundle`。
- `spec/architecture/WORKING.json` 不重写；本轮只取消对来源 SHA-256/精确引文的验证。
- `spec/requirements/001-causal-prototype/sources/` 既有原始来源不改写。
- 维护分支已同步到最终治理基线的 `main`。

## 下一步

1. 产品侧继续 B01 的真实 UE 世界回执、可见反馈、独立包和体验验证，不把核心 6/6 扩大为 B01 完成；
2. B01 完成后进入 B02 跨域契约；当前不发布产品；
3. 首次正式上线前保持 `onlineVersion=null`，不创建 tag；
4. 后续变更按七管线协议正向交付。
