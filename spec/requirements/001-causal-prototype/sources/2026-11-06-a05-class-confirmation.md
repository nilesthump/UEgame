# A05 类图确认

日期：2026-11-06（Asia/Shanghai）。

## 用户原文

> 确认

## 确认语境

上一项提交了 A05 类图及输入/动作/相机值图，询问是否确认七个类的职责、状态归属和生命周期，通过后进入 A06 总体方案。

本次确认覆盖 UExperimentPlayerConfig、AExperimentPlayerController、AExperimentCharacter、UExperimentMovementComponent、AExperimentPlayerCameraManager、experiment::DodgeRules 和 FPlayerDataMapper；输入代次与闪避执行身份分开，位移结束与短窗口到期分开，受击发生时采样保护结果，最终相机更新后交付攻击意图，Director 协调唯一角色复位。

阶段仍由 Session 唯一维护；生命值、硬直及具体伤害/防反顺序留 A06。此次未预先确认 A06 新方案或类图，未授权游戏实现、构建、部署或正式冻结。
