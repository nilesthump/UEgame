# UEgame — 001 因果体验原型

单玩家救援与三十年后回访的因果体验原型。A01–A13 总体与类图已确认；B01 仍在实施，尚无完整可运行游戏。仓库自 2026-11-09 起使用七条固定管线协作。

- **协作入口**：[七管线总表](spec/pipelines/README.md)、[交互协议](spec/pipelines/PROTOCOL.md)、[写入范围](spec/governance/WRITE_SCOPE.md)
- **当前状态**：[进度](spec/progress/CURRENT.md)、[验收证据](spec/evidence/evidence.md)、[目标/线上版本](spec/releases/state.json)
- **产品依据**：[需求](spec/requirements/001-causal-prototype/REQUIREMENTS.md)、[架构汇总](spec/architecture/001-causal-prototype/ARCHITECTURE-SUMMARY.md)、[模块任务](spec/tasks/001-causal-prototype/TASKS.md)
- **文本真源**：[TEXT.csv](spec/contracts/narrative/TEXT.csv)
- **性能约束**：[输入至渲染反馈小于50ms](spec/performance/001-input-to-render.md)

目标版本 `0.1.0`，实际线上版本未发布（无 tag）。本轮治理合并不创建游戏发行包，也不把 `main` 写成已上线 `0.1.0`。

技术边界：一个 UE5.6.1 Runtime 模块，标准 C++20 核心；同份核心源码在 Windows UE/MSVC 与 Linux GCC13/CMake3.28/CTest 分别编译。UE 负责实际世界模拟与表现。

当前可运行的仓库检查（PowerShell 7 为 CI 默认；脚本兼容 Windows PowerShell 5.1）：

```powershell
powershell -NoProfile -File Scripts/Validation/Invoke-RepositoryChecks.ps1
powershell -NoProfile -File Scripts/Validation/Test-NarrativeText.ps1
```

该检查验证治理/文档/版本/文本结构，**不编译游戏，也不证明 50ms 或本人体验**。CI 按改动执行 `repository-baseline`、`narrative-text` 和受影响 `core-linux`；完整状态见[验证契约](spec/governance/VALIDATION.md)。

## 许可

本仓库原创代码与文档采用 [MIT License](LICENSE)。Unreal Engine、Epic 模板及以后引入的第三方素材遵循各自许可；它们不因项目采用 MIT 而被重新许可。第三方素材的来源、使用条件及署名记录在 `Assets/Provenance/`。
