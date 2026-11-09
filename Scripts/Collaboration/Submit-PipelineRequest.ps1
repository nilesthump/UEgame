#Requires -Version 5.1
<#
.SYNOPSIS
Creates a pipeline MR after checking direction, version, body and write scope.
#>
[CmdletBinding()]
param(
    [Parameter(Mandatory = $true)] [string] $To,
    [Parameter(Mandatory = $true)] [string] $Title,
    [Parameter(Mandatory = $true)] [string] $BodyFile,
    [string] $Root = '',
    [switch] $DryRun
)

Set-StrictMode -Version Latest
$ErrorActionPreference = 'Stop'
. (Join-Path $PSScriptRoot 'PipelineProtocol.ps1')

function Get-RepositoryRoot {
    param([string] $RequestedRoot)
    if ([string]::IsNullOrWhiteSpace($RequestedRoot)) {
        $detected = & git rev-parse --show-toplevel 2>$null
        if ($LASTEXITCODE -ne 0 -or [string]::IsNullOrWhiteSpace($detected)) {
            throw 'Not inside a Git worktree. Pass -Root explicitly.'
        }
        $RequestedRoot = $detected
    }
    return [IO.Path]::GetFullPath($RequestedRoot)
}

$repositoryRoot = Get-RepositoryRoot -RequestedRoot $Root
$config = Get-PipelineConfig -RepositoryRoot $repositoryRoot
$branch = ((& git -C $repositoryRoot rev-parse --abbrev-ref HEAD 2>$null) | Out-String).Trim()
$from = Get-PipelineNameFromBranch -Config $config -Branch $branch
if ([string]::IsNullOrWhiteSpace($from)) {
    throw ('Current branch {0} is not a fixed pipeline branch.' -f $branch)
}
Assert-MrDirection -Config $config -From $from -To $To

if (-not [IO.File]::Exists($BodyFile)) {
    throw ('Missing PR body file: {0}' -f $BodyFile)
}
$body = [IO.File]::ReadAllText($BodyFile)
$bodyCheck = Test-PipelineRequestBody -Body $body -TargetVersion ([string]$config.targetVersion)
if (-not $bodyCheck.Passed) {
    foreach ($errorItem in $bodyCheck.Errors) { Write-Output ('FAIL: ' + $errorItem) }
    throw 'PR body does not satisfy the five-section protocol.'
}

$status = ((& git -C $repositoryRoot status --porcelain 2>$null) | Out-String).Trim()
if (-not [string]::IsNullOrWhiteSpace($status)) {
    throw 'Working tree must be clean before creating a pipeline MR.'
}

$baseBranch = if ($To -eq 'main') { 'main' } else { 'pipeline/{0}' -f $To }
if ($To -eq 'main') {
    $scopeBase = 'origin/main'
}
else {
    $scopeBase = 'refs/heads/pipeline/{0}' -f $To
}
if (-not $DryRun) {
    $hostExe = (Get-Process -Id $PID).Path
    $scopeArguments = @('-NoProfile', '-File', (Join-Path $PSScriptRoot 'Test-PipelineScope.ps1'), '-Root', $repositoryRoot, '-Base', $scopeBase)
    if ($To -eq 'main') {
        $scopeArguments += '-Integration'
    }
    & $hostExe @scopeArguments
    if ($LASTEXITCODE -ne 0) {
        throw 'Pipeline write-scope check failed.'
    }
}

$existingRaw = (& gh pr list --head $branch --base $baseBranch --state open --json number,url 2>$null | Out-String).Trim()
$existing = @()
if (-not [string]::IsNullOrWhiteSpace($existingRaw) -and $existingRaw -ne '[]') {
    $existing = @($existingRaw | ConvertFrom-Json)
}
if ($existing.Count -gt 0 -and $null -ne $existing[0]) {
    throw ('An open MR already exists: #{0} {1}' -f $existing[0].number, $existing[0].url)
}

if ($DryRun) {
    Write-Output ('DryRun: gh pr create --head {0} --base {1} --title "{2}" --body-file {3}' -f $branch, $baseBranch, $Title, $BodyFile)
    exit 0
}

$url = (& gh pr create --head $branch --base $baseBranch --title $Title --body-file $BodyFile 2>&1 | Out-String).Trim()
if ($LASTEXITCODE -ne 0) {
    throw ('gh pr create failed: {0}' -f $url)
}
$urlLine = @($url -split "`r?`n" | Where-Object { $_ -match '^https://' })
if ($urlLine.Count -eq 0) {
    throw ('gh pr create returned no URL: {0}' -f $url)
}
Write-Output ('MR={0}' -f $urlLine[$urlLine.Count - 1])
Write-Output ('Direction={0}->{1}' -f $from, $To)
