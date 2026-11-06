# A07 类图确认

日期：2026-11-06（Asia/Shanghai）。

## 用户原文

> 确认

## 确认语境与范围

上一答复提交 A07 类图：UE5.6.1 Navigation/Recast 扩展、标准 C++20/Linux GCC13 纯规则与数学，14 个职责类、数据流、设计原因、行为/移动及导航两张类图，附路径与重建时序。用户确认该类设计。

- Encounter 唯一共同目标与目标版本；Goblin Character、Behavior、AIController 分离装配、独立决策、请求接入；可信指向事实同步提交最终目标，完整行为在移动前推进。
- 提取共用 USphericalCharacterMovementComponent；玩家已有移动类继承它并保留自己的闪避身份与窗口，每个角色仍只有一个移动组件。
- Goblin 的 UnarmedAttackComponent 保存拳击实例，由 A06 CombatWorld 统一推进与真实接触结算；NPC 不产生玩家使用或首次攻击统计。
- NavigationComponent、NavGeometryComponent、PathFollowingComponent 分别承担导航协调、同域派生几何、世界路径跟随；配置、Mapper、纯 GoblinRules 与 NavChartMath 按已展示职责分离。
- 原生 Recast 路径与映回世界空间的跟随表示分开保存；PathFollowing 唯一管理分段进度。旧路线取消只结算旧 Cookie/完整 RouteKey，不取消同目标的新路线，也不重置拾取进展。
- 重启先停止接纳、失效旧身份并取消路径，实际取消并等待旧建图任务退出，再准备新实体/几何、建图和发布就绪；角色组件只经角色入口复位一次。

上述确认包含已展示正文的接口、数据归属与生命周期约束；不代表已通过 UE/GCC 编译、打包或运行验证。接下来提交 A08 总体，之后另行确认 A08 类图。未授权开始游戏实现或正式冻结。
