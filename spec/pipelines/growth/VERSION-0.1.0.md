# growth — 0.1.0 范围与验收条件

- 目标版本：`0.1.0`
- 线上版本：未发布（`null`，无 tag）
- 状态：治理基线交付；产品 B01 未完成
- 依据：[当前工作](CURRENT.md)、[七管线总表](../../pipelines/README.md)、[协议](../../pipelines/PROTOCOL.md)

## 工作项

| ID | 工作项 | 验收条件 | 交付对象 |
| --- | --- | --- | --- |
| GOV-0.1.0-01 | 七管线协作与发布治理 | 固定分支/工作区、规则集、协议、版本、证据、CI 可核验 | tech, narrative, test, ops |
| GOV-0.1.0-02 | 剧情文本单一真源 | 稳定 ID、说话者、场景/结果、正文、制作说明；结构检查通过 | narrative, tech, art |
| GOV-0.1.0-03 | 发布边界与未发布记录 | `targetVersion=0.1.0`、`onlineVersion=null`，无正式 tag | ops, test |

## 0.1.0 接收条件

1. 七条固定分支与独立 sparse 工作区实际存在，旧 `task/*` 保留只读。
2. GitHub Rulesets 禁止新增/删除/强推，关闭自动删支，`main` owner bypass 与生命周期保护分开。
3. 规定 MR 流向、五项 PR 正文、独立审查和 merge commit 实际走通。
4. 文本真源可被技术/美术引用；文本结构检查通过；UE 显示/字幕接入单独标记 NotRun。
5. 当前证据只在 `spec/evidence/evidence.md` 维护。
6. 发布状态明确为未发布，治理合并不得写成产品上线。

## 不在 0.1.0 接收条件

- B01 剩余功能、B02–B15 产品实现、素材制作、Win64 独立包、50ms、本人体验；
- 少量文本的 UE 显示/字幕接入（当前 NotRun）；
- 正式产品发布、tag 或对外发行包。

## 交付记录

本轮由 growth 经 PR 向 tech/narrative 交付范围；实际 PR、CI 和合并提交见 [evidence](../../evidence/evidence.md)。未满足项按 [协议](../../pipelines/PROTOCOL.md) 在原 PR 反馈。
