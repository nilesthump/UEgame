# A09 类图确认

日期：2026-11-06（Asia/Shanghai）。

## 用户原文

> 确认

## 确认语境与范围

上一答复提交 A09 十个职责类、类图、材料事务时序与年代交接时序。用户确认具体类设计，下一项为 A10 总体方案。

- EnvironmentConfig、EnvironmentWorld、GrassPatchActor、EnvironmentTransition、EnvironmentPresentation、ContactQuery、DataMapper，以及纯 C++20 EnvironmentGeometry / EnvironmentRules / EnvironmentOutcomeRules。
- World 唯一持有火湿、燃料、环境推进位置、输入消费和已受烧历史；Transition 只持作业、预算、Token 与只读快照句柄，不复制环境时钟或状态。
- 木体先预演，取共同最早前缀；A08 TreeWorld 复用已有事务，同次发布实际姿态、材料、碰撞、火湿和必要刀预算。环境准备/发布不递归调用提交入口。
- 分裂只裁分已经演算到边界的结果，不重复积分或补燃料；同刻引燃保留实际覆盖，原 Actor 转树桩也更新分区绑定。
- 受烧历史独立保留，父树退役、倒木清理或熄灭不删除已经发生的空间影响。
- 新年代状态、材料绑定、碰撞、导航与实时驱动待命均就绪后才报告环境完成；恢复控制握手成立后才开放合法新输入，旧雨不重播。
- 本次包含已展示的 A06/A08 接口扩展、唯一复位与幂等生命周期设计，核心继续使用标准 C++20/Linux GCC 路线。

确认不代表 UE/GCC 编译、打包或运行验证完成，不授权实现游戏代码或冻结架构，也不提前确认 A10 具体方案。
