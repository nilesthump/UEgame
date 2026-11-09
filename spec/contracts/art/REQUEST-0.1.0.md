# tech → art：0.1.0 接入规格与制作请求

- 目标版本：`0.1.0`
- 请求方：tech
- 接收方：art
- 状态：Blocked；不是可制作资产请求，当前不要求 art 产出声音/演出资产

## 1. 已接受的文本

真源：[TEXT.csv](../narrative/TEXT.csv)；说明：[TEXT-SOURCE.md](../narrative/TEXT-SOURCE.md)；修订：[REVISION-0.1.0.md](../narrative/REVISION-0.1.0.md)。

- 稳定 ID：`NAR-0001`–`NAR-0007`，全部 `draft`。
- 每条含说话者、场景/结果类型、正文、制作说明和 CueId。
- 未定稿；不得据 `draft` 条目批量制作声音或演出。

## 2. 接入规格（未来 final 条目适用）

| 项目 | 要求 |
| --- | --- |
| 文本身份 | 只使用 `NAR-xxxx`，不复制正文另存 |
| 声音 Cue | 以 `CUE-NAR-xxxx` 为稳定请求；绑定版本随文本 revision |
| 源素材路径 | `Assets/Audio/Voices/`、`Assets/Audio/SFX/`；art 独占写入 |
| 导入资产路径 | `Content/UEgame/Audio/Voices/`、`Content/UEgame/Audio/SFX/`；tech 独占写入 |
| 来源/加工记录 | `Assets/Provenance/Audio/`；art 写入 |
| 导入清单 | 由 art 交付；tech 按清单导入，不自行改文本 |
| 声音格式 | 本地声音资产，运行时不得联网补素材 |
| 字幕 | 与文本同一 `id`，不另写一套文案 |

## 3. 制作要求

- 对白由素材 agent 合成；音效先匹配开放资源，无匹配再合成。当前未检索、未合成、未试听。
- 准确文本、发音、说话者、噪声/削波/循环实际听辨；未听辨不得记完成。
- 只有 `final` 条目可批量制作；修订时列出受影响 ID，并同步声音、绑定、字幕和验收状态。
- 不使用 SHA-256/精确引文门禁；使用 `id`、Git 提交和实际执行链接。

## 4. 接收/验收条件

1. 少量文本已在目标显示链完成 UE 显示/字幕接入验证（当前 NotRun）；
2. 文本条目为 `final`，且 art 收到稳定 CueId；
3. 来源、加工/合成、使用条件和署名记录完整；
4. 导入清单与实际资产一致，tech 可复现导入；
5. 实际听辨和独立包试听由 test/B15 取证，不用占位或 TextFallback 替代。

## 5. 当前结论

以上条件均未满足，因此本请求只建立交接规格，不要求 art 制作，也不把计划写成已交付资产。
