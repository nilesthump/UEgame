# 验收证据（当前）

- 目标版本：`0.1.0`
- 实际线上版本：未发布（`onlineVersion=null`，无 tag）
- 更新：2026-11-22
- 维护者：test
- 结论：test 独立运行 Win64 包 8/8 通过。剑术回访中两段草稿字幕依次送达后结束。完整 B01、正式媒体、性能与本人体验仍待验。

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
| PROD-B01 | 0.1.0 | B01 完整功能 | InProgress（剑术回访包 8/8） | [PR #45](https://github.com/nilesthump/UEgame/pull/45)，产品 eb00d4b，候选 4d053b6；[CI](https://github.com/nilesthump/UEgame/actions/runs/44910199222) | 魔法与混用的 UE 回访、完整环境与战斗、卸载恢复、实物输入、性能及本人体验待验 |
| DISPLAY-01 | 0.1.0 | 少量文本 UE 显示/字幕接入 | Partial（NAR-0001、0004、0005 草稿显示通过） | TownDialogue、Aftermath 与 DraftTextSource 各 1/1，截图和回执顺序见下 | NAR-0004 仅草稿探针；正式首批中的 NAR-0002、0003，文本定稿、正常有声路径和听辨待验 |
| PERF-01 | 0.1.0 | 输入→渲染 <50ms | NotRun | 无 | 测量链未建立 |
| FEEL-01 | 0.1.0 | 本人因果体验 | NotRun | 无 | 未试玩 |

```text
Governance=Passed
B01.Core=8/8 (tech GCC 13 JUnit, eb00d4b), core-linux CI passed (#45)
TaskBranches=Removed (7 remote / 9 local / 5 worktrees)
ProductRelease=NotRun
MainIntegration=#17 / #20 (exact SHA in Git)
```

## 0.1.0 / PR #45 本机独立复测

- 2026-11-22 15:32（上海），test 从 00ad6c1 运行 tech 工作树 Deliverables/B01-town-dialogue-20261121/Windows/UEgame.exe。包内 BUILD-IDENTITY.json 标记 Win64 Development、UE 5.6.1 CL44394996、B01Bootstrap、源码 eb00d4b。构建日志显示归档完成、退出码 0。源码至候选 4d053b6 的差异只有 tech 文档。
- test 工作树 .build/b01-town-dialogue-receive-20261122-153205/ 内按用例名保存 index.json 和同名 .log。TownDialogue、Revisit、RescueSuccess、RescueTeardown、Lifecycle、Bootstrap、Aftermath、DraftTextSource 各用同一包启动一次 D3D12，各 1/1 Success，进程及测试退出码均为 0，无失败、跳过或逐例警告和错误。八份日志均提示缺少 HTML 报告模板，原生报告完整。
- TownDialogue 通过软件按键接近群众、离开再返回。第 1 次字幕中断，两项回执均为空。第 2 次完整显示 NAR-0001 后获得群众回执，仍在回访。第 3 次完整显示 NAR-0005 后获得教师回执，两项齐备才进入 Ended。
- 已目视同一归档包 Windows/UEgame/Saved/Screenshots/Windows/ 中本轮后缀 00001.png 的六张图。B01TownCrowd、B01TownTeacher 均显示正确说话者、完整中文和标点，以及 draft / TextFallback。B01Bootstrap 显示就绪，B01Aftermath 显示孩子死亡与 NAR-0004，B01RescueSuccess 显示第五击后两人生还与五个威胁死亡，B01Revisit 显示三十年后剑术教师、学生、玩家与 A/D 移动提示。原生报告、日志和截图存于本机。
- [候选 CI](https://github.com/nilesthump/UEgame/actions/runs/44910199222) 中 repository-baseline、changes、core-linux 成功，narrative-text 按路径跳过。tech 本地 GCC 13 JUnit 8/8，D3D12 宿主 2/2；上游 independent_review 已批准功能和归档包。
- test 独立审查者 receive_review 核对候选差异、构建身份、八份报告与日志、六张新图及验收文档，批准本批受限接收和交付。#45 以 merge commit fb5f491 并入 pipeline/test。
