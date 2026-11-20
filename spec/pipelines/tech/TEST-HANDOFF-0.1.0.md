## 目标版本与工作项

- 版本：0.1.0
- 工作项：TECH-0.1.0-01 / B01 剑术救援灰盒，从发现遇险到进入 TimeSkip。

## 交付内容

- 产品源码 `c7872c3790996d17e499b86b5b46cc0d7ece3c3a`。本地 Win64 Development 包位于 `G:/UEgame/.worktrees/pipelines/tech/Deliverables/B01-rescue-success-20261120/`。包内 BUILD-IDENTITY.json 关联源码、引擎、构建日志和逐例报告。
- 以 `-B01RescueGraybox` 启动包的 `Windows/UEgame.exe`。第一次按 E 发现遇险，随后按五次 E，每次剑击击败一个威胁标记。前四击仍在救援，第五击后核心锁定剑术方式并关闭救援输入。画面应显示等待环境结算、玩家和小孩存活、五个威胁死亡。
- 相对 test 上次接收的 #37，随 tech 历史带入 #38、#40 及 `518b3b9` 对 DISPLAY-01 草稿显示范围的说明。七条文本均为 draft，B13 素材制作仍受限。

## 接收条件

test 在自己的固定工作树核对构建身份、独立审查及当前候选 CI，使用同机完整包，按下面的命令从默认地图分别启动六次。每次都应生成新的原生 JSON 和日志。另目视本轮新生成的 Bootstrap、Aftermath 和 RescueSuccess 三张图，检查 ready、草稿及结束提示、救援结算等待提示与标记状态。用 merge commit 接收后，test 在唯一 evidence 更新本批实际结果。

```powershell
$ErrorActionPreference = 'Stop'
$candidateExe = 'G:/UEgame/.worktrees/pipelines/tech/Deliverables/B01-rescue-success-20261120/Windows/UEgame.exe'
$reportRoot = Join-Path $PWD ('.build/b01-rescue-success-receive-' + (Get-Date -Format 'yyyyMMdd-HHmmss'))
New-Item -ItemType Directory -Path $reportRoot -ErrorAction Stop | Out-Null
$cases = @(
    @{ Name='rescue-success'; Test='UEgame.WorldScenario.B01.RescueSuccess'; Flags=@('-B01RescueGraybox') },
    @{ Name='rescue-teardown'; Test='UEgame.WorldScenario.B01.RescueTeardown'; Flags=@('-B01AftermathGraybox','-B01DraftTextPreview') },
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
    if ($report.tests[0].errors -ne 0 -or $report.tests[0].warnings -ne 0 -or !(Select-String -LiteralPath ($caseReport + '.log') -SimpleMatch 'TEST COMPLETE. EXIT CODE: 0')) { throw ('Inspect native logs: ' + $case.Name) }
    Write-Output ('Passed=' + $case.Test)
}
```

## 最新验证

- 从干净的产品提交构建 Win64 Development，Build、Cook、Stage、Package、Archive 均成功，UAT 退出码 0。日志 `.build/package-rescue-success-20261120.log`。
- 技术侧从归档根程序六次 D3D12 启动，六项各 1/1 Success，逐例 errors/warnings 和进程/测试退出码均为 0。每份原生 `index.json` 及同名日志位于 `.build/b01-rescue-1120-packaged/` 下，分目录名见 BUILD-IDENTITY。六份日志都报告 HTML 模板缺失。人工目视了新包三张 00000 图，默认 ready、失败后果中文草稿、成功等待环境及七标记均清晰可读。
- 本轮 Editor Development 编译成功，真实 D3D12 游戏宿主 RescueSuccess 1/1，报告 `.build/b01-success-editor-game-1120/index.json`。Ubuntu/GCC 13.3 同源核心 6/6，报告 `.build/core-b01-1120/ctest-junit.xml`。
- 独立审查者 `independent_review` 已核对功能提交，并独立核对新包构建身份、UAT 日志、六份包报告和日志、三张截图及交接命令，批准本批候选交付。

## 未满足项

- test 待独立复测新包并按版本更新 evidence；当前候选 CI 结果以对应执行记录为准。
- 救援灰盒止于 TimeSkip。技术后续需接入实际环境结算及完成回执、回访布置和控制恢复，覆盖 30 年后的回访。真实感知与战斗、整个世界卸载和地图切换、旧界面清理、存档恢复、正式字幕与声音送达仍待联调。
- 自动化使用软件输入。实物手柄、本人试玩及输入经核心到实际画面呈现的 50 ms 约束均待实测。目标 0.1.0 尚未发布。
