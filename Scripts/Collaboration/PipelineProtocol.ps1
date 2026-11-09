# Shared PowerShell helpers for the fixed pipeline protocol.
# Dot-source this file; it defines functions and does not perform work at load time.

function Get-PipelineConfig {
    param([string] $RepositoryRoot)
    $configPath = Join-Path $RepositoryRoot 'Scripts/Collaboration/pipelines.json'
    if (-not [IO.File]::Exists($configPath)) {
        throw "Missing $configPath"
    }
    return ([IO.File]::ReadAllText($configPath) | ConvertFrom-Json)
}

function Get-PipelineEntry {
    param([object] $Config, [string] $Name)
    if ($Name -eq 'main') {
        return $null
    }
    foreach ($property in $Config.pipelines.PSObject.Properties) {
        if ($property.Name -eq $Name) {
            return [pscustomobject]@{ Name = $property.Name; Definition = $property.Value }
        }
    }
    throw ('Unknown pipeline: {0}' -f $Name)
}

function Get-PipelineNameFromBranch {
    param([object] $Config, [string] $Branch)
    foreach ($property in $Config.pipelines.PSObject.Properties) {
        if ($property.Value.branch -eq $Branch) {
            return $property.Name
        }
    }
    return $null
}

function Assert-MrDirection {
    param([object] $Config, [string] $From, [string] $To)
    if ($From -eq 'ops' -and $To -eq 'main') {
        return
    }
    if ($To -eq 'main') {
        throw 'Only ops may target main.'
    }
    $entry = Get-PipelineEntry -Config $Config -Name $From
    if (@($entry.Definition.mrTargets) -notcontains $To) {
        throw ('MR direction {0} -> {1} is not allowed.' -f $From, $To)
    }
}

function Test-PipelineRequestBody {
    param([string] $Body, [string] $TargetVersion)
    $required = @(
        '目标版本与工作项',
        '交付内容',
        '接收条件',
        '最新验证',
        '未满足项'
    )
    $errors = New-Object System.Collections.Generic.List[string]
    $lines = $Body -split "`r?`n"
    $headings = New-Object System.Collections.Generic.List[string]
    $headingLines = New-Object System.Collections.Generic.List[int]
    for ($i = 0; $i -lt $lines.Count; $i++) {
        if ($lines[$i] -match '^#{1,6}\s+(.+?)\s*$') {
            $headings.Add($Matches[1])
            $headingLines.Add($i)
        }
    }
    if ($headings.Count -ne 5) {
        $errors.Add(('PR body must contain exactly five headings; found {0}.' -f $headings.Count))
    }
    for ($i = 0; $i -lt [Math]::Min($headings.Count, 5); $i++) {
        if ($headings[$i] -ne $required[$i]) {
            $errors.Add(('Heading {0} must be "{1}", found "{2}".' -f ($i + 1), $required[$i], $headings[$i]))
        }
    }
    if ($headings.Count -eq 5) {
        for ($i = 0; $i -lt 5; $i++) {
            $start = $headingLines[$i] + 1
            if ($i -lt 4) { $end = $headingLines[$i + 1] - 1 } else { $end = $lines.Count - 1 }
            $content = (@($lines[$start..$end]) -join "`n").Trim()
            if ([string]::IsNullOrWhiteSpace($content)) {
                $errors.Add(('Section "{0}" is empty.' -f $required[$i]))
            }
        }
        $goalStart = $headingLines[0] + 1
        $goalEnd = $headingLines[1] - 1
        $goalText = (@($lines[$goalStart..$goalEnd]) -join "`n")
        if ($goalText -notmatch '(?m)^\s*-\s*版本：\s*([0-9]+\.[0-9]+\.[0-9]+)\s*$') {
            $errors.Add('目标版本与工作项 must contain "- 版本：x.y.z".')
        }
        elseif ($Matches[1] -ne $TargetVersion) {
            $errors.Add(('PR version {0} does not match targetVersion {1}.' -f $Matches[1], $TargetVersion))
        }
    }
    return [pscustomobject]@{ Passed = ($errors.Count -eq 0); Errors = @($errors) }
}

function Get-RequiredCheckState {
    param([object] $Pr)
    $result = @()
    foreach ($check in @($Pr.statusCheckRollup)) {
        if ($null -eq $check) { continue }
        $state = ''
        if ($null -ne $check.PSObject.Properties['conclusion'] -and $check.conclusion) {
            $state = [string]$check.conclusion
        }
        elseif ($null -ne $check.PSObject.Properties['state'] -and $check.state) {
            $state = [string]$check.state
        }
        $name = ''
        if ($null -ne $check.PSObject.Properties['name']) { $name = [string]$check.name }
        $result += [pscustomobject]@{ Name = $name; State = $state }
    }
    return $result
}
