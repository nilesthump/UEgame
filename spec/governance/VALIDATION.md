# 仓库验证契约

RepositoryBaseline 验证仓库文档、链接和架构确认来源，不证明游戏功能、构建或性能通过。B01 任务分支新增下述最小核心入口，尚未代表整个 B01 完成。

## B01 最小核心入口

在 Ubuntu24.04/x86_64、GCC13、CMake/CTest3.28 环境从仓库根执行：

```sh
cmake -S . -B .build/core-linux-gcc13 -DCMAKE_CXX_COMPILER=/usr/bin/g++-13 -DBUILD_TESTING=ON -DCMAKE_BUILD_TYPE=Release
cmake --build .build/core-linux-gcc13 --verbose
ctest --test-dir .build/core-linux-gcc13 --no-tests=error --verbose --output-junit ctest-junit.xml
```

当前预期 CaseId 为 `B01.Session.WorldReady`、`B01.Session.IndependentStarts`、`B01.Session.FactIdentityAndDelivery`、`B01.Session.StableBatchDeathPriority`、`B01.Session.AftermathAndMissingMethod`，必须有实际执行的非空报告、全部通过且无跳过。UE Editor 构建后以 `Automation RunTests UEgame.B01.Session`、`-TestExit="Automation Test Queue Empty"` 和独立 `-ReportExportPath` 执行同一检查。NullRHI 宿主仅验证核心，不证明真实渲染/世界交互。机器清单描述 Linux 配置/构建/执行顺序；CI 新增 Linux job，远端运行结果以实际 PR 为准。Windows UE 安装未在托管 runner 配置，UE 验证仅本机执行。

实际证据和限制见[B01实施记录](../progress/B01-BOOTSTRAP.md)。

从仓库根执行：

```powershell
pwsh -NoProfile -File Scripts/Validation/Invoke-RepositoryChecks.ps1
```

入口使用PowerShell 7及.NET，无第三方包、不联网、不写项目文件；可用`-Root`检查隔离副本。[机器配置](validation.json)与[GitHub工作流](../../.github/workflows/repository-checks.yml)调用同一命令。工作流配置已落地；没有远端运行或分支保护的证据时，不记CI已通过。

## 当前检查内容与失败条件

- 必需的根规则、流程、写范围、当前进度、任务和验证文件存在。
- 根说明及spec Markdown本地链接可解析到实际文件；跳过代码示例、网络链接和纯页内锚点。外部机器路径单列EnvironmentReference/NotValidated；本入口不验证锚点标题或外部网页。
- WORKING中每条确认来源都在仓库内，原始字节SHA-256和精确引文同时匹配；来源不能指向WORKING或本机工具目录。零来源也失败。
- 缺文件、坏链接、非法JSON、来源对象/字段缺失、来源越界、摘要/引文不匹配均返回非零。成功只输出RepositoryBaseline=Passed。

历史spec文件在Git中禁用换行转换，确保Windows工作区与CI原始字节一致。不要通过改写原始用户来源“修复”证据；正文合法变更由协调者同步证据和讨论版本。

## 后续能力与证据

| 检查 | 当前状态 | 负责任务及通过依据 |
| --- | --- | --- |
| RepositoryBaseline | AVAILABLE | 当前脚本真实运行，结果见[进度](../progress/CURRENT.md) |
| Linux GCC13同源核心 | B01分支最小WorldReady场景1/1通过，完整核心尚未实现 | B14后续扩充；实际编译、非零用例和逐项报告 |
| UE5.6.1真实世界 | Game/Editor编译及UE宿主核心1/1通过；真实世界场景NotRun | B01最小启动，各领域及B14实际场景、清理和原生证据 |
| Win64 Development独立包 | NotRun | B01最小启动，B15完整范围、包身份和人工检查 |
| 输入→核心→渲染 <50ms | NotRun | B02关联、B05/B06响应、B14测量、B15完整链；遵守[性能契约](../performance/001-input-to-render.md) |
| 本人因果体验 | NotRun | B15取得用户四类结果和两项评价原话 |

后续新增命令必须先有真实工程、实现及所需工具，并同时更新本契约、机器配置和CI可执行范围。没有能力就记录NotRun/Blocked及原因；未发现或执行零用例、Skipped、未知、缺报告、超时、仅进程退出0都不能代替验收。

首轮基线应验证隔离副本中的断链与错误摘要能返回非零，且修复后回到成功。原始输出存入本机忽略的Validation/Runs/，摘要、限制及独立审查记录进入spec/progress。检查入口不得碰普通游戏存档或终止非本任务进程。

B00依赖安装后已取得[Windows安装smoke](../../Validation/Toolchain/B00-WINDOWS-INSTALL-2026-11-06.md)与[Linux安装smoke](../../Validation/Toolchain/B00-LINUX-INSTALL-2026-11-06.md)证据。它们只验证隔离的小程序和工具安装，不计入上表游戏验收。B01新增的真实核心构建及一项共享用例见上文；端到端性能仍NotRun。
