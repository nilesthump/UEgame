# 分支规则与远端保护

状态：2026-11-09 配置。GitHub 传统 branch protection 接口返回 404 不代表没有规则；以 repository rulesets 的实际查询为准。本文件记录规则意图和核验入口，具体 ruleset ID/更新时间以 [evidence](../evidence/evidence.md) 和 GitHub API 为准。

## 规则集分工

| 规则集 | 目标 | 规则 | bypass |
| --- | --- | --- | --- |
| `main-integration` | `refs/heads/main` | 禁止删除、禁止非快进；要求 PR、1 次 GitHub 账号批准、推送后撤销旧批准、解决讨论；仅允许 merge commit；required check `repository-baseline` 严格最新 | 既有 owner bypass（RepositoryRole 5，`pull_request` 模式），与生命周期规则分开 |
| `fixed-branch-lifecycle` | `refs/heads/**` | 禁止新增分支、禁止删除、禁止非快进 | 无 bypass |
| `archived-task-branches` | `refs/heads/task/**` | 禁止更新、删除、非快进；旧任务分支只读 | 无 bypass |

`fixed-branch-lifecycle` 在七条固定分支和既有任务分支创建/推送后启用，避免把“禁止新增分支”误配置成无法建立初始固定分支。`archived-task-branches` 只冻结旧 `task/*`，不影响 `pipeline/*` 的日常提交。

## 仓库设置

- `delete_branch_on_merge=false`：合并后不自动删除源分支。
- 仅允许 merge commit（`main` 规则集把 `allowed_merge_methods` 限定为 `merge`）；不 squash、不 rebase 改写公开历史。
- `main` 的授权接收者为 `ops` 管线；GitHub 账号 `nilesthump` 为当前授权接收者。owner bypass 只用于 `main`，不用于固定分支生命周期规则。
- 独立 agent 审查不是 GitHub 账号批准，不能替代 required approval；owner bypass 也不取消独立审查或 CI。

## 核验命令

```powershell
gh api repos/nilesthump/UEgame/rulesets
gh api repos/nilesthump/UEgame/rules/branches/main
gh api repos/nilesthump/UEgame --jq ".delete_branch_on_merge"
```

分支创建/删除/强推的实际拒绝证据记录在 [evidence](../evidence/evidence.md)；仅写入本文件不算远端保护已生效。

## 固定分支

`pipeline/growth`、`pipeline/narrative`、`pipeline/tech`、`pipeline/art`、`pipeline/test`、`pipeline/ops`、`pipeline/maintenance`；`main` 只记录线上运营版本。旧 `task/*` 保留为只读历史，不删除、不再接收新提交。
