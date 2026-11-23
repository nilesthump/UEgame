# Run a real Win64 presentation case with its game window hidden before automation starts.
[CmdletBinding()]
param(
    [Parameter(Mandatory)][string] $Archive,
    [Parameter(Mandatory)][string] $ReportDirectory,
    [switch] $RequireForeground
)
$ErrorActionPreference = 'Stop'
$launcher = (Get-Item -LiteralPath (Join-Path $Archive 'Windows/UEgame.exe')).FullName
$exe = (Get-Item -LiteralPath (Join-Path $Archive 'Windows/UEgame/Binaries/Win64/UEgame.exe')).FullName
$root = (New-Item -ItemType Directory -Path $ReportDirectory -ErrorAction Stop).FullName
$case = 'UEgame.WorldScenario.B01.MagicTownDialogue'
$log = Join-Path $root 'game.log'
$report = Join-Path $root 'magic'
$observation = Join-Path $root 'window.txt'
if (-not ('TownStartupWindow' -as [type])) {
    Add-Type -TypeDefinition @"
using System;
using System.Collections.Generic;
using System.Runtime.InteropServices;
using System.Text;
public static class TownStartupWindow {
    private delegate bool EnumProc(IntPtr hwnd, IntPtr data);
    [StructLayout(LayoutKind.Sequential)] private struct Rect { public int Left, Top, Right, Bottom; }
    [DllImport("user32.dll")] private static extern bool EnumWindows(EnumProc callback, IntPtr data);
    [DllImport("user32.dll", CharSet=CharSet.Unicode)] private static extern int GetClassName(IntPtr hwnd, StringBuilder name, int count);
    [DllImport("user32.dll")] private static extern bool GetClientRect(IntPtr hwnd, out Rect bounds);
    [DllImport("user32.dll")] public static extern bool ShowWindowAsync(IntPtr hwnd, int command);
    [DllImport("user32.dll")] public static extern bool IsWindowVisible(IntPtr hwnd);
    [DllImport("user32.dll")] public static extern IntPtr GetForegroundWindow();
    [DllImport("user32.dll")] public static extern bool SetForegroundWindow(IntPtr hwnd);
    [DllImport("user32.dll")] private static extern uint GetWindowThreadProcessId(IntPtr hwnd, out uint processId);
    public static bool IsGameWindow(IntPtr hwnd, uint gamePid) {
        uint owner;
        if (GetWindowThreadProcessId(hwnd, out owner) == 0 || owner != gamePid) return false;
        var name = new StringBuilder(128);
        Rect bounds;
        return GetClassName(hwnd, name, name.Capacity) > 0 && name.ToString() == "UnrealWindow" &&
            GetClientRect(hwnd, out bounds) && bounds.Right - bounds.Left == 1280 && bounds.Bottom - bounds.Top == 720;
    }
    public static IntPtr[] VisibleGameWindows(uint gamePid) {
        var matches = new List<IntPtr>();
        EnumWindows(delegate(IntPtr hwnd, IntPtr data) {
            if (!IsGameWindow(hwnd, gamePid) || !IsWindowVisible(hwnd)) return true;
            matches.Add(hwnd);
            return true;
        }, IntPtr.Zero);
        return matches.ToArray();
    }
}
"@
}
$gameArgs = @('-windowed','-ResX=1280','-ResY=720','-unattended','-nosplash','-d3d12','-B01RescueGraybox','-FORCELOGFLUSH',
    ('-ExecCmds="Automation RunTests ' + $case + ';Quit"'),'-TestExit="Automation Test Queue Empty"',
    ('-ReportExportPath="' + $report + '"'),('-abslog="' + $log + '"'))
