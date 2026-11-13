## 目标版本与工作项

- 版本：0.1.0
- 工作项：TECH-0.1.0-01 / B01 默认启动、后果灰盒与中文 draft 探针。
- 状态：本机接收准备，尚未发起本批 MR、尚未被 test 接收；B01 InProgress。本页可直接作为五项 PR 正文，替代旧治理批次内容；历史由 Git 保存。

## 交付内容

- `9f8e6a9`：默认地图、GameMode、真实宿主启动检查。
- `83df86e`：显式灰盒开关、E 输入、稳定快照与终态效果链。
- `fb2861e`：从唯一 TEXT.csv 导入 NAR-0004 中文 draft 探针，不提交媒体送达回执。
- `05d829e` 及引入历史为 main 基线同步，不计本批新增功能；本次交接文档更新不改变产品源码或构建身份。
- 产品提交：`fb2861e0294e0d6479087be13ec70fd1a1e2d78f`。包根 BUILD-IDENTITY.json 保留提交前工作树构建、测试审查后源码原样提交的说明，不宣称干净提交构建。
- 独立源码审查：2026-11-13，本对话 agent `/root/handoff_review` 实查三个功能提交差异、五份 UE 报告、Linux XML、构建身份及三张截图，无阻止受限灰盒候选进入 test 审查的源码问题。地图未重新在编辑器打开，UE/Linux 未重跑；不等于 GitHub 账号批准或 test 验收。

## 接收条件

- 用户已选择本机接收。技术工作区根为 `G:/UEgame/.worktrees/pipelines/tech`，下述证据路径相对该根。本地忽略输出不随 Git/MR 自动传递，迁移机器时须重新建立访问。
- 完整包：`Deliverables/B01-draft-text-20261113/`，包含 BUILD-IDENTITY.json、Windows 根启动程序、实际游戏程序及 Paks；不得仅复制根启动程序。复测使用新报告目录，保留既有证据。
- test 先按 AGENTS 恢复自身管线，核对候选、MR 范围、审查与实际 CI，再直接验证此构建产物。本机文件可读不等于已验收。
- 手动体验：运行包根程序，传 `-B01AftermathGraybox -B01DraftTextPreview -windowed -ResX=1280 -ResY=720`，按 E；默认入口不加两个探针开关。
- test 在自身工作区按下列入口分别启动三个用例。每次使用新的报告目录；检查日志退出码和 index.json 中实际用例，必须非零且无失败/未运行/进行中项。自动化输入不代表实物键盘或手柄体验。

```powershell
$candidateExe = 'G:/UEgame/.worktrees/pipelines/tech/Deliverables/B01-draft-text-20261113/Windows/UEgame.exe'
$reportRoot = Join-Path $PWD ('.build/b01-receive-' + (Get-Date -Format 'yyyyMMdd-HHmmss'))
New-Item -ItemType Directory -Path $reportRoot -ErrorAction Stop | Out-Null
$cases = @(
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
    $report = Get-Content (Join-Path $caseReport 'index.json') -Raw | ConvertFrom-Json
    if ($report.succeeded -ne 1 -or $report.failed -ne 0 -or $report.notRun -ne 0 -or $report.inProcess -ne 0 -or $report.succeededWithWarnings -ne 0) {
        throw ('Report requires investigation: ' + $case.Name)
    }
}
```

- UE 截图仍写入包内 `Windows/UEgame/Saved/Screenshots/Windows/`，可能追加编号；复测前保留原图，test 检查此次新图并记录实际文件，不把旧截图作为新执行。截图不证明呈现延迟。
- 正式结论仅由 test 维护 `spec/evidence/evidence.md`。问题回到本批 MR，由 tech 修复后正向交付；满足条件后由 test 使用 merge commit 接收。

## 最新验证

- 本机接收准备实查：从 test 工作目录以当前同一账号逐个只读打开完整候选 54 个文件（Windows 内 53 个，加包根身份文件），共 878045946 字节；仅证明本机可读，不代表 test 已验收。
- 本次文档检查：RepositoryBaseline 209 文档/681 链接、NarrativeText 7 条、diff 检查及五项正文校验通过；复测命令通过 PowerShell 静态语法解析，未运行。

- 2026-11-13 接手核查实际执行：RepositoryBaseline 209 文档/681 链接、NarrativeText 7 条、diff 检查通过；PipelineScope 相对 `c3875ed3d15c06a749b84f8eaa0f40d2c04f6f83` 为 16 commits/6 merges/118 changed-file 计数通过（交接文档提交前产品 HEAD）。
- 已有执行报告（接手时读取，未重跑）：`.build/b01-draft-editor/index.json` 为 8/8，使用 NullRHI；`.build/b01-draft-game/index.json` 为 D3D12 宿主灰盒 1/1；`.build/b01-draft-packaged/index.json`、`.build/b01-draft-default-packaged/index.json`、`.build/b01-draft-source-packaged/index.json` 各 1/1，均 0 失败/未运行/进行中/带警告成功。对应 `.build/b01-draft-*.log` 测试退出码为 0。
- 默认包一次命令中的第二个 RunTests 未执行，不计通过；文本解析由 source-packaged 单独启动实际执行。
- `.build/package-draft-text-20261113-final.log` 为 BUILD SUCCESSFUL，AutomationTool ExitCode=0；Linux 沿用前批 `.build/core-linux-aftermath/ctest-junit.xml` 实际 6/6，0 失败/禁用/跳过。
- 独立审查目视：`Saved/Screenshots/WindowsEditor/B01Aftermath00001.png`，包内 `Windows/UEgame/Saved/Screenshots/Windows/B01Aftermath00000.png` 和 `B01Bootstrap00000.png`。中文、标点、NAR-0004/draft、七个标记和结束提示完整可读；默认入口为 ready。
- 远端只读核查：tech=`69fb8f7`、main=`e67a6f7`、test=`c3875ed`；旧 PR #27 已合并，无开放 tech PR。本批未推送、无对应远端 CI；发送前重新核对。
- 本页接收命令尚待 test 执行，不把准备命令计作游戏复测通过。

## 未满足项

- tech：本批远端候选、MR、对应 CI 未建立；本机访问准备不代表已交付。完整包、报告、截图仍为本地忽略输出。
- tech：真实玩法/成功路线、正式字幕与媒体送达、其余文本显示未完成；下一步由 test 按本页及 CURRENT 检查受限候选。
- tech/test：输入→核心处理→对应画面实际呈现 <50ms、本人体验、真实手柄未执行。
- narrative：NAR-0001–0007 全部 draft；定稿由 narrative 决定，不启动批量素材制作。
- test：正式接收与验收未完成。B01 InProgress，B02–B15 不启动；onlineVersion=null，不发布、不创建 tag。
