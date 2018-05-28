@echo off
if not "%DF_EXTERNALLIBS%" == "" (
  REM Clean build release solution
  set STARTDIR=%~dp0..\..\..
  set ZLIB=%DF_EXTERNALLIBS%\zlib-1.2.5
  set PNGLIB=%DF_EXTERNALLIBS%\libpng-1.6.7
  set GLEW=%DF_EXTERNALLIBS%\glew
  set BOOST=%DF_EXTERNALLIBS%\boost-1.59.0

  set LINKER=VS2015
  set INCLUDEDIR=%DF_EXTERNALLIBS%\boost-1.59.0\include\;%DF_EXTERNALLIBS%\glew\include;%DF_EXTERNALLIBS%\zlib-1.2.5;%DF_EXTERNALLIBS%\libpng-1.6.7
  set PREPROCESSOR=CLIENT;WIN32;BOOST_ALL_NO_LIB

  set versionfile=..\..\..\EvilEngine\CoreVersion\version.txt
  more %versionfile%

  "C:\Program Files (x86)\Microsoft Visual Studio 14.0\Common7\IDE\DevEnv" "..\..\Client\Windows\VAClient.sln" "..\..\Client\Windows\Client.vcxproj" /rebuild Release /projectconfig "Win32"
)
