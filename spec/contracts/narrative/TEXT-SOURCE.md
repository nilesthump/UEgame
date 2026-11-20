# 剧情文本唯一真源

状态：`0.1.0` 文本契约。唯一数据文件：[TEXT.csv](TEXT.csv)。不另建终稿、导出稿或“最新文本”副本。

## 用途

`narrative` 维护本文件；`tech` 只读导入；`art` 按已定稿条目制作声音和演出资产。技术导入、字幕显示和声音绑定都引用同一 `id`，不得复制正文另存。

## 字段

| 字段 | 含义 |
| --- | --- |
| `id` | 稳定条目 ID，格式 `NAR-xxxx`；发布后不复用、不重编号 |
| `speaker` | 说话者或 `旁白` |
| `scene_or_result` | 场景/结果类型，使用稳定的点分标识 |
| `text` | 正文；UTF-8，中文标点 |
| `production_notes` | 演出、触发、互斥、CueId 等制作说明 |
| `status` | `draft`、`final`、`retired` |

稳定 ID 是业务身份；正文修订不改变 ID。`retired` 只表示停止使用，不删除历史行；需要替换时新增条目并在制作说明引用旧 ID。

## 修订与交付

1. 少量文本先进入 `draft`，通过结构检查后由技术做显示/字幕接入验证。
2. 显示/接入实际通过且文本定稿后，才把条目改为 `final`；批量声音和演出资产只依据 `final` 条目制作。
3. 修订时在 `narrative` 的 MR 列出受影响 `id`，同步声音、绑定和验收状态；不得静默改写已交付条目。
4. `tech` 导入时保留 `id`、`speaker`、`scene_or_result` 和 `status`；运行时只消费已接纳版本。
5. 不使用 SHA-256、精确引文或隐藏特征串作为交接门禁；需要追溯时使用 `id`、Git 提交和实际执行链接。

## 验收限制

当前 7 条均为 `draft`。test 唯一 [evidence / DISPLAY-01](https://github.com/nilesthump/UEgame/blob/pipeline/test/spec/evidence/evidence.md) 为 Partial：仅 NAR-0004 草稿探针显示通过，执行与接收入口见原 [PR #35](https://github.com/nilesthump/UEgame/pull/35)。该探针不提交媒体送达回执；其余条目、正式字幕及声音/媒体仍未验收。

本批只收窄显示说明，不改 TEXT.csv 正文、制作说明或状态。正式首批样本仍为 NAR-0001–NAR-0003；单条草稿显示不代表正式首批门槛通过，不将条目改为 `final`，不解锁 B13，也不宣称整体运行时验收完成。
