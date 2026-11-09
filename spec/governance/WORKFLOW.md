# 仓库工作流程

状态：2026-11-09 起使用七条固定管线；目标版本 `0.1.0`，线上未发布。固定分支、规则集和工作区以实际 Git/GitHub 状态为准；本文描述日常入口，不把“文件已写入”当作远端保护或产品发布已完成。

## 1. 固定分支

| 分支 | 工作区 | 日常职责 |
| --- | --- | --- |
| `pipeline/growth` | `.worktrees/pipelines/growth` | 需求范围、工作项、验收条件 |
| `pipeline/narrative` | `.worktrees/pipelines/narrative` | 剧情/对白/旁白/字幕真源 |
| `pipeline/tech` | `.worktrees/pipelines/tech` | B10/B11、代码与资产集成、共享入口 |
| `pipeline/art` | `.worktrees/pipelines/art` | B13 等素材、声音与演出源资产 |
| `pipeline/test` | `.worktrees/pipelines/test` | 测试、验收结论、证据 |
| `pipeline/ops` | `.worktrees/pipelines/ops` | 发布、反馈、`main` 集成 |
| `pipeline/maintenance` | `.worktrees/pipelines/maintenance` | 线上故障与机动维护 |

`main` 是线上运营记录。固定分支建立后禁止新增分支、删除分支和强推，合并后不自动删除源分支；旧 `task/*` 分支只读。分支生命周期保护与 `main` 的 owner bypass 使用不同 ruleset，详见 [BRANCH_RULES](BRANCH_RULES.md)。

## 2. 工作项与版本

- 需求、任务、交付、验证和发布均带 `x.y.z`。当前工作目标版本记录在 [state.json](../releases/state.json)，实际线上版本同一文件记录；未发布时为 `null`。
- 正常交付链中的 growth、narrative、tech、art、test、ops 围绕同一待发布版本推进；同一版本内可按功能点连续交付。
- 当前版本上线前，下一版本只准备需求，不并入当前实现。提前准备的 0.2.0 需求放在独立文件，不进入 0.1.0 的接收条件。
- 一个可独立验收的功能点或修复形成一个提交；代码、用例、说明和验证同提交。修复另起提交，不改写已公开历史。

## 3. 日常步骤

1. 在所属工作区执行 `Get-PipelineContext.ps1`，读本管线 `CURRENT.md`、协议和必要契约。
2. 按本管线写入范围实现；跨范围修改先在 MR 中说明依赖。
3. 运行与改动相符的检查（见 [VALIDATION](VALIDATION.md)）；按改动决定是否执行 Linux 核心构建、受影响行为验证和发布候选主路径检查。
4. 取得独立 agent 审查；审查实际 diff、验证和未满足项。复用未变化提交的审查，只补充新增/修改内容。
5. 上游用 `Submit-PipelineRequest.ps1` 发起 MR；下游按接收条件合并。合并使用 merge commit，保留功能提交。
6. 发布时由 ops 接收一个已验候选，发起 `ops → main`，组织审核，由 `main` 授权接收者合并；发布成功后登记线上版本。
7. 更新本管线 `CURRENT.md` 和 `spec/evidence/evidence.md` 的当前状态；历史由 Git 保存。

## 4. MR 与审查

常规流向：

```text
growth → tech / narrative
narrative → tech
tech → art / test
art → tech
test → ops
maintenance → ops
ops → main
```

上游发起并独立审查；下游检查接收条件并合并。PR 正文只保留目标版本与工作项、交付内容、接收条件、最新验证、未满足项五项。问题在原 PR 讨论/验收条目反馈，由所属管线修复后正向交付；禁止在另一文件写平行“最终结论”。

`main` 的规则要求 PR、1 次 GitHub 账号批准、讨论解决和 `repository-baseline` 检查，仅允许 merge commit。既有 owner bypass 只用于 `main`；独立 agent 审查和 CI 仍须完成，owner bypass 不冒充账号批准。

## 5. 发布与线上基线

- `maintenance` 从当前 `main` 基线维护；线上补丁走 `maintenance → ops → main`。test 可直接验证维护候选或其构建产物。
- ops 一次接收一个发布候选。发布 PR 合并前必须确认：候选构建身份、验收结论、未满足项、回退方式、线上版本记录。
- 发布成功后更新 `main`：ops 发起并审核发布 PR，发布同一构建产物，确认实际可用；接收方合并后把 `x.y.z` 写入 `state.json` 和发布记录。发布失败保持原线上版本；发布成功但合并失败时完成记录同步或回退。
- 首次正式上线前，`onlineVersion` 保持 `null`；本轮治理合并不改变该状态。正式产品发布后，才允许 `main → pipeline/*` 的基线同步 MR，由接收管线合并并保留各自目标版本。

## 6. 检查与证据

- 本地/CI 共用 `Scripts/Validation/Invoke-RepositoryChecks.ps1`；文本契约由 `Scripts/Validation/Test-NarrativeText.ps1` 检查。
- CI 按改动执行：`repository-baseline` 始终运行；涉及 `Source/`、`Tests/`、构建入口或验证脚本时运行 `core-linux`；涉及剧情文本时运行 `narrative-text`。只有 `repository-baseline` 是 `main` 的 required check，其他检查按改动执行。
- 不执行、零用例、跳过、缺报告或只发现测试不记通过。发布候选的有效结果可复用，但候选内容或构建身份变化后必须重新取得对应证据。
- 所有当前结论只写 `spec/evidence/evidence.md`。失败条目修复通过后原地更新；变更导致结果失效时原地改为待复测。

## 7. 安全边界

不自动发送消息、发布其他仓库/服务或创建游戏发行包。GitHub 操作只沿用本仓库既有公开仓库、任务分支和发布授权。不可逆操作、正式产品发布或范围外新功能需要明确依据。
