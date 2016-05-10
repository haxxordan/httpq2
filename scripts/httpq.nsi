;
; httpq.nis
;
; MakeNSIS v2.0b1
;
!define MAJOR "3"
!define MINOR "1"

; The name of the installer
Name "Winamp httpQ Plugin ${MAJOR}.${MINOR}"
OutFile "../httpq_v${MAJOR}.${MINOR}_win_installer.exe"
;InstallDir $PROGRAMFILES\Winamp\Plugins
InstallDirRegKey HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\Winamp" "UninstallString"
DirText "This will install Winamp httpQ Plugin ${MAJOR}.${MINOR} on your computer."
ComponentText "This will install Winamp httpQ Plugin ${MAJOR}.${MINOR} on your computer."
LicenseText "Press Page Down to see the rest of the agreement."
LicenseData ../LICENSE
Icon setup.ico

Section "Winamp httpQ Plugin (required)" sec1
  
  SetOutPath "$INSTDIR\Plugins"
  File "..\bin\gen_httpq.dll"

SectionEnd

Section "Documentation" sec2
  
  SetOutPath "$INSTDIR\Plugins\httpQ"

  File "..\LICENSE"
  File "..\doc\ReadMe.txt"
  File "..\doc\Winamp httpQ Plugin.url"

SectionEnd


!define SF_SELECTED   1
!define SF_SUBSEC     2
!define SF_SUBSECEND  4
!define SF_BOLD       8
!define SF_RO         16
!define SF_EXPAND     32
!define SF_OFF		  0xFFFFFFFE


Function .onInit
	Push $0

	SectionGetFlags ${sec1} $0
	IntOp $0 $0 | ${SF_SELECTED}
	IntOp $0 $0 | ${SF_RO}
	IntOp $0 $0 | ${SF_BOLD}
	SectionSetFlags ${sec1} $0

	SectionGetFlags ${sec2} $0
	IntOp $0 $0 & ${SF_OFF}
	SectionSetFlags ${sec2} $0

	Pop $0
FunctionEnd

