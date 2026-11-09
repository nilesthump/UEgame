#Requires -Version 5.1
<#
.SYNOPSIS
Checks the repository documentation, governance and release baseline only.
.DESCRIPTION
Run locally and in CI with the same command:
powershell -NoProfile -File Scripts/Validation/Invoke-RepositoryChecks.ps1
No source hashes or exact confirmation quotes are validated. No UE, Linux, game,
performance, or experience checks run here.
#>
[CmdletBinding()]
param(
    [string] $Root = ''
)

Set-StrictMode -Version Latest
$ErrorActionPreference = 'Stop'
if ([string]::IsNullOrWhiteSpace($Root)) {
    $Root = Split-Path -Parent (Split-Path -Parent $PSScriptRoot)
}

$script:CaseInsensitivePaths = ([IO.Path]::DirectorySeparatorChar -eq '\')

function Get-RepoRelativePath {
    param([string] $RootPath, [string] $FullPath)
    $rootFull = [IO.Path]::GetFullPath($RootPath).TrimEnd('\', '/')
    $full = [IO.Path]::GetFullPath($FullPath)
    if ($full.Length -le $rootFull.Length) {
        if ($full -eq $rootFull) { return '.' }
        throw 'Path is outside the repository.'
    }
    $prefix = $rootFull + [IO.Path]::DirectorySeparatorChar
    $comparison = if ($script:CaseInsensitivePaths) { [StringComparison]::OrdinalIgnoreCase } else { [StringComparison]::Ordinal }
    if (-not $full.StartsWith($prefix, $comparison)) {
        throw 'Path is outside the repository.'
    }
    return $full.Substring($prefix.Length).Replace('\', '/')
}

function Resolve-RepositoryPath {
    param([string] $RootPath, [string] $RelativePath)
    if ([string]::IsNullOrWhiteSpace($RelativePath) -or
        $RelativePath -match '^(?:[A-Za-z]:[\\/]|[\\/])') {
        throw 'Expected a nonempty repository-relative path.'
    }
    $nativePath = $RelativePath.Replace('\', [IO.Path]::DirectorySeparatorChar).
        Replace('/', [IO.Path]::DirectorySeparatorChar)
    $fullPath = [IO.Path]::GetFullPath((Join-Path $RootPath $nativePath))
    $relative = Get-RepoRelativePath -RootPath $RootPath -FullPath $fullPath
    if ($relative -eq '..' -or $relative -match '^\.\./') {
        throw 'Relative path leaves the repository.'
    }
    return $fullPath
}

function Remove-MarkdownCode {
    param([string] $Text)
    $output = New-Object System.Text.StringBuilder
    $fenceCharacter = ''
    $fenceLength = 0
    foreach ($line in [regex]::Split($Text, '(?<=\n)')) {
        if ($fenceLength -gt 0) {
            [void] $output.Append([regex]::Replace($line, '[^\r\n]', ' '))
            $closingPattern = '^[ \t]{0,3}' + [regex]::Escape($fenceCharacter) +
                '{' + $fenceLength + ',}[ \t]*\r?\n?$'
            if ($line -match $closingPattern) {
                $fenceLength = 0
            }
        }
        elseif ($line -match '^[ \t]{0,3}(?<fence>`{3,}|~{3,})') {
            $fenceCharacter = $Matches.fence.Substring(0, 1)
            $fenceLength = $Matches.fence.Length
            [void] $output.Append([regex]::Replace($line, '[^\r\n]', ' '))
        }
        else {
            [void] $output.Append($line)
        }
    }
    $masked = [regex]::Replace($output.ToString(), '(?s)<!--.*?-->', {
        param($match)
        [regex]::Replace($match.Value, '[^\r\n]', ' ')
    })
    return [regex]::Replace($masked, '(`+)[^\r\n]*?\1(?!`)', {
        param($match)
        [regex]::Replace($match.Value, '[^\r\n]', ' ')
    })
}

function Test-MarkdownTarget {
    param([string] $RootPath, [string] $DocumentPath, [string] $Target)
    $destination = $Target.Trim()
    if ($destination.StartsWith('<') -and $destination.EndsWith('>')) {
        $destination = $destination.Substring(1, $destination.Length - 2)
    }
    if ($destination -eq '' -or $destination.StartsWith('#')) {
        return [pscustomobject]@{ Kind = 'Fragment'; Passed = $true; Detail = '' }
    }

    $destination = ($destination -split '[#?]', 2)[0] -replace ':\d+(?::\d+)?$', ''
    if ($destination -match '^[A-Za-z][A-Za-z0-9+.-]*:' -and
        $destination -notmatch '^[A-Za-z]:[\\/]') {
        return [pscustomobject]@{ Kind = 'External'; Passed = $true; Detail = 'NotValidated' }
    }
    try {
        $destination = [Uri]::UnescapeDataString($destination)
        $destination = [regex]::Replace($destination, '\\([ ()<>])', '$1')
        if ($destination -match '^(?:[A-Za-z]:[\\/]|[\\/])') {
            $windowsAbsolute = $destination -match '^(?:[A-Za-z]:[\\/]|\\\\)'
            $nativeAbsolute = ($script:CaseInsensitivePaths -and $windowsAbsolute) -or
                (-not $script:CaseInsensitivePaths -and $destination.StartsWith('/'))
            if ($nativeAbsolute) {
                $nativePath = $destination.Replace('\', [IO.Path]::DirectorySeparatorChar).
                    Replace('/', [IO.Path]::DirectorySeparatorChar)
                $absolute = [IO.Path]::GetFullPath($nativePath)
                try {
                    $relative = Get-RepoRelativePath -RootPath $RootPath -FullPath $absolute
                    $exists = [IO.File]::Exists($absolute) -or [IO.Directory]::Exists($absolute)
                    return [pscustomobject]@{
                        Kind = 'Local'; Passed = $exists
                        Detail = $(if ($exists) { '' } else { 'Missing local link target: ' + $Target })
                    }
                }
                catch {
                    return [pscustomobject]@{
                        Kind = 'EnvironmentReference'; Passed = $true
                        Detail = 'NotValidated: absolute environment reference'
                    }
                }
            }
            return [pscustomobject]@{
                Kind = 'EnvironmentReference'; Passed = $true
                Detail = 'NotValidated: absolute environment reference'
            }
        }
        if ($destination -eq '') {
            return [pscustomobject]@{ Kind = 'Fragment'; Passed = $true; Detail = '' }
        }
        $documentRelative = Get-RepoRelativePath -RootPath $RootPath -FullPath $DocumentPath
        $documentDirectory = [IO.Path]::GetDirectoryName($documentRelative)
        if ([string]::IsNullOrWhiteSpace($documentDirectory)) {
            $combined = $destination
        }
        else {
            $combined = Join-Path $documentDirectory $destination
        }
        $resolved = Resolve-RepositoryPath -RootPath $RootPath -RelativePath $combined
        $exists = [IO.File]::Exists($resolved) -or [IO.Directory]::Exists($resolved)
        return [pscustomobject]@{
            Kind = 'Local'; Passed = $exists
            Detail = $(if ($exists) { '' } else { 'Missing local link target: ' + $Target })
        }
    }
    catch {
        return [pscustomobject]@{ Kind = 'Local'; Passed = $false; Detail = $_.Exception.Message }
    }
}

function Get-MarkdownLinks {
    param([string] $Text)
    $masked = Remove-MarkdownCode -Text $Text
    $inlinePattern = '\]\(\s*(?<destination><[^>\r\n]+>|(?:\\.|[^\s()\\]|(?<parenthesis>\()|(?<-parenthesis>\)))+)(?(parenthesis)(?!))(?:[ \t]+(?:"[^"]*"|''[^'']*''|\([^)]*\)))?[ \t]*\)'
    $definitionPattern = '(?m)^[ \t]{0,3}\[(?<id>[^\]\r\n]+)\]:[ \t]*(?<destination><[^>\r\n]+>|\S+)'
    $definitions = New-Object 'System.Collections.Generic.HashSet[string]' ([StringComparer]::OrdinalIgnoreCase)
    foreach ($match in [regex]::Matches($masked, $definitionPattern)) {
        $id = [regex]::Replace($match.Groups['id'].Value.Trim(), '\s+', ' ')
        [void] $definitions.Add($id)
        [pscustomobject]@{
            Target = $match.Groups['destination'].Value
            Line = 1 + [regex]::Matches($masked.Substring(0, $match.Index), '\n').Count
            Error = ''
        }
    }
    foreach ($match in [regex]::Matches($masked, $inlinePattern)) {
        [pscustomobject]@{
            Target = $match.Groups['destination'].Value
            Line = 1 + [regex]::Matches($masked.Substring(0, $match.Index), '\n').Count
            Error = ''
        }
    }
    foreach ($match in [regex]::Matches($masked, '\[(?<label>[^\]\r\n]+)\][ \t]*\[(?<id>[^\]\r\n]*)\]')) {
        $id = $match.Groups['id'].Value
        if ($id -eq '') { $id = $match.Groups['label'].Value }
        $id = [regex]::Replace($id.Trim(), '\s+', ' ')
        if (-not $definitions.Contains($id)) {
            [pscustomobject]@{
                Target = ''
                Line = 1 + [regex]::Matches($masked.Substring(0, $match.Index), '\n').Count
                Error = 'Undefined Markdown link reference: ' + $id
            }
        }
    }
}

function Read-JsonDocument {
    param([string] $RootPath, [string] $RelativePath)
    $path = Resolve-RepositoryPath -RootPath $RootPath -RelativePath $RelativePath
    if (-not [IO.File]::Exists($path)) {
        throw ('Missing JSON document: {0}' -f $RelativePath)
    }
    try {
        return ([IO.File]::ReadAllText($path) | ConvertFrom-Json)
    }
    catch {
        throw ('Invalid JSON: {0}' -f $RelativePath)
    }
}

$failures = New-Object System.Collections.Generic.List[string]
$documentsChecked = 0
$localLinksChecked = 0
$environmentReferences = 0
$requiredChecked = 0
$requiredDocuments = @(
    'AGENTS.md'
    'CLAUDE.md'
    'README.md'
    'spec/pipelines/README.md'
    'spec/pipelines/PROTOCOL.md'
    'spec/pipelines/growth/CURRENT.md'
    'spec/pipelines/narrative/CURRENT.md'
    'spec/pipelines/tech/CURRENT.md'
    'spec/pipelines/art/CURRENT.md'
    'spec/pipelines/test/CURRENT.md'
    'spec/pipelines/ops/CURRENT.md'
    'spec/pipelines/maintenance/CURRENT.md'
    'spec/governance/WORKFLOW.md'
    'spec/governance/WRITE_SCOPE.md'
    'spec/governance/VALIDATION.md'
    'spec/governance/DIRECTORY.md'
    'spec/governance/BRANCH_RULES.md'
    'spec/governance/validation.json'
    'spec/progress/CURRENT.md'
    'spec/evidence/evidence.md'
    'spec/releases/README.md'
    'spec/releases/state.json'
    'spec/releases/0.1.0/PLAN.md'
    'spec/contracts/narrative/TEXT-SOURCE.md'
    'spec/contracts/narrative/TEXT.csv'
    'spec/contracts/art/README.md'
    'spec/tasks/001-causal-prototype/TASKS.md'
    '.github/workflows/repository-checks.yml'
)

try {
    $repositoryRoot = [IO.Path]::GetFullPath($Root)
    if (-not [IO.Directory]::Exists($repositoryRoot)) {
        throw 'Repository root directory is missing.'
    }
    foreach ($required in $requiredDocuments) {
        $requiredChecked++
        $requiredPath = Resolve-RepositoryPath -RootPath $repositoryRoot -RelativePath $required
        if (-not [IO.File]::Exists($requiredPath)) {
            $failures.Add('Missing required document: ' + $required)
        }
        elseif ([string]::IsNullOrWhiteSpace([IO.File]::ReadAllText($requiredPath))) {
            $failures.Add('Empty required document: ' + $required)
        }
    }

    $documents = New-Object System.Collections.Generic.List[string]
    foreach ($rootDocument in @('AGENTS.md', 'CLAUDE.md', 'README.md')) {
        $candidate = Join-Path $repositoryRoot $rootDocument
        if ([IO.File]::Exists($candidate)) { $documents.Add($candidate) }
    }
    $specRoot = Join-Path $repositoryRoot 'spec'
    if ([IO.Directory]::Exists($specRoot)) {
        foreach ($file in Get-ChildItem -LiteralPath $specRoot -Filter '*.md' -File -Recurse) {
            $documents.Add($file.FullName)
        }
    }
    foreach ($document in $documents) {
        $documentsChecked++
        $relativeDocument = Get-RepoRelativePath -RootPath $repositoryRoot -FullPath $document
        foreach ($link in @(Get-MarkdownLinks -Text ([IO.File]::ReadAllText($document)))) {
            if ($link.Error -ne '') {
                $failures.Add(('{0}:{1}: {2}' -f $relativeDocument, $link.Line, $link.Error))
                continue
            }
            $result = Test-MarkdownTarget -RootPath $repositoryRoot -DocumentPath $document -Target $link.Target
            if ($result.Kind -eq 'Local') { $localLinksChecked++ }
            if ($result.Kind -eq 'EnvironmentReference') { $environmentReferences++ }
            if (-not $result.Passed) {
                $failures.Add(('{0}:{1}: {2}' -f $relativeDocument, $link.Line, $result.Detail))
            }
        }
    }
    if ($documentsChecked -eq 0) {
        $failures.Add('No Markdown documents were checked.')
    }

    try {
        $null = Read-JsonDocument -RootPath $repositoryRoot -RelativePath 'spec/architecture/WORKING.json'
    }
    catch {
        $failures.Add($_.Exception.Message)
    }

    $state = $null
    try {
        $state = Read-JsonDocument -RootPath $repositoryRoot -RelativePath 'spec/releases/state.json'
    }
    catch {
        $failures.Add($_.Exception.Message)
    }
    if ($null -ne $state) {
        if ([string]$state.targetVersion -notmatch '^[0-9]+\.[0-9]+\.[0-9]+$') {
            $failures.Add('state.targetVersion must use x.y.z.')
        }
        if ($null -eq $state.PSObject.Properties['onlineVersion']) {
            $failures.Add('state.onlineVersion is required (null when unreleased).')
        }
        elseif ($null -ne $state.onlineVersion -and
            ([string]$state.onlineVersion -notmatch '^[0-9]+\.[0-9]+\.[0-9]+$')) {
            $failures.Add('state.onlineVersion must be null or x.y.z.')
        }
        if ($null -eq $state.PSObject.Properties['onlineTag']) {
            $failures.Add('state.onlineTag is required (null when unreleased).')
        }
        if ([string]::IsNullOrWhiteSpace([string]$state.status)) {
            $failures.Add('state.status is required.')
        }
    }

    $pipelineConfig = $null
    try {
        $pipelineConfig = Read-JsonDocument -RootPath $repositoryRoot -RelativePath 'Scripts/Collaboration/pipelines.json'
    }
    catch {
        $failures.Add($_.Exception.Message)
    }
    if ($null -ne $pipelineConfig) {
        $requiredPipelines = @('growth', 'narrative', 'tech', 'art', 'test', 'ops', 'maintenance')
        $seenBranches = New-Object 'System.Collections.Generic.HashSet[string]' ([StringComparer]::Ordinal)
        foreach ($name in $requiredPipelines) {
            if ($null -eq $pipelineConfig.pipelines.PSObject.Properties[$name]) {
                $failures.Add(('pipeline config is missing {0}.' -f $name))
                continue
            }
            $entry = $pipelineConfig.pipelines.$name
            if (-not $seenBranches.Add([string]$entry.branch)) {
                $failures.Add(('duplicate pipeline branch {0}.' -f $entry.branch))
            }
            $pipelineDoc = Join-Path $repositoryRoot ('spec/pipelines/{0}/CURRENT.md' -f $name)
            if (-not [IO.File]::Exists($pipelineDoc)) {
                $failures.Add(('missing pipeline document for {0}.' -f $name))
            }
        }
    }

    $parallelEvidence = @(Get-ChildItem -LiteralPath (Join-Path $repositoryRoot 'spec') -Recurse -File |
        Where-Object { $_.Name -match '^evidence-(latest|final)\.md$' })
    foreach ($file in $parallelEvidence) {
        $relative = Get-RepoRelativePath -RootPath $repositoryRoot -FullPath $file.FullName
        $failures.Add(('parallel evidence file is not allowed: {0}' -f $relative))
    }
}
catch {
    $failures.Add('Repository check could not complete: ' + $_.Exception.Message)
}

Write-Output 'Scope=RepositoryBaseline only'
Write-Output ('RequiredDocuments.Checked={0}' -f $requiredChecked)
Write-Output ('Markdown.DocumentsChecked={0}' -f $documentsChecked)
Write-Output ('Markdown.LocalLinksChecked={0}' -f $localLinksChecked)
Write-Output ('Markdown.EnvironmentReferences.NotValidated={0}' -f $environmentReferences)
Write-Output 'SourceHashGates=Removed'
Write-Output 'UE=NotRun'
Write-Output 'Linux=NotRun'
Write-Output 'Performance=NotRun'
Write-Output 'GameTests.Executed=0; GameTests.Status=NotRun'
Write-Output 'GameAcceptance=NotEvaluated'
if ($failures.Count -gt 0) {
    foreach ($failure in $failures) { Write-Output ('FAIL: ' + $failure) }
    Write-Output ('RepositoryBaseline=Failed; Failures={0}' -f $failures.Count)
    exit 1
}
Write-Output 'RepositoryBaseline=Passed'
exit 0
