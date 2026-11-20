## 目标版本与工作项

- 版本：0.1.0
- 工作项：TECH-0.1.0-01 / B01，剑术救援成功后的环境结算、三十年回访标记布置与控制恢复。

## 交付内容

- 产品源码 `85bb5691eae059255e356a762c33a45c22abe121`。同机 Win64 Development 包位于 `G:/UEgame/.worktrees/pipelines/tech/Deliverables/B01-revisit-20261120/`。BUILD-IDENTITY.json 记录源码、引擎、构建日志和用例报告。
- 传入 `-B01RescueGraybox` 启动默认地图。按一次 E 发现遇险，再按五次 E 完成剑术救援。过渡遮挡旧现场后消费旧时代第 180、300 秒的雨计划，布置镇上玩家、剑术教师与学生，再淡入。按 D 向画面右侧移动，按 A 返回。
- 回访用例在教师暂时不可用时检查遮罩和输入门槛，再恢复组件，核对回访计时起点、旧现场退役及 A/D 的屏幕方向。核心用例独立核对三项就绪的六种顺序和失效回执。

## 接收条件

test 核对构建身份和本候选 CI，从同一个新包按以下命令中的七项分别启动，保留每次新生成的 JSON 和日志，核对实际设备为 D3D12。目视本次新生成的 Bootstrap、Aftermath、RescueSuccess、Revisit 四张截图，分别检查启动、失败草稿、第五击过渡和回访的角色及控制提示。通过后以 merge commit 接收，更新唯一 evidence 并按协议交付 ops。

```powershell
$ErrorActionPreference = 'Stop'
$candidateExe = 'G:/UEgame/.worktrees/pipelines/tech/Deliverables/B01-revisit-20261120/Windows/UEgame.exe'
$reportRoot = Join-Path $PWD ('.build/b01-revisit-receive-' + (Get-Date -Format 'yyyyMMdd-HHmmss'))
New-Item -ItemType Directory -Path $reportRoot -ErrorAction Stop | Out-Null
$cases = @(
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

- 从干净的产品提交构建 Win64 Development。Build、Cook、Stage、Package、Archive 成功，AutomationTool 退出码 0，日志 `.build/package-revisit-20261120.log`。
- 从归档根程序七次 D3D12 启动，每项 1/1 Success，逐例 warnings/errors 为 0，进程及日志退出码均为 0。七份原生 JSON 和同名日志见 `.build/b01-revisit-packaged/`，七份日志均报告 HTML 模板缺失。四张 00000 截图在包内 `Windows/UEgame/Saved/Screenshots/Windows/`，已目视启动提示、失败草稿、第五击的渐变过渡、三十年后回访角色与操作提示。
- Linux/GCC 13.3 核心 7/7。UE Editor Development 编译成功，D3D12 游戏宿主 Revisit 与 RescueSuccess 各 1/1。执行路径和修复情况见本管线 CURRENT。
- 独立审查者 review 已批准功能提交，并核对构建身份、UAT 日志、七份原生报告与日志、四张截图和交接命令，批准本批灰盒候选交付 test。

## 未满足项

- test 待独立复测，当前候选 CI 以原 PR 为准。B01 仍在实施，目标 0.1.0 未发布。
- 本批剑术灰盒只结算两个固定雨事件。完整火湿和破坏痕迹、真实战斗和导航、世界卸载与恢复、触发并实际听到群众和教师的关键内容、正式声音与字幕仍待对应管线集成。
- 回访使用屏幕上的角色标记验证 A/D 移动，自动化输入来自软件。实物手柄、本人试玩及输入至对应画面实际呈现小于 50 ms 均待实测。
