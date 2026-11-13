# tech — 当前工作

- 目标版本：`0.1.0`；线上未发布，`onlineVersion=null`。
- 状态：B01 InProgress；B02–B15 未启动。
- 分支/工作区：`pipeline/tech` / `.worktrees/pipelines/tech`。
- 接收基线：`main e67a6f7`；通过 merge commit `05d829e` 同步，保留历史。旧 task 分支已退役，不重建。

## 当前工作项

`TECH-0.1.0-01`：维持 B01 同源核心，补齐最小真实 UE 启动与灰盒观察链。

- 稳定批次、效果资格和世界绑定已进入 main；旧 [B01 并入记录](B01-INTEGRATION-0.1.0.md) 是历史快照。
- 本批新增 `B01Bootstrap.umap`、默认地图/Cook 配置和 `ABootstrapGameMode`，装配单一 Director、相机与光源。Session 接受真实 BeginPlay 就绪回执后，Director 才显示 ready 文字。
- 空场景最初缺光，实际截图为黑屏；加入定向光后重新编译、打包和截图，不能以组件文字属性代替画面观察。
- 禁用本 Win64 工程不使用的 AndroidFileServer 插件，避免 Cook 自动写入无关网络配置及生成凭据。
- 写入边界以 [WRITE_SCOPE](../../governance/WRITE_SCOPE.md) 为准；`Scripts/Collaboration/**` 属 ops，tech 不写。

## 前批启动验证与复现入口

以下为技术交付执行记录，正式验收由 test 接收后在唯一 [evidence](../../evidence/evidence.md) 更新。路径相对 tech 工作区；构建、原生报告和截图是忽略的本地输出，不随 Git 自动传递。

- UE 5.6.1 / CL44394996；UBT 实际使用 MSVC 14.38.33145（工具集目录 14.38.33130）、SDK 10.0.22621.0；Editor 与 Game Development 编译成功。
- 既有 Editor 自动化 7/7，0 失败、0 未运行：`.build/b01-ue-20261113/index.json`。包含 6 个核心用例和世界绑定/旧回调隔离用例。
- 新真实游戏宿主用例 `UEgame.WorldScenario.B01.Bootstrap`，ClientContext + NonNullRHI：`.build/b01-light/index.json`，1/1；实际视点、字体/材质、单 Director、核心 Rescue 和回执均检查。
- 最终本地包：`Deliverables/B01-bootstrap-20261113-r4/Windows/UEgame.exe`；Build/Cook/Stage/Package/Archive 成功，日志 `.build/package-20261113-r4.log`。
- 最终归档包实际默认启动：`.build/b01-packaged-20261113-r4/index.json` 为 1/1、0 失败、0 未运行；日志退出码 0，D3D12。已目视检查包内 `Windows/UEgame/Saved/Screenshots/Windows/B01Bootstrap00000.png`，1280×720 图像完整显示 ready 文字。这不是物理显示呈现时刻的测量。
- 仓库检查 209 文档/681 链接通过，NarrativeText 7 条通过；`git diff --check` 通过。
- Linux 核心源未修改；沿用 main 已执行的 6/6，本批没有重跑 Linux，不将 UE 用例冒充 Linux 结果。

构建（引擎根 `G:/Epic Games/UE_5.6`）：

```powershell
$Engine = 'G:/Epic Games/UE_5.6'
& "$Engine/Engine/Build/BatchFiles/RunUAT.bat" BuildCookRun "-project=$PWD/UEgame.uproject" -noP4 -platform=Win64 -clientconfig=Development -build -cook -map=/Game/UEgame/Maps/B01Bootstrap -stage -pak -package -archive "-archivedirectory=$PWD/Deliverables/B01-bootstrap-local" -unattended -utf8output
```

运行归档后的根启动程序，不传地图覆盖，以验证默认入口：

```powershell
& ./Deliverables/B01-bootstrap-local/Windows/UEgame.exe -windowed -ResX=1280 -ResY=720 -unattended '-ExecCmds=Automation RunTests UEgame.WorldScenario.B01.Bootstrap;Quit' '-TestExit=Automation Test Queue Empty' "-ReportExportPath=$PWD/.build/b01-packaged-local" "-abslog=$PWD/.build/b01-packaged-local.log"
```

测试延迟截图写到包内 `Windows/UEgame/Saved/Screenshots/Windows/`；必须实际检查图像。测试通过只证明断言，不能自动证明画面或端到端延迟。

