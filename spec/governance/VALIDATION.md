# 仓库验证契约

版本：`0.1.0`。验证分三层：仓库结构/文档、受影响行为、发布候选主路径。未执行、零用例、跳过、缺报告或只发现测试不记通过。验证不证明游戏、性能或本人体验通过。

## 1. 入口

从仓库根执行：

```powershell
powershell -NoProfile -File Scripts/Validation/Invoke-RepositoryChecks.ps1
powershell -NoProfile -File Scripts/Validation/Test-NarrativeText.ps1
```

PowerShell 7 为 CI 默认；脚本同时兼容 Windows PowerShell 5.1，便于本机直接执行。入口不安装工具、不联网、不写产品文件。

Linux 核心检查（Ubuntu 24.04/x86_64、GCC 13、CMake/CTest 3.28）：

```sh
cmake -S . -B .build/core-linux-gcc13 -DCMAKE_CXX_COMPILER=/usr/bin/g++-13 -DBUILD_TESTING=ON -DCMAKE_BUILD_TYPE=Release
cmake --build .build/core-linux-gcc13 --verbose
ctest --test-dir .build/core-linux-gcc13 --no-tests=error --verbose --output-junit ctest-junit.xml
```

当前显式用例为 `B01.Session.WorldReady`、`B01.Session.IndependentStarts`、`B01.Session.FactIdentityAndDelivery`。期待非空报告、全部通过且无跳过；NullRHI/核心宿主只证明核心，不证明真实渲染或世界交互。

## 2. 仓库基线检查内容

- 必需文件存在且非空：根规则、七管线文档、协议、写入范围、验证、任务、进度、证据、发布状态和文本真源。
- `spec/**`、根 Markdown 的本地相对链接可解析；跳过代码块、网络链接和纯页内锚点。外部绝对路径只记为 EnvironmentReference/NotValidated。
- `spec/architecture/WORKING.json` 可解析为 JSON；不再校验来源 SHA-256 或精确引文。
- `spec/releases/state.json` 的目标版本符合 `x.y.z`；`onlineVersion` 为 `null` 或 `x.y.z`；未发布时不得伪造标签。
- 七个管线目录存在；`spec/pipelines/pipelines.json` 的管线名与目录一致。
- 不存在 `evidence-latest.md`、`evidence-final.md` 等平行结论文件。
- 缺文件、坏链接、非法 JSON、版本非法、平行证据文件均返回非零。成功只输出 `RepositoryBaseline=Passed`。

## 3. 文本契约检查

`Test-NarrativeText.ps1` 读取 `spec/contracts/narrative/TEXT.csv`，检查：

- 表头为 `id,speaker,scene_or_result,text,production_notes,status`；
- 至少一条；ID 唯一且匹配 `NAR-[0-9]{4}`；
- 说话者、场景/结果类型、正文、制作说明非空；
- `status` 只能是 `draft`、`final`、`retired`。

文本检查只证明条目结构可导入，不等于 UE 内显示/音频接入通过。少量文本的显示与接入验证在 0.1.0 中仍为 NotRun，详见 [evidence](../evidence/evidence.md)。

## 4. CI 与 required checks

`.github/workflows/repository-checks.yml`：

- `repository-baseline`：始终运行，是 `main` 唯一 required check；
- `core-linux`：只有在 `Source/**`、`Tests/**`、构建入口、验证脚本或相关 CI 变更时运行；
- `narrative-text`：只有在 `spec/contracts/narrative/**` 或 `Scripts/Narrative/**` 变更时运行。

`main` 还要求 PR、1 次 GitHub 账号批准、讨论解决、仅 merge commit。`core-linux`/`narrative-text` 的跳过不等于通过；涉及对应改动时必须取得实际成功。候选内容或构建身份变化后重新执行受影响检查。

## 5. 状态

| 检查 | 当前状态 | 说明 |
| --- | --- | --- |
| RepositoryBaseline | 本治理分支实际执行后记录 | 见 evidence |
| NarrativeText | 待执行 | 文本结构，不等于运行时显示 |
| Linux GCC13 shared core | GitHub Actions 实际运行 | 3 个 session 用例；完整 B01 未完成 |
| UE 真实世界场景 | NotRun | 现有 NullRHI 核心宿主不等于真实世界 |
| Win64 独立包 | NotRun | 未 Cook/Stage/启动 |
| 输入→渲染 <50ms | NotRun | 测量链和场景未建立 |
| 本人因果体验 | NotRun | 未试玩 |

## 6. 证据维护

- 当前结论只在 `spec/evidence/evidence.md` 按版本和验收项维护；修复通过后原地更新失败条目，结果失效时原地改为待复测。
- 当前文件只保留最新结论和执行链接；历史由 Git 保存，不再新增 `evidence-latest.md`、`evidence-final.md` 等文件。
- 测试不得用未集成分支、零用例、模拟手柄或字幕降级路径替代真实验收；UE 二进制资产和共享入口的写入权仍按 [WRITE_SCOPE](WRITE_SCOPE.md)。
