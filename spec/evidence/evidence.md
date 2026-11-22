# 验收证据（当前）

- 目标版本：`0.1.0`
- 实际线上版本：未发布（`onlineVersion=null`，无 tag）
- 更新：2026-11-22
- 维护者：test
- 结论：test 在同一 Win64 包独立复测 10/10。魔法、混用、剑术三条灰盒回访均在群众和教师两份字幕回执齐备后结束。B01 仍在实施。

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
| PROD-B01 | 0.1.0 | B01 完整功能 | InProgress（三条灰盒回访通过） | [PR #47](https://github.com/nilesthump/UEgame/pull/47)，产品 ac9257a，候选 ddab47d；[CI](https://github.com/nilesthump/UEgame/actions/runs/45001871284)，同包执行记录见下 | 完整战斗、导航、环境火湿与破坏、世界卸载恢复、实物输入、性能与本人体验待验 |
| DISPLAY-01 | 0.1.0 | 少量文本 UE 显示/字幕接入 | Partial（六条回访草稿与 NAR-0004 草稿探针可见） | 三条路线各核对两张字幕截图，Aftermath 核对失败旁白；截图编号及文本 ID 见下 | 正式文本、语音与字幕同步、听辨待验 |
| PERF-01 | 0.1.0 | 输入→渲染 <50ms | NotRun | 无 | 测量链未建立 |
| FEEL-01 | 0.1.0 | 本人因果体验 | NotRun | 无 | 未试玩 |

```text
Governance=Passed
B01.Core=8/8 (tech GCC 13 JUnit, ac9257a), core-linux CI passed (#47)
TaskBranches=Removed (7 remote / 9 local / 5 worktrees)
ProductRelease=NotRun
MainIntegration=#17 / #20 (exact SHA in Git)
```

## 0.1.0 / PR #47 本机独立复测

- 2026-11-22 16:23（上海），test 从 56dfbc9 直接启动 tech 工作树 `Deliverables/B01-revisit-routes-20261122/Windows/UEgame/Binaries/Win64/UEgame.exe`，使用正常窗口和 D3D12。BUILD-IDENTITY.json 对应 ac9257a、Win64 Development、UE 5.6.1 CL44394996、B01Bootstrap。UAT 归档退出码 0，产品至候选 ddab47d 仅变更 tech 文档。
- test 工作树 `.build/b01-routes-receive-20261122-162312/` 保存逐例 index.json 与同名 .log。MagicTownDialogue、MixedTownDialogue、TownDialogue、Revisit、RescueSuccess、RescueTeardown、Lifecycle、Bootstrap、Aftermath、DraftTextSource 各执行 1/1，进程及测试退出码均为 0。十份原生报告均为 Success，逐例错误和警告为 0。十份日志均报 HTML 模板缺失。
- 软件按键 E 发现后，分别执行五次 F、两次 E 再三次 F、五次 E。每条路线首次靠近群众后离开，两项回执均为空。返回后第 2 次字幕完整送达，回访继续。第 3 次字幕由教师送达，双回执齐备进入 Ended。
- 已目视归档包 `Windows/UEgame/Saved/Screenshots/Windows/` 中本轮十张 PNG。魔法 B01MagicTownCrowd/Teacher 后缀 00002（镇民乙 NAR-0002、教师 NAR-0006），混用 B01MixedTownCrowd/Teacher 后缀 00001（镇民丙 NAR-0007、教师 NAR-0003），剑术 B01TownCrowd/Teacher 后缀 00001（镇民甲 NAR-0001、教师 NAR-0005）。六张均显示对应路线标记及 draft / TextFallback，中文全文和标点与 TEXT.csv 一致。
- 四张回归截图后缀均为 00001。B01Bootstrap 显示就绪；B01Aftermath 显示失败后果及 NAR-0004；B01RescueSuccess 显示玩家与孩子生还、五个威胁死亡；B01Revisit 显示三十年后剑术教师、学生与移动提示。
- tech 的 GCC 13 JUnit 为 8/8，D3D12 宿主三条路线为 3/3。上游独立审查者 review 已审查构建、运行证据和截图。tech 首轮隐藏启动器的魔法用例失败，日志未记录焦点，原因未确认。本轮结果仅覆盖正常窗口的包内程序启动方式。
- 独立审查者 receive_review 核对候选差异、归档身份、十份原生报告与日志、十张截图及验收记录，批准本批灰盒接收。#47 以 merge commit 717b4f7 并入 pipeline/test。
