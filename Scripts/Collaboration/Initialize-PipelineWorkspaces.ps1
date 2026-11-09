#Requires -Version 5.1
<#
.SYNOPSIS
Creates or refreshes the seven fixed pipeline worktrees with sparse-checkout.
.DESCRIPTION
Existing worktrees are never deleted or reset. Only a missing worktree is added.
The sparse file is local to each worktree and is not committed.
#>
[CmdletBinding()]
param(
    [string] $Root = '',
    [string[]] $Names = @()
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

function Invoke-Git {
    param([string] $RepositoryRoot, [string[]] $Arguments)
    $previousPreference = $ErrorActionPreference
    $ErrorActionPreference = 'Continue'
    try {
        $output = & git -C $RepositoryRoot @Arguments 2>&1
        $exitCode = $LASTEXITCODE
    }
    finally {
        $ErrorActionPreference = $previousPreference
    }
    if ($exitCode -ne 0) {
        throw ('git {0} failed: {1}' -f ($Arguments -join ' '), (($output | Out-String).Trim()))
    }
    return $output
}

function Get-CanonicalRoot {
    param([string] $StartRoot)
    $lines = & git -C $StartRoot worktree list --porcelain 2>$null
    if ($LASTEXITCODE -ne 0) {
        throw 'Unable to read the Git worktree list.'
    }
    foreach ($line in @($lines)) {
        if ($line -like 'worktree *') {
            return [IO.Path]::GetFullPath($line.Substring(9))
        }
    }
    throw 'The canonical worktree could not be identified.'
}

$startRoot = Get-RepositoryRoot -RequestedRoot $Root
$repositoryRoot = Get-CanonicalRoot -StartRoot $startRoot
$configPath = Join-Path $startRoot 'Scripts/Collaboration/pipelines.json'
if (-not [IO.File]::Exists($configPath)) {
    $configPath = Join-Path $repositoryRoot 'Scripts/Collaboration/pipelines.json'
}
$config = [IO.File]::ReadAllText($configPath) | ConvertFrom-Json
$allNames = @($config.pipelines.PSObject.Properties | ForEach-Object { $_.Name })
if ($Names.Count -eq 0) {
    $Names = $allNames
}

foreach ($name in $Names) {
    if ($allNames -notcontains $name) {
        throw ('Unknown pipeline: {0}' -f $name)
    }
    $definition = $config.pipelines.$name
    $branch = [string]$definition.branch
    $worktreeRelative = [string]$definition.worktree
    $worktreePath = [IO.Path]::GetFullPath((Join-Path $repositoryRoot $worktreeRelative))

    if (-not [IO.Directory]::Exists($worktreePath)) {
        New-Item -ItemType Directory -Force -Path (Split-Path -Parent $worktreePath) | Out-Null
        Invoke-Git -RepositoryRoot $repositoryRoot -Arguments @('worktree', 'add', $worktreePath, $branch) | Out-Null
        Write-Output ('Created {0} -> {1}' -f $worktreeRelative, $branch)
    }
    else {
        $actualBranch = ((Invoke-Git -RepositoryRoot $worktreePath -Arguments @('rev-parse', '--abbrev-ref', 'HEAD')) | Out-String).Trim()
        if ($actualBranch -ne $branch) {
            throw ('Worktree {0} is on {1}, expected {2}; refusing to reset it.' -f $worktreeRelative, $actualBranch, $branch)
        }
        Write-Output ('Reused {0} -> {1}' -f $worktreeRelative, $branch)
    }

    Invoke-Git -RepositoryRoot $worktreePath -Arguments @('sparse-checkout', 'init', '--no-cone') | Out-Null
    $sparsePath = ((Invoke-Git -RepositoryRoot $worktreePath -Arguments @('rev-parse', '--git-path', 'info/sparse-checkout')) | Out-String).Trim()
    if (-not [IO.Path]::IsPathRooted($sparsePath)) {
        $sparsePath = [IO.Path]::GetFullPath((Join-Path $worktreePath $sparsePath))
    }
    $sparseDirectory = Split-Path -Parent $sparsePath
    if (-not [IO.Directory]::Exists($sparseDirectory)) {
        New-Item -ItemType Directory -Force -Path $sparseDirectory | Out-Null
    }
    $patterns = @($definition.sparse | ForEach-Object { [string]$_ } | Select-Object -Unique)
    $utf8 = New-Object System.Text.UTF8Encoding($false)
    [IO.File]::WriteAllLines($sparsePath, [string[]]$patterns, $utf8)
    Invoke-Git -RepositoryRoot $worktreePath -Arguments @('sparse-checkout', 'reapply') | Out-Null
    Write-Output ('SparsePatterns={0}' -f $patterns.Count)
}

Write-Output 'PipelineWorkspaces=Ready'
