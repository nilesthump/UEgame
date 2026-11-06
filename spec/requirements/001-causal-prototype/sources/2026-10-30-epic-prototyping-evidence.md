# Epic 官方原型指导：核验摘要

核验日期：2026-10-30（Asia/Shanghai）。本文件是官方文档的简短摘要和出处记录，不是架构方案或实现计划。只采用官方 Unreal Engine 文档作为主要依据，未将社区用户帖子视为 Epic 官方建议。

## S1 — Project Setup and Level Blockout

官方链接：https://dev.epicgames.com/documentation/en-us/unreal-engine/designer-01-project-setup-and-level-blockout-in-unreal-engine

定位：Sketch Your Ideas、Block Out Your Map、Test Early and Often、Make it Easily Understood。

核验的短引文：

> Before thinking about finished art and polished details

> Test Early and Often

官方建议摘要：先梳理玩法和范围；以简单形状搭建可玩的灰盒，优先检查空间、可玩性和玩法流程，再完善美术；尽早、频繁试玩，以便及时修改。场景应让玩家易于理解。文档也说明游戏设计不只有一种正确方法。

适用边界：教程的示例关卡、第一人称模板和具体操作不是本项目已经选定的方案；不因此升级引擎、选择资产包或删除已经确认的第三人称等需求。

## S2 — Complete the Level

官方链接：https://dev.epicgames.com/documentation/en-us/unreal-engine/designer-10-complete-the-level-in-unreal-engine

定位：开头的 gameplay loop / win condition 定义，以及 From Concept to Creation。

官方建议摘要：可玩流程需要有完成条件；教学案例根据试玩发现的问题调整玩法与关卡。后续美术、声音和视觉效果完善有独立的教学阶段。

适用边界：采用完成条件和反馈迭代原则，不照搬示例中的胜利画面、关卡跳转或自动重置。世界不回档等用户要求继续有效。

## S3 — Playing and Simulating

官方链接：https://dev.epicgames.com/documentation/en-us/unreal-engine/playing-and-simulating-in-unreal-engine

定位：开头对编辑器内试玩与模拟的说明。

官方说明摘要：编辑器内预览能够快速检查玩法和资源修改，支持短周期迭代。

适用边界：这里只确认官方支持快速试玩迭代的原则；交付形式、具体验证入口及实现方式尚未确定。

## 本实验的应用解释

用户已要求采用官方推荐方法，因此采用上述灰盒优先、玩法可读、尽早试玩、根据反馈迭代的原则。原型无需以成品美术品质作为先决条件。

结合已确认目标，素材至少应帮助辨认剑术/魔法、威胁小孩的攻击和回访后的教学变化。这是对本游戏需求的应用判断，不是 Epic 原文规定的专用标准。简单模型、占位动作或音效的具体选用仍应以不妨碍这些体验为准，尚未指定资产或技术方案。

Epic 文档没有为本实验规定 5+2+3 分钟、不可回档、具体测试人数或通过率；这些分别来自用户需求或仍待澄清。模拟手柄信号不等于真实手柄测试。
