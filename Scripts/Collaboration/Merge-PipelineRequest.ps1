#Requires -Version 5.1
<#
.SYNOPSIS
Validates and merges a pipeline MR with a merge commit.
.DESCRIPTION
The review record is posted to the original PR before merging. No branch is deleted.
main merges require the configured authorized receiver and use the existing owner bypass.
#>
[CmdletBinding()]
param(
    [Parameter(Mandatory = $true)] [int] $Pr,
    [Parameter(Mandatory = $true)] [string] $From,
    [Parameter(Mandatory = $true)] [string] $To,
    [Parameter(Mandatory = $true)] [string] $Reviewer,
    [Parameter(Mandatory = $true)] [string] $ReviewEvidence,
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
Assert-MrDirection -Config $config -From $From -To $To

if ([string]::IsNullOrWhiteSpace($Reviewer) -or [string]::IsNullOrWhiteSpace($ReviewEvidence)) {
    throw 'An independent reviewer and review evidence are required.'
}

$json = (& gh pr view $Pr --json number,state,baseRefName,headRefName,headRefOid,mergeStateStatus,statusCheckRollup,body,url,isDraft 2>&1 | Out-String).Trim()
if ($LASTEXITCODE -ne 0) {
    throw ('gh pr view failed: {0}' -f $json)
}
$prInfo = $json | ConvertFrom-Json
if ($prInfo.state -ne 'OPEN') { throw ('PR is {0}, not OPEN.' -f $prInfo.state) }
if ($prInfo.isDraft) { throw 'Draft PRs cannot be received.' }
if ($prInfo.headRefName -ne ('pipeline/{0}' -f $From)) {
    throw ('PR head is {0}, expected pipeline/{1}.' -f $prInfo.headRefName, $From)
}
$expectedBase = if ($To -eq 'main') { 'main' } else { 'pipeline/{0}' -f $To }
if ($prInfo.baseRefName -ne $expectedBase) {
    throw ('PR base is {0}, expected {1}.' -f $prInfo.baseRefName, $expectedBase)
}

$bodyCheck = Test-PipelineRequestBody -Body $prInfo.body -TargetVersion ([string]$config.targetVersion)
if (-not $bodyCheck.Passed) {
    foreach ($errorItem in $bodyCheck.Errors) { Write-Output ('FAIL: ' + $errorItem) }
    throw 'PR body does not satisfy the five-section protocol.'
}

$checks = @(Get-RequiredCheckState -Pr $prInfo)
foreach ($check in $checks) {
    if ($check.State -match 'FAILURE|ERROR|CANCELLED|TIMED_OUT|ACTION_REQUIRED') {
        throw ('Check {0} is {1}.' -f $check.Name, $check.State)
    }
}
if ($To -eq 'main') {
    $baseline = @($checks | Where-Object { $_.Name -eq 'repository-baseline' -and $_.State -eq 'SUCCESS' })
    if ($baseline.Count -eq 0) {
        throw 'main merge requires a successful repository-baseline check.'
    }
    $login = ((& gh api user --jq .login 2>$null) | Out-String).Trim()
    if ($login -ne [string]$config.mainReceiverLogin) {
        throw ('Current GitHub user {0} is not the authorized main receiver {1}.' -f $login, $config.mainReceiverLogin)
    }
}

if ($DryRun) {
    Write-Output ('DryRun: merge PR #{0} {1}->{2} at {3}' -f $Pr, $From, $To, $prInfo.headRefOid)
    exit 0
}

$comment = @(
    '## 接收与独立审查记录',
    ('- 方向：{0} -> {1}' -f $From, $To),
    ('- 独立审查：{0}' -f $Reviewer),
    ('- 审查证据：{0}' -f $ReviewEvidence),
    ('- 接收时头提交：{0}' -f $prInfo.headRefOid)
) -join "`n"
& gh pr comment $Pr --body $comment | Out-Null
if ($LASTEXITCODE -ne 0) { throw 'Unable to record independent review on the PR.' }

$mergeArguments = @('pr', 'merge', [string]$Pr, '--merge', '--match-head-commit', [string]$prInfo.headRefOid)
if ($To -eq 'main') {
    $mergeArguments += '--admin'
}
$mergeOutput = (& gh @mergeArguments 2>&1 | Out-String).Trim()
if ($LASTEXITCODE -ne 0) {
    throw ('gh pr merge failed: {0}' -f $mergeOutput)
}
$after = (& gh pr view $Pr --json state,mergeCommit,url 2>&1 | Out-String).Trim() | ConvertFrom-Json
if ($after.state -ne 'MERGED') {
    throw ('PR state is {0}, expected MERGED.' -f $after.state)
}
Write-Output ('Merged=#{0}' -f $Pr)
Write-Output ('MergeCommit={0}' -f $after.mergeCommit.oid)
Write-Output ('Url={0}' -f $after.url)
