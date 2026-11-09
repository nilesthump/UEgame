# tech — 当前工作

- 目标版本：`0.1.0`
- 状态：B01 InProgress；B02/B10/B11 未启动；本轮治理集成
- 分支/工作区：`pipeline/tech` / `.worktrees/pipelines/tech`

## 当前工作项

`TECH-0.1.0-01` 接管共享入口和 UE 二进制资产，建立技术导入/集成规则；保持 B01 现有核心和 CI 可用。

- 唯一写入：`Source/**`、`Tests/**`、`CMakeLists.txt`、`UEgame.uproject`、`Config/**`、`Content/**`、`.github/**`、`Scripts/Collaboration/**`。
- 只读：剧情文本真源、`Assets/**` 源素材、`spec/evidence/**`、`spec/releases/**`。
- B01 远程已集成 WorldReady、独立起点/统计；本地 `task/B01-completion` 仍有未推送提交，未合并前不得写成已集成。
- B10/B11 运行逻辑和真实世界回执尚未实现；不得用 NullRHI 核心测试替代。

## 本轮交付

- 读取 growth 的范围、narrative 的文本契约；
- 向 art 提供接入规格和稳定文本 ID；
- 向 test 提供可运行成果和构建身份；
- 将文本显示/接入验证列为 NotRun，直到真实 UE 场景执行。

## MR

上游：`growth`、`narrative`；下游：`art`、`test`。
