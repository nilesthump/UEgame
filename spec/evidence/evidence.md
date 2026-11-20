# 验收证据（当前）

- 目标版本：`0.1.0`
- 实际线上版本：未发布（`onlineVersion=null`，无 tag）
- 更新：2026-11-20
- 维护者：test
- 结论：B01 剑术救援灰盒及回归共六项 Win64 包用例经 test 独立复测通过；NAR-0004 draft 画面已目视验证。完整 B01、正式媒体、性能与本人体验仍未验收。

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
| PROD-B01 | 0.1.0 | B01 完整功能 | InProgress（受限独立包 6/6，Linux CI 6/6） | [PR #41](https://github.com/nilesthump/UEgame/pull/41)，候选 `4b31325`，产品 `c7872c3`；[CI](https://github.com/nilesthump/UEgame/actions/runs/44750096597)；本机独立报告见下 | 剑术灰盒止于 TimeSkip，环境结算、回访、真实战斗、正式媒体、实物输入、性能与本人体验待验 |
| DISPLAY-01 | 0.1.0 | 少量文本 UE 显示/字幕接入 | Partial（NAR-0004 draft 显示通过） | 同一包 Aftermath 与 DraftTextSource 各 1/1；本轮新图 `B01Aftermath00001.png` 中文、标点及 ID/draft 完整可读 | 仅单条草稿探针；不提交媒体送达回执，其余条目、正式字幕及声音未验收 |
| PERF-01 | 0.1.0 | 输入→渲染 <50ms | NotRun | 无 | 测量链未建立 |
| FEEL-01 | 0.1.0 | 本人因果体验 | NotRun | 无 | 未试玩 |

```text
Governance=Passed
B01.Core=Linux CI 6/6 (4b31325 / PR #41)
TaskBranches=Removed (7 remote / 9 local / 5 worktrees)
ProductRelease=NotRun
MainIntegration=#17 / #20 (exact SHA in Git)
```

## 0.1.0 / PR #41 本机独立复测

- test 于 2026-11-20 22:21（Asia/Shanghai）从 `pipeline/test` 基线 `238b334` 执行。直接启动 tech 完整包 `Deliverables/B01-rescue-success-20261120/Windows/UEgame.exe` 六次，使用默认地图。BUILD-IDENTITY 记录产品 `c7872c3`、UE 5.6.1 CL44394996、Win64 Development。构建日志记录 Archive 成功、UAT 退出码 0。
- test 本机报告根为 `.build/b01-rescue-success-receive-20261120-222110/`。子目录 rescue-success、rescue-teardown、lifecycle、default、aftermath、text-source 的 index.json 各精确执行一个指定用例，分别为 RescueSuccess、RescueTeardown、Lifecycle、Bootstrap、Aftermath、DraftTextSource。六次进程退出码和同名日志的测试退出码均为 0。六份报告均为 Success，failed、notRun、inProcess、succeededWithWarnings 和逐例 errors/warnings 均为 0。六份日志均记录 HTML 模板缺失，原生报告完整。
- RescueSuccess 使用软件注入 E 发现遇险，五次剑击各击败一个威胁。前四次仍处于 Rescue，第五次转入 TimeSkip 并拒绝继续攻击。玩家和小孩均存活。RescueTeardown、Lifecycle 同时回归同一 UWorld 的销毁、跨帧 GC、弱引用及旧轮隔离。
- 已目视包内 `Windows/UEgame/Saved/Screenshots/Windows/` 的本轮三图。B01Bootstrap00001.png 显示 ready；B01Aftermath00001.png 显示小孩死亡、五个威胁存活，NAR-0004 draft 中文及标点可读；B01RescueSuccess00001.png 显示等待环境结算、玩家和小孩存活、五个威胁死亡。报告、日志、截图均为本机文件，跨机复核须传递实际产物。
- [候选 CI](https://github.com/nilesthump/UEgame/actions/runs/44750096597) 四项成功，core-linux 日志为 6/6、0 失败。上游另在 D3D12 游戏宿主执行 RescueSuccess 1/1；test 本次独立运行的是 Win64 包。
- [PR #41](https://github.com/nilesthump/UEgame/pull/41) 的候选为 `4b31325`，由上游 independent_review 审查。test 独立审查者 receive_review 复核候选及本轮执行证据，批准本批受限接收。接收 merge commit 为 `e63ab7c`。
- 环境结算回执、30 年后回访布置与恢复控制、真实感知和战斗、世界卸载/切图、旧界面清理、存档恢复、正式字幕和声音、实物手柄、本人试玩及输入→核心→对应画面实际呈现 <50 ms 均待验证。B01 仍在实施，全部 NAR 仍为 draft，B13 素材制作仍受限。
