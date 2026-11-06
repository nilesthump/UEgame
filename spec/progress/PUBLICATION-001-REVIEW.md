# 公开仓库与提交规范：审查和执行记录

日期：2026-11-06。授权：[用户原文](../requirements/001-causal-prototype/sources/2026-11-06-public-repository-and-commits.md)。

- 发布工作树：`.worktrees/repository-publication`，分支`task/repository-publication`，基线`c858048`；协调者负责LICENSE/README、AGENTS/WORKFLOW及新增来源。
- 独立审查者：本聊天子代理`publication_review`。按仓库读序，核对原始单提交历史、411个跟踪文件、待提交diff和MIT全文；6类常见凭据模式无命中，本机工具/凭据目录、运行日志、引擎二进制和UE资产未纳入。未发现具体P1/P2；不等于穷尽秘密扫描或第三方权属认证。
- 独立复跑RepositoryBaseline：174份文档、598处本地链接、528条确认来源，Passed；已跟踪diff空白检查通过。首次暂存检查另发现MIT模板末尾多余空行，作者删除该空行后暂存检查通过；许可正文不变。
- 按两个功能点提交：`963cc48`添加MIT许可；`c834714`规定一功能/修复一提交及本次远端授权。canonical/main快进集成后RepositoryBaseline再次Passed。
- 明确授权范围内创建并推送[公开仓库](https://github.com/nilesthump/UEgame)。GitHub实际返回visibility=PUBLIC、licenseInfo.key=mit、defaultBranch=main；origin跟踪已建立。
- 首轮[托管CI](https://github.com/nilesthump/UEgame/actions/runs/42226638076)对应`c834714be94ae6ce917e974bd8b5b829ddd030a2`，repository-baseline job实际完成success。未执行UE/Linux游戏构建、游戏用例或性能测量。
- main保护查询返回`protected=false`。没有建立或宣称GitHub账号批准、强制PR门槛或管理员保护。
- 既有检查脚本、机器契约、CI文件与WORKING原始字节均未改变。WORKING SHA-256为`8E4350992F0A37B0449F11B0AEC687675310F355B79E1FE50F47E54BB30F1CDA`，仍为discussion，未冻结。
- 发布前原始输出与SHA-256清单保留于canonical忽略目录`Validation/Runs/publication-20261106/`；云端CI日志由上述运行链接提供。
