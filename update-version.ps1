# PowerShell script to update version in BotWebServer Program.cs
param(
    [Parameter(Mandatory=$false)]
    [string]$Version,
    
    [Parameter(Mandatory=$false)]
    [switch]$AutoIncrement,
    
    [Parameter(Mandatory=$false)]
    [switch]$Help
)

if ($Help) {
    Write-Host "Update Version Script for BotWebServer"
    Write-Host ""
    Write-Host "Usage:"
    Write-Host "  .\update-version.ps1 -Version '1.2.3'      # Set specific version"
    Write-Host "  .\update-version.ps1 -AutoIncrement        # Increment patch version"
    Write-Host "  .\update-version.ps1 -Help                 # Show this help"
    Write-Host ""
    exit 0
}

$programFile = "BotWebServer\Program.cs"

if (-not (Test-Path $programFile)) {
    Write-Error "Program.cs file not found at: $programFile"
    exit 1
}

# Read current content
$content = Get-Content $programFile -Raw

# Extract current version
$currentVersionMatch = $content | Select-String 'public static string Version = "([^"]*)"'
if (-not $currentVersionMatch) {
    Write-Error "Could not find version string in Program.cs"
    exit 1
}

$currentVersion = $currentVersionMatch.Matches[0].Groups[1].Value
Write-Host "Current version: $currentVersion"

# Determine new version
if ($Version) {
    $newVersion = $Version
} elseif ($AutoIncrement) {
    $versionParts = $currentVersion.Split('.')
    if ($versionParts.Count -ne 3) {
        Write-Error "Current version format is not semantic versioning (x.y.z): $currentVersion"
        exit 1
    }
    
    $major = [int]$versionParts[0]
    $minor = [int]$versionParts[1]
    $patch = [int]$versionParts[2]
    $patch++
    
    $newVersion = "$major.$minor.$patch"
} else {
    Write-Error "Please specify either -Version or -AutoIncrement"
    Write-Host "Use -Help for usage information"
    exit 1
}

Write-Host "New version: $newVersion"

# Update the version in the file
$newContent = $content -replace 'public static string Version = "[^"]*";', "public static string Version = `"$newVersion`";"

# Write back to file
Set-Content $programFile -Value $newContent -NoNewline

Write-Host "✅ Version updated successfully in $programFile"

# Verify the change
$verifyContent = Get-Content $programFile -Raw
$verifyMatch = $verifyContent | Select-String 'public static string Version = "([^"]*)"'
if ($verifyMatch -and $verifyMatch.Matches[0].Groups[1].Value -eq $newVersion) {
    Write-Host "✅ Verification successful: Version is now $newVersion"
} else {
    Write-Error "❌ Verification failed: Version update may not have worked correctly"
    exit 1
}