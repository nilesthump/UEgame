# narrative — 0.1.0 文本修订记录

- 目标版本：`0.1.0`
- 真源：[TEXT.csv](TEXT.csv)
- 状态：全部条目仍为 `draft`；仅 NAR-0004 草稿探针显示通过，正式字幕与其余条目未验收
- 修订：2026-11-13

## 本次说明修订

本批仅收窄 NAR-0004 的显示范围说明；TEXT.csv 七条正文、制作说明及全部 `draft` 状态不变。验收依据为 test 唯一 [evidence / DISPLAY-01](https://github.com/nilesthump/UEgame/blob/pipeline/test/spec/evidence/evidence.md)（Partial）及原 [PR #35](https://github.com/nilesthump/UEgame/pull/35)，不另维护验收表。正式首批样本仍为 NAR-0001–NAR-0003；本次不修改 DISPLAY-GATE，不解锁 B13。

## 既有文本条目修订

| ID | 类型 | 本次变化 | 后续 |
| --- | --- | --- | --- |
| NAR-0001–NAR-0006 | 对白/旁白 | 纳入 0.1.0 文本真源，保持 `draft` | 等待少量文本显示验证 |
| NAR-0007 | 混用方向群众闲聊 | 新增 `draft` 条目 | 与 NAR-0001/0002 互斥使用 |

## 修订规则

- 稳定 ID 不因正文修订改变；新增/停用写清受影响 ID。
- `final` 只在显示/字幕接入实际通过且文本定稿后设置；批量声音/演出资产只依据 `final`。
- 本次不生成声音、绑定、字幕导入或 UE 资产；正式字幕与声音/媒体仍未验收，当前结论以 test 唯一 evidence 为准。
- 不使用 SHA-256/精确引文门禁；交接使用 `id`、Git 提交和实际执行链接。

## 交付给技术/美术

- 技术导入必须先读 [TEXT-SOURCE.md](TEXT-SOURCE.md)，保留 `id`/`speaker`/`scene_or_result`/`status`。
- 美术只能按 `final` 条目制作；当前没有可批量制作的条目。
- 技术导入结构检查不等于运行时显示；NAR-0004 的单条草稿显示结果不提交媒体送达回执，也不替代其余条目、正式字幕或声音验收。后续由 test 在唯一 evidence 原位维护结果，narrative 据此更新对应说明。
