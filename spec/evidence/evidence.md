# 验收证据（当前）

- 目标版本：`0.1.0`
- 实际线上版本：未发布（`onlineVersion=null`，无 tag）
- 更新：2026-11-17
- 维护者：test
- 结论：B01 受限 Win64 候选五个用例经 test 独立复测通过；NAR-0004 draft 画面已目视验证。完整 B01、正式媒体、性能与本人体验仍未验收。

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
| PROD-B01 | 0.1.0 | B01 完整功能 | InProgress（受限独立包 5/5；Linux CI 6/6） | [PR #37](https://github.com/nilesthump/UEgame/pull/37)，候选 `36b0529` / 产品 `b4f94e6`；[CI 43410022113](https://github.com/nilesthump/UEgame/actions/runs/43410022113) baseline/core-linux 成功，narrative-text 跳过不记通过；test 新报告与截图见下 | 真实玩法/成功路线、正式媒体、端到端 <50ms、本人体验、真实手柄未完成 |
| DISPLAY-01 | 0.1.0 | 少量文本 UE 显示/字幕接入 | Partial（NAR-0004 draft 显示通过） | 同一包 Aftermath 与 DraftTextSource 各 1/1；本轮新图 `B01Aftermath00001.png` 中文、标点及 ID/draft 完整可读 | 仅单条草稿探针；不提交媒体送达回执，其余条目、正式字幕及声音未验收 |
| PERF-01 | 0.1.0 | 输入→渲染 <50ms | NotRun | 无 | 测量链未建立 |
| FEEL-01 | 0.1.0 | 本人因果体验 | NotRun | 无 | 未试玩 |

```text
Governance=Passed
B01.Core=Linux CI 6/6 (36b0529 / PR #37)
TaskBranches=Removed (7 remote / 9 local / 5 worktrees)
ProductRelease=NotRun
MainIntegration=#17 / #20 (exact SHA in Git)
```

## 当前说明

- test 在自身工作区直接复测 PR #37 的同一完整包，不重新构建或更改产品源码、BUILD-IDENTITY；此前 #35 的记录保留于 Git 历史。
- 本批只覆盖同一 UWorld 内未结束 Rescue 的 Actor 销毁、跨帧完整 GC、弱发送者解析及旧 Run 隔离，并回归生命周期、默认启动、后果灰盒与单条 draft 文本。不证明真实异步回调调度系统。
- 完整 B01 仍 InProgress；世界卸载、地图切换、Slate 残留、持久恢复、真实玩法/成功路线、正式媒体、其他文本、端到端输入→核心→对应画面实际呈现 <50ms、本人体验及实物输入均未验收。软件注入不代表实物测试，截图不证明呈现延迟。
- 全部 NAR 仍 draft，B02–B15 不启动；onlineVersion=null，不发布、不建 tag。

## 0.1.0 / PR #37 本机独立复测

- 执行者：本对话 test agent；2026-11-13 20:59–21:00（Asia/Shanghai）。执行时 HEAD `15d31034edffe1ad9045fd4e717d9a62a039033f`；候选 `36b0529c4db8cfa40c4aa138cc162e7741334512`。11 月 17 日完成接收检查，未重复运行游戏。
- 完整包：`G:/UEgame/.worktrees/pipelines/tech/Deliverables/B01-rescue-teardown-20261113/`。构建源码 `b4f94e68a834a2f0e95bdb90718de7d89d1b812a`；BUILD-IDENTITY 与构建日志一致，UE 5.6.1 CL44394996 / Win64 Development；后续仅文档变化，不改变构建输入。
- 按原 PR 命令分别启动根 `Windows/UEgame.exe` 五次，不覆盖地图；RescueTeardown/Lifecycle/Aftermath 使用 `-B01AftermathGraybox` 和 `-B01DraftTextPreview`，Bootstrap/DraftTextSource 不使用。五进程及日志 TEST COMPLETE EXIT CODE 均为 0。
- 报告根（相对 test）：`.build/b01-rescue-teardown-receive-20261113-205935/`。子目录 `rescue-teardown/lifecycle/default/aftermath/text-source` 的 index.json 分别精确执行 `UEgame.WorldScenario.B01.RescueTeardown`、`UEgame.WorldScenario.B01.Lifecycle`、`UEgame.WorldScenario.B01.Bootstrap`、`UEgame.WorldScenario.B01.Aftermath`、`UEgame.B01.DraftTextSource`。每份 1/1 Success，failed/notRun/inProcess/succeededWithWarnings 与逐例 errors/warnings 均为 0；根目录保留对应同名 .log。
- 新截图（相对包根）：`Windows/UEgame/Saved/Screenshots/Windows/B01Bootstrap00001.png` 和 `B01Aftermath00001.png`。主 agent 与独立审查均目视：默认 ready；灰盒 ended、七标记、中文标点及 NAR-0004/draft 完整清楚。旧 00000 图保留。RescueTeardown/Lifecycle 无截图，不用 Aftermath 证明 Slate 清理。
- 五日志均有 HTML 模板缺失 Error 和 ROOT certificate store Warning；未生成 HTML。原生 JSON、日志和截图完整，不声称日志零错误/零警告。输出是本机忽略文件，不随 Git 自动交付，跨机器须提供完整包和报告。
- [CI 43410022113](https://github.com/nilesthump/UEgame/actions/runs/43410022113) 对应候选 `36b0529`：repository-baseline/core-linux 成功；11 月 17 日使用已登录 gh CLI 实读 core-linux 日志，6/6、0 failures。narrative-text skipped 不计通过。本地 Editor/Linux 未重跑。
- 上游独立审查 `/root/rescue_review` 见原 PR。本轮 `/root/receive37_review` 已实查三文件 diff、构建身份/日志、包文件、五份新报告/日志与两张截图，独立重跑 RepositoryBaseline 通过，批准受限 merge commit 接收，并补审本次 evidence/CURRENT 两文档、批准证据提交；未重跑游戏，进程退出码来自主 agent 实际执行记录，不等于 GitHub 账号批准。
- 接收状态：PR #37 已以 merge commit `019a840c32f2a38585a1027786069ec077ee77cb` 合入 `pipeline/test`，固定源分支保留。RepositoryBaseline 209 文档/681 链接、上游 PipelineScope（2 commits/0 merges/4 逐提交文件计数）与 diff 检查通过。检查使用仓库 CI 同款 `pwsh -NoProfile -File`，未修改执行策略。
