# 验收证据（当前）

- 目标版本：`0.1.0`
- 实际线上版本：未发布（`onlineVersion=null`，无 tag）
- 更新：2026-11-09
- 维护者：test
- 结论：治理基线已集成到 `main`；B01 核心 Linux 6/6 通过，真实 UE、独立包、性能与本人体验未验收

本文件只保留按版本和验收项的当前结论及执行链接。修复通过就近更新原条目；变更导致结果失效时原地改为“待复测”。历史由 Git 保存，禁止新增 `evidence-latest.md`、`evidence-final.md` 等平行结论文件。

| ID | 版本 | 验收项 | 当前结论 | 最新验证/执行链接 | 未满足项 |
| --- | --- | --- | --- | --- | --- |
| GOV-01 | 0.1.0 | 七条固定管线分支与独立工作区 | Passed | 分支与工作区见 [总表](../pipelines/README.md)；实际 `git worktree list` 含 7 个 `.worktrees/pipelines/*` | 旧任务分支保留只读，未删除 |
| GOV-02 | 0.1.0 | 分支生命周期规则集与自动删支 | Passed | [main](https://github.com/nilesthump/UEgame/rules/26700391)、[lifecycle](https://github.com/nilesthump/UEgame/rules/26855799)、[archive](https://github.com/nilesthump/UEgame/rules/26855800)；create/delete/force 拒绝见 [BRANCH_RULES](../governance/BRANCH_RULES.md) | 无 |
| GOV-03 | 0.1.0 | 文件所有权与 MR 协议 | Passed | [WRITE_SCOPE](../governance/WRITE_SCOPE.md)、[PROTOCOL](../pipelines/PROTOCOL.md)；独立审查/接收记录：[#9](https://github.com/nilesthump/UEgame/pull/9)、[#10](https://github.com/nilesthump/UEgame/pull/10)、[#11](https://github.com/nilesthump/UEgame/pull/11)、[#12](https://github.com/nilesthump/UEgame/pull/12)、[#13](https://github.com/nilesthump/UEgame/pull/13)、[#14](https://github.com/nilesthump/UEgame/pull/14)、[#15](https://github.com/nilesthump/UEgame/pull/15) | 产品功能不在本项 |
| GOV-04 | 0.1.0 | sparse-checkout 工作区 | Passed | 七个工作区由 `Initialize-PipelineWorkspaces.ps1` 建立；ops/tech/narrative 检查使用 GitTree(sparse) 通过 | 无 |
| GOV-05 | 0.1.0 | 剧情文本唯一真源与结构检查 | Passed（结构） / NotRun（显示） | [TEXT.csv](../contracts/narrative/TEXT.csv) 7 条；[CI 42630315326](https://github.com/nilesthump/UEgame/actions/runs/42630315326) `narrative-text` 成功 | UE 显示/字幕/声音接入未验证 |
| GOV-06 | 0.1.0 | 证据收敛与无平行结论文件 | Passed | 本文件为唯一当前结论；`RepositoryBaseline` 检查无 `evidence-latest/final` | 历史文件仍在 Git，不另建当前副本 |
| GOV-07 | 0.1.0 | 精简 CI 与 required checks | Passed | [PR #14 检查](https://github.com/nilesthump/UEgame/actions/runs/42632616056) baseline+narrative-text 成功；最终 [PR #17 检查](https://github.com/nilesthump/UEgame/actions/runs/42634934355) baseline+narrative-text+core-linux 全部成功；[#27 检查](https://github.com/nilesthump/UEgame/actions/runs/42640294190) repository-baseline+core-linux 成功，narrative-text 按路径跳过 | 未执行产品构建 |
| GOV-08 | 0.1.0 | 发布状态/流程与未发布记录 | Passed | [state.json](../releases/state.json)：`onlineVersion=null`、`onlineTag=null`、`unreleased` | 无正式产品发布 |
| FLOW-01 | 0.1.0 | 九个规定 MR 流向 | Passed（9/9） | growth→tech [#9](https://github.com/nilesthump/UEgame/pull/9)；growth→narrative [#10](https://github.com/nilesthump/UEgame/pull/10)；narrative→tech [#11](https://github.com/nilesthump/UEgame/pull/11)；tech→art [#12](https://github.com/nilesthump/UEgame/pull/12)；art→tech [#13](https://github.com/nilesthump/UEgame/pull/13)；tech→test [#14](https://github.com/nilesthump/UEgame/pull/14)；maintenance→ops [#15](https://github.com/nilesthump/UEgame/pull/15)；test→ops [#16](https://github.com/nilesthump/UEgame/pull/16)；ops→main [#17](https://github.com/nilesthump/UEgame/pull/17) | 无（证据修正 PR #18 不改变九条流向） |
| MAIN-01 | 0.1.0 | `ops → main` 治理集成 | Passed | [#17](https://github.com/nilesthump/UEgame/pull/17) 合并为 `3bd0c84`；证据同步 [#20](https://github.com/nilesthump/UEgame/pull/20)；`main` 已更新，线上仍未发布 | 无产品发布 |
| PROD-B01 | 0.1.0 | B01 完整功能 | InProgress（Linux 核心 6/6；真实 UE/独立包/性能/体验 NotRun） | [PR #27](https://github.com/nilesthump/UEgame/pull/27) 合并 `6a89844`（源 `69fb8f7`），[CI 42640294190](https://github.com/nilesthump/UEgame/actions/runs/42640294190) core-linux 成功；本工作区按 VALIDATION §1 在 WSL Ubuntu 24.04/GCC 13.3.0/CMake 3.28.3 复跑 `cmake ... && ctest --no-tests=error --verbose`，结果 `100% tests passed, 0 tests failed out of 6`，无跳过，JUnit `tests=6 failures=0 skipped=0`（`.build/core-linux-gcc13/ctest-junit.xml`，不提交） | 真实 UE 世界回执、显示/字幕/声音呈现、Win64 独立包、输入→渲染 <50ms、本人因果体验均未验收 |
| DISPLAY-01 | 0.1.0 | 少量文本 UE 显示/字幕接入 | NotRun | 无 | 需要真实 UE 场景执行 |
| PERF-01 | 0.1.0 | 输入→渲染 <50ms | NotRun | 无 | 测量链未建立 |
| FEEL-01 | 0.1.0 | 本人因果体验 | NotRun | 无 | 未试玩 |

```text
Governance=Passed
B01.Core=Linux 6/6 (6a89844 / PR #27)
ProductRelease=NotRun
MainIntegration=#17 / #20 (exact SHA in Git)
```

## 当前说明

- 治理验收只覆盖分支/规则/协议/版本/文本结构/CI/证据，不覆盖产品、素材、性能或本人体验。
- `ops → main` #17 是治理基线集成，不创建发布 tag，也不改变 `onlineVersion=null`。
- 本次证据更新记录 #27 并入 `pipeline/test` 后由 test 独立复跑核心用例的结果，不改代码、候选内容或发布状态。
- `PROD-B01` 的 InProgress 只代表 Linux 核心 6 个显式用例通过；#27 的 `ExperimentAdapter`/`ExperimentSubsystem` 未被 Linux 核心宿主编译，真实 UE 世界回执、独立包、50ms 和本人体验仍为 NotRun，不构成 B01 完成或产品发布。
- `main` 的精确 SHA 由 Git 记录，不在证据正文自引用，避免后续每次合并产生新的过期状态。
- 最新 PR 检查失败或跳过时，本表对应条目改为“待复测”，不能以本地脚本或旧候选结果替代。
