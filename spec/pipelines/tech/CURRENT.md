# tech — 当前工作

- 目标版本：`0.1.0`；线上未发布，`onlineVersion=null`。
- 状态：B01 InProgress；B02–B15 未启动。
- 分支/工作区：`pipeline/tech` / `.worktrees/pipelines/tech`。
- 接收基线：`main e67a6f7`；通过 merge commit `05d829e` 同步，保留历史。旧 task 分支已退役，不重建。

## 当前工作项

`TECH-0.1.0-01`：维持 B01 同源核心，补齐最小真实 UE 启动路径。

- 稳定批次、效果资格和世界绑定已进入 main；旧 [B01 并入记录](B01-INTEGRATION-0.1.0.md) 是历史快照。
- 本批新增 `B01Bootstrap.umap`、默认地图/Cook 配置和 `ABootstrapGameMode`，装配单一 Director、相机与光源。Session 接受真实 BeginPlay 就绪回执后，Director 才显示 ready 文字。
- 空场景最初缺光，实际截图为黑屏；加入定向光后重新编译、打包和截图，不能以组件文字属性代替画面观察。
- 禁用本 Win64 工程不使用的 AndroidFileServer 插件，避免 Cook 自动写入无关网络配置及生成凭据。
- 写入边界以 [WRITE_SCOPE](../../governance/WRITE_SCOPE.md) 为准；`Scripts/Collaboration/**` 属 ops，tech 不写。

## 本批验证与复现入口

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

## 未满足项与下一步

- 本批仅是启动/WorldReady 骨架；完整“输入/事实→核心→类型化效果→实际世界回执→对应画面”交互链尚未打通，B01 不得结清。
- 输入至画面实际呈现 <50ms、本人体验、真实手柄未执行。
- NAR-0001–0007 仍 draft；少量中文文本/字幕显示与接入 NotRun，不改 final，不启动 B13。
- test 尚未接收本批；本地报告不能替代独立验收和远端 CI。后续按 tech → test → ops → main 正向交付，不直接修改证据或发布状态。
- 本批未推送、未发 MR、未发布、未创建 tag；main 未修改。

## MR

上游：`growth`、`narrative`、`art`；下游：`art`、`test`。
