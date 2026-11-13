# narrative — 当前工作

- 目标版本：`0.1.0`
- 状态：InProgress（文本契约；仅 NAR-0004 draft 显示通过，正式字幕与其余条目未验收）
- 分支/工作区：`pipeline/narrative` / `.worktrees/pipelines/narrative`

## 当前工作项

`NAR-0.1.0-01` 建立唯一剧情文本真源，供技术导入与美术制作共用。范围：对白、旁白、字幕和回访叙事文案；稳定 ID；说话者、场景/结果类型、正文、制作说明。当前仅少量条目，不批量制作。

## 文本真源

唯一真源：[TEXT.csv](../../contracts/narrative/TEXT.csv)。说明见 [TEXT-SOURCE.md](../../contracts/narrative/TEXT-SOURCE.md)。不使用哈希或精确特征串门禁；条目身份由 `NAR-xxxx` 和 Git 提交保证。

## 接收/交付条件

- 技术导入前：文本结构检查通过，说话者/场景/正文/制作说明完整；
- 美术制作前：对应条目 `status=final`，且少量文本已在目标显示链完成目视/实际接入验证；
- 修订时列出受影响条目，并同步声音、绑定和验收状态。

## 未满足项

- 当前 7 条仍为 `draft`；仅 NAR-0004 草稿探针显示通过，不等于正式字幕或其余条目通过；
- 未批量制作声音资产，未生成绑定；
- 本批 `narrative → tech` 仅修订显示范围说明，不改变正文或条目状态，不宣称整体运行时接入通过。验收依据为 test 唯一 [evidence / DISPLAY-01](https://github.com/nilesthump/UEgame/blob/pipeline/test/spec/evidence/evidence.md)（Partial）及原 [PR #35](https://github.com/nilesthump/UEgame/pull/35)。
- 正式首批样本仍为 NAR-0001–NAR-0003；NAR-0004 草稿探针不替代该批门槛，不解锁 B13。DISPLAY-GATE 文档不在本批修改范围。

## MR

上游：`growth`；下游：`tech`。
