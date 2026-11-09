# tech — 0.1.0 集成状态

- 目标版本：`0.1.0`
- 分支：`pipeline/tech`；本记录基线提交：`593bb54`
- 状态：治理集成完成；产品 B01 仍 InProgress

## 已接收

- growth `0.1.0` 范围与 0.2.0 分离（PR #9）；
- narrative 文本真源 `NAR-0001`–`NAR-0007` 与修订记录（PR #11）。
- 上游独立审查结果已记录在原 PR：growth 无 P1/P2；narrative P2 已由 narrative 修复并复审。

## 本轮技术交付

- 共享入口与 UE 二进制资产唯一写入边界见 [WRITE_SCOPE](../../governance/WRITE_SCOPE.md)。
- 向 art 发出 [REQUEST-0.1.0](../../contracts/art/REQUEST-0.1.0.md)，明确文本/声音接入和验收条件。
- 现有 Linux 核心检查仍为 3 个 session 用例；完整 B01 未完成。
- 文本导入、UE 显示、字幕、声音绑定均 **NotRun**；本轮没有新增运行时实现。

## 已知限制

- 本地 `task/B01-completion` 的未推送提交不属于已集成结果，未纳入本分支。
- `Content/**` 没有新增 `.uasset`/`.umap`；无资产可交付 test。
- 无 Win64 独立包、50ms 测量或本人体验证据。

## 交付给 test

- 可运行成果仍限于现有核心/CI 用例；不把治理文档检查当作产品验收。
- 构建/候选身份后续由 ops 发布记录引用；当前无产品发布候选。
