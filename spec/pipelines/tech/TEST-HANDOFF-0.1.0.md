## 目标版本与工作项

- 版本：0.1.0
- 工作项：TECH-0.1.0-01 / B01，剑术回访中群众和教师两段关键字幕的实际呈现与完成判定。

## 交付内容

- 产品提交 `eb00d4bf21c173fbac49dec98565ce0e5d519856`。本机 Win64 Development 完整包位于 `G:/UEgame/.worktrees/pipelines/tech/Deliverables/B01-town-dialogue-20261121/`，构建身份见包根目录 BUILD-IDENTITY.json。
- 传入 `-B01RescueGraybox`。按六次 E 完成救援并进入镇子。从中心按四次 A 接近群众，或四次 D 接近教师。从群众位置按八次 D 可到教师面前。全文显示期间可离开，返回后重新计时。两段均送达后结束回访。
- 剑术分支使用 `NAR-0001` 与 `NAR-0005`。当前显示身份为 draft / TextFallback，本批包未绑定语音。群众需近距，教师还需屏内且无遮挡。回执取自本轮有效视口中全文连续绘制的时段。

## 接收条件

test 核对源码身份和候选 CI，从同一归档包运行以下八项用例。保持游戏窗口和视口有焦点，关闭控制台及菜单。目视新生成的 B01TownCrowd、B01TownTeacher 两张截图，检查说话者、完整中文和标点，并核对日志中群众首次中断、重返后送达、教师送达的顺序。按既有范围复核 Bootstrap、Aftermath、RescueSuccess、Revisit 四张截图，再在唯一 evidence 原地登记本轮结论。由 test 执行接收合并。

```powershell
$ErrorActionPreference = 'Stop'
$candidateExe = 'G:/UEgame/.worktrees/pipelines/tech/Deliverables/B01-town-dialogue-20261121/Windows/UEgame.exe'
$reportRoot = Join-Path $PWD ('.build/b01-town-dialogue-receive-' + (Get-Date -Format 'yyyyMMdd-HHmmss'))
New-Item -ItemType Directory -Path $reportRoot -ErrorAction Stop | Out-Null
$cases = @(
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
    $process = Start-Process -FilePath $candidateExe -ArgumentList $runArgs -WindowStyle Hidden -Wait -PassThru
    if ($process.ExitCode -ne 0) { throw ('Process failed: ' + $case.Name) }
    Write-Output ('ProcessExit=' + $process.ExitCode + '; Case=' + $case.Name)
    $report = Get-Content (Join-Path $caseReport 'index.json') -Raw | ConvertFrom-Json
    if (@($report.tests).Count -ne 1 -or $report.tests[0].fullTestPath -ne $case.Test -or $report.tests[0].state -ne 'Success') { throw ('Unexpected case: ' + $case.Name) }
    if ($report.succeeded -ne 1 -or $report.failed -ne 0 -or $report.notRun -ne 0 -or $report.inProcess -ne 0 -or $report.succeededWithWarnings -ne 0) {
        throw ('Report requires investigation: ' + $case.Name)
    }
    if ($report.tests[0].errors -ne 0 -or $report.tests[0].warnings -ne 0 -or !(Select-String -LiteralPath ($caseReport + '.log') -SimpleMatch 'TEST COMPLETE. EXIT CODE: 0')) { throw ('Inspect native logs: ' + $case.Name) }
    if (!(Select-String -LiteralPath ($caseReport + '.log') -Pattern 'LogRHI: Loading RHI module D3D12RHI')) { throw ('Missing D3D12 evidence: ' + $case.Name) }
    Write-Output ('Passed=' + $case.Test)
}
```

## 最新验证

- 从干净产品提交构建 Win64 Development，Build、Cook、Stage、Package、Archive 均完成，AutomationTool 退出码 0，日志 `.build/package-town-dialogue-20261121.log`。归档程序独立启动八次，八项 D3D12 各 1/1 Success，进程与测试退出码均为 0，逐例 warnings/errors 为 0。原生 JSON 与日志在 `.build/town-packaged-1121/`。八份日志均报告 HTML 模板缺失。
- 归档包截图在 `Windows/UEgame/Saved/Screenshots/Windows/`。两段完整字幕分别见 B01TownCrowd00000.png、B01TownTeacher00000.png。TownDialogue 日志记录群众第 1 代中断、第 2 代送达、教师第 3 代送达。
- WSL Ubuntu / GCC 13.3 实跑核心 8/8（`.build/core-content-1121/ctest-junit.xml`）。Editor Development 编译成功，D3D12 游戏宿主两项各 1/1（`.build/town-editor-r2/`）。独立审查者 independent_review 批准功能提交。详见 tech 的 CURRENT。

- 独立审查者 independent_review 另行核对本包构建身份、八份原生报告和运行日志，并目视全部六张包内截图。

## 未满足项

- test 需独立复测并更新 `spec/evidence/evidence.md`。DISPLAY-01 正式首批还包括 NAR-0002、NAR-0003。剧情定稿归 narrative，正式语音素材制作与导入分属 art、tech，正常有声路径及听辨待 test 验证。
- 魔法与混用回访目前仅有核心规则覆盖。完整战斗与导航、火湿和破坏后果、世界卸载恢复、实物输入、输入到对应画面实际呈现小于 50 ms、本人因果体验仍待验证。目标版本尚未发布。
