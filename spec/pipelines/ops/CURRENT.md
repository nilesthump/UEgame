# ops — 当前工作

- 目标版本 `0.1.0`，线上未发布，`onlineVersion=null`。
- 固定分支 `pipeline/ops`，工作树 `.worktrees/pipelines/ops`。
- `OPS-0.1.0-01` 治理集成已完成。B01 仍在实施，完整验收与发布条件待满足。

## 当前接收

2026-11-21，ops 通过 [PR #44](https://github.com/nilesthump/UEgame/pull/44) 接收三十年回访灰盒的受限验收记录，合并提交 `e08636d`，test 源提交 `00ad6c1`。[PR #43](https://github.com/nilesthump/UEgame/pull/43) 对应 tech 候选 `68f2cee`，同一 Win64 Development 包的产品提交为 `85bb569`。test 已验证旧时代两次雨事件结算、教师暂不可用时维持遮挡，以及回访就绪后恢复 A/D 移动。

- test 在 D3D12 下执行七项用例，并目视检查启动、失败草稿、第五击过渡及回访四张截图。上游独立审查者 `receive_review` 核对构建、报告、日志、截图及验收文档后批准受限接收。
- ops 复核构建身份、七份原生 JSON、对应日志及四张截图的位置。每份报告各 1/1 Success，七份日志的测试退出码均为 0。HTML 报告因模板缺失未生成。ops 本轮游戏用例执行数为 0。产物位置见 test 维护的[验收记录](../../evidence/evidence.md)。
- [#44 CI](https://github.com/nilesthump/UEgame/actions/runs/44872087748) 的仓库基线及 Linux 核心检查成功，剧情文本检查按路径跳过。[候选 CI](https://github.com/nilesthump/UEgame/actions/runs/44761269763) 的 Linux 核心用例 7/7 通过。

## 下一步

tech 继续接入回访后群众与教师关键内容的实际送达，经 test 复测后正向交付。完整环境后果、真实战斗与导航、世界卸载与恢复、正式声音和字幕、实物输入、输入到对应画面实际呈现 <50 ms 及本人试玩仍待验证。B01 为 InProgress，DISPLAY-01 为 Partial，仅 NAR-0004 草稿显示通过。

## 发布边界

ops 按[交互协议](../PROTOCOL.md)接收 test 或 maintenance 的候选。完整 B01 与发布条件仍待满足。目标版本、线上版本和标签以[发布状态](../../releases/state.json)为准；正式发布依照[发布流程](../../releases/README.md)办理。