$guard = [Threading.Mutex]::new($false, 'Local\UEgame-HiddenTownStartup')
$ownsGuard = $false
$bootstrap = $null
$game = $null
try {
    try { $ownsGuard = $guard.WaitOne(0) }
    catch [Threading.AbandonedMutexException] { $ownsGuard = $true }
    if (-not $ownsGuard -or (Get-Process -Name UEgame -ErrorAction SilentlyContinue)) {
        throw 'Run the hidden-window regression with no other UEgame instance active'
    }
    $bootstrap = Start-Process -FilePath $launcher -ArgumentList $gameArgs -WindowStyle Hidden -PassThru
    $clock = [Diagnostics.Stopwatch]::StartNew()
    do {
        if ($bootstrap.HasExited -or $clock.Elapsed.TotalSeconds -gt 90) { throw 'Game startup failed' }
        Start-Sleep -Milliseconds 50
        if (-not $game) {
            $child = Get-CimInstance Win32_Process -Filter "ParentProcessId=$($bootstrap.Id) AND Name='UEgame.exe'" |
                Where-Object { $_.ExecutablePath -eq $exe } | Select-Object -First 1
            if ($child) { $game = Get-Process -Id $child.ProcessId -ErrorAction Stop }
        }
        $hwnd = [IntPtr]::Zero
        if ($game) {
            $matchesFound = [TownStartupWindow]::VisibleGameWindows($game.Id)
            if ($matchesFound.Length -gt 1) { throw 'Multiple matching viewport windows before startup' }
            if ($matchesFound.Length -eq 1) { $hwnd = $matchesFound[0] }
        }
    } until ($hwnd -ne [IntPtr]::Zero -and (Test-Path -LiteralPath $log) -and
        (Select-String -LiteralPath $log -SimpleMatch -Pattern 'Game Engine Initialized.' -Quiet))
    $foreground = [TownStartupWindow]::GetForegroundWindow()
    "Selected=$(Get-Date -Format o) LauncherPID=$($bootstrap.Id) GamePID=$($game.Id) HWND=$hwnd Class=UnrealWindow Client=1280x720 ForegroundHWND=$foreground RequireForeground=$RequireForeground" | Set-Content -LiteralPath $observation
    if ($RequireForeground -and $foreground -ne $hwnd -and [TownStartupWindow]::IsGameWindow($hwnd, $game.Id)) {
        $granted = [TownStartupWindow]::SetForegroundWindow($hwnd)
        "ForegroundRequested=$(Get-Date -Format o) Granted=$granted" | Add-Content -LiteralPath $observation
        Write-Host 'Foreground reproduction: select the game window now if Windows did not activate it.'
        $clock.Restart()
        while ([TownStartupWindow]::GetForegroundWindow() -ne $hwnd -and $clock.Elapsed.TotalSeconds -lt 4) {
            if (Select-String -LiteralPath $log -SimpleMatch -Pattern 'Test Started.' -Quiet) { break }
            Start-Sleep -Milliseconds 50
        }
    }
    $foreground = [TownStartupWindow]::GetForegroundWindow()
    "BeforeHide=$(Get-Date -Format o) HWND=$hwnd ForegroundHWND=$foreground" | Add-Content -LiteralPath $observation
    if ($RequireForeground -and $foreground -ne $hwnd) {
        throw 'Experiment setup unmet: the selected game viewport was not confirmed foreground before hiding'
    }
    if (-not [TownStartupWindow]::IsGameWindow($hwnd, $game.Id) -or
        -not [TownStartupWindow]::IsWindowVisible($hwnd)) { throw 'Selected viewport changed before hiding' }
    if (Select-String -LiteralPath $log -SimpleMatch -Pattern 'Test Started.' -Quiet) {
        throw 'Experiment setup unmet: automation already started before hiding'
    }
    if (-not [TownStartupWindow]::ShowWindowAsync($hwnd, 0)) { throw 'Could not hide the game window' }
    $clock.Restart()
    while ([TownStartupWindow]::IsWindowVisible($hwnd)) {
        if ($clock.Elapsed.TotalSeconds -gt 3) { throw 'Window remained visible' }
        Start-Sleep -Milliseconds 20
    }
    if (Select-String -LiteralPath $log -SimpleMatch -Pattern 'Test Started.' -Quiet) {
        throw 'Window was hidden too late to exercise startup'
    }
    if (-not [TownStartupWindow]::IsGameWindow($hwnd, $game.Id)) { throw 'Viewport handle changed before the test' }
    "HiddenBeforeTest=$(Get-Date -Format o) Valid=True ForegroundHWND=$([TownStartupWindow]::GetForegroundWindow())" | Add-Content -LiteralPath $observation
    # Only the game can restore its window. A hidden or failed run must not count as a pass.
    $visibleAgain = $false
    $otherWindows = [Collections.Generic.HashSet[IntPtr]]::new()
    while (-not $bootstrap.WaitForExit(50)) {
        if ($clock.Elapsed.TotalSeconds -gt 200) { throw 'Automation timed out' }
        foreach ($visible in [TownStartupWindow]::VisibleGameWindows($game.Id)) {
            if ($visible -eq $hwnd -and -not $visibleAgain) {
                $visibleAgain = $true
                "VisibleAgain=$(Get-Date -Format o) HWND=$hwnd Valid=True" | Add-Content -LiteralPath $observation
            }
            if ($visible -ne $hwnd -and $otherWindows.Add($visible)) {
                "OtherVisibleWindow=$(Get-Date -Format o) HWND=$visible GamePID=$($game.Id) Class=UnrealWindow Client=1280x720" | Add-Content -LiteralPath $observation
            }
        }
    }
    $launcherExit = $bootstrap.ExitCode
    "LauncherExit=$launcherExit VisibleAgain=$visibleAgain OtherVisibleCount=$($otherWindows.Count) ValidAtExit=$([TownStartupWindow]::IsGameWindow($hwnd, $game.Id))" | Add-Content -LiteralPath $observation
    if (-not $game.WaitForExit(5000)) { throw 'Game process outlived its launcher' }
    $result = Get-Content -LiteralPath (Join-Path $report 'index.json') -Raw -Encoding UTF8 | ConvertFrom-Json
    if (-not $visibleAgain -or
        -not ($result.tests[0].entries.event.message -contains 'Game viewport restored and visibly shown at startup') -or
        $null -eq $launcherExit -or $launcherExit -ne 0 -or @($result.tests).Count -ne 1 -or
        $result.tests[0].fullTestPath -ne $case -or $result.tests[0].state -ne 'Success' -or
        $result.succeeded -ne 1 -or $result.tests[0].errors -ne 0 -or $result.tests[0].warnings -ne 0 -or
        -not (Select-String -LiteralPath $log -SimpleMatch -Pattern 'B01 town text delivered: NAR-0002 ' -Quiet) -or
        -not (Select-String -LiteralPath $log -SimpleMatch -Pattern 'B01 town text delivered: NAR-0006 ' -Quiet) -or
        -not (Select-String -LiteralPath $log -SimpleMatch -Pattern 'TEST COMPLETE. EXIT CODE: 0' -Quiet) -or
        -not (Select-String -LiteralPath $log -SimpleMatch -Pattern 'LogRHI: Loading RHI module D3D12RHI' -Quiet)) {
        throw "Hidden-window regression failed. Inspect $root"
    }
    Write-Output "Passed=$case Report=$report"
}
finally {
    try {
        if ($bootstrap) {
            if (-not $bootstrap.HasExited) { $bootstrap.Kill(); $bootstrap.WaitForExit() }
            if (-not $game) {
                # Startup can fail before the game handle is acquired.
                $child = Get-CimInstance Win32_Process -Filter "ParentProcessId=$($bootstrap.Id) AND Name='UEgame.exe'" |
                    Where-Object { $_.ExecutablePath -eq $exe } | Select-Object -First 1
                if ($child) { $game = Get-Process -Id $child.ProcessId -ErrorAction SilentlyContinue }
            }
            if ($game -and -not $game.HasExited) { $game.Kill(); $game.WaitForExit() }
        }
    }
    finally {
        if ($game) { $game.Dispose() }
        if ($bootstrap) { $bootstrap.Dispose() }
        if ($ownsGuard) { $guard.ReleaseMutex() }
        $guard.Dispose()
    }
}
