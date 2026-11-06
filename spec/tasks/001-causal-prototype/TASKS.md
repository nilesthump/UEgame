# UEgame 模块任务与写入范围

日期：2026-11-06。范围：根据已确认的 A01–A13 规范，安排仓库初始化后的实施任务；本文不构成正式架构冻结。用户本轮授权规范目录、分配任务和初始化仓库，没有据此启动游戏实现、安装工具或制作素材。

**分配的是职责角色，不是承诺真实人数、工期或预算。下列 B00–B15 的实施初始状态均为“已分配角色；未启动”。** 当前筹备子代理只整理规范与任务，不等于这些开发任务已经运行。后续由主任务在已有授权范围内调度，超出本轮范围的实际实施不由本任务表自动启动。

依据：[架构汇总及详细类图入口](../../architecture/001-causal-prototype/ARCHITECTURE-SUMMARY.md)、[目录规范](../../governance/DIRECTORY.md)、[工作流程](../../governance/WORKFLOW.md)及[性能契约](../../performance/001-input-to-render.md)。A01/A02 的状态归属以后续 A03 为准；A08 事务以后续 A09 为准；A10 媒体以后续 A13 为准；A11 现行完整恢复替代历史“仅终态保存”。

## 1. 路径和并行规则

下面所有路径均相对仓库根。业务目录只是同一个 UEgame Runtime 模块内部的文件分组，不增加 Editor、Server 或第二个 Runtime 模块。

| 简写 | 根路径及用途 |
| --- | --- |
| UE | Source/UEgame/Private/ |
| CoreH | Source/UEgame/Private/ExperimentCore/include/experiment/ |
| CoreS | Source/UEgame/Private/ExperimentCore/src/ |
| SharedTests | Tests/Shared/；标准 C++ header-only 检查支持 |
| WorldTests | Source/UEgame/Private/Tests/；仅开发测试条件编译 |
| Content | Content/UEgame/；最终 UE 资源 |
| Config | Config/；引擎和工程配置，由集成角色统一修改 |

