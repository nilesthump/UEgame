# tech — B01 功能并入记录（0.1.0）

- 目标版本：`0.1.0`
- 分支/工作区：`pipeline/tech` / `.worktrees/pipelines/tech`
- 同步基线：`main` = `8badcc0`（`git merge --ff-only main` 后）
- 来源：只读旧分支 `task/B01-completion`（merge base `05c308d`）
- 状态：三个 B01 功能提交已并入本地 `pipeline/tech` 集成 merge commit；**未推送、未进入 `main`、B01 未完成**
- 本文件取代 [INTEGRATION-0.1.0](INTEGRATION-0.1.0.md) 中“本地 `task/B01-completion` 未纳入本分支”的旧说明

## 1. 并入的功能提交

保留原始提交，未 squash、未改写：

| 提交 | 功能 | 主要文件 |
| --- | --- | --- |
| `3755ff4` | 稳定救援批次、死亡优先 | `ExperimentSession.h/.cpp`、`Tests/Shared/Session/SettlementChecks.h` |
| `da1af65` | 按有序一次性请求身份约束效果派发 | `ExperimentSession.h/.cpp`、`Tests/Shared/Session/EffectChecks.h` |
| `55c5bdd` | Session 绑定真实 bootstrap 世界生命周期 | `Source/UEgame/Private/ExperimentAdapter/**`、`Source/UEgame/Private/Tests/Bootstrap/WorldBindingChecks.cpp`、`Tests/Shared/CoreCaseCatalog.h` |

- 旧分支的 `df858bb`（task 级 PR 粒度说明）及其治理文档不随本批进入。
- 集成方式：`git merge --no-commit --no-ff task/B01-completion`，冲突解决后再形成 merge commit。

## 2. 冲突处理

| 文件 | 处理 |
| --- | --- |
| `AGENTS.md` | 保留当前 `pipeline/tech` 版本，不回退旧治理文本 |
| `spec/governance/VALIDATION.md` | 保留当前版本 |
| `spec/governance/WORKFLOW.md` | 保留当前版本 |
| `spec/progress/CURRENT.md` | 保留当前版本 |
| `spec/tasks/001-causal-prototype/TASKS.md` | 保留当前版本 |
| `CMakeLists.txt` | 采用并集：现有 3 个用例 + 新增 3 个测试 |

丢弃的旧治理/进度文件（不进入本分支，也不进入 `main`）：

- `spec/requirements/001-causal-prototype/sources/2026-11-06-task-pr-granularity.md`
- `spec/progress/B01-BINDING.md`
- `spec/progress/B01-EFFECTS.md`
- `spec/progress/B01-SETTLEMENT.md`

代码与测试全部保留，未做语义改写。`CMakeLists.txt` 只做并入整理：一次 merge 自动追加的重复 `add_test` 会触发 CMake “test NAME already exists”，整理为 6 个用例各注册一次、共用 `TIMEOUT 10`。现有 3 个用例与新增 3 个用例均无需修改核心实现即可编译通过。

## 3. 测试清单（实际执行）

Linux 核心检查，Ubuntu 24.04/x86_64（WSL2）、GCC 13.3.0、CMake 3.28.3：

```sh
cmake -S . -B .build/core-linux-gcc13 -DCMAKE_CXX_COMPILER=/usr/bin/g++-13 -DBUILD_TESTING=ON -DCMAKE_BUILD_TYPE=Release
cmake --build .build/core-linux-gcc13 --verbose
ctest --test-dir .build/core-linux-gcc13 --no-tests=error --verbose --output-junit ctest-junit.xml
```

结果：构建成功，`100% tests passed, 0 tests failed out of 6`，无跳过；JUnit 报告 `.build/core-linux-gcc13/ctest-junit.xml`（本地构建产物，不提交）。

| 用例 | 来源 | 结果 |
| --- | --- | --- |
| `B01.Session.WorldReady` | 现有 | Passed |
| `B01.Session.IndependentStarts` | 现有 | Passed |
| `B01.Session.FactIdentityAndDelivery` | 现有 | Passed |
| `B01.Session.StableBatchDeathPriority` | `3755ff4` | Passed |
| `B01.Session.AftermathAndMissingMethod` | `3755ff4` | Passed |
| `B01.Session.EffectEligibility` | `da1af65` | Passed |

仓库检查：`Scripts/Validation/Invoke-RepositoryChecks.ps1` 与 `Scripts/Collaboration/Test-PipelineScope.ps1 -Base refs/heads/pipeline/ops` 另行执行，结果见本次集成交付报告。

## 4. 仍未完成

以下均为 **NotRun**，不得由本次核心用例通过推导：

- **真实 UE 回执**：`ExperimentSubsystem`/`ExperimentWorldDirector` 的 UE 编译与真实 `UWorld` 生命周期回执未执行；Linux 核心宿主不编译 `Source/UEgame/Private/ExperimentAdapter/**`。
- **可见反馈**：Rescue/TimeSkip 效果只证明核心命令与请求身份；真实画面、字幕、声音呈现未验证。
- **Win64 独立包**：未 Cook/Stage/启动。
- **性能**：输入→核心处理→画面呈现 <50 ms 未测量，测量链和场景未建立。
- **任务 PR/CI/canonical 集成**：本地 merge 未推送、未发起 PR、未进入 `main`。
- **B01 其余范围**：B01 仍 InProgress；B02/B10/B11 未启动。

## 5. 交付边界

- 本批只含三个已并入的功能提交、集成 merge commit 和本记录；不含旧治理文档、旧进度快照或发布状态。
- 验收结论仍只在 [evidence.md](../../evidence/evidence.md) 由 test 维护；本文件不写验收通过。
