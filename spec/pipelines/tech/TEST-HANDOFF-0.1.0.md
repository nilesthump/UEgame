## 目标版本与工作项

- 版本：0.1.0
- 工作项：TECH-0.1.0-01 / B01，跟进 [#51 的接收记录](https://github.com/nilesthump/UEgame/pull/51)，核验 Win64 回访时隐藏游戏视口对字幕送达的影响。

## 交付内容

运行时从游戏视口取得原生窗口，在字幕绘制时核对 Slate 绘制目标、Win32 可见性和最小化状态。窗口失去呈现资格会中断该段计时。恢复后，回执需要同一窗口重新积满该条字幕的阅读时长。可选参数 `-B01TraceTownViewport` 记录帧号、事件采样 UTC、窗口、字幕代号、绘制资格和时长。

## 接收条件

从 test 工作树串行运行同一个 Win64 Development 包。核对包根 BUILD-IDENTITY.json 所列的 tech 提交，再运行两种窗口回归。运行中隐藏用例应在群众字幕第二次显示后将原 HWND 隐藏七秒，期间无合格绘制和回执。恢复后第一次合格绘制从零开始，群众和教师各获得一次回执。隐藏启动用例继续要求同一窗口恢复可见。保存每次的 window.txt、game.log 和原生报告供复核。

```powershell
$tech = 'G:/UEgame/.worktrees/pipelines/tech'
$archive = Join-Path $tech 'Deliverables/B01-town-viewport-20261123'
$fixture = Join-Path $tech 'Tests/Bootstrap/Test-HiddenTownStartup.ps1'
$batch = Get-Date -Format yyyyMMdd-HHmmss
& $fixture -Archive $archive -ReportDirectory ".build/runtime-hide-$batch" -DuringDialogue
& $fixture -Archive $archive -ReportDirectory ".build/startup-hide-$batch"
```

十项正常窗口回归沿用下列入口。核对 D3D12、逐项报告和本轮新生成的截图。

```powershell
$ErrorActionPreference = 'Stop'
$candidateExe = 'G:/UEgame/.worktrees/pipelines/tech/Deliverables/B01-town-viewport-20261123/Windows/UEgame/Binaries/Win64/UEgame.exe'
$reportRoot = Join-Path $PWD ('.build/b01-viewport-receive-' + (Get-Date -Format 'yyyyMMdd-HHmmss'))
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

- 上批产品 a35d3f0 本轮对照中，外部采样显示窗口隐藏七秒，恢复前无群众回执，恢复后生成第三代并完成送达。见 `.build/runtime-hide-baseline-actions.txt` 和对应游戏日志。#51 所述旧包仍缺实际视口的同帧标识，其历史原因尚未证实。
- 修改后的 Win64 Development 检验包完成 Build/Cook/Stage/Package/Archive。D3D12 魔法用例 1/1。外部和游戏均记录 HWND 526092，隐藏前第二代已有 1.304 秒，7.039 秒隐藏区间内 211 个游戏更新样本均无合格绘制或回执。恢复后群众第三代从零累计到 5.858 秒（门槛 5.833 秒），教师第四代累计 5.668 秒（门槛 5.667 秒），两份回执与业务日志一致。见 `.build/runtime-visibility-check2/`。
- 第一版诊断因大量日志延迟写出而超时。夹具现以事件发生时采样的 UTC 与外部观测对齐，并在本项诊断中限帧到 30。Linux 核心构建后 8/8，JUnit 见 `.build/core-routes-1122/runtime-visibility.xml`。干净提交归档的身份和同包复测见本批 tech → test PR 的最新验证。

## 未满足项

由 test 独立复测并原地维护 DISPLAY-01。上述记录覆盖 Win64 原生窗口可见性和 Slate 合格绘制，物理显示时刻、端到端 50 ms、实物输入、正式语音和本人试玩仍待验证。B01 实施中，0.1.0 尚未发布。
