#Requires -Version 5.1
<#
.SYNOPSIS
Prints the target and online release state without changing it.
#>
[CmdletBinding()]
param(
    [string] $Root = ''
)

Set-StrictMode -Version Latest
$ErrorActionPreference = 'Stop'

if ([string]::IsNullOrWhiteSpace($Root)) {
    $detected = & git rev-parse --show-toplevel 2>$null
    if ($LASTEXITCODE -ne 0 -or [string]::IsNullOrWhiteSpace($detected)) {
        throw 'Not inside a Git worktree. Pass -Root explicitly.'
    }
    $Root = $detected
}
$statePath = Join-Path ([IO.Path]::GetFullPath($Root)) 'spec/releases/state.json'
$state = [IO.File]::ReadAllText($statePath) | ConvertFrom-Json
Write-Output ('TargetVersion={0}' -f $state.targetVersion)
if ($null -eq $state.onlineVersion) {
    Write-Output 'OnlineVersion=Unreleased'
}
else {
    Write-Output ('OnlineVersion={0}' -f $state.onlineVersion)
}
Write-Output ('OnlineTag={0}' -f $state.onlineTag)
Write-Output ('Status={0}' -f $state.status)