## 2026-11-13 灰盒观察链续作

- 实施者：本对话 tech agent；基线 `9f8e6a9`。写入限 `Source/UEgame/Private/ExperimentAdapter/`、`Source/UEgame/Private/Tests/Bootstrap/`、`Source/UEgame/UEgame.Build.cs` 与本文件。
- `-B01AftermathGraybox` 显式启用 7 个可见 Actor 标记：玩家存活、小孩预设死亡、5 个威胁存活。默认启动仍为原 ready 骨架。该 fixture 不实现 B05/B06/B07 的控制、战斗或感知。
- E 键触发后果观察；Subsystem 生成事实上下文，Session 返回 `RequestStableSnapshot`，适配层校验绑定并消费效果资格，Director 从已注册 Actor 读取稳定快照。同步快照回执保留时序，待当前效果批结束后交回 Session；核心提交 Ended 后依次执行 StopRescue/StopRun，关闭灰盒输入并显示结束提示。
- 执行入口仅向 Subsystem 开放；缺失/未注册标记、旧轮输入、重复观察不能制造结束。未实现 TimeSkip 不回报完成。叙事真源未改，所有 NAR 保持 draft。
- 首次 Editor 编译发现 TObjectPtr 范围遍历推导错误，修正后编译成功。独立审查发现效果执行入口过宽，已收为 private + friend；最终审查见下方验证记录。
- Editor 既有 7/7：`.build/b01-chain-editor/index.json`；真实 D3D12 游戏宿主灰盒 1/1：`.build/b01-chain-game/index.json`，两者无失败/未运行，日志测试退出码 0。新测试经 PlayerController 注入 E 事件，覆盖旧轮拒绝、缺失注册实体、终态去重和效果反馈；只证明软件输入路径，不代表实物键盘/手柄。
- 已目视 `Saved/Screenshots/WindowsEditor/B01Aftermath00000.png`：结束提示及 7 个标记清晰完整；截图不证明呈现延迟。
- 本批实际重跑 WSL Ubuntu/GCC 13.3 核心 6/6，0 失败；报告 `.build/core-linux-aftermath/ctest-junit.xml`。核心源码未修改。
- 新 Win64 Development 包 `Deliverables/B01-aftermath-20261113/Windows/UEgame.exe` 已 Build/Cook/Stage/Package/Archive 成功，退出码 0。初次 UBA 编译因内存阈值持续等待后停止，重试使用 `-UbtArgs="-NoUBA -MaxParallelActions=1"`；日志 `.build/package-aftermath-20261113-final.log`。
- 同一归档包灰盒观察 `.build/b01-aftermath-packaged/index.json` 与默认启动 `.build/b01-aftermath-default-packaged/index.json` 各 1/1，0 失败/未运行，日志测试退出码均为 0。包内 `Windows/UEgame/Saved/Screenshots/Windows/B01Aftermath00000.png` 和 `B01Bootstrap00000.png` 均已目视检查，标记和文字完整清晰。
- 运行包内灰盒：向该根启动程序传 `-B01AftermathGraybox`，手动按 E；自动化沿用上方测试参数并将用例改为 `UEgame.WorldScenario.B01.Aftermath`。不传开关则为原默认入口。构建身份位于包根 `BUILD-IDENTITY.json`。
- 仓库基线 209 文档/681 链接与 NarrativeText 7 条通过，diff 检查通过。独立 agent 已批准最终源码、宿主、Linux 及新包两项报告和截图，无剩余阻断项；批准仅限灰盒骨架，不替代正式验收。所有原生输出为本地忽略文件，正式 evidence 未修改。

复现真实游戏宿主（编译 UEgameEditor 后）：

```powershell
& 'G:/Epic Games/UE_5.6/Engine/Binaries/Win64/UnrealEditor.exe' "$PWD/UEgame.uproject" -game -B01AftermathGraybox -windowed -ResX=1280 -ResY=720 -unattended '-ExecCmds=Automation RunTests UEgame.WorldScenario.B01.Aftermath;Quit' '-TestExit=Automation Test Queue Empty' "-ReportExportPath=$PWD/.build/b01-chain-game" "-abslog=$PWD/.build/b01-chain-game.log"
```

## 2026-11-13 中文草稿显示探针续作

