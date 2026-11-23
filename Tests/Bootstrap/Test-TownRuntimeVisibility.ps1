# Invoked by Test-HiddenTownStartup.ps1 with the launcher's verified viewport.
[CmdletBinding()]
param(
    [Parameter(Mandatory)][IntPtr] $Handle,
    [Parameter(Mandatory)][Diagnostics.Process] $Game,
    [Parameter(Mandatory)][Diagnostics.Process] $Launcher,
    [Parameter(Mandatory)][string] $Log,
    [Parameter(Mandatory)][string] $Observation
)
$ErrorActionPreference = 'Stop'
function Assert-TargetWindow {
    if (-not [TownStartupWindow]::IsGameWindow($Handle, $Game.Id)) { throw 'Lost original game viewport' }
}
$timer = [Diagnostics.Stopwatch]::StartNew()
# The first crowd generation deliberately ends when the player walks away.
# Interrupt the second one after it has had time to accumulate real paints.
$cue = '^\[[\d.:-]+\]\[\s*\d+\]LogTemp: Display: B01 town text fallback \(no bound voice\): NAR-0002 '
do {
    $cueLines = @(Select-String -LiteralPath $Log -Pattern $cue)
    if ($cueLines.Count -ge 2) { break }
    if ($Launcher.HasExited -or $timer.Elapsed.TotalSeconds -gt 45) { throw 'Second crowd generation missing' }
    Start-Sleep -Milliseconds 100
} while ($true)
if ($cueLines[1].Line -notmatch 'generation=(\d+)') { throw 'Missing generation in crowd cue' }
$interruptedGeneration = $Matches[1]
Start-Sleep -Milliseconds 1200
Assert-TargetWindow
if (-not [TownStartupWindow]::IsWindowVisible($Handle) -or
    -not [TownStartupWindow]::ShowWindowAsync($Handle, 0)) { throw 'Could not hide the visible game viewport' }
$timer.Restart()
while ([TownStartupWindow]::IsWindowVisible($Handle)) {
    if ($timer.Elapsed.TotalSeconds -gt 2) { throw 'Viewport stayed visible' }
    Start-Sleep -Milliseconds 10
}
$hidden = [DateTime]::UtcNow
"RuntimeHidden=$($hidden.ToString('o')) HWND=$Handle" | Add-Content -LiteralPath $Observation
$timer.Restart()
while ($timer.Elapsed.TotalSeconds -lt 7) {
    Assert-TargetWindow
    if ([TownStartupWindow]::IsWindowVisible($Handle)) { throw 'Viewport reappeared before requested restore' }
    Start-Sleep -Milliseconds 25
}
$restore = [DateTime]::UtcNow
"RuntimeRestoreRequested=$($restore.ToString('o')) HWND=$Handle" | Add-Content -LiteralPath $Observation
if (-not [TownStartupWindow]::ShowWindowAsync($Handle, 9)) { throw 'Could not restore viewport' }
$timer.Restart()
while (-not [TownStartupWindow]::IsWindowVisible($Handle)) {
    if ($timer.Elapsed.TotalSeconds -gt 2) { throw 'Viewport stayed hidden' }
    Start-Sleep -Milliseconds 10
}
Assert-TargetWindow
[void][TownStartupWindow]::SetForegroundWindow($Handle)
"RuntimeVisible=$([DateTime]::UtcNow.ToString('o')) HWND=$Handle" | Add-Content -LiteralPath $Observation
if (-not $Launcher.WaitForExit(90000)) { throw 'Automation timed out after restoring viewport' }

