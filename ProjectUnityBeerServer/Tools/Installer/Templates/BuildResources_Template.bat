@echo off

set folder=Temp\Client.$BuildRevision.$BuildDate
set resfolder=..\..\Data\Resources
set LINKER=VS2015

rmdir "%folder%" /S /Q
mkdir "%folder%"

REM Store where the file was generated and when
echo %COMPUTERNAME% - %DATE% > "%folder%\build.txt"

REM Copy the binary
xcopy ..\..\..\Build\VAClient.%LINKER%.Release.Win32\Bin\VAClient.exe %folder%\ /Y /Q

REM Copy the shaders
mkdir "%folder%\Assets\WindowsShaders"
xcopy %resfolder%\WindowsShaders\*.* %folder%\Assets\WindowsShaders /Y /Q

REM Copy the XML files
mkdir "%folder%\Assets\Gui"
xcopy %resfolder%\Gui\*.* %folder%\Assets\Gui /Y /Q

REM Copy the Graphics
mkdir "%folder%\Assets"
xcopy %resfolder%\*.dds %folder%\Assets /Y /Q
xcopy %resfolder%\*.png %folder%\Assets /Y /Q
xcopy %resfolder%\*.bmp %folder%\Assets /Y /Q