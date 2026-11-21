# 验收证据（当前）

- 目标版本：`0.1.0`
- 实际线上版本：未发布（`onlineVersion=null`，无 tag）
- 更新：2026-11-21
- 维护者：test
- 结论：B01 剑术救援后三十年回访灰盒及回归共七项 Win64 包用例经 test 独立复测通过；NAR-0004 draft 画面已目视验证。完整 B01、正式媒体、性能与本人体验仍未验收。

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
| PROD-B01 | 0.1.0 | B01 完整功能 | InProgress（Win64 灰盒 7/7，Linux 核心 CI 7/7） | [PR #43](https://github.com/nilesthump/UEgame/pull/43)，候选 `68f2cee`，产品 `85bb569`；[本批 CI](https://github.com/nilesthump/UEgame/actions/runs/44761269763)；本机报告见下 | 已验至回访恢复控制；完整环境后果、战斗、关键内容送达、实物输入、性能与本人体验待验 |
| DISPLAY-01 | 0.1.0 | 少量文本 UE 显示/字幕接入 | Partial（NAR-0004 draft 显示通过） | 同一包 Aftermath 与 DraftTextSource 各 1/1；本轮新图 `B01Aftermath00001.png` 中文、标点及 ID/draft 完整可读 | 仅单条草稿探针；不提交媒体送达回执，其余条目、正式字幕及声音未验收 |
| PERF-01 | 0.1.0 | 输入→渲染 <50ms | NotRun | 无 | 测量链未建立 |
| FEEL-01 | 0.1.0 | 本人因果体验 | NotRun | 无 | 未试玩 |

```text
Governance=Passed
B01.Core=Linux CI 7/7 (68f2cee / PR #43)
TaskBranches=Removed (7 remote / 9 local / 5 worktrees)
ProductRelease=NotRun
MainIntegration=#17 / #20 (exact SHA in Git)
```

## 0.1.0 / PR #43 本机独立复测

- test 于 2026-11-21 13:33（Asia/Shanghai）从基线 `dddcd81` 启动 tech 工作树内 `Deliverables/B01-revisit-20261120/Windows/UEgame.exe` 七次。BUILD-IDENTITY.json 记录产品 `85bb569`、UE 5.6.1 CL44394996、Win64 Development 和默认 B01Bootstrap 地图。构建日志 `.build/package-revisit-20261120.log` 显示 Archive 完成，UAT 退出码 0。产品提交至候选头仅改 tech 交接文档。
- test 工作树报告根 `.build/b01-revisit-receive-20261121-133318/`。Revisit、RescueSuccess、RescueTeardown、Lifecycle、Bootstrap、Aftermath、DraftTextSource 各有独立子目录的 `index.json` 与根目录的同名日志。七次 D3D12 启动各执行指定用例 1/1 Success，进程与测试日志退出码均为 0，失败、未运行、执行中、带警告成功及逐例错误和警告均为 0。日志均提示 HTML 模板缺失，原生 JSON 完整。
- 救援成功后，旧时代第 180 秒开雨、第 300 秒停雨两个计划均已消费。教师暂不可用时场景保持遮挡，控制恢复回执等待；恢复绑定后进入回访并开始计时，七个旧标记退出画面。软件注入 D 使玩家向画面右移，A 返回原屏幕位置。此批只覆盖同一世界的剑术标记场景。
- 已目视包内 `Windows/UEgame/Saved/Screenshots/Windows/` 的四张本轮新图。`B01Bootstrap00001.png` 显示 ready；`B01Aftermath00001.png` 显示小孩死亡及 NAR-0004 draft 中文旁白；`B01RescueSuccess00001.png` 显示第五击后的过渡、两名生还者及五个死亡威胁；`B01Revisit00001.png` 显示三十年后玩家、剑术教师、学生及 A/D 移动提示。原生报告、日志和截图在本机，跨机复核需传递实际产物。
- [候选 CI](https://github.com/nilesthump/UEgame/actions/runs/44761269763) 中 changes、repository-baseline、core-linux 成功，Linux 核心 7/7，narrative-text 按路径跳过。上游独立审查者 review 已批准产品与交付证据。test 独立审查者 receive_review 核对差异、七份原生报告、日志、四图及验收文档后批准受限接收。#43 接收提交为 `557d522`。
- 完整火湿与破坏痕迹、真实战斗和导航、世界卸载与恢复、群众与教师关键内容实际送达、正式声音与字幕、实物手柄、本人试玩及输入→核心处理→对应画面实际呈现 <50 ms 待验证。
