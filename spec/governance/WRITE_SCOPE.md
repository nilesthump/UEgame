# 写入范围与唯一所有者

权威目录：仓库根。固定管线及分支见[管线总表](../pipelines/README.md)；角色名是责任分工，不是已雇用人数。除本表列出的路径外默认只读。跨范围修改必须先由上游 MR 说明影响，由受影响管线评审，不能以“方便”直接写入。

## 1. 管线所有权

| 管线 | 独占写入范围 | 明确不写 |
| --- | --- | --- |
| growth | `spec/requirements/**`（`sources/**` 只可新增来源，不改写既有来源）；`spec/pipelines/growth/**` | 架构、代码、资产、证据结论、发布状态 |
| narrative | `spec/contracts/narrative/**`；`spec/pipelines/narrative/**`；`Scripts/Narrative/**` | 运行时逻辑、UE 资产、美术源文件、test 证据 |
| tech | `Source/**`；`Tests/**`；`CMakeLists.txt`；`UEgame.uproject`；`Config/**`；`Content/**`；`.github/**`；`Scripts/Collaboration/**`；`spec/pipelines/tech/**`；`spec/contracts/art/REQUEST-*.md` | 剧情真源、美术源素材、验收结论、发布状态 |
| art | `Assets/**`；`Scripts/Materials/**`；`spec/pipelines/art/**`；`spec/contracts/art/RESPONSE-*.md` | `Content/**` 的 UE 二进制资产、运行时逻辑、剧情真源、发布状态 |
| test | `Validation/**`；`Scripts/Validation/**`；`Tests/Acceptance/**`；`spec/evidence/**`；`spec/pipelines/test/**` | 业务代码、美术源素材、剧情真源、发布状态 |
| ops | `AGENTS.md`；`README.md`；`spec/governance/**`；`spec/pipelines/README.md`；`spec/pipelines/PROTOCOL.md`；`spec/pipelines/ops/**`；`spec/releases/**`；`spec/evidence/**`（只与 test 串行，见下）；`spec/progress/**`；`spec/tasks/**`；`spec/performance/**`；`spec/architecture/**`；`Scripts/Release/**` | 代码、资产、剧情真源、测试执行结果 |
| maintenance | `spec/pipelines/maintenance/**`；已登记事故的临时热修文件（见第 3 节） | 正常版本功能、共享入口、UE 二进制资产 |

`spec/pipelines/<name>/CURRENT.md` 由对应管线唯一维护。`spec/architecture/**` 由 `ops` 作为记录编辑者，涉及技术、剧情或美术的变更须由对应管线在 MR 中评审；不得把讨论状态或未确认设计写成已冻结。

## 2. 共享入口与二进制资产的唯一写入者

| 共享对象 | 唯一写入者 | 其他管线交付方式 |
| --- | --- | --- |
| `UEgame.uproject`、根 `CMakeLists.txt`、`Source/*.Target.cs`、`Source/UEgame/UEgame.Build.cs`、模块入口 | tech | 构建/源清单变更请求，附受影响验证 |
| `Config/**`、主地图/联合场景/总装配资产 | tech | 配置片段、场景布局和接入需求 |
| `Content/**` 下的 `.uasset`、`.umap` 等 UE 二进制资产 | tech | art 交付源素材、来源/加工记录和导入清单；narrative 交付稳定文本 ID |
| `.github/**`、`Scripts/Collaboration/**` | tech | 其他管线的 CI/工具变更建议 |
| `spec/contracts/narrative/TEXT.csv` 与叙事文本真源 | narrative | tech 只读导入；art 只按定稿条目制作 |
| `spec/evidence/evidence.md` | test | 各管线提供执行结果和未满足项；test 形成当前结论 |
| `spec/releases/state.json` 与发布记录 | ops | test 交付验收结论；tech 交付构建身份 |

同一文件同一时间只能有一个管线写入。`ops` 与 `test` 对 `spec/evidence/**` 的交叉仅限：test 维护结论，ops 在发布/回滚记录中引用，不另写第二份结论。

## 3. 维护管线的临时热修例外

`maintenance` 平时只写自己的 `spec/pipelines/maintenance/**`。线上故障需要改代码时：

1. 在 `spec/pipelines/maintenance/` 登记事故、候选基线、受影响文件清单和回退方式；
2. 仅对清单内文件取得临时唯一写权，`tech` 不得并行编辑同一文件；
3. `tech` 作为独立审查者检查修复和回归；修复经 `maintenance → ops` 交付，不直接进 `tech` 或 `main`；
4. 修复合并后临时写权结束，后续功能开发回到 `tech`。共享入口和 `Content/**` 二进制资产仍只由 `tech` 写入。

## 4. 分支与历史

- `main` 只记录实际线上运营版本，只接收 `ops → main` 的 merge commit。
- 固定分支建立后不再新增分支；旧 `task/*` 分支只读保留，不接收新提交、不删除。
- 每个工作项使用 `x.y.z` 目标版本；同版本内按功能点提交，一次提交只包含一个可独立验收的功能或修复。
- 历史的执行事实由 Git 保存；当前结论只写 `spec/evidence/evidence.md`，不新增 `evidence-latest.md`、`evidence-final.md` 等平行文件。
- 文档身份使用 Git 提交和稳定业务 ID，不再以 SHA-256、精确引文或文本交接特征串作为写入/合并门禁。
