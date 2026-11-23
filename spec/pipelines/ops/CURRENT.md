# ops — 当前工作

- 目标版本 `0.1.0`，线上未发布，`onlineVersion=null`。
- 固定分支 `pipeline/ops`，工作树 `.worktrees/pipelines/ops`。
- `OPS-0.1.0-01` 治理集成已完成。B01 仍在实施，完整验收与发布条件待满足。

## 当前接收

2026-11-23，ops 通过 [PR #52](https://github.com/nilesthump/UEgame/pull/52) 接收 B01 回访字幕自动化用例的启动修复及复测记录。ops 合并提交为 `cde5fd0`，test 验收提交为 `446446b`。[PR #51](https://github.com/nilesthump/UEgame/pull/51) 的产品提交为 `a35d3f0`，候选为 `8b7d2c4`，test 接收合并为 `df2d04f`。

- ops 核对了同一 Win64 Development 归档包的构建身份、原生报告和窗口操作记录。受控隐藏后，同一游戏窗口在启动用例时恢复可见，魔法路线 1/1 通过，群众与教师各登记一份回执。正常窗口 D3D12 十项各通过 1/1，零错误、零警告，进程均退出 0。
- 运行中最小化负例出现两次预期超时，零回执，进程退出 255。旧包对照中，外部采样未观察到所选窗口恢复，游戏仍登记两份回执。实际视口与回执的逐帧对应关系待 tech 核实。
- 上游独立审查者 `verify` 核对了差异、报告、12 张新截图和验收文档，同意按启动修复范围接收。执行链接见 test 维护的[验收记录](../../evidence/evidence.md)。[#52 CI](https://github.com/nilesthump/UEgame/actions/runs/45150011672) 的仓库基线与 Linux 核心检查通过，剧情文本检查按路径跳过。

## 下一步

tech 从 [#51 原讨论](https://github.com/nilesthump/UEgame/pull/51) 继续核对实际视口窗口标识、可见状态、绘制与字幕回执，验证隐藏期间零送达、恢复后重新累计完整显示时长，再交 test 复测。完整战斗、导航、环境后果、世界卸载与恢复仍待 tech 交付。正式字幕与声音、实物输入、输入到对应画面实际呈现小于 50 ms 及本人试玩仍待验证。B01 为 InProgress，DISPLAY-01 为 Partial。

## 发布边界

ops 按[交互协议](../PROTOCOL.md)接收 test 或 maintenance 的候选。完整 B01 与发布条件仍待满足。目标版本、线上版本和标签以[发布状态](../../releases/state.json)为准；正式发布依照[发布流程](../../releases/README.md)办理。
