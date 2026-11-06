# A01 类图确认

来源：本聊天；归档日期：2026-11-05（Asia/Shanghai）。

## 用户原文

> 确认

## 确认语境与范围

上轮问题为“是否确认这版 A01 类图？确认后进入 A02：实验阶段、三个计时起点与成败判定。”用户确认 A01 类设计：UExperimentSubsystem、AExperimentWorldDirector、UExperimentConfig、UExperimentLogWriter、IExperimentParticipant，以及 FExperimentRunState、FExperimentResult、FExperimentViewState；接受已展示的职责、继承/组合/依赖、状态归属与生命周期边界。

接受的关键边界包括：协调器弱引用当前世界入口，配置与日志对象明确保活；复位需恢复缺失初始实体并清理新增对象；完整成功日志等回访完成后生成；同次玩法结算完成后才判定成败。

此前 A01 总体方案已经确认，因此 A01 两步完成，继续 A02 总体方案讨论。此次不预先确认 A02 的具体方案，不授权游戏实现或正式架构冻结。
