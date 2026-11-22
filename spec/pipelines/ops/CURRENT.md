# ops — 当前工作

- 目标版本 `0.1.0`，线上未发布，`onlineVersion=null`。
- 固定分支 `pipeline/ops`，工作树 `.worktrees/pipelines/ops`。
- `OPS-0.1.0-01` 治理集成已完成。B01 仍在实施，完整验收与发布条件待满足。

## 当前接收

2026-11-22，ops 通过 [PR #50](https://github.com/nilesthump/UEgame/pull/50) 接收 B01 开发包 HTML 报告导出与最小化窗口字幕诊断。ops 合并提交为 `c70b35e`，test 验收提交为 `9385393`。[PR #49](https://github.com/nilesthump/UEgame/pull/49) 的产品提交为 `3517bab`，候选为 `da873ae`，test 接收合并为 `5de495f`。

- ops 核对构建身份和执行记录。同一 Win64 Development 包在正常窗口下执行十项 D3D12 用例，各通过 1/1、零错误、零警告，进程均退出 0。test 已目视核对十张截图。
- 两轮魔法路线最小化负例各执行 1 例，进程均退出 255。首轮窗口非活动，首错为群众字幕缺失，共五个超时；次轮窗口活动，首错为群众回执缺失，共两个超时。两轮均确认窗口已最小化，均无送达回执。十项正常用例和两轮负例各有 JSON 和非空 HTML。
- 上游独立审查者 `receive_review` 批准本批受限接收。逐例报告和窗口操作记录见 test 维护的[验收记录](../../evidence/evidence.md)。[#50 CI](https://github.com/nilesthump/UEgame/actions/runs/45023629189) 的仓库基线与 Linux 核心检查通过，剧情文本检查按路径跳过。

## 下一步

tech 继续确认历史隐藏启动首错的根因，交 test 复测。完整战斗、导航、环境后果、世界卸载与恢复仍待 tech 交付。narrative 定稿、art 提供声音素材、tech 集成后，由 test 复测正式字幕与声音。实物输入、输入到对应画面实际呈现小于 50 ms 及本人试玩仍待验证。B01 为 InProgress，DISPLAY-01 为 Partial。

## 发布边界

ops 按[交互协议](../PROTOCOL.md)接收 test 或 maintenance 的候选。完整 B01 与发布条件仍待满足。目标版本、线上版本和标签以[发布状态](../../releases/state.json)为准；正式发布依照[发布流程](../../releases/README.md)办理。
