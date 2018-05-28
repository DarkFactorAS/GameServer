;Product Info
!define PRODUCT "Valyrian Adventures" ; Software name
!define CLIENT "VAClient"   ; Executable name
!define CREATOR "Darkfactor" ; Company name
!define PLATFORM "32"        ; Define what how many bit OS is run under
!define VERSION "$BuildRevision"	; What version we are creating from
CRCCheck On
; Script create for version 2.0rc1/final (from 12.jan.04) with GUI NSIS (c) by Dirk Paehl. Thank you for use my program

!include "MUI2.nsh"
 
;--------------------------------
;Configuration
  OutFile "${CLIENT}-${VERSION}-Win${PLATFORM}-installer.exe"

;Folder selection page
  InstallDir "$PROGRAMFILES\${PRODUCT} ${PLATFORM}"

;Remember install folder
  InstallDirRegKey HKCU "Software\${CREATOR}\${PRODUCT} ${PLATFORM}" ""

;--------------------------------
;Pages
  !insertmacro MUI_PAGE_DIRECTORY
  !insertmacro MUI_PAGE_INSTFILES
  !insertmacro MUI_UNPAGE_CONFIRM
  !insertmacro MUI_UNPAGE_INSTFILES
  
  !define MUI_FINISHPAGE_RUN
  !define MUI_FINISHPAGE_RUN_TEXT "Start ${PRODUCT} ? "
  !define MUI_FINISHPAGE_RUN_FUNCTION "LaunchProduct"
  !insertmacro MUI_PAGE_FINISH

 !define MUI_ABORTWARNING
 
;--------------------------------
 ;Language
 
  !insertmacro MUI_LANGUAGE "English"
;--------------------------------

     
Section "section_1" section_1
  SetOutPath "$INSTDIR"
  File /r "Client.$BuildRevision.$BuildDate\*.*"
  ;File /r "*.*"
  ;File "Windows${PLATFORM}\changelist.html"
SectionEnd

Section "section_2" section_2
  WriteRegDWORD HKCU "Software\${CREATOR}\${PRODUCT}\Installed" "${CLIENT}-${VERSION}-${PLATFORM}" 0x1
SectionEnd

Section Shortcuts
  SetShellVarContext all
  CreateDirectory "$SMPROGRAMS\${PRODUCT}"
  CreateShortCut "$SMPROGRAMS\${PRODUCT}\${PRODUCT} ${PLATFORM}.lnk" "$INSTDIR\${CLIENT}.exe"
SectionEnd

Section Uninstaller
  SetShellVarContext all
  CreateShortCut "$SMPROGRAMS\${PRODUCT}\Uninstall ${PRODUCT} ${PLATFORM}.lnk" "$INSTDIR\uninst.exe" "" "$INSTDIR\uninst.exe" 0
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${PRODUCT}" "DisplayName" "${PRODUCT} ${VERSION}"
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${PRODUCT}" "DisplayVersion" "${VERSION}"
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${PRODUCT}" "URLInfoAbout" "http://www.darkfactor.net/"
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${PRODUCT}" "Publisher" "${CREATOR}"
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${PRODUCT}" "UninstallString" "$INSTDIR\Uninst.exe"
  WriteRegStr HKCU "Software\${PRODUCT}" "" $INSTDIR
  WriteUninstaller "$INSTDIR\Uninst.exe" 
SectionEnd
 
 
Function un.onUninstSuccess
  HideWindow
  MessageBox MB_ICONINFORMATION|MB_OK "$(^PRODUCT) was successfully removed from your computer.."
FunctionEnd
  
Function un.onInit 
  MessageBox MB_ICONQUESTION|MB_YESNO|MB_DEFBUTTON2 "Are you sure you want to completely remove $(^PRODUCT) and all of its components?" IDYES +2
  Abort
FunctionEnd

Function LaunchProduct
  ExecShell "" "$INSTDIR\${CLIENT}.exe"
FunctionEnd
 
Section "Uninstall" 
  SetShellVarContext all
  Delete "$SMPROGRAMS\${PRODUCT}\${PRODUCT} ${PLATFORM}.lnk"
  Delete "$SMPROGRAMS\${PRODUCT}\Uninstall ${PRODUCT} ${PLATFORM}.lnk"
  RMDir "$SMPROGRAMS\${PRODUCT}"
  DeleteRegKey HKLM "SOFTWARE\${PRODUCT}${PLATFORM}"
  DeleteRegKey HKLM "SOFTWARE\Microsoft\Windows\CurrentVersion\Uninstall\${PRODUCT}${PLATFORM}"
  RMDir $INSTDIR                          
SectionEnd
               
   
;eof