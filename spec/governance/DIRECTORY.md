# 目录规范

沿已确认A03/A12建立下列布局。保留现有`spec/requirements/001-causal-prototype/`及`spec/architecture/001-causal-prototype/`路径，避免破坏历史来源与图文链接。业务目录用于分工，运行时仍只有一个UEgame模块。

```text
AGENTS.md / CLAUDE.md / README.md
spec/
  requirements/001-causal-prototype/  # 唯一需求正文和不可改写的原始来源
  architecture/                      # WORKING及已确认逐项架构
  governance/                        # 流程、范围、验证与目录规范
  tasks/001-causal-prototype/         # B00–B15任务分配
  progress/                          # 当前执行、审查及集成记录
  performance/                       # 端到端50ms目标与测量口径
Source/UEgame/Private/
  ExperimentCore/include/experiment/ # 标准C++头，按功能分区
  ExperimentCore/src/                # 同份核心实现，UBT/CMake分别编译
  ExperimentAdapter/                # 共享Session/时钟/效果适配
  Contracts/                        # 跨域UE值契约，集成角色维护
  World/{Spherical,Navigation,Movement,Player,Goblin,Trees}/
  Combat/ Environment/ Revisit/ Audio/ Persistence/
  UI/{Spherical,Player,Revisit,Audio,Recovery}/
  Tests/                             # 仅开发条件编入的UE测试
Tests/{Shared,ExperimentCore}/        # 模块外纯C++检查
Content/UEgame/                       # 按领域导入的UE资产；主Maps由集成管理
Config/                              # 工程配置，共享入口集成管理
Assets/Provenance/                    # 素材请求、来源/合成/加工、导入记录
Scripts/Validation/                  # 本地与CI共用验证入口
Validation/                         # 版本化检查计划与记录格式
.github/workflows/                  # 远端接入后调用相同入口
.build/ Saved/ Intermediate/ Binaries/ DerivedDataCache/
Deliverables/ .worktrees/             # 输出/工作树，不纳入源码Git
```

已创建的占位目录不宣称已有实现。B00/B01负责创建真实`.uproject`、Target/Build规则和CMake入口；本次不生成空测试或假成功的游戏构建脚本。

核心头文件与实现使用相同的领域子目录：`Session/ Spherical/ Navigation/ Player/ Combat/ Goblin/ Trees/ Environment/ Revisit/ Checkpoint/`。这些名称与[任务范围](../tasks/001-causal-prototype/TASKS.md)保持一致；共享类型和注册由B02统一。UE类文件按现有类图命名，不能为目录方便复制状态或新增Runtime模块。

`Content/UEgame/<Domain>/`保存导入后的项目资产；原始检索/生成临时文件留忽略的`Assets/Incoming/`、`Assets/Generated/`，可复用来源与处理记录进入`Assets/Provenance/`。不将引擎安装、SDK、个人凭据、运行快照和报告缓存提交为项目代码。大型资产的版本策略在首次实际导入时核对，本次未启用LFS或远端存储。

文本默认UTF-8/LF。历史`spec/**`在Git中禁用换行自动转换，保留来源SHA-256依据；改正文时应由协调者同步当前证据引用，已有原始来源不得为排版改字节。二进制图/UE资产保持二进制处理。