1. 领域纯规则同时拥有对应 CoreH/<功能>/ 与 CoreS/<功能>/，不复制核心源码。CMake 和 UBT 编译同一组文件；共享清单由集成更新。
2. 跨任务新增文件先登记精确路径和唯一 owner。未列出的路径默认无写权；目录授权不得吞并另一个任务的子目录。一个文件同一时间只能由一个任务写入。
3. 每个领域任务同步提供自己的纯规则检查、真实世界场景支持、只读采样，以及 A11 的捕获/校验/装入接入。它们不直接改 Session、Director、Subsystem 或其他领域的类。
4. 所有 Content 二进制资源按文件独占；不把同一个地图、DataAsset、Blueprint 或声音绑定交给多个 agent 并行保存。
5. 领域数据配置放在各自 Content 子目录；全部 Config/*.ini、工程/构建清单、主地图、总装配资产等共享文件由集成角色统一修改。
6. 每个实际实施任务使用独立工作区/分支和显式写范围。提交范围外需求交 owner；集成角色串行合并、维护共享文件并重跑受影响验证，不通过静默越界实现“协作”。
7. 根 AGENTS、spec、进度和任务台账等治理文件归主任务/集成维护；领域代理只提交必要变更建议，不擅自修改已确认规范。

### 共享文件的唯一写入角色

| 文件/职责 | 唯一写入角色 | 其他任务交付方式 |
| --- | --- | --- |
| UEgame.uproject；Source/UEgame*.Target.cs；Source/UEgame/UEgame.Build.cs；UE/UEgame.cpp；根 CMakeLists.txt | 集成 | 依赖/源清单修改请求，附理由和对应验证 |
| CoreH/Session/**、CoreS/Session/**：ExperimentSession、RunState、FlowRules、时间、事实、效果、受限投影、结果及跨域身份 | 集成 | 领域事实/效果/恢复字段契约与检查；禁止第二份可写 RunState |
| UE/ExperimentAdapter/**：Subsystem、Director、DataMapper、总 ExperimentConfig、参与者/装配入口 | 集成 | 领域的窄接入接口和装配需求；不在领域 owner 之外直接编辑已有类 |
| UE/Contracts/**：跨域 UE 值契约及三组接口接入声明；其纯核心部分归 CoreH/Session/** | 集成 | B02 契约提交后各 owner 只实现各自一侧；不创建新世界管理器或状态真源 |
| Config/**；Content/UEgame/Maps/**；Content/UEgame/Bootstrap/** | 集成 | 各领域的配置片段、资源引用、场景布局需求 |
| Tests/Shared/CoreCaseCatalog.h；Tests/ExperimentCore/**；WorldTests/Framework/**；共享测试注册/发现清单 | B01先建立最小可执行检查；B01集成后显式移交B14，禁止重叠写入 | 各领域独立用例及 CaseId；集中登记，不能并行编辑总表 |
| Scripts/Validation/**；Validation/Cases/**、Validation/Schemas/**、Validation/Templates/** | 验证集成角色（B14） | 各领域的阶段/证据要求；不擅自复写已产生的执行结果 |

表中目录是初始化规划，不表示对应类、资源或工程文件已存在。B00、B01、B02、B15 对集成共享文件的写入是同一角色按阶段串行推进，不是四个并行 writer。

## 2. 任务总表

| ID | 工作包 | owner 角色 | 实施依赖 | 初始状态 |
| --- | --- | --- | --- | --- |
| B00 | 工具链和构建契约 | 工具链/集成 | 初始化规范可读；实际工具准备按后续实施安排 | 已分配角色；未启动 |
| B01 | 最小纵向骨架与核心流程 | 核心/集成 | B00 | 已分配角色；未启动 |
| B02 | 跨域接口基线 | 接口/集成 | B01；先固定契约再开放领域并行 | 已分配角色；未启动 |
| B03 | 球面世界与小地图 | 世界/地图 | B02 | 已分配角色；未启动 |
| B04 | 球面导航与共用移动 | 导航/移动 | B02、B03 | 已分配角色；未启动 |
| B05 | 玩家输入、相机与闪避 | 玩家控制 | B02、B03、B04 的共用移动接口 | 已分配角色；未启动 |
| B06 | 战斗、弹道与有序结算 | 战斗 | B02、B03、B05 的输入/瞄准/落地接口 | 已分配角色；未启动 |
| B07 | 哥布林与遭遇行为 | AI/遭遇 | B02、B04、B06 | 已分配角色；未启动 |
| B08 | 树木切削、几何和木体运动 | 木体几何 | B02、B03、B06；导航消费接口取 B04 | 已分配角色；未启动 |
| B09 | 火雨湿、材料环境与年代交接 | 环境 | B02、B03、B06；与 B08 按已固定事务接口并行 | 已分配角色；未启动 |
| B10 | 回访、叙事与媒体聚合 | 回访内容 | B02、B03、B05 的控制/视口接口 | 已分配角色；未启动 |
| B11 | 音频运行时与语音实例 | 音频运行时 | B02；最终绑定依赖 B10 的文本清单、B13 资源 | 已分配角色；未启动 |
| B12 | 完整快照、文件提交与恢复 | 持久化/恢复 | B02；各领域装入接入随 B03–B11 交付 | 已分配角色；未启动 |
| B13 | 独立声音素材准备 | 素材 agent | B02；对白须 B10 的准确文本/版本；B11 资源需求 | 已分配角色；未启动 |
| B14 | 四层验收工具、共享检查与证据 | 验证集成 | B00、B01；随各领域加入用例 | 已分配角色；未启动 |
| B15 | 全场集成、性能及交付验收 | 总集成/验证 | B00–B14 所需交付；实际本人反馈另行取得 | 已分配角色；未启动 |

“某接口依赖”必须有可审查且已合入的契约/里程碑，不以聊天中的口头承诺解锁。B02、B14 可先交最小可用基线，后续集成修改串行进入；领域完成不等于四层验收完成。

## 3. 每项明确写入范围及验收

### B00 — 工具链和构建契约

- owner：工具链/集成。A03 与 A12 基线。
- 代码/脚本范围：Scripts/Toolchain/**；集成拥有的根构建文件、Target、Build.cs 和忽略规则；本阶段只在允许实际工程准备时创建可执行构建入口。
- Tests：Validation/Toolchain/** 的环境核对记录/清单；不冒充运行测试结果。Content：无。Config：仅集成角色的实际必要工程设置。
- 交付：验证 UE5.6.1、MSVC14.38 的实际完整版本、SDK10.0.22621.0、PowerShell7；Linux Ubuntu24.04/x86_64、GCC13、CMake3.28/CTest。记录真实可用路径和缺项，个人路径不进入版本化构建规则。
- 验收：实际发现的版本匹配；同源两条构建路径与唯一检查入口明确；MSVC14.44、MinGW、Docker/WSL 入口存在均不替代已选环境。禁止在 Build.cs 安装、联网或运行 CMake。缺条件保持未满足，不能填入猜测补丁版本。

### B01 — 最小纵向骨架与核心流程

- owner：核心/集成。A01/A02/A03；后续 A11/A13 的共享扩展仍由同一 owner 合入。
- 代码范围：CoreH/Session/**、CoreS/Session/**、UE/ExperimentAdapter/**、UE/UEgame.cpp，以及集成拥有的工程构建入口。
- Tests：SharedTests/Session/**、WorldTests/Bootstrap/**；向 B14 提供首批 CaseId。Content：Content/UEgame/Bootstrap/**、Content/UEgame/Maps/**。Config：由集成写必要入口地图、启动/输入设置。
- 交付：一个 Session 唯一拥有实验状态；Subsystem 采样和适配；Director 唯一装配。先打通一条“输入/事实 → 核心 → 类型化效果 → 实际世界回执 → 可见反馈”最小链，形成独立启动路径，再推进各领域。
- 为完成本阶段实际检查，先创建共享用例目录、核心测试main和UE最小测试入口；合入时在进度台账移交这些共享文件给B14，B01不等待完整四层验收工具。
- 验收：Linux 和 UE 编译同源核心；显式核心场景实际运行；Game/Editor 唯一 Runtime 模块；最小 Win64 独立包实际启动。初始/观察/攻击三个起点、同批结算顺序、旧轮事实隔离按 A02 验证。灰盒链只证明骨架，不能结清完整玩法、媒体或本人体验验收。

### B02 — 跨域接口基线

- owner：接口/集成；领域 owner 评审各自端点。
- 代码范围：UE/Contracts/**；必要共享纯值/身份/事实归 CoreH/Session/**；Director/Subsystem 的接线由集成串行修改。不得引入另一个状态管理器。
- Tests：SharedTests/Contracts/**、WorldTests/Contracts/**。Content：无。Config：无。
- **B02-TXN（A06/A08/A09/A07）**：固定接触组/共同前缀、材料谱系、CutId、刀预算差量、几何/碰撞准备、环境裁分 Prepare/Validate/Publish 和版本通知接口。A08 协调木体整组事务，A09 实现自己的环境候选与发布；A06 保留攻击实例/刀预算所有权；A07 只消费提交后的真实几何版本。
- **B02-MEDIA（A10/A13）**：固定播放身份、文本/声音版本、有效字幕证据、语音结束候选、聚合收尾、TextOnlyChosen/TextFallback、新代次和取消接口。B10 唯一写 Narrative/Presentation/Mapper/RevisitRules；B11 只写声音组件及状态回执；两项关键送达仍由 Session 唯一提交。
- **B02-RESTORE（A11/全场）**：固定各领域 CaptureCheckpointValue/ValidateCheckpointValue/RestoreCheckpointValue、冻结证明、配置/实体/版本清单、取消排空、重建/实际就绪接口。B12 组织快照与恢复，各原 owner 扩展自己的类；Session 的导出/装入入口由集成写。
- 验收：三个接口清单逐一列到双方 owner、文件、输入/输出、失败/取消/旧代次行为和用例；领域可据此独立编译其实现与契约检查。跨域实际场景的通过证据在 B15 取得，不能凭声明已存在判通过。

### B03 — 球面世界与小地图

- owner：世界/地图。A04。
- 代码范围：UE/World/Spherical/**、UE/UI/Spherical/**；CoreH/Spherical/**、CoreS/Spherical/**（SphericalMath）；FSphericalDataMapper 同归本任务。
- Tests：SharedTests/Spherical/**、WorldTests/Spherical/**。Content：Content/UEgame/World/Spherical/**、Content/UEgame/UI/Spherical/**。Config：无直接写权。
- 交付：WorldRoot、只读球面配置/空间基、局部活动地表和空气墙；MinimapComponent/Rig/Widget。全局北轴/本初子午线等 A06 接入扩展仍由本 owner 修改自己的 Root/Config。
- 验收：非活动区不建模；移动/弹道/导航同源空间与单位；小地图只显示允许内容，旧绑定失效；稳定空间身份及恢复后重绑有效。

### B04 — 球面导航与共用移动

- owner：导航/移动。A07 的完整导航链与通用移动基类。
- 代码范围：UE/World/Navigation/**、UE/World/Movement/**；CoreH/Navigation/**、CoreS/Navigation/**（NavChartMath）。
- Tests：SharedTests/Navigation/**、WorldTests/Navigation/**。Content：Content/UEgame/World/Navigation/**。Config：无直接写权。
- 交付：USphericalCharacterMovementComponent、Navigation/Geometry/PathFollowing 与配置；球面坐标域、几何导出、查询、跟随、实际到达及 CancelAndDrain。
- 验收：真实坡面和球面导航端到端通过；移动速度/接地只归一个 Movement；几何提交/倒木更新使旧路径失效，取消和恢复不能晚写旧世界。若确需引擎改动，先向集成提交明确文件/理由和隔离方案，不自行扩大到安装目录。
- 接口里程碑：共用移动声明和绑定协议合入后 B05 可开工；完整导航场景仍由本任务负责，不能把接口里程碑标成导航完成。

### B05 — 玩家输入、相机与闪避

- owner：玩家控制。A05，含 A06 已确认的玩家侧扩展。
- 代码范围：UE/World/Player/**、UE/UI/Player/**；CoreH/Player/**、CoreS/Player/**（DodgeRules）。
- Tests：SharedTests/Player/**、WorldTests/Player/**。Content：Content/UEgame/Player/**、Content/UEgame/Input/**、Content/UEgame/UI/Player/**。Config：无直接写权；输入上下文为本任务资源，工程注册交集成。
- 交付：Controller/Character/派生 Movement/CameraManager/配置/Mapper；输入代次、最终 AimFrame、双视角、径向运动及闪避；按住瞄准/松开发射/取消不发射均在自己的 Controller 中实现。
- 验收：键鼠和模拟手柄走有效运行时输入链；相机/移动/伤害保护不存重复状态；失焦、切模式、停止、恢复和测试清理先失效手势，再松键，不额外发射。实物手柄保持未测标记。
- 接口里程碑：输入/瞄准帧、攻击意图、落地/硬直与取消接口合入后 B06 可开工；B06 不能直接修改本任务类。

### B06 — 战斗、弹道与有序结算

- owner：战斗。A06，含 A07 徒手接入、A08 刀预算及 A09 环境步接口。
- 代码范围：UE/Combat/**；CoreH/Combat/**、CoreS/Combat/**（LatitudeTrajectory、AttackDirectionRules、CombatRules）。
- Tests：SharedTests/Combat/**、WorldTests/Combat/**。Content：Content/UEgame/Combat/**。Config：无直接写权。
- 交付：CombatWorld/Combatant、Weapon/Projectile、接触查询与 Mapper；本 owner 持有所有 Weapon 文件及 FWeaponCutProgress，其他任务提交扩展需求。
- 验收：纬度/航向/俯仰合法域和真实扫掠一致；剑/魔法统计按实际使用登记；同时间接触按已确认顺序整组结算；唯一生命、已命中集合、EndOnce、首次落地、跨木段刀预算在恢复后均不重复消费。
- A07 的 UUnarmedAttackComponent 文件归 B07，但只由本任务 CombatWorld 的唯一战斗推进入口调用；不能另加自主 Tick。

### B07 — 哥布林与遭遇行为

- owner：AI/遭遇。A07 的行为部分。
- 代码范围：UE/World/Goblin/**；CoreH/Goblin/**、CoreS/Goblin/**（GoblinRules）；UUnarmedAttackComponent 位于本任务目录并由本 owner 实现。
- Tests：SharedTests/Goblin/**、WorldTests/Goblin/**。Content：Content/UEgame/Goblin/**。Config：无直接写权。
- 交付：EncounterBehavior、GoblinCharacter/Behavior/AIController/Config/Mapper 与徒手攻击实例；通过 B04 求路/移动，通过 B06 真实攻击与伤害。
- 验收：共同目标/指向事实、脱战、原棒对应和拾取失败/无进展游标符合规则；拾取失败不清空历史、徒手只由 Combat 推进；快照保留逻辑目标，恢复重求路径，不复用旧句柄。

### B08 — 树木切削、几何事务与木体运动

- owner：木体几何。A08，以 A09 后续事务细化为准。
- 代码范围：UE/World/Trees/**；CoreH/Trees/**、CoreS/Trees/**（TreeCutGeometry、TreeCutRules）。
- Tests：SharedTests/Trees/**、WorldTests/Trees/**。Content：Content/UEgame/World/Trees/**。Config：无直接写权。
- 交付：TreeWorld/MaterialActor/Geometry/Motion、MeshBuilder/MotionQuery/Mapper；整组准备和原子发布；暂存组件在发布前不参加真实碰撞/导航/环境登记。
- 验收：真实缺口、严格大于 80% 断开规则和烧细不自动倒满足；复合碰撞实际可用后整组发布；失败不部分扣预算、不改变环境；倒落仅一个调度入口，角色支撑/阻挡有真实证据；木体/谱系/断面和刀预算恢复后对齐。

### B09 — 火雨湿、材料环境与年代交接

- owner：环境。A09。
- 代码范围：UE/Environment/**；CoreH/Environment/**、CoreS/Environment/**（EnvironmentGeometry、EnvironmentRules、EnvironmentOutcomeRules）。
- Tests：SharedTests/Environment/**、WorldTests/Environment/**。Content：Content/UEgame/Environment/**。Config：无直接写权。
- 交付：唯一 EnvironmentWorld 状态、Grass、Transition、Presentation、Query/Mapper；自己的 PrepareMaterialTransfer/ValidatePreparedPatch/PublishPreparedPatchNoFail；通过 B02-TXN 接入 B08，禁止直接写 Tree 或 Weapon 文件。
- 验收：火雨湿来源/边界、消耗、雨程、历史覆盖按共同前缀推进；材料裁分、火湿谱系与木体同次提交；失败不能前移游标或重复消费；旧年代封存可终止，回访痕迹来自已提交后果；恢复不重复 BeginEpoch 清空载荷。

### B10 — 回访、叙事与媒体聚合

- owner：回访内容。A10，合并 A13 对既有类的全部扩展。
- 代码范围：UE/Revisit/**、UE/UI/Revisit/**；CoreH/Revisit/**、CoreS/Revisit/**（RevisitRules）；Narrative、Presentation、Mapper 均由本 owner 唯一编辑。
- Tests：SharedTests/Revisit/**、WorldTests/Revisit/**。Content：Content/UEgame/Revisit/**、Content/UEgame/UI/Revisit/**。Config：无直接写权。
- 交付：Layout、Participant、单通道 Narrative、Presentation、遮罩/字幕视图和 FText 内容真源；准确 SegmentId/ContentRevision/TextDigest 清单交 B13；聚合 B11 声音状态与有效可读证据。
- 验收：三方向群众与教师内容、Optional 区别正确；实际遮罩后才改世界，环境/布局/导航联合验证后淡入，第三 Ready 后实际恢复控制；Narrative 先封存完成再交 Session，两项关键事实齐备先结束后停调度。语音结束回调、开始对话或字幕创建均不能单独算送达。
- 模块内可用受控回执检查分支；最终有声送达、静音/故障降级和全场恢复必须由 B15 取真实证据。

### B11 — 音频运行时与语音实例

- owner：音频运行时。A13。
- 代码范围：UE/Audio/**、UE/UI/Audio/**；不直接修改 B10 的 Narrative/Presentation/RevisitRules；如需核心字段交集成或 B10 owner。
- Tests：WorldTests/Audio/**；纯值检查如需新增置 SharedTests/Audio/**，不造第二套送达规则。Content：Content/UEgame/Audio/Runtime/**、Content/UEgame/UI/Audio/**；不写 B13 的 Voices/SFX 资产。Config：无直接写权。
- 交付：AudioConfig、WorldAudio、VoiceComponent、SettingsWidget；版本化声音绑定、语音实例代次、混音租约和冻结/清理回执。Subsystem 权威音量字段由集成按本任务接口实现。
- 验收：完整实例身份隔离旧通知；语音软件状态只输出候选；暂停/故障/重播不复用旧可读证据；结束、新轮、卸载取消顺序正确，无残留世界声音或 duck；Cook 后绑定和正常有声路径与 B13/B15 联合检查。

### B12 — 完整快照、文件提交与恢复

- owner：持久化/恢复。A11，包含 A13 媒体冻结接入。
- 代码范围：UE/Persistence/**、UE/UI/Recovery/**；CoreH/Checkpoint/**、CoreS/Checkpoint/**（CheckpointRules、CheckpointClockRules）。LogWriter 文件放 UE/Persistence 并归本 owner，不由 B01 另造一个日志 writer。
- Tests：SharedTests/Checkpoint/**、WorldTests/Checkpoint/**。Content：Content/UEgame/Persistence/**、Content/UEgame/UI/Recovery/**。Config：无直接写权。
- 交付：Coordinator、SnapshotComponent、Codec、Store、恢复视图和已提交结果导出。按 B02-RESTORE 汇合各领域的快照三件套；各领域状态扩展由各自 owner 实现，Session/Director/Subsystem 修改交集成。
- 验收：捕获为同一个完整稳定切点；固定字节首次提交加最多三次重试；Committed/NotCommitted/Indeterminate/Conflict 分流真实；一次故障自动恢复至多一次、成功继续点后才激活；旧回调/Ready 无效、终态不复活、JSONL 导出失败不回滚世界。
- 真实 IO 故障及跨进程恢复由 B14/B15 在隔离目录取证。单元规则通过不能替代真实存储、导航/碰撞/控制及媒体恢复。

### B13 — 独立声音素材准备

- owner：独立素材 agent；不得同时并行修改 B10 文本真源或 B11 运行配置。
- 代码：无玩法代码写权；必要素材处理脚本仅 Scripts/Materials/**。Tests：Validation/Materials/** 的来源/加工/音文核对/导入记录；实际执行证据进独立输出目录。Content：Content/UEgame/Audio/Voices/**、Content/UEgame/Audio/SFX/**。原始/成品制作文件：Assets/Audio/**。Config：无。
- 交付：根据 CueId/SegmentId 请求准备声音，维护 AudioMaterialRequest/Provenance/ImportRecord，原始和成品摘要、版本、使用条件及必要署名。
- 来源与导入记录由本任务独占写入Assets/Provenance/Audio/**；检索/生成临时文件放忽略的Assets/Incoming/、Assets/Generated/，Assets/Audio/**仅纳入明确需要版本化的制作源文件。
- **对白由该素材任务 agent 合成；音效先检索适配开放资源，没有合适匹配才合成。** 记录已看候选和匹配/不匹配理由；工具不可用/检索失败不冒充“无匹配”。不自行购买或替换为预录来源。
- 验收：准确文本、发音、说话者、噪声/削波/循环实际听辨；SoundWave 导入、资源引用和 Cook 清单完整；独立包试听交 B15。无声占位和 TextFallback 不算素材完成，运行时不联网补素材。
- 本次仅分配职责，不检索、合成、加工、导入或试听素材。

### B14 — 四层验收工具、共享检查与证据

- owner：验证集成。A12；本 owner 维护共享框架，领域 owner 维护自己的用例支持。
- 从已集成B01接收共享测试入口及注册清单，再扩展完整四层工具；接收前不与B01并行编辑共享文件。
- 代码/脚本：Scripts/Validation/**；WorldTests/Framework/** 的四个开发测试类/Driver；Tests/ExperimentCore/**；共享 CoreCaseCatalog 和检查支持基本值。Tests：Validation/Cases/**、Validation/Schemas/**、Validation/Templates/**。Content：Content/UEgame/Validation/Framework/**。Config：无直接写权，测试宿主设置提交集成。
- 交付：Orchestrator/ProcessRunner/Evidence 三职责模块、薄入口、Linux 证据导入、共享 header-only 核心用例、UE 宿主发现/执行、实际输入和文件故障测试扩展、人工记录模板。
- 验收：测试发现和执行分开；非空预期清单、逐 Case 完成状态、原生报告/退出/清理/源码与 Build 身份全部核验；未知/跳过/缺报告/超时不判通过；仅终止自己的进程；测试重跑不套用游戏三次重试。
- 技术验证、独立包人工观察和本人反馈分别记录；JSON 与 Markdown 同源，失败/Blocked/NotRun 也生成摘要。B14 交付的是验收能力，不能因工具就绪宣布游戏交付通过。

### B15 — 全场集成、性能与交付验收

- owner：总集成/验证，具体人工体验结论来自用户本人。
- 代码：只写上表集成共享文件；性能接入的各领域代码由相应 owner 按明确变更任务完成，不能让性能任务获得所有 Source 的通配写权。脚本：Scripts/Performance/**。Tests：Validation/Performance/**、WorldTests/Integration/**；跨域 CaseId 由 B14 合入集中目录。Content：Content/UEgame/Maps/**、Content/UEgame/Bootstrap/**、Content/UEgame/Validation/Integration/**。Config：全部共享 Config 由本 owner 串行集成。
- 交付：B02 三组接口的真实集成证据；双平台核心、UE 场景、Win64 Development 独立包、正常声音/完整字幕/普通操作检查和本人四类比较记录；构建输入/资产及交付包完整身份。
- **性能硬目标：同一有效 InputId 从客户端收到输入，到首个对应视觉反馈实际呈现，小于 50ms。** 覆盖输入排队、核心处理、UE 世界响应、渲染和呈现等待；50ms 不是帧预算、20FPS 指标或核心单步预算。不能用“命令已入队”“CPU 渲染提交完成”或无关 UI 闪动替代对应反馈实际呈现。
- 实施前记录硬件、系统/显示设置、分辨率/画质、构建配置、实际场景/负载、输入种类与对应反馈，明确起止采样方法、时钟关联和测量误差。基于可验证测量链取得证据；只到 GPU/CPU 提交的记录须标为代理指标，不能据此宣称满足端到端目标。
- 保留全部有效慢样本、最大观测值、场景和来源，平均/P99 不覆盖超过阈值的失败。所有受测有效样本逐项检查 <50ms；有限测试范围及未覆盖情况如实记录，不宣称所有机器/负载均已证明。
- 验收：源/资产/包身份一致；跨事务失败无部分发布；三项就绪/真实恢复控制和媒体送达正确；全场恢复/实际文件故障/旧代次隔离通过；端到端性能达到已记录条件下目标。缺工具/测量条件保持未满足。
- 本人实际比较剑术、魔法、混用与小孩遇难四类，保留原话，分别评价“能说明因果链”“感觉选择改变世界”。恢复、媒体模式、未知耗时独立标记；模拟手柄不冒充实物已测，TextFallback 不结清正常有声验收。不预设试玩轮数。

## 4. 排程与完成定义

1. 先完成 B00 的真实工具基线，再打通 B01 双编译/最小独立启动；领域大规模实现不能越过这一步。
2. B02 接口基线先合入。B03 空间基准 → B04 共用移动接口 → B05 玩家接口 → B06 战斗后，B07/B08/B09 可按依赖展开；B04 完整导航验证继续独立完成。
3. B10、B11、B12、B14 可在自身接口已具备时并行，不直接争抢共享类。B13 按已确定的文本和声音请求独立执行，具体素材工具先实际核对。
4. B15 按里程碑串行装配和验证；模块通过只覆盖本模块用例，跨域、独立包和体验证据必须另取。
5. 状态按工作流程统一为Assigned → Ready → InProgress → Validated → Reviewed → Integrated；Blocked注明实际原因。当前全为Assigned（已分配角色；未启动）。每次实际启动记录agent/工作区/精确写范围/基线版本，不填写虚构实施者、完成日期或通过证据。
6. 构建、缓存、运行存档和本轮证据按既定规则分别进入 .build/、Saved/ 或 Deliverables/<BuildId>/ 等输出目录，不能污染 Source、复用普通用户存档或跨任务覆盖证据。
7. 正式冻结、GitHub 远端/CI/分支保护、工程实现和本机工具安装都单独按实际授权与可用条件推进；“本地仓库已初始化”“已分配 owner”“设计已确认”均不等于这些事项已完成。
