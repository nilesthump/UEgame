# 美术/声音交接契约

本目录只放跨管线契约，不放最终资产。

- `REQUEST-<version>.md`：`tech` 编写，说明接入规格、定稿文本 `NAR-xxxx`/`final` 要求、制作要求和验收方式。
- `RESPONSE-<version>.md`：`art` 编写，说明源素材、来源/加工记录、导入清单、未满足项。
- `Content/**` 的 `.uasset`/`.umap` 等 UE 二进制资产由 `tech` 唯一导入和写入；`art` 在 `Assets/**` 交付源素材。
- 文本以 [narrative/TEXT.csv](../narrative/TEXT.csv) 为唯一真源；不得复制正文。
- 当前 `0.1.0` 只有交接边界，没有已制作资产。少量文本的 UE 显示/接入仍为 NotRun，不能据此批量制作。
