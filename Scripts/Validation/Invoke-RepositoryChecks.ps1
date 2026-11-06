#Requires -Version 7.0
<#
.SYNOPSIS
Checks the documentation and governance repository baseline only.
.DESCRIPTION
Run locally and in CI with the same command:
pwsh -NoLogo -NoProfile -File Scripts/Validation/Invoke-RepositoryChecks.ps1
-Root overrides the repository root (the default is two directories above this script).
No tools are installed and no UE, Linux, game, performance, or experience checks run.
#>
[CmdletBinding()]
param(
    [string] $Root = (Split-Path -Parent (Split-Path -Parent $PSScriptRoot))
)

Set-StrictMode -Version Latest
$ErrorActionPreference = 'Stop'

function Resolve-RepositoryPath {
    param([string] $RootPath, [string] $RelativePath)
    if ([string]::IsNullOrWhiteSpace($RelativePath) -or
        $RelativePath -match '^(?:[A-Za-z]:[\\/]|[\\/])') {
        throw 'Expected a nonempty repository-relative path.'
    }
    $nativePath = $RelativePath.Replace('\', [IO.Path]::DirectorySeparatorChar).
        Replace('/', [IO.Path]::DirectorySeparatorChar)
    $fullPath = [IO.Path]::GetFullPath((Join-Path $RootPath $nativePath))
    $relative = [IO.Path]::GetRelativePath($RootPath, $fullPath)
    if ($relative -eq '..' -or $relative -match '^\.\.[\\/]' -or
        [IO.Path]::IsPathRooted($relative)) {
        throw 'Relative path leaves the repository.'
    }
    return $fullPath
}

function Test-ConfirmationSource {
    param(
        [string] $RootPath,
        [string] $Path,
        [string] $Sha256,
        [string] $Quote
    )
    # Return only diagnostics, never the user's quote or source contents.
    try {
        $sourcePath = Resolve-RepositoryPath -RootPath $RootPath -RelativePath $Path
        if ($Path -match '(?i)(^|[\\/])(?:\.git|\.agents)([\\/]|$)' -or
            $Path -match '(?i)(^|[\\/])WORKING(?:[.\\/]|$)') {
            throw 'A confirmation source must be a separate repository evidence file.'
        }
        if (-not [IO.File]::Exists($sourcePath)) {
            throw 'Confirmation source file is missing.'
        }
        if ($Sha256 -notmatch '^[A-Fa-f0-9]{64}$') {
            throw 'Expected a 64-character SHA-256 digest.'
        }
        $actual = (Get-FileHash -LiteralPath $sourcePath -Algorithm SHA256).Hash
        if (-not [string]::Equals($actual, $Sha256, [StringComparison]::OrdinalIgnoreCase)) {
            throw 'SHA-256 mismatch.'
        }
        if ([string]::IsNullOrWhiteSpace($Quote)) {
            throw 'Confirmation quote is empty.'
        }
        $content = [IO.File]::ReadAllText($sourcePath)
        if (-not $content.Contains($Quote, [StringComparison]::Ordinal)) {
            throw 'Exact confirmation quote is absent from its source.'
        }
        return [pscustomobject]@{ Passed = $true; Path = $Path; Detail = 'Verified' }
    }
    catch {
        return [pscustomobject]@{ Passed = $false; Path = $Path; Detail = $_.Exception.Message }
    }
}

function Get-ConfirmationSourceReferences {
    param([AllowNull()] $Value, [string] $PropertyName = '')
    # A malformed source must reach the validator too; collecting only complete
    # triples silently skips an individual source with a deleted field.
    if ($PropertyName -eq 'source' -and $Value -isnot [System.Collections.IDictionary]) {
        ,@{}
        return
    }
    if ($Value -is [System.Collections.IDictionary]) {
        if ($PropertyName -eq 'source' -or $Value.Contains('sha256') -or $Value.Contains('quote')) {
            ,$Value
        }
        if ($PropertyName -match '(?i)evidence$' -and $Value.Contains('kind') -and
            -not $Value.Contains('source')) {
            ,@{}
        }
        foreach ($entry in $Value.GetEnumerator()) {
            Get-ConfirmationSourceReferences -Value $entry.Value -PropertyName $entry.Key
        }
    }
    elseif ($Value -is [System.Collections.IEnumerable] -and $Value -isnot [string]) {
        foreach ($child in $Value) {
            Get-ConfirmationSourceReferences -Value $child
        }
    }
}

function Remove-MarkdownCode {
    param([string] $Text)
    # Preserve character positions/newlines so diagnostics still name the source line.
    $output = [Text.StringBuilder]::new()
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

    # Strip URL fragment/query and optional editor :line or :line:column suffix.
    # Do this before URI classification: README.md:12 is a file, not a URI scheme.
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
            $nativeAbsolute = ($IsWindows -and $windowsAbsolute) -or
                (-not $IsWindows -and $destination.StartsWith('/'))
            if ($nativeAbsolute) {
                $nativePath = $destination.Replace('\', [IO.Path]::DirectorySeparatorChar).
                    Replace('/', [IO.Path]::DirectorySeparatorChar)
                $absolute = [IO.Path]::GetFullPath($nativePath)
                $relative = [IO.Path]::GetRelativePath($RootPath, $absolute)
                if ($relative -ne '..' -and $relative -notmatch '^\.\.[\\/]' -and
                    -not [IO.Path]::IsPathRooted($relative)) {
                    $exists = [IO.File]::Exists($absolute) -or [IO.Directory]::Exists($absolute)
                    return [pscustomobject]@{
                        Kind = 'Local'; Passed = $exists
                        Detail = $(if ($exists) { '' } else { 'Missing local link target: ' + $Target })
                    }
                }
            }
            # References outside the checkout (or for another OS) are environment
            # evidence, not portable repository dependencies.
            return [pscustomobject]@{
                Kind = 'EnvironmentReference'; Passed = $true
                Detail = 'NotValidated: absolute environment reference'
            }
        }
        if ($destination -eq '') {
            return [pscustomobject]@{ Kind = 'Fragment'; Passed = $true; Detail = '' }
        }
        $documentDirectory = [IO.Path]::GetDirectoryName($DocumentPath)
        $fromRoot = [IO.Path]::GetRelativePath($RootPath, $documentDirectory)
        $combined = Join-Path $fromRoot $destination
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
    # Inline links/images, <paths with spaces>, escaped delimiters, and balanced
    # parentheses. Link titles are not part of the destination.
    $inlinePattern = '\]\(\s*(?<destination><[^>\r\n]+>|(?:\\.|[^\s()\\]|(?<parenthesis>\()|(?<-parenthesis>\)))+)(?(parenthesis)(?!))(?:[ \t]+(?:"[^"]*"|''[^'']*''|\([^)]*\)))?[ \t]*\)'
    $definitionPattern = '(?m)^[ \t]{0,3}\[(?<id>[^\]\r\n]+)\]:[ \t]*(?<destination><[^>\r\n]+>|\S+)'
    $definitions = [Collections.Generic.HashSet[string]]::new([StringComparer]::OrdinalIgnoreCase)
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

$failures = [Collections.Generic.List[string]]::new()
$documentsChecked = 0
$localLinksChecked = 0
$environmentReferences = 0
$sourcesChecked = 0
$requiredChecked = 0
$requiredDocuments = @(
    'AGENTS.md'
    'CLAUDE.md'
    'README.md'
    'spec/governance/WORKFLOW.md'
    'spec/governance/WRITE_SCOPE.md'
    'spec/governance/VALIDATION.md'
    'spec/progress/CURRENT.md'
    'spec/tasks/001-causal-prototype/TASKS.md'
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

    $documents = [Collections.Generic.List[string]]::new()
    foreach ($rootDocument in @('AGENTS.md', 'CLAUDE.md', 'README.md')) {
        $candidate = Join-Path $repositoryRoot $rootDocument
        if ([IO.File]::Exists($candidate)) { $documents.Add($candidate) }
    }
    $specRoot = Join-Path $repositoryRoot 'spec'
    if ([IO.Directory]::Exists($specRoot)) {
        # PowerShell does not recurse into directory symlinks without -FollowSymlink.
        foreach ($file in Get-ChildItem -LiteralPath $specRoot -Filter '*.md' -File -Recurse) {
            if ($file.Name -notmatch '^WORKING(?:\.|$)') {
                $documents.Add($file.FullName)
            }
        }
    }
    foreach ($document in $documents) {
        $documentsChecked++
        $relativeDocument = [IO.Path]::GetRelativePath($repositoryRoot, $document)
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

    $workingPath = Join-Path $repositoryRoot 'spec/architecture/WORKING.json'
    if (-not [IO.File]::Exists($workingPath)) {
        $failures.Add('Missing confirmation index: spec/architecture/WORKING.json')
    }
    else {
        try {
            $working = [IO.File]::ReadAllText($workingPath) | ConvertFrom-Json -AsHashtable -Depth 100
            $sources = @(Get-ConfirmationSourceReferences -Value $working)
            if ($sources.Count -eq 0) {
                $failures.Add('Confirmation index contains no path/sha256/quote source records.')
            }
            foreach ($source in $sources) {
                $sourcesChecked++
                if (-not $source.Contains('path') -or -not $source.Contains('sha256') -or
                    -not $source.Contains('quote')) {
                    $failures.Add('Confirmation source is missing path/sha256/quote fields.')
                    continue
                }
                if ($source.path -isnot [string] -or $source.sha256 -isnot [string] -or
                    $source.quote -isnot [string]) {
                    $failures.Add('Confirmation source has non-string path/sha256/quote fields.')
                    continue
                }
                $result = Test-ConfirmationSource -RootPath $repositoryRoot -Path $source.path -Sha256 $source.sha256 -Quote $source.quote
                if (-not $result.Passed) {
                    $failures.Add(('Confirmation source {0}: {1}' -f $result.Path, $result.Detail))
                }
            }
        }
        catch {
            # Parser exceptions can embed JSON text; do not expose quotes through them.
            $failures.Add('Unable to parse or inspect spec/architecture/WORKING.json.')
        }
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
Write-Output ('ConfirmationSources.Checked={0}' -f $sourcesChecked)
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
