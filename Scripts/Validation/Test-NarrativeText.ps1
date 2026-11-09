#Requires -Version 5.1
<#
.SYNOPSIS
Checks the narrative text source structure without hashes or exact-quote gates.
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
$repositoryRoot = [IO.Path]::GetFullPath($Root)
$textPath = Join-Path $repositoryRoot 'spec/contracts/narrative/TEXT.csv'
if (-not [IO.File]::Exists($textPath)) {
    Write-Output 'FAIL: Missing spec/contracts/narrative/TEXT.csv'
    exit 1
}

$expectedHeader = 'id,speaker,scene_or_result,text,production_notes,status'
$lines = [IO.File]::ReadAllLines($textPath)
if ($lines.Count -lt 2) {
    Write-Output 'FAIL: Narrative text source is empty.'
    exit 1
}
if ($lines[0].Trim() -ne $expectedHeader) {
    Write-Output ('FAIL: Header must be: {0}' -f $expectedHeader)
    exit 1
}

$rows = @($lines[1..($lines.Count - 1)] | Where-Object { -not [string]::IsNullOrWhiteSpace($_) })
if ($rows.Count -eq 0) {
    Write-Output 'FAIL: Narrative text source has no entries.'
    exit 1
}

$failures = New-Object System.Collections.Generic.List[string]
$ids = New-Object 'System.Collections.Generic.HashSet[string]' ([StringComparer]::Ordinal)
$validStatuses = @('draft', 'final', 'retired')
$rowNumber = 1
foreach ($row in $rows) {
    $rowNumber++
    $fields = @()
    try {
        $parsed = $row | ConvertFrom-Csv -Header @('id', 'speaker', 'scene_or_result', 'text', 'production_notes', 'status')
        if (@($parsed).Count -ne 1) { throw 'Expected one record.' }
        $record = @($parsed)[0]
        $fields = @($record.id, $record.speaker, $record.scene_or_result, $record.text, $record.production_notes, $record.status)
    }
    catch {
        $failures.Add(('line {0}: cannot parse CSV record.' -f $rowNumber))
        continue
    }
    if ($fields.Count -ne 6) {
        $failures.Add(('line {0}: expected six fields.' -f $rowNumber))
        continue
    }
    $id = [string]$fields[0]
    if ($id -notmatch '^NAR-[0-9]{4}$') {
        $failures.Add(('line {0}: invalid stable id "{1}".' -f $rowNumber, $id))
    }
    elseif (-not $ids.Add($id)) {
        $failures.Add(('line {0}: duplicate id {1}.' -f $rowNumber, $id))
    }
    for ($i = 0; $i -lt 5; $i++) {
        if ([string]::IsNullOrWhiteSpace([string]$fields[$i])) {
            $failures.Add(('line {0}: required field {1} is empty.' -f $rowNumber, $i))
        }
    }
    if ($validStatuses -notcontains [string]$fields[5]) {
        $failures.Add(('line {0}: status must be draft, final or retired.' -f $rowNumber))
    }
}

Write-Output ('NarrativeEntries.Checked={0}' -f $rows.Count)
if ($failures.Count -gt 0) {
    foreach ($failure in $failures) { Write-Output ('FAIL: ' + $failure) }
    Write-Output ('NarrativeText=Failed; Failures={0}' -f $failures.Count)
    exit 1
}
Write-Output 'NarrativeText=Passed'
exit 0
