# 目录规范

沿已确认 A03/A12 保留现有 `spec/requirements/001-causal-prototype/`、`spec/architecture/001-causal-prototype/`、`Source/`、`Content/` 路径，避免破坏历史来源与图文链接。业务目录仍属于同一个 UEgame Runtime 模块，不新增 Runtime 模块。

```text
AGENTS.md / CLAUDE.md / README.md
spec/
  pipelines/                       # 七管线总表、协议、各管线 CURRENT.md
    growth|narrative|tech|art|test|ops|maintenance/
  contracts/
    narrative/                     # 唯一剧情文本真源和导入说明
    art/                           # tech 请求 / art 响应
  evidence/evidence.md             # 当前验收结论（唯一）
  releases/
    state.json                     # 目标版本与实际线上版本
    0.1.0/                         # 当前版本计划
  requirements/001-causal-prototype/  # 需求正文和只读原始来源
  architecture/                    # WORKING 及已确认逐项架构
  governance/                      # 流程、范围、验证、目录、分支规则
  tasks/001-causal-prototype/      # B00–B15 任务分配（映射七管线）
  progress/                        # 当前进度与历史实施记录
  performance/                     # 端到端 50ms 目标与测量口径
Source/UEgame/                     # 代码与共享构建入口；tech 唯一写
Tests/                             # 共享/UE 测试；tech 写，test 管验收用例
Content/UEgame/                    # UE 二进制资产；tech 唯一写
Config/                            # 工程配置；tech 唯一写
Assets/                            # 美术/声音源素材与制作记录；art 写
Scripts/
  Collaboration/                   # 固定管线/工作区/MR 工具；tech 写
  Narrative/                       # 文本导入辅助；narrative 写
  Materials/                       # 素材处理；art 写
  Release/                         # 发布状态查询；ops 写
  Validation/                      # 本地/CI 检查；test 写
Validation/                        # 执行记录、验收用例；test 写
.github/workflows/                 # CI；tech 写
.build/ Saved/ Intermediate/ Binaries/ DerivedDataCache/
Deliverables/ .worktrees/          # 输出/工作树，不纳入源码 Git
```

## 工作区 sparse-checkout

每个固定分支对应 `.worktrees/pipelines/<name>`，由 `Scripts/Collaboration/Initialize-PipelineWorkspaces.ps1` 建立并配置 sparse-checkout。工作区只显示：

- 根 `AGENTS.md`、`CLAUDE.md`、`README.md`、`.gitignore`；
- `spec/pipelines/README.md`、`PROTOCOL.md` 和本管线 `CURRENT.md`；
- `spec/governance/WRITE_SCOPE.md`、`VALIDATION.md`；
- `spec/releases/`、`spec/contracts/` 中与本管线任务相关的契约；
- 本管线执行所需的代码/脚本/资产子目录。

不要求每个工作区看到全部 `spec/requirements/`、`spec/architecture/` 或历史进度；当前任务需要时按 `CURRENT.md` 链接读取。sparse 配置是工作区本地状态，不提交为第二套目录规范。

## 文件约定

- 文本 UTF-8/LF；`.gitattributes` 保持 `spec/**` 的稳定字节，但验证不再依赖 SHA-256 或精确引文。
- `Content/` 的 `.uasset`、`.umap` 等二进制资产只由 tech 写入；art 在 `Assets/` 交付源素材和导入清单。
- 原始素材临时文件放忽略的 `Assets/Incoming/`、`Assets/Generated/`；来源和加工记录进 `Assets/Provenance/`。
- `spec/requirements/**/sources/` 既有原始来源不改写；新用户指示作为新来源文件登记。
- 不提交引擎安装、SDK、凭据、运行快照或报告缓存。大型资产版本策略在首次实际导入时核对；本轮未启用 LFS。
