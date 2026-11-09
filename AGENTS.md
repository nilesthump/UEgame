# UEgame agent 工作约定

本仓库使用七条固定管线协作。**不要新建分支，不要直接写 `main`，不要删除或强推固定分支。** 先运行 `git branch --show-current` 确认当前管线，再按以下最小读序恢复上下文：

1. [管线总表](spec/pipelines/README.md)与[交互协议](spec/pipelines/PROTOCOL.md)。
2. 当前管线文档：`spec/pipelines/<pipeline>/CURRENT.md`。分支名与管线名的对应关系见总表。
3. [写入范围](spec/governance/WRITE_SCOPE.md)、[验证契约](spec/governance/VALIDATION.md)与[发布状态](spec/releases/state.json)。
4. 当前工作项直接引用的共享契约：`spec/contracts/narrative/` 或 `spec/contracts/art/`。
5. 仅当任务明确涉及产品玩法时，再读[任务表](spec/tasks/001-causal-prototype/TASKS.md)中对应条目及其链接的架构/需求正文。

当前只有一条正式需求轮次（001）。架构 A01–A13 为已确认设计，尚未正式冻结；B01 仍在实施中。目标版本与线上版本分开记录：目标版本见 `spec/releases/state.json`，未发布时 `onlineVersion` 必须为 `null`，不能填写猜测的线上版本。

## 硬约束

- 只在所属固定管线分支提交。跨管线变更通过上游发起的 MR 交付，`main` 只通过 `ops → main` 集成；合并保留 merge commit，不 squash、不改写已公开历史。
- 写入范围以 [WRITE_SCOPE](spec/governance/WRITE_SCOPE.md) 为准。共享构建入口、`Content/` 下的 UE 二进制资产由 `tech` 唯一写入；`art` 只交付源素材和导入清单；剧情文本唯一真源由 `narrative` 维护；验收结论由 `test` 维护；发布状态由 `ops` 维护。
- 一个可独立验收的功能点或修复形成一个提交，代码、用例、必要说明和验证同提交。提交前运行与改动相符的检查，并取得独立 agent 审查；修复另起提交。
- 待合并 MR 的源分支只接收本批交付的修正。审查意见回到原 PR 的讨论/验收条目，由所属管线修复后正向交付，不另开平行结论文件。
- 验收证据只维护 `spec/evidence/evidence.md` 中按版本和验收项的当前状态；修复通过原地更新，结果失效原地改为待复测。禁止 `evidence-latest.md`、`evidence-final.md` 等平行文件。
- 不保留文档来源的 SHA-256、精确引文或文本交接特征串门禁；需要身份时使用 Git 提交、稳定业务 ID 和实际执行链接。
- 50 ms 约束仍是**输入→核心处理→对应画面实际呈现**的端到端延迟。未执行、零用例、缺报告或只发现测试不得记通过；模拟手柄不得记为实物测试。
- 不自动发送消息、发布仓库、创建远端服务或发布游戏包。GitHub 操作只沿用本仓库既有授权和已登录账号。

## 完成与交接

任务完成后，在当前对话最终回复中给出可直接复制的 handoff prompt，包含仓库/分支/提交/工作区状态、已完成工作、真实验证与独立审查结果、未完成项、下一步入口和写入边界。要求接手者先读本文件并按上述顺序恢复上下文。不得只回复“可继续”或仅留链接。
