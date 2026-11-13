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

## 未满足项与下一步

- 本批新增可选灰盒死亡后果观察链；真实角色感知/战斗与成功路线尚未实现，不能将预设死亡 Marker 当作真实战斗结果，B01 不得结清。
- 输入至画面实际呈现 <50ms、本人体验、真实手柄未执行。
- NAR-0001–0007 仍 draft；少量中文文本/字幕显示与接入 NotRun，不改 final，不启动 B13。
- test 尚未接收本批；本地报告不能替代独立验收和远端 CI。后续按 tech → test → ops → main 正向交付，不直接修改证据或发布状态。
- 本批未推送、未发 MR、未发布、未创建 tag；main 未修改。

## MR

上游：`growth`、`narrative`、`art`；下游：`art`、`test`。
