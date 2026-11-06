# UEgame agent 工作约定

每次任务先依序读取：

1. [当前进度](spec/progress/CURRENT.md)。
2. [架构入口](spec/architecture/README.md)与[001汇总](spec/architecture/001-causal-prototype/ARCHITECTURE-SUMMARY.md)。
3. [写入范围](spec/governance/WRITE_SCOPE.md)与[工作流程](spec/governance/WORKFLOW.md)。
4. [模块任务](spec/tasks/001-causal-prototype/TASKS.md)中本任务的依赖、范围、验收及关联详细架构。
5. [需求正文](spec/requirements/001-causal-prototype/REQUIREMENTS.md)、[验证契约](spec/governance/VALIDATION.md)与[性能契约](spec/performance/001-input-to-render.md)。

只实现已授权任务的最小可靠方案。业务目录不等于新增UE模块：当前只有一个UEgame Runtime模块；纯ExperimentCore不依赖UE，Windows/Linux编译同份源码。不要复制Session/世界领域状态，不用测试假回执代替真实集成。

任务仅能写任务表允许的路径；其他模块和共享入口只读。发现跨模块接口修改先记录依赖，由集成角色协调；既有用户授权范围内的例行修正直接推进，不重复索要许可。范围外的新功能、不可逆变更或正式架构选择需明确依据。

默认使用独立任务分支与worktree；根canonical为`G:/UEgame`，机器迁移后由协调者更新本机进度记录，不把该绝对路径写进产品构建。首次仓库初始化在canonical/main完成；B00–B15后续实现通过任务worktree进行。任务agent不创建新根规则，不修改其他agent的工作树或清除其数据。

提交前运行与改动相符的验证，取得独立审查，集成后在canonical复核并更新进度。当前只有RepositoryBaseline检查已经落地；UE/Linux/性能等执行能力以验证契约状态为准，未执行、零用例、缺报告或只发现测试不得记通过。

`spec/architecture/WORKING.json`是唯一可写讨论状态，仅协调者使用本机架构运行器和expectedRevision更新。其他agent只提交变更建议；worktree里的状态副本不可独立演化。`sources/`已有原始来源不改写。正式冻结另行处理，不能把Git初始化或已确认讨论伪装成冻结版本。

50 ms约束是**输入→核心处理→渲染反馈的端到端延迟**，不等于帧预算或核心单步。素材agent须合成对白；音效先匹配开放资源，无匹配再合成。所有测量、检索、合成和验收只报告实际执行结果。

不自动发送消息、发布仓库或创建远端服务。GitHub PR/CI是远端接入后的协作路线，当前远端能力状态以进度文件为准。

Git提交粒度为一个功能点：每实现或修复一个可独立验收的功能点，完成对应验证和独立审查后立即提交，不积攒多个功能点。功能所需的代码、用例和说明同提交；后续修复另提交，不改写已发布历史。依据见[本轮授权](spec/requirements/001-causal-prototype/sources/2026-11-06-public-repository-and-commits.md)。本轮明确授权将本仓库公开发布并采用MIT；后续任务分支推送、PR与集成沿本仓库流程执行，不扩大为发布其他仓库或服务。
