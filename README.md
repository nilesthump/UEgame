# UEgame — 001 因果体验原型

单玩家救援与三十年后回访的因果体验原型。A01–A13总体及类图已逐项确认；当前建立目录、模块任务和本地Git基线，尚无可运行游戏。

- [当前进度](spec/progress/CURRENT.md)
- [需求](spec/requirements/001-causal-prototype/REQUIREMENTS.md)与[架构汇总](spec/architecture/001-causal-prototype/ARCHITECTURE-SUMMARY.md)
- [模块任务与责任人](spec/tasks/001-causal-prototype/TASKS.md)
- [目录规范](spec/governance/DIRECTORY.md)与[协作流程](spec/governance/WORKFLOW.md)
- [输入至渲染反馈小于50ms](spec/performance/001-input-to-render.md)

技术边界：一个UE5.6.1 Runtime模块，标准C++20核心；同份核心源码在Windows UE/MSVC与Linux GCC13/CMake3.28/CTest分别编译。UE负责实际世界模拟与表现；未来Docker宿主另行设计。

当前可运行的仓库检查（PowerShell 7）：

```powershell
pwsh -NoProfile -File Scripts/Validation/Invoke-RepositoryChecks.ps1
```

该命令检查目录、治理、文档链接和确认来源，**不编译游戏，也不证明50ms达标**。[验证契约](spec/governance/VALIDATION.md)说明后续检查。GitHub Actions使用相同入口，但远端仓库尚未配置。

`Source/`、`Content/`等目前是按职责建立的目录占位；`.uproject`、CMake和UBT实际入口由B00/B01任务交付。所有源码/资产版本都应沿任务范围维护，运行数据及构建产物放入忽略目录。

## 许可

本仓库原创代码与文档采用 [MIT License](LICENSE)。Unreal Engine、Epic 模板及以后引入的第三方素材遵循各自许可；它们不因项目采用 MIT 而被重新许可。第三方素材的来源、使用条件及署名记录在 Assets/Provenance/。