$pattern = '^\[(?<utc>[\d.:-]+)\]\[\s*\d+\]LogTemp: Display: B01 town viewport: event=(?<event>tick|paint|receipt) frame=(?<frame>\d+) hwnd=(?<hwnd>\d+) visible=(?<visible>[01]) minimized=(?<min>[01]) id=(?<id>\S+) generation=(?<gen>\d+) elapsed=(?<elapsed>[.\d]+) required=(?<required>[.\d]+) qualified=(?<qualified>[01]) utcTicks=(?<ticks>\d+)'
$culture = [Globalization.CultureInfo]::InvariantCulture
$hiddenTicks = 0
$preCredit = 0.0
$firstPaint = $null
$receipts = @()
$traceKeys = @()
$deliveryKeys = @()
$lastFrame = -1
foreach ($line in [IO.File]::ReadLines($Log)) {
    if ($line -match '^\[[\d.:-]+\]\[\s*\d+\]LogTemp: Display: B01 town text delivered: (\S+) run=\d+ generation=(\d+)') {
        $deliveryKeys += $Matches[1] + '/' + $Matches[2]
    }
    if ($line -notmatch $pattern) { continue }
    if ([long]$Matches.hwnd -ne $Handle.ToInt64()) { throw "Trace selected a different viewport: $line" }
    $frame = [long]$Matches.frame
    if ($frame -lt $lastFrame) { throw "Trace frame moved backwards: $line" }
    $lastFrame = $frame
    # Use the event clock sampled on the game thread. UE timestamps the
    # normal log prefix when its asynchronous writer flushes the message.
    $utc = [DateTime]::new([long]$Matches.ticks,[DateTimeKind]::Utc)
    $event = $Matches.event
    $elapsed = [double]::Parse($Matches.elapsed,$culture)
    # Allow one millisecond for rounding at the external clock boundaries.
    if ($utc -gt $hidden.AddMilliseconds(1) -and $utc -lt $restore.AddMilliseconds(-1)) {
        if ($event -eq 'receipt' -or $Matches.visible -ne '0' -or $Matches.min -ne '0' -or
            ($event -eq 'paint' -and $Matches.qualified -eq '1')) { throw "Unexpected hidden viewport sample: $line" }
        if ($event -eq 'tick') { $hiddenTicks++ }
    }
    if ($event -eq 'paint' -and $Matches.qualified -eq '1' -and $Matches.id -eq 'NAR-0002') {
        if ($utc -lt $hidden -and $Matches.gen -eq $interruptedGeneration) { $preCredit = [Math]::Max($preCredit,$elapsed) }
        if ($null -eq $firstPaint -and $utc -ge $restore.AddMilliseconds(-1)) {
            if ($elapsed -gt 0.001) { throw "First restored crowd paint retained credit: $line" }
            $firstPaint = $utc
        }
    }
    if ($event -ne 'receipt') { continue }
    if ($Matches.visible -ne '1' -or $Matches.min -ne '0' -or $utc -lt $restore) {
        throw "Receipt before confirmed restoration: $line"
    }
    $required = [double]::Parse($Matches.required,$culture)
    if ($required -le 0 -or $elapsed + 0.001 -lt $required) { throw "Incomplete reading time at receipt: $line" }
    $traceKeys += $Matches.id + '/' + $Matches.gen
    $receipts += $Matches.id
    if ($Matches.id -eq 'NAR-0002') {
        if ($null -eq $firstPaint -or ($restore-$hidden).TotalSeconds -le $required -or
            ($utc-$firstPaint).TotalSeconds + 0.002 -lt $required) {
            throw "Incomplete restored crowd display: $line"
        }
        "CrowdReceiptFrame=$frame FirstRestoredPaintUTC=$($firstPaint.ToString('o')) ReceiptUTC=$($utc.ToString('o')) Elapsed=$elapsed Required=$required" | Add-Content -LiteralPath $Observation
    }
}
if ($hiddenTicks -lt 2 -or $preCredit -lt 0.5 -or ($receipts -join ',') -ne 'NAR-0002,NAR-0006' -or
    ($traceKeys -join ',') -ne ($deliveryKeys -join ',')) {
    throw "Incomplete trace: hiddenTicks=$hiddenTicks preCredit=$preCredit receipts=$receipts"
}
"HiddenTicks=$hiddenTicks PriorCrowdCredit=$preCredit Receipts=$($receipts -join ',')" | Add-Content -LiteralPath $Observation
