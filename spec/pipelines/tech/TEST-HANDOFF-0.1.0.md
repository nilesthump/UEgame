## 目标版本与工作项

- 版本：0.1.0
- 工作项：TECH-0.1.0-01 / B01，救援时的魔法及混用动作决定三十年后镇上的群众和教师对白。

## 交付内容

- 产品提交 `ac9257a97fd76f26f021450b043905877480b286`。Win64 Development 本机归档包位于 G:/UEgame/.worktrees/pipelines/tech/Deliverables/B01-revisit-routes-20261122/，包根目录 BUILD-IDENTITY.json 对应此提交。
- 启动参数 `-B01RescueGraybox`。先按一次 E 发现，再以 E 剑击或 F 施法共五次。五次 F 对应魔法路线，两次 E 加三次 F 可复现混用路线。镇中按四次 A 接近群众，从群众处按八次 D 到教师面前。剑术路线继续使用五次 E。
- 魔法群众/教师使用 NAR-0002/0006，混用使用 NAR-0007/0003，剑术沿用 NAR-0001/0005。均从 TEXT.csv 加载，保留说话者、ID 和 draft 状态，显示形式为 TextFallback。两段分别形成有效回执，第二份到达才结束回访。

## 接收条件

test 在上述同一归档包逐项复测并目视新生成的截图。直接运行包内 Binaries/Win64 下的游戏程序，以正常窗口启动。游戏窗口和视口需保持焦点，关闭菜单及控制台。三条路线均应能从群众处离开，中断时无回执，回来后重新累计；送达群众字幕后仍可去见教师。核对六张字幕截图中的角色、ID、中文全文和标点，另复核 Bootstrap、Aftermath、RescueSuccess、Revisit 的四张回归截图。验收状态由 test 在唯一 evidence 原地维护。

```powershell
$ErrorActionPreference = 'Stop'
$candidateExe = 'G:/UEgame/.worktrees/pipelines/tech/Deliverables/B01-revisit-routes-20261122/Windows/UEgame/Binaries/Win64/UEgame.exe'
$reportRoot = Join-Path $PWD ('.build/b01-routes-receive-' + (Get-Date -Format 'yyyyMMdd-HHmmss'))
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
    Write-Output ('Passed=' + $case.Test)
}
```

## 最新验证

- 产品提交在构建前和归档后工作树均干净。Build/Cook/Stage/Package/Archive 完成，UAT 退出码 0（`.build/package-routes-1122.log`）。独立启动十次、执行十项 D3D12 用例，各 1/1，进程和测试退出码均为 0。报告与日志在 `.build/routes-packaged-r2-1122/`。各份原生报告的 errors/warnings 为 0，十份日志均报 HTML 模板缺失。
- 包内截图位于 Windows/UEgame/Saved/Screenshots/Windows/。本轮魔法群众和教师取 00001，混用、剑术和其余四张回归取 00000。每条路线的群众第 1 代中断、第 2 代送达、教师第 3 代送达。
- 此前从包根启动器以隐藏窗口启动时，魔法用例失败（进程 255，测试退出码 -1）。报告 `.build/routes-packaged-1122/MagicTownDialogue/index.json` 和两张魔法 00000 截图保留；画面只有路线标记，没有字幕。该次日志未记录窗口焦点。复测沿用同包、同一套送达条件。
- 对同一 `ac9257a` 归档包再次运行 MagicTownDialogue。包根启动器、包内游戏程序分别以 PowerShell 的 Hidden 和 Normal 启动，各执行一次。四份 D3D12 原生报告各 1/1，两条字幕均送达，进程和测试退出码均为 0。报告、日志及 Win32 前台采样见 `.build/window-comparison-20261122-182026/`。四轮均采到游戏处于前台，Hidden 启动参数本身不能复现持续失焦。历史失败的首个断言是群众字幕未出现，具体阻断条件仍未知。
- 提交前诊断包 `Deliverables/B01-routes-window-diagnostic-r2/` 的 BuildCookRun 退出码 0（`.build/window-diagnostic-r2-package.log`）。同包运行两次 D3D12 魔法用例，报告、日志和进程记录在 `.build/window-r2-20261122-182833/`。正常窗口实际 1/1 成功，两条字幕顺序送达，进程与测试退出码均为 0。负例在群众第 1 代字幕出现后最小化游戏窗口，Win32 确认 IsIconic=True。该轮 1 例失败，2 个超时、0 警告，进程 255、测试退出码 -1。两次超时均为回访阶段（phase=4），windowMinimized=1、windowActive=1、viewportFocused=1，当前文本依次为 NAR-0002 和 NAR-0006，均未收到送达回执。最小化负例的首错为群众回执缺失，与历史首错不同。复测遇到超时，先查看首个失败断言后的阶段、窗口、视口及当前文本，再对照各代显示和送达日志。
- Linux 核心 8/8，Editor Development 构建成功，真实 D3D12 宿主三条路线各 1/1。源码、宿主执行和四张新路线截图经独立审查者 review 核准，位置见 tech 的 CURRENT。

- 独立审查者 review 核实本轮归档身份、UAT 和十份 D3D12 原生报告/日志，逐张目视成功轮十张截图，并复核首次失败截图，批准本候选交付。

## 未满足项

- test 待独立复测。正式声音、实物输入、输入到对应画面实际呈现小于 50 ms 和本人试玩仍待验证。剧情定稿归 narrative，声音源素材归 art。
- 本次以标记模拟动作和威胁消除。完整战斗、导航、环境火湿与破坏后果、世界卸载恢复由 tech 后续推进。B01 仍在实施，0.1.0 尚未发布。
