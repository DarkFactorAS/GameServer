@echo off

REM Make sure we are using the right repository
set P4CONFIG=..\..\p4config.txt
set script=..\..\..\EvilEngine\CoreVersion\PerforceScripts\convert_p4version_to_version.py
set versionfile=..\..\..\EvilEngine\CoreVersion\version.txt
set NsInstaller=%DF_EXTERNALLIB%\nsis-2.42

rmdir Temp /S /Q
mkdir Temp

REM Make sure we have rebuilt the client
call RecreateReleaseBuild.bat

REM Build the runscripts with the correct version number from the templates
more %versionfile%
python %script% %versionfile% Templates\BuildResources_Template.bat  Temp\BuildResources.bat
python %script% %versionfile% Templates\BuildInstaller_Template.nsi  Temp\BuildInstaller.nsi

REM Build the resources and pack the installer
call Temp\BuildResources.bat
call "%NSINSTALLER%\makensis.exe" Temp\BuildInstaller.nsi

echo "Installer complete ..."
pause