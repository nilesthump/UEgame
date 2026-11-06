# A06 类图确认

日期：2026-11-06（Asia/Shanghai）。

## 用户原文

> 确认

## 确认语境与范围

上一答复提交 A06 类图、技术栈、类职责表、数据流、理由及魔法瞄准流程，并链接完整设计和两份 PUML。用户确认该类设计，包含：

- 10 个职责类：CombatConfig、CombatWorldComponent、CombatantComponent、Weapon、Projectile、ContactSampler、DataMapper、LatitudeTrajectory、AttackDirectionRules、CombatRules，以及 A04/A05/Director 的相关扩展。
- Controller 拥有尚未执行的魔法瞄准手势；武器/弹体拥有已执行攻击；Combatant 唯一拥有生命/硬直/击飞落地状态；无敌窗口仍归 Movement。核心规则无 UE 对象依赖。
- 两种法术分别保存手势，松开哪个就请求发射哪个；挥砍与瞄准可以并存，实际防反硬直统一取消未执行输入。按住瞄准本身不计使用、仇恨或首次攻击计时。
- CombatWorld 统一战斗推进及攻击序号分配，Director 统一实体身份；同组先形成完整计划再提交，整步全部接触组、新意图及派生结果完成后才请求一次稳定快照并判定。
- 掉棒由统一战斗步作径向重力扫掠，保留实际落地位置，不并行启用另一套物理推进；角色及其组件遵守唯一复位入口。

按“总体确认 → 类图确认 → 下一项”继续 A07 哥布林行为总体。具体 A07 技术路线及新增行为取舍尚未提交、未获确认。本次没有授权游戏实现、安装工具链、构建或正式冻结。
