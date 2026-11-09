# 七管线协作总表

目标版本：`0.1.0`。线上版本：未发布（`spec/releases/state.json` 的 `onlineVersion` 为 `null`）。本表是日常协作入口；详细协议见 [PROTOCOL](PROTOCOL.md)，文件写入边界见 [WRITE_SCOPE](../governance/WRITE_SCOPE.md)。

## 固定管线

| 管线 | 固定分支 | 独立工作区 | 职责 | 上游接收 | 下游交付 |
| --- | --- | --- | --- | --- | --- |
| growth | `pipeline/growth` | `.worktrees/pipelines/growth` | 需求范围、版本工作项、验收条件 | — | `tech`, `narrative` |
| narrative | `pipeline/narrative` | `.worktrees/pipelines/narrative` | 剧情、对白、旁白、字幕和叙事文案真源 | `growth` | `tech` |
| tech | `pipeline/tech` | `.worktrees/pipelines/tech` | B10/B11 运行逻辑、代码与资产集成、共享入口 | `growth`, `narrative`, `art` | `art`, `test` |
| art | `pipeline/art` | `.worktrees/pipelines/art` | B13 等素材制作、声音和演出源资产 | `tech` | `tech` |
| test | `pipeline/test` | `.worktrees/pipelines/test` | 测试、验收结论、证据状态 | `tech` | `ops` |
| ops | `pipeline/ops` | `.worktrees/pipelines/ops` | 发布、上线、反馈、`main` 集成 | `test`, `maintenance` | `main` |
| maintenance | `pipeline/maintenance` | `.worktrees/pipelines/maintenance` | 线上故障诊断与机动维护，保持当前线上基线 | `main` 基线同步 | `ops` |

`main` 只记录实际线上运营版本；首次正式上线前明确无线上版本。固定分支建立后由 GitHub Rulesets 禁止新增分支、删除分支和强推；合并后不自动删除源分支。旧 `task/*` 已在 B01 迁移后全部删除。

## 常规 MR 流向

```text
growth    → tech
growth    → narrative
narrative → tech
tech      → art
art       → tech
tech      → test
test      → ops
maintenance → ops
ops       → main
```

上游发起 MR 并完成独立审查；下游检查接收条件后执行 merge commit。不得绕过下游直接向 `main` 提交。`ops → main` 由运营发起并组织审核，由 `main` 的授权接收者合并。

## 版本与证据

- 需求、任务、交付、验证和发布均带 `x.y.z`。同一待发布版本内可按功能点连续交付；当前版本上线前，下一版本只作为提前准备，不混入当前执行范围。
- 发布状态：[state.json](../releases/state.json)；发布流程：[releases/README.md](../releases/README.md)。
- 当前验收结论：[evidence.md](../evidence/evidence.md)。历史由 Git 保存，不新增平行结论文件。
- 剧情文本真源：[spec/contracts/narrative/TEXT.csv](../contracts/narrative/TEXT.csv)。

## 工作区

每个工作区只显示本管线文档、共享协议和任务所需契约。初始化/刷新命令：

```powershell
powershell -NoProfile -File Scripts/Collaboration/Initialize-PipelineWorkspaces.ps1
```

查看当前管线：

```powershell
powershell -NoProfile -File Scripts/Collaboration/Get-PipelineContext.ps1
```
