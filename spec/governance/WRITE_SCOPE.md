# 写入范围与唯一所有者

权威目录：`G:/UEgame`。协调角色：`integration-owner`（当前初始化由本聊天协调agent执行）。任务角色和B00–B15精确范围见[任务表](../tasks/001-causal-prototype/TASKS.md)。角色名是责任分工，不是已雇用人数或已启动agent。

| 范围 | 所有者与写入规则 |
| --- | --- |
| 根AGENTS/CLAUDE/README、治理/进度/任务、`.github/`、Git配置 | integration-owner；本次bootstrap授权内维护 |
| `spec/architecture/WORKING.json` | canonical协调者经本机版本绑定运行器/expectedRevision唯一更新；其他agent及worktree副本只读 |
| `spec/requirements/**/sources/`既有文件 | 不改写；新用户指示新增来源文件，由协调者登记引用 |
| 需求正文、架构总体与类图 | 协调者基于明确用户变更维护；模块agent提出补丁建议，不能自行改变已确认玩法 |
| `.uproject`、根CMake、`Source/*.Target.cs`、`UEgame.Build.cs`、模块入口、总Config、共享注册 | B00/B01/B02与integration-owner串行交付，不接受多个领域并行改动 |
| Session/RunState/共享事实、Director/Subsystem及共同生命周期入口 | B01/B02/B15集成责任；领域规则与适配按任务专属子目录维护 |
| 领域代码/测试/资产 | 对应任务owner唯一写入，具体子目录见任务表；跨域修改先协调依赖 |
| 主地图、联合场景、总输入映射/总配置 | integration-owner；领域owner交付独立资产/场景及导入清单后集成 |
| 共享测试入口、框架和注册清单 | B01先建最小可执行检查，集成后在进度记录移交B14；同一时间唯一writer |
| `Assets/Provenance/`与音频准备 | B13素材owner；素材来源/加工/合成记录必须与实际执行一致 |
| `Scripts/Validation/`与公共检查模板 | B14 validation-owner；bootstrap仅交付RepositoryBaseline入口，后续游戏验证由B14实现 |
| `Validation/Toolchain/`、`Validation/Materials/`、`Validation/Performance/` | 分别B00、B13、B15；B14不拥有这些子目录，具体范围见任务表 |
| `.build/`、`Saved/`等输出 | 仅操作自己任务创建并验证归属的子目录，保留故障证据；不得清理用户试玩数据 |
| `.agents/`、`.codex/`、`.aws/`、本机UE/工具安装 | 当前不修改、不纳入Git；需要环境准备时由B00另按实际授权执行 |

根目录已经授权的初始化文件不再逐项索要确认。超出任务范围的功能或跨任务修改，不以方便为由直接写入；由协调者调整范围并记录依据。工作树、分支或文件已被他人改动时保留内容并核对，不强制重置/清理。
