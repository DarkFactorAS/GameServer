@echo off
:: Build script for BotWebServer with version management
:: Usage: build-botwebserver.cmd [/version:VERSION] [/auto-increment] [/skip-version] [/help]

setlocal enabledelayedexpansion

set "SCRIPT_DIR=%~dp0"
set "PROGRAM_FILE=%SCRIPT_DIR%BotWebServer\Program.cs"
set "VERSION="
set "AUTO_INCREMENT=false"
set "SKIP_VERSION=false"

:: Parse command line arguments
:parse_args
if "%~1"=="" goto end_parse
if "%~1"=="/help" goto show_help
if "%~1"=="/?" goto show_help
if "%~1"=="/skip-version" (
    set "SKIP_VERSION=true"
    shift
    goto parse_args
)
if "%~1"=="/auto-increment" (
    set "AUTO_INCREMENT=true"
    shift
    goto parse_args
)
if "%~1"=="/version:" (
    echo Error: Version parameter requires a value. Use /version:1.2.3
    exit /b 1
)
if "%~1"=="/version" (
    echo Error: Version parameter requires a value. Use /version:1.2.3
    exit /b 1
)
echo %~1 | findstr /r "^/version:" >nul
if %errorlevel%==0 (
    set "VERSION=%~1"
    set "VERSION=!VERSION:/version:=!"
    shift
    goto parse_args
)
echo Unknown option: %~1
echo Use /help for usage information
exit /b 1

:end_parse

:: Update version using PowerShell script
if "%SKIP_VERSION%"=="false" (
    if defined VERSION (
        echo Updating version to: !VERSION!
        powershell.exe -ExecutionPolicy Bypass -File "%SCRIPT_DIR%update-version.ps1" -Version "!VERSION!"
        if errorlevel 1 (
            echo Failed to update version
            exit /b 1
        )
    ) else if "%AUTO_INCREMENT%"=="true" (
        echo Auto-incrementing version...
        powershell.exe -ExecutionPolicy Bypass -File "%SCRIPT_DIR%update-version.ps1" -AutoIncrement
        if errorlevel 1 (
            echo Failed to auto-increment version
            exit /b 1
        )
    ) else (
        echo ℹ️  Keeping current version (use /auto-increment or /version:X.Y.Z to update)
    )
)

echo 🔨 Building BotWebServer...

:: Navigate to project directory
cd /d "%SCRIPT_DIR%BotWebServer"

:: Restore dependencies
echo 📦 Restoring NuGet packages...
dotnet restore
if errorlevel 1 (
    echo ❌ Failed to restore NuGet packages
    exit /b 1
)

:: Build the project
echo 🏗️  Building project...
dotnet build --configuration Release
if errorlevel 1 (
    echo ❌ Build failed!
    exit /b 1
)

echo ✅ Build completed successfully!

:: Display current version
for /f "tokens=2 delims=^" %%a in ('findstr /r "Version = \".*\"" "%PROGRAM_FILE%"') do (
    set "CURRENT_LINE=%%a"
)
for /f "tokens=2 delims=^"" %%b in ("!CURRENT_LINE!") do (
    echo 📋 Current version: %%b
)

goto :eof

:show_help
echo Build script for BotWebServer with version management
echo.
echo Usage: %~nx0 [OPTIONS]
echo.
echo Options:
echo   /version:VERSION     Set specific version (e.g., /version:1.2.3)
echo   /auto-increment      Increment patch version automatically
echo   /skip-version        Skip version update
echo   /help, /?           Show this help message
echo.
echo Examples:
echo   %~nx0                           Build with current version
echo   %~nx0 /auto-increment          Build and increment patch version
echo   %~nx0 /version:2.1.0          Build and set version to 2.1.0
echo.
exit /b 0