# 验收证据（当前）

- 目标版本：`0.1.0`
- 实际线上版本：未发布（`onlineVersion=null`，无 tag）
- 更新：2026-11-23
- 维护者：test
- 结论：test 在同一 Win64 包独立复测隐藏启动 1/1、正常窗口 10/10。魔法、混用、剑术三条灰盒回访均在群众和教师两份字幕回执齐备后结束。B01 仍在实施。

本文件只保留按版本和验收项的当前结论及执行链接。修复通过就近更新原条目；变更导致结果失效时原地改为“待复测”。历史由 Git 保存，禁止新增 `evidence-latest.md`、`evidence-final.md` 等平行结论文件。

| ID | 版本 | 验收项 | 当前结论 | 最新验证/执行链接 | 未满足项 |
| --- | --- | --- | --- | --- | --- |
| GOV-01 | 0.1.0 | 七条固定管线分支与独立工作区 | Passed | 七条固定分支及 `.worktrees/pipelines/*` 保留；旧 7 个远端 task、9 个本地 task 分支和 5 个 task 工作区已删除 | 无 |
| GOV-02 | 0.1.0 | 分支生命周期规则集与自动删支 | Passed | [main](https://github.com/nilesthump/UEgame/rules/26700391)、[lifecycle](https://github.com/nilesthump/UEgame/rules/26855799)；archive ruleset 26855800 已随 task 退役删除；create/delete/force 拒绝见 [BRANCH_RULES](../governance/BRANCH_RULES.md) | 无 |
| GOV-03 | 0.1.0 | 文件所有权与 MR 协议 | Passed | [WRITE_SCOPE](../governance/WRITE_SCOPE.md)、[PROTOCOL](../pipelines/PROTOCOL.md)；独立审查/接收记录：[#9](https://github.com/nilesthump/UEgame/pull/9)、[#10](https://github.com/nilesthump/UEgame/pull/10)、[#11](https://github.com/nilesthump/UEgame/pull/11)、[#12](https://github.com/nilesthump/UEgame/pull/12)、[#13](https://github.com/nilesthump/UEgame/pull/13)、[#14](https://github.com/nilesthump/UEgame/pull/14)、[#15](https://github.com/nilesthump/UEgame/pull/15) | 产品功能不在本项 |
| GOV-04 | 0.1.0 | sparse-checkout 工作区 | Passed | 七个工作区由 `Initialize-PipelineWorkspaces.ps1` 建立；ops/tech/narrative 检查使用 GitTree(sparse) 通过 | 无 |
| GOV-05 | 0.1.0 | 剧情文本唯一真源与结构检查 | Passed（结构）；单条显示见 DISPLAY-01 | [TEXT.csv](../contracts/narrative/TEXT.csv) 7 条均 draft；既有 [CI 42630315326](https://github.com/nilesthump/UEgame/actions/runs/42630315326)；本批文本源包用例见下 | 正式字幕与声音接入未验收 |
| GOV-06 | 0.1.0 | 证据收敛与无平行结论文件 | Passed | 本文件为唯一当前结论；`RepositoryBaseline` 检查无 `evidence-latest/final` | 历史文件仍在 Git，不另建当前副本 |
| GOV-07 | 0.1.0 | 精简 CI 与 required checks | Passed | [PR #14 检查](https://github.com/nilesthump/UEgame/actions/runs/42632616056) baseline+narrative-text 成功；最终 [PR #17 检查](https://github.com/nilesthump/UEgame/actions/runs/42634934355) baseline+narrative-text+core-linux 全部成功；[#27 检查](https://github.com/nilesthump/UEgame/actions/runs/42640294190) repository-baseline+core-linux 成功，narrative-text 按路径跳过 | 未执行产品构建 |
| GOV-08 | 0.1.0 | 发布状态/流程与未发布记录 | Passed | [state.json](../releases/state.json)：`onlineVersion=null`、`onlineTag=null`、`unreleased` | 无正式产品发布 |
| FLOW-01 | 0.1.0 | 九个规定 MR 流向 | Passed（9/9） | growth→tech [#9](https://github.com/nilesthump/UEgame/pull/9)；growth→narrative [#10](https://github.com/nilesthump/UEgame/pull/10)；narrative→tech [#11](https://github.com/nilesthump/UEgame/pull/11)；tech→art [#12](https://github.com/nilesthump/UEgame/pull/12)；art→tech [#13](https://github.com/nilesthump/UEgame/pull/13)；tech→test [#14](https://github.com/nilesthump/UEgame/pull/14)；maintenance→ops [#15](https://github.com/nilesthump/UEgame/pull/15)；test→ops [#16](https://github.com/nilesthump/UEgame/pull/16)；ops→main [#17](https://github.com/nilesthump/UEgame/pull/17) | 无（证据修正 PR #18 不改变九条流向） |
| MAIN-01 | 0.1.0 | `ops → main` 治理集成 | Passed | [#17](https://github.com/nilesthump/UEgame/pull/17) 合并为 `3bd0c84`；证据同步 [#20](https://github.com/nilesthump/UEgame/pull/20)；`main` 已更新，线上仍未发布 | 无产品发布 |
| PROD-B01 | 0.1.0 | B01 完整功能 | InProgress（三条灰盒回访通过） | [PR #51](https://github.com/nilesthump/UEgame/pull/51)，产品 a35d3f0，候选 8b7d2c4；[CI](https://github.com/nilesthump/UEgame/actions/runs/45147291663)，同包执行记录见下 | 完整战斗、导航、环境火湿与破坏、世界卸载恢复、实物输入、性能与本人体验待验 |
| DISPLAY-01 | 0.1.0 | 少量文本 UE 显示/字幕接入 | Partial（六条回访草稿与 NAR-0004 草稿探针可见） | 三条路线各核对两张字幕截图，Aftermath 核对失败旁白；截图编号及文本 ID 见下 | 运行时隐藏窗口回执有效性待 tech 核实；正式文本、语音与字幕同步、听辨待验 |
| PERF-01 | 0.1.0 | 输入→渲染 <50ms | NotRun | 无 | 测量链未建立 |
| FEEL-01 | 0.1.0 | 本人因果体验 | NotRun | 无 | 未试玩 |

```text
Governance=Passed
B01.Core=8/8 (tech GCC 13 JUnit, upstream handoff), core-linux CI passed (#51)
TaskBranches=Removed (7 remote / 9 local / 5 worktrees)
ProductRelease=NotRun
MainIntegration=#17 / #20 (exact SHA in Git)
```

## 0.1.0 / PR #51 本机独立复测

- 2026-11-23，test 从 9385393 启动 B01-hidden-startup-20261123 归档。BUILD-IDENTITY.json 对应产品 a35d3f0、Win64 Development、UE 5.6.1 CL44394996、B01Bootstrap。上游 UAT 退出 0，交付提交 8b7d2c4 只更新交接文档。
- 从根启动器受控隐藏子进程的游戏窗口。在用例开始前确认 HWND 199310 已隐藏，随后同一窗口恢复可见，期间未发现第二个匹配窗口。D3D12 魔法用例 1/1，启动器与测试退出 0，群众 NAR-0002 和教师 NAR-0006 送达。[窗口记录](G:/UEgame/.worktrees/pipelines/test/.build/receive51-hidden-1123/window.txt) · [原生报告](G:/UEgame/.worktrees/pipelines/test/.build/receive51-hidden-1123/magic/index.json)
- 正常窗口串行执行 MagicTownDialogue、MixedTownDialogue、TownDialogue、Revisit、RescueSuccess、RescueTeardown、Lifecycle、Bootstrap、Aftermath、DraftTextSource。十项各 1/1，零错误、零警告，每项均核对 D3D12、进程和测试退出 0、原生 JSON 及 HTML 导出。[逐例报告](G:/UEgame/.worktrees/pipelines/test/.build/receive51-1123/normal/) · [进程记录](G:/UEgame/.worktrees/pipelines/test/.build/receive51-1123/normal-process.txt)
- 独立审查者 verify 逐张核对 12 张新截图。魔法群众及教师的隐藏启动图后缀为 00002，正常回归为 00003，其余八图为 00001。三条路线的角色、字幕 ID 及正文符合草稿契约，另外四张呈现启动、救援成功、回访及死亡旁白。[截图目录](G:/UEgame/.worktrees/pipelines/tech/Deliverables/B01-hidden-startup-20261123/Windows/UEgame/Saved/Screenshots/Windows/)
- 运行中最小化负例实跑一例。首次出现 NAR-0002 后，17:40:14（上海）确认 IsIconic=True。群众回执超时，随后回访结束超时，对应当前字幕为 NAR-0002、NAR-0006。两次均为 windowMinimized=1，零送达，测试退出 -1、进程 255。[负例报告](G:/UEgame/.worktrees/pipelines/test/.build/receive51-1123/negative-r2/index.json) · [窗口操作](G:/UEgame/.worktrees/pipelines/test/.build/receive51-1123/negative-actions.txt)
- verify 独立核对候选 diff、包身份、正负例报告和截图，同意按启动范围接收。[#51](https://github.com/nilesthump/UEgame/pull/51) 通过 merge commit df2d04f 并入 test。[CI](https://github.com/nilesthump/UEgame/actions/runs/45147291663) 仓库基线及核心通过，文本检查按路径跳过。
- 运行时可见性单独交 tech。旧包在所选 HWND 459102 隐藏后未观测到恢复，同轮仍登记了 NAR-0002 和 NAR-0006。需在实际视口 HWND 上逐帧关联可见状态、绘制及回执，验证隐藏期间零送达、恢复后重新累计连续显示时间。11 月 22 日历史首败缺少当时的窗口采样，仍未归因。[旧包对照](G:/UEgame/.worktrees/pipelines/tech/.build/startup-final-red-1123/)
