# 讨论状态

第 001 轮需求及 A01–A13 总体与类图均已逐项确认，含核心分层、任意航向弹道及素材准备修订。本轮按“技术栈、数据流、原因与 PUML 确认 → 类图确认”完成讨论，A13 按用户要求提前于 A12。当前已完成 A12 类图确认并汇总核对；已有 B01 最小工程/核心和部分实现，但尚未正式冻结，也未完成完整游戏、素材准备及验收。

- [001 架构讨论汇总](001-causal-prototype/ARCHITECTURE-SUMMARY.md)：13项确认清单、当前技术边界与数据流、历史替代关系、未执行/未固定事项及后续工作边界。
- [当前仓库进度](../progress/CURRENT.md)、[七管线总表](../pipelines/README.md)、[目录规范](../governance/DIRECTORY.md)与[模块任务](../tasks/001-causal-prototype/TASKS.md)：日常交付进入固定管线，仍未正式冻结。
- [端到端性能契约](../performance/001-input-to-render.md)：输入→核心处理→渲染反馈小于50ms，实际性能尚未验证。

- [A01：整体运行架构与状态归属](001-causal-prototype/A01-runtime-and-state.md)：总体方案已确认，含后续确认顺序。
- [A01 类图设计](001-causal-prototype/A01-class-design.md)：历史确认基线；核心类型和状态所有者以已确认的修订图为准。
- [A02：实验阶段、计时与成败判定](001-causal-prototype/A02-experiment-flow.md)：总体方案及计时口径已确认。
- [A02 类图设计](001-causal-prototype/A02-class-design.md)：历史确认基线；核心类型和提交职责以已确认的修订图为准。
- [核心与 UE 分层方向](001-causal-prototype/A03-core-boundary.md)：已确认；允许未来 UE 权威服务端。
- [核心与 UE 适配类图](001-causal-prototype/A03-core-class-design.md)：已确认。
- [A03：工具链与依赖边界](001-causal-prototype/A03-toolchain-and-dependencies.md)：总体及构建规则类图均已确认。
- [A03：Linux/GCC 与 Docker 补充](001-causal-prototype/A03-linux-core-build.md)：已确认；正式容器宿主协议后续设计。
- [A03 构建规则类图](001-causal-prototype/A03-build-class-design.md)：已确认，含 UE 类、模块入口、CMake 目标及目录隔离。
- [A04：球体世界、局部区域与小地图](001-causal-prototype/A04-spherical-world.md)：总体已确认，包含空气墙与非活动区不建模。
- [A04 类图](001-causal-prototype/A04-class-design.md)：已确认，含球面配置/数学、局部世界与空气墙、地图表现及生命周期。
- [A05：输入、双视角、移动与闪避](001-causal-prototype/A05-input-camera-locomotion.md)：总体已确认，含闪避边缘规则与地图操作方式。
- [A05 类图](001-causal-prototype/A05-class-design.md)：已确认，含输入/动作/相机状态归属、纯闪避规则、视图帧和清理责任。
- [A06 纬度圆弹道公式与图示](001-causal-prototype/A06-latitude-trajectory-derivation.md)：纬度圆、任意航向推广、北轴与极点范围约定已确认。
- [A06 旧固定球心平面候选](001-causal-prototype/A06-spherical-projectile-pitch.md)：历史提案，未获确认，已由纬度圆修订替代。
- [A06：挥砍、投射物、命中、防反、误伤与伤害](001-causal-prototype/A06-combat-and-damage.md)：总体已确认，含六组取舍及魔法瞄准输入修订。
- [A06 类图：瞄准、攻击与有序结算](001-causal-prototype/A06-class-design.md)：已确认，含类职责、并发手势、唯一状态归属、整批结算与掉棒推进。
- [A07：哥布林行为、脱战、捡棒与绕行](001-causal-prototype/A07-goblin-behavior.md)：总体及六组取舍已确认，优先扩展 UE Navigation/Recast，覆盖完整 World/Nav 转换与动态更新链。
- [A07 类图：行为与球面导航适配](001-causal-prototype/A07-class-design.md)：已确认；14 个职责类、共用移动基类、A06 徒手接入、共享目标、两种路径表示与建图屏障，含两份类图和一份时序图。
- [A08：砍树切面、80% 阈值、倒木与分段阻挡](001-causal-prototype/A08-tree-cutting-and-fallen-logs.md)：总体及六组取舍已确认，含多口联合断桥保留连接、受控无伤倒落与同步碰撞提交。
- [A08 类图：切口、几何事务与木体运动](001-causal-prototype/A08-class-design.md)：已确认；十职责类、A06 单刀预算、共同前缀、整组提交与唯一运动调度，含类图和事务时序。
- [A09：火灾、降雨、潮湿与跨阶段环境](001-causal-prototype/A09-fire-rain-and-environment.md)：总体与六组取舍已确认；规则火焰、树桩统一可燃、雨湿来源区别、材料事务和旧时代结算。
- [A09 类图：环境状态、材料事务与年代交接](001-causal-prototype/A09-class-design.md)：已确认；十职责类、共同前缀、材料事务参与者、受烧历史与唯一驱动交接，含类图和两份时序图。
- [A10：三十年跳时、教师方向、群众闲聊、揭示与挽留](001-causal-prototype/A10-revisit-and-narrative.md)：总体与六组取舍已确认；三项就绪、预设内容、非模态触发/中断/补播、关键片段交付及剧情人物边界。
- [A10 类图：回访装配、内容作业与呈现回执](001-causal-prototype/A10-class-design.md)：已确认；九职责类、联合布局凭据、单通道播放与取消代次、Session 唯一送达事实，含类图与两份时序图。
- [A11 修订：快照记录点、三次重试与一致恢复](001-causal-prototype/A11-checkpoint-save-and-recovery.md)：总体已确认；共同记录点/完整载荷、首次加三次重试、最近有效点恢复、新代次隔离、终态保护、日志重建与恢复计时已确定。
- [A11 类图：快照协调、持久提交与全场恢复](001-causal-prototype/A11-class-design.md)：已确认；八个新增职责类、唯一所有权、不可变提交对象、四类存储结果、分阶段恢复激活和统一重试/导出，含类图和两份协作时序图。
- [A11 历史提案](001-causal-prototype/A11-results-reset-and-errors.md)：未获总体确认，已被快照方案替代，保留旧提案和本机文件接口证据。
- [A13 音效、语音、字幕与内容交付](001-causal-prototype/A13-audio-and-voice.md)：总体已确认；UE 原生声音、agent 合成对白/开放音效优先、单语音字幕作业、明确字幕降级与媒体收尾、A11 冻结恢复，含两份 PUML。
- [A13 类图：声音实例、媒体收尾与素材准备](001-causal-prototype/A13-class-design.md)：类图已确认；四个新增职责类、当前音量值、版本化实例/duck/冻结凭据、既有内容链扩展与素材准备契约，含类图、媒体协作和素材流程三份 PUML。
- [A12 分层验收、证据与原型交付](001-causal-prototype/A12-validation-and-delivery.md)：总体已确认；核心/真实UE/独立包/本人体验四层证据，完整报告与版本核对、恢复和字幕模式标记、交付内容及性能约束，含两份 PUML；[最新提前安排](../requirements/001-causal-prototype/sources/2026-11-06-audio-voice-order-change.md)仍有效。
- [A12 类图：测试驱动、验证编排与验收记录](001-causal-prototype/A12-class-design.md)：类图已确认；四个开发测试类、三个外部脚本模块、共享检查入口、身份/证据/反馈值与清理生命周期，含两份 PUML。
- [A01 PlantUML](001-causal-prototype/A01-runtime-and-state.puml)：本项源图。
- [架构逐项确认授权](../requirements/001-causal-prototype/sources/2026-10-31-architecture-confirmation-request.md)：更新此前仅需求分析的阶段边界；未授权游戏实现。

- [需求轮次索引](../requirements/README.md)
- [当前需求轮次](../requirements/CURRENT.md)
- [WORKING.json](WORKING.json)：全局唯一可写讨论状态，保持现有身份、版本和运行器入口。
- [第 001 轮来源资料](../requirements/001-causal-prototype/sources/README.md)：需求来源的唯一保存位置，讨论状态和历史证据均引用这里。

当前完整需求正文和来源均在第 001 轮目录。原 `sources/` 中的 35 份重复文件及空目录已移除，迁移后的来源原文与校验值保持不变。后续新增来源写入对应轮次的 `sources/`。
