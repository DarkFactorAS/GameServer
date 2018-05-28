@echo off

REM Make sure we are using the right repository
set P4CONFIG=p4config.txt

if not "%DF_EXTERNALLIBS%" == "" (

  REM Setup the external libraries
  set STARTDIR=%~dp0..
  set ZLIB=%DF_EXTERNALLIBS%\zlib-1.2.5
  set BOOST=%DF_EXTERNALLIBS%\boost-1.59.0
  set MYSQL=%DF_EXTERNALLIBS%\mysql\mysql-connector-1.1.9
  set OPENSSL=%DF_EXTERNALLIBS%\openssl-1.0.0.e
  set CURL=%DF_EXTERNALLIBS%\libcurl-7.36.0

  set LINKER=VS2015
  set INCLUDEDIR=%DF_EXTERNALLIBS%\mysql\mysql-connector-1.1.9\include;%DF_EXTERNALLIBS%\boost-1.59.0\include\;%DF_EXTERNALLIBS%\zlib-1.2.5;%DF_EXTERNALLIBS%\openssl-1.0.0.e\include
  set PREPROCESSOR=SERVER;WIN32;_CONSOLE;BOOST_ALL_NO_LIB;DATABASE;CPPDBC_PUBLIC_FUNC;USE_OPENSSL

  Start "Unity B.E.E.R. Server" Server\Windows\UnityBeerServer.sln

  REM Start the p4 operation after we have started Visual Studio to make it "optional"
  p4 changes -m1 //depot/Multiplatform/Projects/UnityBeerServer/... > ..\EvilEngine\CoreVersion\versiontmp.txt
  python ..\EvilEngine\CoreVersion\PerforceScripts/convert_p4version_to_version.py ..\EvilEngine\CoreVersion\versiontmp.txt ..\EvilEngine\CoreVersion\versiontmp.txt ..\EvilEngine\CoreVersion\version.txt
  more ..\EvilEngine\CoreVersion\version.txt
) else  (

  echo You must set DF_EXTERNALLIBS to point to the externallibraries folder!
  pause

)
