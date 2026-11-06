# A04 类图确认

来源：本聊天；归档日期：2026-11-05（Asia/Shanghai）。

## 用户原文

> 确认

## 确认语境与范围

上一轮提交 A04 类图，末尾询问是否确认类职责、引用关系和重置清理策略，确认后进入 A05「输入、双视角、移动与闪避」总体方案。用户本次确认。

确认 USphericalWorldConfig、ASphericalWorldRoot、experiment::SphericalMath、FSphericalDataMapper、USphericalMinimapComponent、ASphericalMinimapRig、USphericalMinimapWidget 七个类的职责；球面纯值、地图显示帧及 UE 类型/单位转换边界；Director 仍组织实际世界准备，Session 仍独占实验状态，本地 PlayerController 持有地图组件。

确认地图组件统一创建、重绑与清理 Rig/Widget，资源强引用与跨对象弱绑定，RunId＋BindingGeneration 校验，旧快照、标记及纹理失效策略；地图旋转只作用于表现，隐藏不暂停实验。空间仅建模活动区，空气墙限制范围；小镇/森林地区轮廓保留。

本次没有预先确认 A05 的输入插件、具体映射、移动框架、闪避边缘规则或相机实现；这些作为下一项总体方案单独提交。仍未授权游戏实现、安装、构建发布或正式冻结。
