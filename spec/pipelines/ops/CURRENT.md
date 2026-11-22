# ops — 当前工作

- 目标版本 `0.1.0`，线上未发布，`onlineVersion=null`。
- 固定分支 `pipeline/ops`，工作树 `.worktrees/pipelines/ops`。
- `OPS-0.1.0-01` 治理集成已完成。B01 仍在实施，完整验收与发布条件待满足。

## 当前接收

2026-11-22，ops 通过 [PR #46](https://github.com/nilesthump/UEgame/pull/46) 接收 B01 剑术回访草稿字幕的阶段验收，合并提交为 `9bd1d90`，test 验收提交为 `56dfbc9`。[PR #45](https://github.com/nilesthump/UEgame/pull/45) 交付的 tech 候选为 `4d053b6`，产品提交为 `eb00d4b`。

- test 在同一 Win64 Development 包上独立启动八次，八项 D3D12 用例各通过 1/1，并目视检查六张截图。镇民甲字幕 NAR-0001 首次中断时未产生回执，返回后完整送达。教师字幕 NAR-0005 完整送达后回访结束。
- 上游独立审查者 `receive_review` 批准了本批受限接收与交付。test 留存了八份原生 JSON 及对应日志，HTML 报告因缺少模板未生成。执行记录、截图和验收结论见 test 维护的[验收记录](../../evidence/evidence.md)。
- [#46 CI](https://github.com/nilesthump/UEgame/actions/runs/44999137772) 的仓库基线及 Linux 核心检查成功，剧情文本检查按路径跳过。

## 下一步

tech 继续实现魔法与混用路线的真实 UE 回访、完整环境后果、战斗与导航、世界卸载与恢复。narrative 定稿后，art 与 tech 制作并接入正式声音。test 后续验证正常有声路径、实物输入、输入到对应画面实际呈现小于 50 ms 及本人试玩。B01 仍为 InProgress，DISPLAY-01 为 Partial，已通过的草稿显示限于 NAR-0001、NAR-0004 和 NAR-0005。

## 发布边界

ops 按[交互协议](../PROTOCOL.md)接收 test 或 maintenance 的候选。完整 B01 与发布条件仍待满足。目标版本、线上版本和标签以[发布状态](../../releases/state.json)为准；正式发布依照[发布流程](../../releases/README.md)办理。
