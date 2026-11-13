## 目标版本与工作项

- 版本：0.1.0
- 工作项：TECH-0.1.0-01 / B01 真实生命周期与旧轮隔离的 Win64 独立包验证。
- 状态：新候选已完成本机技术验证，已推送并发起 [PR #35](https://github.com/nilesthump/UEgame/pull/35)，test 尚未接收；B01 InProgress。上批 PR #33 已接收，其历史由 Git 保留。本页作为新批次五项 PR 正文与本机接收入口。

## 交付内容

- `6d7cd2e`：同步上批 PR #33 已接收状态。
- `74772e2`：新增 `UEgame.WorldScenario.B01.Lifecycle`，在同一真实 UWorld 内连续三轮终态 Actor 销毁重建，检查 Marker 清理、新 Run/ready、旧 ready/观察/解绑隔离，最后通过 PlayerController 注入 E 验证重建输入与结束反馈。
- 本轮构建源码：`74772e2569360880bebc337a74a36a35c6ce367f`，构建开始及归档结束时 Git 工作区干净；本次仅补交接文档，不改变产品源码。生产逻辑沿用原灰盒，新增用例不实现地图切换或持久恢复。
- 新完整包：`Deliverables/B01-lifecycle-20261113/`；包根 `BUILD-IDENTITY.json` 记录源码提交、UE 5.6.1 CL44394996、Win64 Development、地图、构建日志及报告位置。旧 `B01-draft-text-20261113` 包保留。

## 接收条件

- 本机接收沿用用户已选方式。技术工作区根 `G:/UEgame/.worktrees/pipelines/tech`；本页执行输出路径相对此根。本地包及报告为忽略文件，不随 Git/MR 自动交付；跨机器须提供完整包与对应输出，不能只复制根启动程序。
- test 先读 AGENTS 并恢复自身管线，再核对新候选、独立审查及对应 CI。满足接收条件后用 merge commit 接收；正式验收只写 test 的唯一 evidence，问题回到该批 MR。
- 分别启动根 `Windows/UEgame.exe` 四次，不传地图覆盖；每次使用新报告目录，核对精确用例名称、非零用例数、逐例 Success 与进程/日志退出码。Lifecycle 和 Aftermath 同时传两个探针开关，默认入口与文本解析不传。

```powershell
$ErrorActionPreference = 'Stop'
$candidateExe = 'G:/UEgame/.worktrees/pipelines/tech/Deliverables/B01-lifecycle-20261113/Windows/UEgame.exe'
$reportRoot = Join-Path $PWD ('.build/b01-lifecycle-receive-' + (Get-Date -Format 'yyyyMMdd-HHmmss'))
New-Item -ItemType Directory -Path $reportRoot -ErrorAction Stop | Out-Null
$cases = @(
    @{ Name='lifecycle'; Test='UEgame.WorldScenario.B01.Lifecycle'; Flags=@('-B01AftermathGraybox','-B01DraftTextPreview') },
    @{ Name='default'; Test='UEgame.WorldScenario.B01.Bootstrap'; Flags=@() },
    @{ Name='aftermath'; Test='UEgame.WorldScenario.B01.Aftermath'; Flags=@('-B01AftermathGraybox','-B01DraftTextPreview') },
    @{ Name='text-source'; Test='UEgame.B01.DraftTextSource'; Flags=@() }
)
foreach ($case in $cases) {
    $caseReport = Join-Path $reportRoot $case.Name
    $runArgs = @('-windowed','-ResX=1280','-ResY=720','-unattended') + $case.Flags + @(
        ('-ExecCmds="Automation RunTests ' + $case.Test + ';Quit"'),
        '-TestExit="Automation Test Queue Empty"',
        ('-ReportExportPath="' + $caseReport + '"'),
        ('-abslog="' + $caseReport + '.log"')
    )
    $process = Start-Process -FilePath $candidateExe -ArgumentList $runArgs -WindowStyle Hidden -Wait -PassThru
    if ($process.ExitCode -ne 0) { throw ('Process failed: ' + $case.Name) }
    Write-Output ('ProcessExit=' + $process.ExitCode + '; Case=' + $case.Name)
    $report = Get-Content (Join-Path $caseReport 'index.json') -Raw | ConvertFrom-Json
    if (@($report.tests).Count -ne 1 -or $report.tests[0].fullTestPath -ne $case.Test -or $report.tests[0].state -ne 'Success') { throw ('Unexpected case: ' + $case.Name) }
    if ($report.succeeded -ne 1 -or $report.failed -ne 0 -or $report.notRun -ne 0 -or $report.inProcess -ne 0 -or $report.succeededWithWarnings -ne 0) {
        throw ('Report requires investigation: ' + $case.Name)
    }
    Write-Output ('Passed=' + $case.Test)
}
```

- 对应日志还须确认 `TEST COMPLETE. EXIT CODE: 0`，检查 errors/warnings 与其他错误。当前包缺 HTML 报告模板，日志会报该错误；接收依据为完整原生 JSON、日志及实际截图，不能声称日志没有错误。
- 新截图写入包内 `Windows/UEgame/Saved/Screenshots/Windows/`。本次技术验证 `B01Bootstrap00000.png` 与 `B01Aftermath00000.png` 保留；test 必须检查其复测产生的新编号截图。Lifecycle 本身不生成截图，不可用 Aftermath 截图证明生命周期中的 Slate 清理或呈现。

## 最新验证

- Build/Cook/Stage/Package/Archive 全部实际成功，进程与 AutomationTool 退出码 0：`.build/package-lifecycle-20261113.log`。实际 MSVC 14.38.33145、SDK 10.0.22621.0，使用 `-UbtArgs="-NoUBA -MaxParallelActions=1"`。
- 本轮技术运行：2026-11-13 17:46–17:47（Asia/Shanghai），新包四次启动均 D3D12。报告根 `.build/b01-lifecycle-packaged/`；`lifecycle/index.json`、`default/index.json`、`aftermath/index.json`、`text-source/index.json` 各 1/1 Success，failed/notRun/inProcess/succeededWithWarnings 均为 0，逐例 errors/warnings=0；四个进程与对应同名日志测试退出码均为 0。
- 已目视该新包的 `B01Bootstrap00000.png`：默认 ready；`B01Aftermath00000.png`：结束提示、七个标记、中文标点和 NAR-0004/draft 完整可读。截图只证明该画面，不证明端到端延迟。
- 四份运行日志均有 HTML 模板缺失错误，未生成 HTML；原生 JSON 和日志完整。不得以报告逐例零错误宣称日志无错误。
- Editor 宿主 Lifecycle 1/1 是上轮已执行结果 `.build/b01-lifecycle-game/index.json`，本轮未重跑；Linux 核心未改，本轮未重跑。上批 CI 只对应旧候选，不证明新候选 CI。

- 本轮 RepositoryBaseline 209 文档/681 链接、NarrativeText 7 条、diff、五项正文结构及接收命令 PowerShell 语法解析通过。命令执行正文与本次四用例脚本一致，仅复测报告根改为新时间戳目录。

- 独立 agent `/root/package_review` 已核对 `74772e2` 用例、实际构建日志、BUILD-IDENTITY、四份精确报告/日志、两张新包截图及本轮文档 diff，批准受限本机候选交接，无阻断项；未代替 test 验收或远端 CI。

## 未满足项

- tech：新候选已推送，PR #35 的 CI 与接收状态以原 PR 为准；对应头提交的 CI 实际执行结果记在 PR 正文，不以旧头结果替代。本机包仍为源码 `74772e2`，后续交接文档不改变构建身份。
- test：新包尚未独立接收，完整 B01 未验收；上批 PR #33 的受限结论不自动覆盖新包。
- tech/test：不覆盖未结束 Rescue 卸载、GC 后回调、地图切换、Slate 控件残留、持久恢复、真实玩法/成功路线、正式媒体、端到端 <50ms、本人体验或实物输入。软件注入 E 不冒充实物测试。
- narrative：全部 NAR 仍 draft；不改文本定稿、不启动 B13。B01 保持 InProgress，B02–B15 不启动，onlineVersion=null；不发布、不创建 tag。
