# A10 类图确认

日期：2026-11-06（Asia/Shanghai）。

## 用户原文

> 确认

## 确认语境与范围

上一答复提交 A10 回访类图，包含技术栈、九职责类、两条数据流、设计原因、完整设计及三份 PlantUML。用户确认该类图，下一项进入 A11 总体方案。

- 九职责类为 URevisitConfig、URevisitLayoutComponent、ARevisitParticipantActor、URevisitNarrativeComponent、URevisitPresentationComponent、UTimeSkipWidget、URevisitDialogueWidget、FRevisitDataMapper 和纯 C++20 RevisitRules。
- Session 唯一保存阶段、锁定方向、两项完成事实；Director 统一协调世界就绪和实际控制恢复；UE 处理真实世界采样、装配及呈现回执。
- 遮罩实际覆盖后才能改变世界，环境与人物共用最终布局/导航凭据；必要变更使本地凭据失效，保留原先一次恢复请求，重新验证和淡入后才能恢复。
- 单通道播放绑定完整身份、玩家及视口代次；取消先失效再停止。合法关键片段封存后提交唯一事实并等核心处理；Optional 完成不等核心回执，拒绝/过期回执不补成完成。
- 剧情人物没有 Combatant/生命值，但保留玩家通行和弹体真实接触，剑不被其截断多目标轨迹。
- Ended 停止推进并保留现场；手动重启才恢复初始世界，卸载仅销毁而不重建。复位继续沿已有各领域唯一入口。

本次确认类图与接口设计，不代表 UE/GCC 编译或游戏运行验证完成，不授权游戏实现或架构冻结。音效与语音仍放最后的 A13。
