# A02 类图确认

来源：本聊天；归档日期：2026-11-05（Asia/Shanghai）。

## 用户原文

> 确认

## 确认语境与范围

上轮问题为“是否确认这版 A02 类图？确认后进入 A03：UE 小版本、C++ 工具链与依赖边界。”

用户接受 A02 展示的类职责与数据设计：UExperimentSubsystem 唯一提交状态，新增无状态的 FExperimentFlowRules，细化 FExperimentRunState 与 FExperimentTiming，使用瞬时稳定 FEncounterSnapshot 和最小 FExperimentFlowDecision；类型化事实区分使用招式、朝哥布林攻击及观察等，旧轮/旧请求回调受归属限制。

已展示的关键边界包括：独立可选起点与共享阶段边界；同动作的两种事实分别处理；观察死亡后果也能触发稳定判定；失效引用不能当作死亡；实际恢复控制时才开始回访。

A02 总体方案和类图两步完成，继续 A03 总体方案讨论。此回答不预先确认 A03 版本、工具链或模块选择，不授权游戏实现、安装配置或正式架构冻结。
