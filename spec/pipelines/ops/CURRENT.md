# ops — 当前工作

- 目标版本 `0.1.0`，线上未发布，`onlineVersion=null`。
- 固定分支 `pipeline/ops`，工作树 `.worktrees/pipelines/ops`。
- `OPS-0.1.0-01` 治理集成已完成。B01 仍在实施，完整验收与发布条件待满足。

## 当前接收

2026-11-23，ops 通过 [PR #54](https://github.com/nilesthump/UEgame/pull/54) 接收 B01 运行时字幕回执修复。ops 合并提交为 `48ac4e6`，test 验收提交为 `df15b96`。[PR #53](https://github.com/nilesthump/UEgame/pull/53) 的产品与 Win64 包均对应 `b58e07d`，test 接收合并为 `ef87e18`。

- ops 核对了包身份、窗口记录及原生报告。test 在字幕显示时隐藏同一窗口 7.020 秒，210 个采样帧内零合格绘制、零回执。恢复首绘后重新计时，群众和教师字幕分别累计 5.860 秒和 5.669 秒后各送达一次。
- 隐藏启动恢复原窗口，1/1 通过。正常窗口 D3D12 十项全部通过，零错误、零警告。持续最小化时零回执，原生报告记录四项预期超时。#51 旧包异常的历史原因仍未证实。
- 独立审查者 `verify` 核对了产品差异、逐帧记录、原生报告和 14 张正例截图，同意阶段性交付。实际执行入口见 test 维护的[验收记录](../../evidence/evidence.md)。[#54 CI](https://github.com/nilesthump/UEgame/actions/runs/45155905721) 的仓库基线与 Linux 核心检查通过，剧情文本检查按路径跳过。

## 下一步

完整战斗、导航、环境后果、世界卸载与恢复仍待 tech 交付，再由 test 验收。正式字幕与声音、实物输入、输入到对应画面实际呈现小于 50 ms 及本人试玩仍待验证。B01 为 InProgress，DISPLAY-01 为 Partial。

## 发布边界

ops 按[交互协议](../PROTOCOL.md)接收 test 或 maintenance 的候选。完整 B01 与发布条件仍待满足。目标版本、线上版本和标签以[发布状态](../../releases/state.json)为准；正式发布依照[发布流程](../../releases/README.md)办理。