- 实施者：本对话 tech agent；基线 `83df86e`。写入范围为 `Source/UEgame/Private/ExperimentAdapter/BootstrapTextPreview.{h,cpp}`、`ExperimentWorldDirector.{h,cpp}`、`Source/UEgame/Private/Tests/Bootstrap/BootstrapTextChecks.cpp`、`BootstrapAftermathChecks.cpp`、`Source/UEgame/UEgame.Build.cs` 及本文件。
- 在 `-B01AftermathGraybox` 基础上另加 `-B01DraftTextPreview`，按 E 结束后显示 NAR-0004 的中文草稿预览；默认入口仍为 ready，单独灰盒开关仍不展示草稿。字幕探针带 ID/draft 标识，不代表 B10 媒体实现或内容送达，不向 Session 提交送达回执。
- 按文本契约直接读取唯一 `spec/contracts/narrative/TEXT.csv`，保留 id/speaker/scene/status；由 RuntimeDependencies 作为 UFS 文件随包提供，不另存正文副本。不接受缺失、重复、错误场景或非 draft 的 NAR-0004；EndPlay 移除视口控件。
- Editor Development 编译成功，日志 `.build/b01-draft-editor-build.log`；Editor 自动化 `.build/b01-draft-editor/index.json` 为 8/8（原 7 项与新文本源用例），真实 D3D12 游戏宿主 `.build/b01-draft-game/index.json` 为 1/1，均无失败/未运行，日志退出码 0。
- 新本地包 `Deliverables/B01-draft-text-20261113/Windows/UEgame.exe` 已 Build/Cook/Stage/Package/Archive 成功，退出码 0；最终日志 `.build/package-draft-text-20261113-final.log`。Editor 自动生成的无关 DefaultInput.ini 已移至 `.build/DefaultInput-editor-generated.ini`，随后重新打包，没有将该配置纳入源码或最终构建输入。
- 最终包灰盒 `.build/b01-draft-packaged/index.json`、默认入口 `.build/b01-draft-default-packaged/index.json`、文本解析 `.build/b01-draft-source-packaged/index.json` 各 1/1，0 失败/未运行，进程退出码 0。文本解析单独执行，不把一次命令中第二个未执行的 RunTests 算作通过。
- 已目视宿主 `Saved/Screenshots/WindowsEditor/B01Aftermath00001.png` 与包内 `Windows/UEgame/Saved/Screenshots/Windows/B01Aftermath00000.png`：中文、标点、ID/draft 完整可读；包内 `B01Bootstrap00000.png` 显示原 ready 入口。中文可见性来自截图目视检查，自动化只证明解析和原灰盒断言，不证明中文字形像素或媒体送达。
- 核心源码未改；Linux 沿用前批实际 6/6，本轮未重跑。仓库 209 文档/681 链接、NarrativeText 7 条、diff 检查通过。独立 agent 已批准源码、实际报告/截图和本节说明，无阻断项；最终提交后运行 PipelineScope。原生报告、截图及 BUILD-IDENTITY.json 均为忽略的本地输出。

复现中文探针：向上述新包根程序同时传 `-B01AftermathGraybox -B01DraftTextPreview -windowed -ResX=1280 -ResY=720`，按 E。自动化复用灰盒命令，报告目录使用 `.build/b01-draft-packaged`；文本解析用例为 `UEgame.B01.DraftTextSource`，须单独启动一次执行。

## 未满足项与下一步

- 本批新增可选灰盒死亡后果观察链；真实角色感知/战斗与成功路线尚未实现，不能将预设死亡 Marker 当作真实战斗结果，B01 不得结清。
- 输入至画面实际呈现 <50ms、本人体验、真实手柄未执行。
- NAR-0001–0007 仍 draft；NAR-0004 单条中文显示探针已有技术执行和截图，正式字幕/媒体送达及其余条目未验证，不改 final，不启动 B13。下一步由 test 接收技术结果；narrative 独立决定文本定稿。
- test 尚未接收本批；本地报告不能替代独立验收和远端 CI。后续按 tech → test → ops → main 正向交付，不直接修改证据或发布状态。
- 本批未推送、未发 MR、未发布、未创建 tag；main 未修改。

## MR

上游：`growth`、`narrative`、`art`；下游：`art`、`test`。

- 本批五项 PR 正文与本机接收入口：[TEST-HANDOFF-0.1.0.md](TEST-HANDOFF-0.1.0.md)。用户选择本机接收；复用完整包，不改写构建身份。
- 产品候选保持 `fb2861e`；交接文档提交与产品构建身份分开记录。尚未推送或发送 MR，test 尚未接收。
