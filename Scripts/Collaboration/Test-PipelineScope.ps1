#Requires -Version 5.1
<#
.SYNOPSIS
Checks that a pipeline branch only changes files owned by that pipeline.
.DESCRIPTION
Compares the working branch with -Base (default origin/main). For a normal MR,
pass the downstream branch as -Base so only the proposed delivery is checked.
ops -> main is an integration PR: use -Integration after every upstream PR has
passed its own scope check.
#>
[CmdletBinding()]
param(
    [string] $Root = '',
    [string] $Base = 'origin/main',
    [string] $Pipeline = '',
    [string] $IncidentId = '',
    [switch] $Integration
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
    return (($output | Out-String).Trim())
}

function Convert-GlobToRegex {
    param([string] $Pattern)
    $escaped = [regex]::Escape($Pattern)
    $escaped = $escaped.Replace('\*\*', '.*')
    $escaped = $escaped.Replace('\*', '[^/]*')
    $escaped = $escaped.Replace('\?', '[^/]')
    return ('^{0}$' -f $escaped)
}

function Test-PathList {
    param([string] $Path, [object[]] $Patterns)
    foreach ($pattern in @($Patterns)) {
        if ($Path -match (Convert-GlobToRegex -Pattern ([string]$pattern))) {
            return $true
        }
    }
    return $false
}

$repositoryRoot = Get-RepositoryRoot -RequestedRoot $Root
$config = [IO.File]::ReadAllText((Join-Path $repositoryRoot 'Scripts/Collaboration/pipelines.json')) | ConvertFrom-Json
$branch = Invoke-GitText -RepositoryRoot $repositoryRoot -Arguments @('rev-parse', '--abbrev-ref', 'HEAD')

$entry = $null
foreach ($property in $config.pipelines.PSObject.Properties) {
    if ($property.Value.branch -eq $branch) {
        $entry = [pscustomobject]@{ Name = $property.Name; Definition = $property.Value }
        break
    }
}
if ($null -eq $entry) {
    if ($branch -eq 'main' -and -not $Integration) {
        throw 'main is integration-only; work in a pipeline worktree.'
    }
    throw ('Branch {0} is not a fixed pipeline branch.' -f $branch)
}
if (-not [string]::IsNullOrWhiteSpace($Pipeline) -and $Pipeline -ne $entry.Name) {
    throw ('Current branch is {0}, not requested pipeline {1}.' -f $entry.Name, $Pipeline)
}
if ($Integration -and $entry.Name -ne 'ops') {
    throw 'Only ops may prepare the ops -> main integration scope.'
}

$baseCommit = Invoke-GitText -RepositoryRoot $repositoryRoot -Arguments @('rev-parse', '--verify', ($Base + '^{commit}'))
if ($Integration) {
    Write-Output ('Pipeline={0}' -f $entry.Name)
    Write-Output ('Base={0}' -f $baseCommit)
    Write-Output 'PipelineScope=IntegrationBypass (upstream PR scopes must have passed)'
    exit 0
}

function Test-CommitAncestor {
    param([string] $RepositoryRoot, [string] $Commit, [string] $BranchRef)
    & git -C $RepositoryRoot merge-base --is-ancestor $Commit $BranchRef 2>$null
    return ($LASTEXITCODE -eq 0)
}

$pipelineRefs = @{}
foreach ($property in $config.pipelines.PSObject.Properties) {
    $ref = 'refs/heads/' + [string]$property.Value.branch
    & git -C $repositoryRoot show-ref --verify --quiet $ref 2>$null
    if ($LASTEXITCODE -eq 0) {
        $pipelineRefs[$property.Name] = $ref
    }
}

$commits = @((Invoke-GitText -RepositoryRoot $repositoryRoot -Arguments @('rev-list', '--reverse', ($baseCommit + '..HEAD'))) -split "`n" |
    ForEach-Object { $_.Trim() } |
    Where-Object { -not [string]::IsNullOrWhiteSpace($_) })
$changedCount = 0
$mergeSkipped = 0
$failures = New-Object System.Collections.Generic.List[string]

foreach ($commit in $commits) {
    $parents = @((Invoke-GitText -RepositoryRoot $repositoryRoot -Arguments @('rev-list', '--parents', '-n', '1', $commit)) -split '\s+' |
        Where-Object { -not [string]::IsNullOrWhiteSpace($_) })
    if ($parents.Count -gt 2) {
        $mergeSkipped++
        continue
    }
    $diffText = Invoke-GitText -RepositoryRoot $repositoryRoot -Arguments @('diff-tree', '--no-commit-id', '--name-status', '-r', $commit)
    foreach ($line in ($diffText -split "`n")) {
        $item = $line.Trim()
        if ($item -eq '') { continue }
        $parts = $item -split "`t"
        if ($parts.Count -lt 2) { continue }
        $status = $parts[0]
        $path = $parts[$parts.Count - 1]
        $changedCount++

        $allowed = Test-PathList -Path $path -Patterns @($entry.Definition.owns)
        if (-not $allowed) {
            $upstreamOwners = New-Object System.Collections.Generic.List[string]
            foreach ($name in $pipelineRefs.Keys) {
                if ($name -eq $entry.Name) { continue }
                if (Test-CommitAncestor -RepositoryRoot $repositoryRoot -Commit $commit -BranchRef $pipelineRefs[$name]) {
                    $upstreamOwners.Add($name)
                }
            }
            foreach ($owner in $upstreamOwners) {
                if ($null -eq $config.pipelines.$owner) { continue }
                if (Test-PathList -Path $path -Patterns @($config.pipelines.$owner.owns)) {
                    $allowed = $true
                    break
                }
            }
        }
        if (-not $allowed -and $entry.Name -eq 'maintenance' -and -not [string]::IsNullOrWhiteSpace($IncidentId)) {
            $incidentPath = Join-Path $repositoryRoot ('spec/pipelines/maintenance/INCIDENT-{0}.md' -f $IncidentId)
            if ([IO.File]::Exists($incidentPath) -and
                (Test-PathList -Path $path -Patterns @($entry.Definition.hotfix))) {
                $allowed = $true
            }
        }
        if (-not $allowed) {
            $failures.Add(('{0} {1}: {2} is outside {3} ownership and no upstream branch owns it.' -f $commit.Substring(0, 7), $status, $path, $entry.Name))
        }

        foreach ($property in $config.pipelines.PSObject.Properties) {
            if ($null -ne $property.Value.PSObject.Properties['immutable'] -and
                $status -notmatch '^A' -and
                (Test-PathList -Path $path -Patterns @($property.Value.immutable))) {
                $failures.Add(('{0} {1}: existing immutable source {2} must not be rewritten.' -f $commit.Substring(0, 7), $status, $path))
            }
        }
    }
}

Write-Output ('Pipeline={0}' -f $entry.Name)
Write-Output ('Base={0}' -f $baseCommit)
Write-Output ('CommitsChecked={0}' -f $commits.Count)
Write-Output ('MergeCommitsSkipped={0}' -f $mergeSkipped)
Write-Output ('ChangedFiles={0}' -f $changedCount)

if ($failures.Count -gt 0) {
    foreach ($failure in $failures) { Write-Output ('FAIL: ' + $failure) }
    Write-Output ('PipelineScope=Failed; Failures={0}' -f $failures.Count)
    exit 1
}

Write-Output 'PipelineScope=Passed'
exit 0
