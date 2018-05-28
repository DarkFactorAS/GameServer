rem @echo off
if not "%DF_EXTERNALLIBS%" == "" (
  REM Build release solution
  set THISDIR=%~dp0
  set STARTDIR=%~dp0..\..\..
  set ZLIB=%DF_EXTERNALLIBS%\zlib-1.2.5
  set PNGLIB=%DF_EXTERNALLIBS%\libpng-1.6.7
  set GLEW=%DF_EXTERNALLIBS%\glew
  set BOOST=%DF_EXTERNALLIBS%\boost-1.59.0

  set LINKER=VS2015
  set INCLUDEDIR=%DF_EXTERNALLIBS%\boost-1.59.0\include\;%DF_EXTERNALLIBS%\glew\include;%DF_EXTERNALLIBS%\zlib-1.2.5;%DF_EXTERNALLIBS%\libpng-1.6.7
  set PREPROCESSOR=CLIENT;WIN32;BOOST_ALL_NO_LIB

  set versionfile=..\..\..\EvilEngine\CoreVersion\version.txt
  rem more %versionfile%

  echo Start directory %~dp0..\..\..

  REM Make sure to purge the .exe so that we are not running an old version
  del %~dp0..\..\..\Build\VAClient.%LINKER%.Release.Win32\Bin\VAClient.exe

  REM Build the solution
  REM "C:\Program Files (x86)\Microsoft Visual Studio 14.0\Common7\IDE\DevEnv" "..\..\Client\Windows\VAClient.sln" "..\..\Client\Windows\Client.vcxproj" /build Release /projectconfig "Win32"

  REM Launch the file
  cd %~dp0..\..\..\Build\
  cd VAClient.%LINKER%.Release.Win32
  rem cd %~dp0..\..\..\Build\VAClient.%LINKER%.Release.Win32\Bin\
  VAClient.exe -DEBUGCONSOLE
  cd %THISDIR%

  pause
)
