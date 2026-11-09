# 0.1.0 治理与 B01 核心集成候选

- 目标版本：`0.1.0`
- 候选类型：治理基线 + B01 核心集成；不是产品发布候选
- 内容基线：`pipeline/ops` 接收 `#27`/`#28` 后；本记录由最终 `ops → main` PR 承载
- 实际线上版本：未发布（`onlineVersion=null`，无 tag）
- 发布决定：不发布产品包，不创建 tag

## 候选范围

- 七条固定管线、独立 sparse 工作区、分支生命周期规则集；
- 文件所有权、五项 PR 协议、九条 MR 流向；
- `x.y.z` 目标/线上版本分离与发布流程；
- 剧情文本单一真源（7 条 `draft`）；
- 单一当前证据文件、按改动执行的 CI；
- B01 稳定批次/死亡优先、效果资格、真实 bootstrap 世界绑定并入；
- 旧 task 分支与工作区全部删除，迁移 allowlist 已移除。

## 验收输入

- [evidence.md](../../evidence/evidence.md)：`Governance=Passed`；`PROD-B01=InProgress`，Linux 核心 6/6，真实 UE/独立包/性能/体验 NotRun；`onlineVersion=null`。
- B01 核心：PR #27 合并 `6a89844`，WSL CTest 6/6 与 CI 42640294190 `core-linux` 成功；PR #28 更新证据。
- 旧 task 分支退役见 [BRANCH_RULES](../../governance/BRANCH_RULES.md)。

## 构建与发布身份

- 没有产品构建产物、Cook/Stage 包或可启动 UE 场景。
- Linux 核心 6 个用例实际通过，但不编译 `ExperimentAdapter`，不证明真实 UE 世界回执。
- 不把核心/文档候选写成产品构建候选；没有可发布的构建产物。

## 集成入口

- `ops → main` PR：由 ops 最终创建；授权接收者 `nilesthump`，使用既有 main owner bypass，独立审查和 CI 仍须完成。
- 合并后：`main` 记录治理+B01 核心基线；`state.json` 仍为 `unreleased`；maintenance 再同步到新 `main`。

## 回退

- 合并前失败：`main` 保持当前基线，不改变线上状态。
- 合并后发现回归：以新修复 PR 回退/修正，不改写已公开历史；不创建发布 tag。
