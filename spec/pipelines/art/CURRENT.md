# art — 当前工作

- 目标版本：`0.1.0`
- 状态：Blocked（等待文本 final/显示验证和技术导入规格）；本轮仅确认交接契约
- 分支/工作区：`pipeline/art` / `.worktrees/pipelines/art`

## 当前工作项

`ART-0.1.0-01` 接收 B13 素材制作要求，确认源素材、声音和演出资产的交付边界。

- 资产入场检查项、当前阻塞事实和写入边界见 [ASSET-GATE-0.1.0](ASSET-GATE-0.1.0.md)。
- 唯一写入：`Assets/**`、`Scripts/Materials/**`、`spec/pipelines/art/**`、`spec/contracts/art/RESPONSE-*.md`。
- 不写 `Content/**` 的 UE 二进制资产；由 tech 导入。
- 不写剧情真源；按 `NAR-xxxx` 和 `status=final` 制作。
- 对白由素材 agent 合成；音效先匹配开放资源，无合适匹配再合成。当前未执行检索、合成、导入或试听。

## 接收条件

- tech 提供接入规格、定稿文本/版本、制作要求和验收方式；
- 少量文本显示/接入已实际验证；
- 资产请求有稳定 CueId/SegmentId、来源/加工记录和导入清单。

## 未满足项

上述条件未满足，故当前不交付资产、不把占位或计划写成完成。`art → tech` 只交付就绪/阻塞结论。

## MR

上游：`tech`；下游：`tech`。
