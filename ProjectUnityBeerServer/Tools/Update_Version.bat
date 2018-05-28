@echo off

REM Make sure we are using the right repository
set P4CONFIG=p4config.txt

if not "%DF_EXTERNALLIBS%" == "" (

  REM Start the p4 operation after we have started Visual Studio to make it "optional"
  p4 changes -m1 //depot/Multiplatform/Projects/ValyrianAdventures/... > ..\EvilEngine\CoreVersion\versiontmp.txt
  python ..\EvilEngine\CoreVersion\PerforceScripts/convert_p4version_to_version.py ..\EvilEngine\CoreVersion\versiontmp.txt ..\EvilEngine\CoreVersion\versiontmp.txt ..\EvilEngine\CoreVersion\version.txt
  more ..\EvilEngine\CoreVersion\version.txt
  pause

) else  (

  echo You must set DF_EXTERNALLIBS to point to the externallibraries folder!
  pause

)

