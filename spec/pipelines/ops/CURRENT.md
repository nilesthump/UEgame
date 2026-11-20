# ops — 当前工作

- 目标版本 `0.1.0`，线上未发布，`onlineVersion=null`。
- 固定分支 `pipeline/ops`，工作树 `.worktrees/pipelines/ops`。
- `OPS-0.1.0-01` 治理集成已完成。B01 仍在实施，完整验收与发布条件待满足。

## 当前接收

2026-11-20，ops 通过 [PR #42](https://github.com/nilesthump/UEgame/pull/42) 接收 test 的剑术救援灰盒 Win64 验收记录，合并提交 `b32b88d3310e7dbc6bf57ef771b13ecebf21d5fd`，test 源提交 `dddcd8187f45963014d0dffe9e87faa977b55d33`。test 此前通过 [PR #41](https://github.com/nilesthump/UEgame/pull/41) 接收 tech 候选 `4b31325`。本次受测包来自产品提交 `c7872c3`，配置为 Win64 Development。软件注入的五次剑击全部完成后，玩家和小孩存活，游戏进入 TimeSkip 等待环境结算。

- 独立审查者 `/root/receive_review` 核对本批 18 个文件的变更范围、灰盒代码与用例、构建身份、UAT 归档日志、test 六份原生报告及对应日志，并目视三张本轮截图。审查批准本批受限接收。记录见原 PR。
- test 从同一包执行六次，每份原生报告各 1/1 Success。六份日志的测试退出码均为 0，均记录 HTML 模板缺失。进程退出码取自 test 执行记录。ops 本轮复核已有产物，本地游戏用例执行数为 0。
- [PR #42 CI](https://github.com/nilesthump/UEgame/actions/runs/44753876138) 四项作业成功，Linux 核心六例全部执行并通过。接收前 ops 仓库基线通过，test 源分支写入范围与候选 diff 检查通过。
- 构建、原生报告、日志与截图保存在本机。复核位置及当前验收结论见 test 维护的 [evidence.md](../../evidence/evidence.md)，关注 `PROD-B01`、`DISPLAY-01`、`PERF-01` 和 `FEEL-01`。跨机复核须取得同一批产物。

## 下一步

tech 从 TimeSkip 接入旧时代环境结算回执、30 年后回访布置与控制恢复，经 test 复测后交付 ops。世界卸载、地图切换、持久恢复、真实战斗、正式字幕与媒体、实物手柄、输入到对应画面实际呈现 <50 ms 和本人体验仍待验证。七条叙事文本均为 draft，当前显示证据仅覆盖 NAR-0004 草稿。B02–B15 尚未启动。

## 发布边界

ops 按[交互协议](../PROTOCOL.md)接收 test 或 maintenance 的候选。完整 B01 与发布条件仍待满足。目标版本、线上版本和标签以[发布状态](../../releases/state.json)为准；正式发布依照[发布流程](../../releases/README.md)办理。
