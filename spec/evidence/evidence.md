# 验收证据（当前）

- 目标版本：`0.1.0`
- 实际线上版本：未发布（`onlineVersion=null`，无 tag）
- 更新：2026-11-13
- 维护者：test
- 结论：B01 受限 Win64 候选三个用例经 test 独立复测通过；NAR-0004 draft 画面已目视验证。完整 B01、正式媒体、性能与本人体验仍未验收。

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
| PROD-B01 | 0.1.0 | B01 完整功能 | InProgress（受限独立包 3/3；Linux CI 6/6） | [PR #33](https://github.com/nilesthump/UEgame/pull/33)，候选 `a5251c6` / 产品 `fb2861e`；[CI 43392806576](https://github.com/nilesthump/UEgame/actions/runs/43392806576) baseline/core-linux 成功，narrative-text 跳过不记通过；test 新报告与截图见下 | 真实玩法/成功路线、正式媒体、端到端 <50ms、本人体验、真实手柄未完成 |
| DISPLAY-01 | 0.1.0 | 少量文本 UE 显示/字幕接入 | Partial（NAR-0004 draft 显示通过） | 同一包 Aftermath 与 DraftTextSource 各 1/1；本轮新图 `B01Aftermath00001.png` 中文、标点及 ID/draft 完整可读 | 仅单条草稿探针；不提交媒体送达回执，其余条目、正式字幕及声音未验收 |
| PERF-01 | 0.1.0 | 输入→渲染 <50ms | NotRun | 无 | 测量链未建立 |
| FEEL-01 | 0.1.0 | 本人因果体验 | NotRun | 无 | 未试玩 |

```text
Governance=Passed
B01.Core=Linux CI 6/6 (a5251c6 / PR #33)
TaskBranches=Removed (7 remote / 9 local / 5 worktrees)
ProductRelease=NotRun
MainIntegration=#17 / #20 (exact SHA in Git)
```

## 当前说明

- 治理验收只覆盖分支/规则/协议/版本/文本结构/CI/证据，不覆盖产品、素材、性能或本人体验。
- `ops → main` #17 是治理基线集成，不创建发布 tag，也不改变 `onlineVersion=null`。
- 本次由 test 在自身工作区直接复测 PR #33 的同一集成构建产物；不重新构建、不更改产品源码或 BUILD-IDENTITY。MR 接收/合并状态以原 PR 为准。
- `PROD-B01` 仍为 InProgress：本轮仅验证默认启动、预设死亡灰盒观察链和单条草稿探针。软件注入 E 不代表实物键盘或手柄体验，截图不证明画面呈现延迟。B02–B15 不启动。
- `main` 的精确 SHA 由 Git 记录，不在证据正文自引用，避免后续每次合并产生新的过期状态。
- 最新 PR 检查失败或跳过时，本表对应条目改为“待复测”，不能以本地脚本或旧候选结果替代。

## 0.1.0 / PR #33 本机独立复测

- 执行者：本对话 test agent；2026-11-13 16:30–16:31（Asia/Shanghai）。工作区 `G:/UEgame/.worktrees/pipelines/test`，执行时 HEAD `c3875ed3d15c06a749b84f8eaa0f40d2c04f6f83`；候选 `a5251c62531fea800b85f82d86a10af0f62450d1`。
- 同一完整包：`G:/UEgame/.worktrees/pipelines/tech/Deliverables/B01-draft-text-20261113/`。产品提交 `fb2861e0294e0d6479087be13ec70fd1a1e2d78f`；BUILD-IDENTITY 保留提交前工作树构建、测试审查后源码原样提交的说明，不宣称干净提交构建。
- 按原 PR 接收命令分别启动根 `Windows/UEgame.exe` 三次；实际使用 D3D12，默认入口无探针开关，Aftermath 同时使用两个探针开关。三个进程退出码及日志 TEST COMPLETE EXIT CODE 均为 0。
- 原生报告根（相对 test）：`.build/b01-receive-20261113-163037/`。`default/index.json` 实际执行 `UEgame.WorldScenario.B01.Bootstrap`；`aftermath/index.json` 实际执行 `UEgame.WorldScenario.B01.Aftermath`；`text-source/index.json` 实际执行 `UEgame.B01.DraftTextSource`。每份 succeeded=1、failed/notRun/inProcess/succeededWithWarnings=0，逐例 Success、errors/warnings=0；对应同名 `.log` 保留执行和退出结果。
- 新图（相对包根）：`Windows/UEgame/Saved/Screenshots/Windows/B01Bootstrap00001.png` 与 `B01Aftermath00001.png`；旧 `00000` 图保留。test 已目视新图：默认 ready；灰盒 ended、七个标记、中文标点及 NAR-0004/draft 完整清楚。自动化断言不单独证明中文字形，显示结论结合本轮新图。
- 原生输出为本机忽略文件，不随 Git 自动交付；跨机器接收须另行提供同一包与报告。未重跑 Editor/Linux 本地构建；Linux 引用同候选 CI，未将跳过的 narrative-text 记通过。
- 上游独立审查 `/root/handoff_review` 的范围与限制见原 PR；本轮 `/root/receive_review` 独立核对三份新报告、日志、两张新图及文档 diff，修正旧 NotRun 残留并披露 HTML 限制后批准；不等于 GitHub 账号批准。
- 报告限制：三份日志均出现 HTML 模板缺失错误，未生成 HTML 页面；实际 index.json 完整、逐例成功且日志退出码 0。本次以已核查的原生 JSON、日志和新截图为依据，不声称日志无任何错误。此限制随原 MR 接收记录保留。
- 接收状态：另一 test 会话按原 PR 完成接收记录并于 2026-11-13 16:33 合并 #33，merge commit `b848a0de9fa04d6a83ef2045b4d8fc43fdcb73a5`；本会话核对远端与本地一致。用户随后指定本会话为唯一文档维护者，另一会话已确认停止写入。本页引用本会话 163037/00001 证据，不混用另一轮 163124/00002 输出。
