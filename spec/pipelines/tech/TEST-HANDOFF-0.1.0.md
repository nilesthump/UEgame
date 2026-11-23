## 目标版本与工作项

- 版本：0.1.0
- 工作项：TECH-0.1.0-01 / B01，Win64 回访字幕自动化用例在游戏窗口隐藏时的启动恢复。跟进 [#49 诊断](https://github.com/nilesthump/UEgame/pull/49)。

## 交付内容

- 产品提交 `a35d3f09ec5eeadc8a0ce8f87ede6df037f76e88`；本机 Win64 Development 归档 `G:/UEgame/.worktrees/pipelines/tech/Deliverables/B01-hidden-startup-20261123/`，身份见包根 BUILD-IDENTITY.json。
- 剑术、魔法及混用的三项回访字幕用例在开始前检查本进程游戏视口的原生窗口。已隐藏且未最小化时，恢复同一窗口一次，核对可见性后继续。该逻辑仅在 Win64 开发自动化入口执行。群众中断重播、教师靠近条件及连续绘制送达规则沿用 #49。

## 接收条件

从 test 工作树在同一包串行运行。先执行受控隐藏启动夹具。夹具通过根启动器启动游戏，在用例开始前隐藏游戏子进程的 1280×720 UnrealWindow。核对 window.txt 中同一 HWND 隐藏后重新可见、LauncherExit=0；在报告与游戏日志中核对恢复记录、群众 NAR-0002 中断再送达、教师 NAR-0006 送达、D3D12 和测试退出 0。将采样与本次新生成的两张截图一起复核。夹具保存在 tech 工作树，可在接收前调用：

```powershell
$tech = 'G:/UEgame/.worktrees/pipelines/tech'
$archive = Join-Path $tech 'Deliverables/B01-hidden-startup-20261123'
& (Join-Path $tech 'Tests/Bootstrap/Test-HiddenTownStartup.ps1') -Archive $archive -ReportDirectory (Join-Path $PWD ('.build/hidden-startup-' + (Get-Date -Format 'yyyyMMdd-HHmmss')))
```

按以下入口做十项正常窗口回归，核对原生 JSON/HTML 及新生成的十张截图。最后按 #49 的最小化负例，在独立运行的魔法用例首次显示 NAR-0002 后最小化并保持至结束，记录时刻、窗口状态、超时和回执。该项应失败并且没有送达。由 test 在唯一 evidence 原地维护结论。

```powershell
$ErrorActionPreference = 'Stop'
$candidateExe = 'G:/UEgame/.worktrees/pipelines/tech/Deliverables/B01-hidden-startup-20261123/Windows/UEgame/Binaries/Win64/UEgame.exe'
$reportRoot = Join-Path $PWD ('.build/b01-hidden-receive-' + (Get-Date -Format 'yyyyMMdd-HHmmss'))
New-Item -ItemType Directory -Path $reportRoot -ErrorAction Stop | Out-Null
$cases = @(
    @{ Name='magic'; Test='UEgame.WorldScenario.B01.MagicTownDialogue'; Flags=@('-B01RescueGraybox') },
    @{ Name='mixed'; Test='UEgame.WorldScenario.B01.MixedTownDialogue'; Flags=@('-B01RescueGraybox') },
    @{ Name='town-dialogue'; Test='UEgame.WorldScenario.B01.TownDialogue'; Flags=@('-B01RescueGraybox') },
    @{ Name='revisit'; Test='UEgame.WorldScenario.B01.Revisit'; Flags=@('-B01RescueGraybox') },
    @{ Name='rescue-success'; Test='UEgame.WorldScenario.B01.RescueSuccess'; Flags=@('-B01RescueGraybox') },
    @{ Name='rescue-teardown'; Test='UEgame.WorldScenario.B01.RescueTeardown'; Flags=@('-B01AftermathGraybox','-B01DraftTextPreview') },
    @{ Name='lifecycle'; Test='UEgame.WorldScenario.B01.Lifecycle'; Flags=@('-B01AftermathGraybox','-B01DraftTextPreview') },
    @{ Name='default'; Test='UEgame.WorldScenario.B01.Bootstrap'; Flags=@() },
    @{ Name='aftermath'; Test='UEgame.WorldScenario.B01.Aftermath'; Flags=@('-B01AftermathGraybox','-B01DraftTextPreview') },
    @{ Name='text-source'; Test='UEgame.B01.DraftTextSource'; Flags=@() }
)
foreach ($case in $cases) {
    $caseReport = Join-Path $reportRoot $case.Name
    $runArgs = @('-windowed','-ResX=1280','-ResY=720','-unattended','-d3d12') + $case.Flags + @(
        ('-ExecCmds="Automation RunTests ' + $case.Test + ';Quit"'),
        '-TestExit="Automation Test Queue Empty"',
        ('-ReportExportPath="' + $caseReport + '"'),
        ('-abslog="' + $caseReport + '.log"')
    )
    $process = Start-Process -FilePath $candidateExe -ArgumentList $runArgs -WindowStyle Normal -Wait -PassThru
    if ($process.ExitCode -ne 0) { throw ('Process failed: ' + $case.Name) }
    Write-Output ('ProcessExit=' + $process.ExitCode + '; Case=' + $case.Name)
    $report = Get-Content (Join-Path $caseReport 'index.json') -Raw | ConvertFrom-Json
    if (@($report.tests).Count -ne 1 -or $report.tests[0].fullTestPath -ne $case.Test -or $report.tests[0].state -ne 'Success') { throw ('Unexpected case: ' + $case.Name) }
    if ($report.succeeded -ne 1 -or $report.failed -ne 0 -or $report.notRun -ne 0 -or $report.inProcess -ne 0 -or $report.succeededWithWarnings -ne 0) {
        throw ('Report requires investigation: ' + $case.Name)
    }
    if ($report.tests[0].errors -ne 0 -or $report.tests[0].warnings -ne 0 -or !(Select-String -LiteralPath ($caseReport + '.log') -SimpleMatch 'TEST COMPLETE. EXIT CODE: 0')) { throw ('Inspect native logs: ' + $case.Name) }
    if (!(Select-String -LiteralPath ($caseReport + '.log') -Pattern 'LogRHI: Loading RHI module D3D12RHI')) { throw ('Missing D3D12 evidence: ' + $case.Name) }
    if ((Get-Item -LiteralPath (Join-Path $caseReport 'index.html') -ErrorAction Stop).Length -eq 0) { throw ('Empty HTML report: ' + $case.Name) }
    if (!(Select-String -LiteralPath ($caseReport + '.log') -SimpleMatch 'Successfully wrote html results file!')) { throw ('Missing HTML export confirmation: ' + $case.Name) }
    Write-Output ('Passed=' + $case.Test)
}
```

## 最新验证

- 原始旧包按 11 月 22 日命令自然重放 1/1，#49 诊断包相同条件 9/9。启动器的 Hidden 参数单独未复现。旧包运行中实际隐藏游戏窗口 25 秒，首错为 crowd subtitle missing，恢复后两段均送达。#49 包启动前隐藏实际窗口后，原生用例重现五条超时，进程 255，测试 -1。临时探针在玩家距群众 80 cm 时记录 windowActive=0、viewportFocused=1、字幕为空，旧包受控实验在恢复窗口后送达了群众和教师两段。报告目录见 tech/CURRENT。
- 同一受控启动操作在修后检验包实跑 1/1。独立审查者 review 核对修前修后执行、原生及外部窗口记录和运行中最小化负例，批准测试入口修复。Linux GCC 13.3 本地 8/8。
- 干净提交 a35d3f0 的 Build/Cook/Stage/Package/Archive 与 UAT 退出码 0 见 `.build/package-hidden-startup-1123.log`。同包隐藏启动夹具 1/1，外部采样确认原 HWND 395078 从隐藏恢复可见，无第二匹配窗口；游戏报告恢复记录和两份送达，启动器和测试退出码均为 0。见 `.build/startup-archive-green-1123/`。
- 正常窗口 D3D12 十次独立启动、每次 1/1，进程和测试退出码均为 0，各无警告或错误；十份原生 JSON/HTML 完整，见 `.build/startup-archive-suite-1123/`。包内成功截图在 Windows/UEgame/Saved/Screenshots/Windows/；隐藏启动的魔法两图为 00000，正常窗口魔法两图为 00001，其余正常回归八图为 00000。
- 运行中最小化单列为一项预期失败。Win32 确认 IsIconic=True；两个超时中 windowMinimized=1、当前字幕分别为 NAR-0002/0006，未产生送达回执。进程 255，测试退出 -1，见 `.build/during-min-archive-1123/` 和 `.build/during-min-archive-actions-1123.txt`。

- 独立审查者 review 核对本包身份、UAT、全部正负例报告与日志、同一窗口的状态记录，并逐张目视隐藏启动两图和正常回归十图，批准本批交付。

## 未满足项

- 11 月 22 日历史首败缺少当时的窗口采样，本轮确定了一种充分触发条件。另一次修前对照的原生报告为 Success，外部未观察到所选 HWND 再次可见，测试夹具判失败。运行中隐藏时，仍须将游戏端实际视口 HWND 与外部窗口和绘制时间逐帧关联，以验明呈现有效性。
- 待 test 独立接收。B01 InProgress，DISPLAY-01 仍按 test 记录为 Partial。正式声音、实物输入、输入到对应画面实际呈现小于 50 ms、本人试玩、完整战斗与导航、环境后果及世界卸载恢复仍待验证；目标版本未发布。
