#Requires -Version 5.1
<#
.SYNOPSIS
Shows the current pipeline, its documents and allowed MR targets.
#>
[CmdletBinding()]
param(
    [string] $Root = ''
)

Set-StrictMode -Version Latest
$ErrorActionPreference = 'Stop'

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

function Invoke-GitText {
    param([string] $RepositoryRoot, [string[]] $Arguments)
    $output = & git -C $RepositoryRoot @Arguments 2>$null
    if ($LASTEXITCODE -ne 0) {
        throw ('git {0} failed' -f ($Arguments -join ' '))
    }
    return ($output | Out-String).Trim()
}

$repositoryRoot = Get-RepositoryRoot -RequestedRoot $Root
$configPath = Join-Path $repositoryRoot 'Scripts/Collaboration/pipelines.json'
if (-not [IO.File]::Exists($configPath)) {
    throw "Missing $configPath"
}
$config = [IO.File]::ReadAllText($configPath) | ConvertFrom-Json
$branch = Invoke-GitText -RepositoryRoot $repositoryRoot -Arguments @('rev-parse', '--abbrev-ref', 'HEAD')

Write-Output ('Repository={0}' -f $repositoryRoot)
Write-Output ('Branch={0}' -f $branch)
Write-Output ('TargetVersion={0}' -f $config.targetVersion)

if ($branch -eq 'main') {
    Write-Output 'Pipeline=main'
    Write-Output 'MainReceiver=ops'
    Write-Output 'Read=spec/pipelines/README.md; spec/releases/README.md; spec/evidence/evidence.md'
    Write-Output 'Write=never directly; merge only through reviewed ops -> main PR'
    exit 0
}

$entry = $null
foreach ($property in $config.pipelines.PSObject.Properties) {
    if ($property.Value.branch -eq $branch) {
        $entry = [pscustomobject]@{
            Name = $property.Name
            Definition = $property.Value
        }
        break
    }
}

if ($null -eq $entry) {
    throw ('Branch {0} is not one of the fixed pipeline branches.' -f $branch)
}

Write-Output ('Pipeline={0}' -f $entry.Name)
Write-Output ('Current={0}/spec/pipelines/{1}/CURRENT.md' -f $repositoryRoot, $entry.Name)
Write-Output 'Protocol=spec/pipelines/PROTOCOL.md'
Write-Output 'WriteScope=spec/governance/WRITE_SCOPE.md'
Write-Output ('MRTargets={0}' -f (($entry.Definition.mrTargets) -join ','))
Write-Output 'SparseCheckout=local worktree setting; refresh with Scripts/Collaboration/Initialize-PipelineWorkspaces.ps1'
