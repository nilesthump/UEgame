# ops — 当前工作

- 目标版本 `0.1.0`，线上未发布，`onlineVersion=null`。
- 固定分支 `pipeline/ops`，工作树 `.worktrees/pipelines/ops`。
- `OPS-0.1.0-01` 治理集成已完成。B01 仍在实施，完整验收与发布条件待满足。

## 当前接收

2026-11-22，ops 通过 [PR #48](https://github.com/nilesthump/UEgame/pull/48) 接收 B01 正常窗口 D3D12 下剑术、魔法、混用三条灰盒回访路线的草稿字幕及所列回归。合并提交为 `0af8ff4`，test 验收提交为 `ccc7dc6`。[PR #47](https://github.com/nilesthump/UEgame/pull/47) 的产品提交为 `ac9257a`，候选为 `ddab47d`，test 接收合并为 `717b4f7`。

- test 在同一 Win64 Development 包上实际执行十项，各通过 1/1，并目视核对十张截图。三条路线均要求中断的群众字幕重播，群众与教师的回执齐备后才结束回访。逐例结果和执行入口见 test 维护的[验收记录](../../evidence/evidence.md)。
- ops 核对了十份原生报告和对应日志，每例错误和警告均为 0，测试退出码均为 0。十份日志均记录 HTML 模板加载失败。上游独立审查者 `verify_checks` 批准 `ccc7dc6` 按上述范围交付。
- [#48 CI](https://github.com/nilesthump/UEgame/actions/runs/45005070117) 的仓库基线与 Linux 核心检查成功，剧情文本检查按路径跳过。

## 下一步

tech 排查隐藏窗口首轮魔法用例失败及 HTML 报告导出问题，交 test 复测。完整战斗、导航、环境后果、世界卸载与恢复仍待 tech 交付。narrative 定稿、art 提供声音素材、tech 集成后，由 test 复测正式字幕与声音。实物输入、输入到对应画面实际呈现小于 50 ms 及本人试玩仍待验证。B01 为 InProgress，DISPLAY-01 为 Partial。

## 发布边界

ops 按[交互协议](../PROTOCOL.md)接收 test 或 maintenance 的候选。完整 B01 与发布条件仍待满足。目标版本、线上版本和标签以[发布状态](../../releases/state.json)为准；正式发布依照[发布流程](../../releases/README.md)办理。
