## 目标版本与工作项

- 版本：0.1.0
- 工作项：TECH-0.1.0-01 / B01 未结束 Rescue 的 Actor 销毁、GC 后弱发送者与旧轮隔离。
- PR #35 的 Lifecycle 候选已由 test 接收（merge `4ba150c`，test 证据 `15d3103`）。本页为下一批 RescueTeardown 的交接快照；MR/CI 最新状态以对应原 PR 为准，新包尚未由 test 接收，B01 InProgress。

## 交付内容

- 构建源码：`b4f94e68a834a2f0e95bdb90718de7d89d1b812a`，只新增 RescueTeardown 开发用例及 tech 说明，无生产逻辑/核心修改；后续交接文档不改变构建输入。
- 在同一真实 UWorld 未结束 Rescue 中销毁 Director，跨帧完整 GC，确认旧 Director/七个 Marker 实际回收；弱发送者解析后的 ready/observe 与 unbind 不影响新轮，旧 Run 无法操作新 Actor，新轮仍能观察结束。
- 新完整包：`G:/UEgame/.worktrees/pipelines/tech/Deliverables/B01-rescue-teardown-20261113/`。`BUILD-IDENTITY.json` 记录干净提交构建、UE 5.6.1 CL44394996、Win64 Development、地图、构建日志和报告路径。旧包保留。

## 接收条件

- 沿用本机完整包接收；包与原生输出为忽略文件，不随 Git 自动交付。跨机器须提供完整目录，不能只复制根启动程序。
- test 在自身固定工作区按 AGENTS 读序恢复，核对版本/源码/独立审查及对应 CI；分别启动根程序五次，不覆盖地图。Lifecycle、RescueTeardown、Aftermath 使用两个探针开关，另两项不使用。
- 下述报告路径使用新时间戳，逐例检查精确名称、1/1 Success、全部失败/未执行/进行中/警告成功为零；还需检查逐例 errors/warnings、日志 TEST COMPLETE EXIT CODE 0 与进程退出码。

```powershell
$ErrorActionPreference = 'Stop'
$candidateExe = 'G:/UEgame/.worktrees/pipelines/tech/Deliverables/B01-rescue-teardown-20261113/Windows/UEgame.exe'
$reportRoot = Join-Path $PWD ('.build/b01-rescue-teardown-receive-' + (Get-Date -Format 'yyyyMMdd-HHmmss'))
New-Item -ItemType Directory -Path $reportRoot -ErrorAction Stop | Out-Null
$cases = @(
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
    Write-Output ('Passed=' + $case.Test)
}
```

- 五日志均有 HTML 模板缺失错误，未生成 HTML。原生 JSON、日志与截图完整；不得声称日志无错误。
- test 必须目视本次复测新编号的 `Windows/UEgame/Saved/Screenshots/Windows/B01Bootstrap*.png` 和 `B01Aftermath*.png`；技术本轮 `00000` 图保留。RescueTeardown/Lifecycle 不生成截图，不能用 Aftermath 图证明其 Slate 清理或呈现。
- 满足条件后用 merge commit 接收；正式当前结论只由 test 写唯一 evidence，问题回到原 PR，由 tech 修复。

## 最新验证

- 干净提交 `b4f94e6` 构建前及归档后工作区干净；Build/Cook/Stage/Package/Archive 实际全部成功，进程与 AutomationTool 退出码 0：`.build/package-rescue-teardown-20261113.log`。MSVC 14.38.33145 / SDK 10.0.22621.0，禁用 UBA、单并发编译。
- 2026-11-13 18:29–18:30（Asia/Shanghai），新包五次 D3D12 启动实际通过：`.build/b01-rescue-teardown-packaged/{rescue-teardown,lifecycle,default,aftermath,text-source}/index.json` 各精确 1/1 Success，failed/notRun/inProcess/succeededWithWarnings=0，逐例 errors/warnings=0；五进程与日志测试退出码均 0。
- 已目视新包 `B01Bootstrap00000.png`（ready）及 `B01Aftermath00000.png`（ended、七标记、中文标点及 NAR-0004/draft 完整可读）。截图不证明端到端延迟。
- Editor r2 实际编译成功：`.build/b01-rescue-teardown-editor-build-r2.log`；真实 D3D12 宿主 `.build/b01-rescue-teardown-game-r2/index.json` 新用例 1/1、进程/测试退出码0。宿主有两条 VisionOS 编辑器图标缺失警告；包日志 HTML 限制如上。
- 独立 agent `/root/rescue_review` 已核对最终源码、r2 编译/宿主报告与说明并批准。同一独立 agent 已另行核对新包 BUILD-IDENTITY、实际 UAT 日志、五份精确报告/日志、两张新包截图、文档 diff 与复测命令，批准本批受限候选交接，无阻断项；本地 Linux 未重跑（核心未改），新头远端 CI 尚待实际执行，不复用旧头 CI 作为通过。

## 未满足项

- test：新完整包尚未独立接收。全部本机输出须完整保留；旧 PR #35 的验收不自动覆盖新候选。
- tech/test：只覆盖同一 UWorld 的 Actor 销毁及通过弱引用解析的迟到调用；不覆盖整个 UWorld 卸载、地图切换、Slate 残留、持久恢复、真实玩法/成功路线、正式媒体、端到端 <50ms、本人体验或实物输入。不声称存在真实异步回调调度系统；软件注入不冒充实物测试。
- narrative：全部 NAR 仍 draft；不改 final、不启动 B13。B01 InProgress，B02–B15 不启动，onlineVersion=null；不发布、不建 tag。
