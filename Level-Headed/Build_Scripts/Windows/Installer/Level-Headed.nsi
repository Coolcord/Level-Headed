############################################################################################

!define APP_NAME "Level-Headed"
!define COMP_NAME "Coolcord"
!define VERSION "0.3.7.0"
!define COPYRIGHT ""
!define DESCRIPTION "Random Level Generator"
!define LICENSE_TXT "C:\Installer\LICENSE.txt"
!define INSTALLER_NAME "C:\Installer\Level-Headed.Setup.exe"
!define MAIN_APP_EXE "Level-Headed.exe"
!define INSTALL_TYPE "SetShellVarContext current"
!define REG_ROOT "HKCU"
!define REG_APP_PATH "Software\Microsoft\Windows\CurrentVersion\App Paths\${MAIN_APP_EXE}"
!define UNINSTALL_PATH "Software\Microsoft\Windows\CurrentVersion\Uninstall\${APP_NAME}"

######################################################################

VIProductVersion  "${VERSION}"
VIAddVersionKey "ProductName"  "${APP_NAME}"
VIAddVersionKey "CompanyName"  "${COMP_NAME}"
VIAddVersionKey "LegalCopyright"  "${COPYRIGHT}"
VIAddVersionKey "FileDescription"  "${DESCRIPTION}"
VIAddVersionKey "FileVersion"  "${VERSION}"

######################################################################

Unicode true
SetCompressor ZLIB
Name "${APP_NAME}"
Caption "${APP_NAME}"
OutFile "${INSTALLER_NAME}"
BrandingText "${APP_NAME}"
XPStyle on
InstallDirRegKey "${REG_ROOT}" "${REG_APP_PATH}" ""
InstallDir "$PROGRAMFILES\Coolcord\Level-Headed"

######################################################################

!include "MUI.nsh"

!define MUI_ABORTWARNING
!define MUI_UNABORTWARNING

!insertmacro MUI_PAGE_WELCOME

!ifdef LICENSE_TXT
!insertmacro MUI_PAGE_LICENSE "${LICENSE_TXT}"
!endif

!insertmacro MUI_PAGE_DIRECTORY

!ifdef REG_START_MENU
!define MUI_STARTMENUPAGE_NODISABLE
!define MUI_STARTMENUPAGE_DEFAULTFOLDER "Coolcord\Level-Headed"
!define MUI_STARTMENUPAGE_REGISTRY_ROOT "${REG_ROOT}"
!define MUI_STARTMENUPAGE_REGISTRY_KEY "${UNINSTALL_PATH}"
!define MUI_STARTMENUPAGE_REGISTRY_VALUENAME "${REG_START_MENU}"
!insertmacro MUI_PAGE_STARTMENU Application $SM_Folder
!endif

!insertmacro MUI_PAGE_INSTFILES

!define MUI_FINISHPAGE_RUN "$INSTDIR\${MAIN_APP_EXE}"
!insertmacro MUI_PAGE_FINISH

!insertmacro MUI_UNPAGE_CONFIRM

!insertmacro MUI_UNPAGE_INSTFILES

!insertmacro MUI_UNPAGE_FINISH

!insertmacro MUI_LANGUAGE "English"

######################################################################

Section -MainProgram
${INSTALL_TYPE}
SetOverwrite on
SetOutPath "$INSTDIR"
ExecWait "TASKKILL /F /IM Level-Headed.exe"

# Remove files from old versions
RmDir /r "$INSTDIR\Levels\SMB1"
RmDir /r "$INSTDIR\Plugins\Git"


File "C:\Installer\Level-Headed.exe"
File "C:\Installer\Files\libcrypto-1_1.dll"
File "C:\Installer\Files\libgcc_s_dw2-1.dll"
File "C:\Installer\Files\libssl-1_1.dll"
File "C:\Installer\Files\libstdc++-6.dll"
File "C:\Installer\Files\libwinpthread-1.dll"
File "C:\Installer\Files\qt.conf"
File "C:\Installer\Files\Qt5Core.dll"
File "C:\Installer\Files\Qt5Gui.dll"
File "C:\Installer\Files\Qt5Network.dll"
File "C:\Installer\Files\Qt5Widgets.dll"
SetOutPath "$INSTDIR\Data\SMB1"
File "C:\Installer\Files\Data\SMB1\Graphics.sa"
File "C:\Installer\Files\Data\SMB1\Music.sa"
File "C:\Installer\Files\Data\SMB1\ROMs.sa"
SetOutPath "$INSTDIR\Levels\SMB1"
File "C:\Installer\Files\Levels\SMB1\(Tech Demo) It Plays Itself.lvls"
File "C:\Installer\Files\Levels\SMB1\(Tech Demo) Powerup Test.lvls"
File "C:\Installer\Files\Levels\SMB1\(Tech Demo) Vertical Limit Test.lvls"
File "C:\Installer\Files\Levels\SMB1\Super Mario Bros. 1 (Original Levels without Castle Loops).lvls"
SetOutPath "$INSTDIR\Plugins"
File "C:\Installer\Files\Plugins\Hexagon.dll"
File "C:\Installer\Files\Plugins\Sequential_Archive.dll"
SetOutPath "$INSTDIR\Plugins\Generators"
File "C:\Installer\Files\Plugins\Generators\SMB1_Compliance_Generator.dll"
SetOutPath "$INSTDIR\Plugins\Interpreters"
File "C:\Installer\Files\Plugins\Interpreters\SMB1_Compliance_To_SMB1.dll"
SetOutPath "$INSTDIR\Plugins\Writers"
File "C:\Installer\Files\Plugins\Writers\SMB1_Writer.dll"
SetOutPath "$INSTDIR\Qt\platforms"
File "C:\Installer\Files\Qt\platforms\qwindows.dll"
SectionEnd

######################################################################

Section -Icons_Reg
SetOutPath "$INSTDIR"
WriteUninstaller "$INSTDIR\uninstall.exe"

!ifdef REG_START_MENU
!insertmacro MUI_STARTMENU_WRITE_BEGIN Application
CreateDirectory "$SMPROGRAMS\$SM_Folder"
CreateShortCut "$SMPROGRAMS\$SM_Folder\${APP_NAME}.lnk" "$INSTDIR\${MAIN_APP_EXE}"
!ifdef WEB_SITE
WriteIniStr "$INSTDIR\${APP_NAME} website.url" "InternetShortcut" "URL" "${WEB_SITE}"
CreateShortCut "$SMPROGRAMS\$SM_Folder\${APP_NAME} Website.lnk" "$INSTDIR\${APP_NAME} website.url"
!endif
!insertmacro MUI_STARTMENU_WRITE_END
!endif

!ifndef REG_START_MENU
CreateDirectory "$SMPROGRAMS\Coolcord\Level-Headed"
CreateShortCut "$SMPROGRAMS\Coolcord\Level-Headed\${APP_NAME}.lnk" "$INSTDIR\${MAIN_APP_EXE}"
!ifdef WEB_SITE
WriteIniStr "$INSTDIR\${APP_NAME} website.url" "InternetShortcut" "URL" "${WEB_SITE}"
CreateShortCut "$SMPROGRAMS\Coolcord\Level-Headed\${APP_NAME} Website.lnk" "$INSTDIR\${APP_NAME} website.url"
!endif
!endif

WriteRegStr ${REG_ROOT} "${REG_APP_PATH}" "" "$INSTDIR\${MAIN_APP_EXE}"
WriteRegStr ${REG_ROOT} "${UNINSTALL_PATH}"  "DisplayName" "${APP_NAME}"
WriteRegStr ${REG_ROOT} "${UNINSTALL_PATH}"  "UninstallString" "$INSTDIR\uninstall.exe"
WriteRegStr ${REG_ROOT} "${UNINSTALL_PATH}"  "DisplayIcon" "$INSTDIR\${MAIN_APP_EXE}"
WriteRegStr ${REG_ROOT} "${UNINSTALL_PATH}"  "DisplayVersion" "${VERSION}"
WriteRegStr ${REG_ROOT} "${UNINSTALL_PATH}"  "Publisher" "${COMP_NAME}"

!ifdef WEB_SITE
WriteRegStr ${REG_ROOT} "${UNINSTALL_PATH}"  "URLInfoAbout" "${WEB_SITE}"
!endif
SectionEnd

######################################################################

Section Uninstall
${INSTALL_TYPE}
ExecWait "TASKKILL /F /IM Level-Headed.exe"
Delete "$INSTDIR\${MAIN_APP_EXE}"
Delete "$INSTDIR\libcrypto-1_1.dll"
Delete "$INSTDIR\libgcc_s_dw2-1.dll"
Delete "$INSTDIR\libssl-1_1.dll"
Delete "$INSTDIR\libstdc++-6.dll"
Delete "$INSTDIR\libwinpthread-1.dll"
Delete "$INSTDIR\qt.conf"
Delete "$INSTDIR\Qt5Core.dll"
Delete "$INSTDIR\Qt5Gui.dll"
Delete "$INSTDIR\Qt5Network.dll"
Delete "$INSTDIR\Qt5Widgets.dll"
Delete "$INSTDIR\Data\SMB1\Graphics.sa"
Delete "$INSTDIR\Data\SMB1\Music.sa"
Delete "$INSTDIR\Data\SMB1\ROMs.sa"
Delete "$INSTDIR\Levels\SMB1\(Tech Demo) It Plays Itself.lvls"
Delete "$INSTDIR\Levels\SMB1\(Tech Demo) Powerup Test.lvls"
Delete "$INSTDIR\Levels\SMB1\(Tech Demo) Vertical Limit Test.lvls"
Delete "$INSTDIR\Levels\SMB1\Super Mario Bros. 1 (Original Levels without Castle Loops).lvls"
Delete "$INSTDIR\Plugins\Hexagon.dll"
Delete "$INSTDIR\Plugins\Sequential_Archive.dll"
Delete "$INSTDIR\Plugins\Generators\SMB1_Compliance_Generator.dll"
Delete "$INSTDIR\Plugins\Interpreters\SMB1_Compliance_To_SMB1.dll"
Delete "$INSTDIR\Plugins\Writers\SMB1_Writer.dll"
Delete "$INSTDIR\Qt\platforms\qwindows.dll"
Delete "$INSTDIR\Config\Level-Headed.cfg"
Delete "$INSTDIR\Config\SMB1_Compliance_To_SMB1.cfg"

RmDir /r "$INSTDIR\Config"
RmDir /r "$INSTDIR\Data\SMB1"
RmDir /r "$INSTDIR\Data"
RmDir /r "$INSTDIR\Levels\SMB1"
RmDir /r "$INSTDIR\Levels"
RmDir /r "$INSTDIR\Plugins\Generators"
RmDir /r "$INSTDIR\Plugins\Interpreters"
RmDir /r "$INSTDIR\Plugins\Writers"
RmDir /r "$INSTDIR\Plugins"
RmDir /r "$INSTDIR\Qt\platforms"
RmDir /r "$INSTDIR\Qt"
 
Delete "$INSTDIR\uninstall.exe"
!ifdef WEB_SITE
Delete "$INSTDIR\${APP_NAME} website.url"
!endif

RmDir "$INSTDIR"

!ifdef REG_START_MENU
!insertmacro MUI_STARTMENU_GETFOLDER "Application" $SM_Folder
Delete "$SMPROGRAMS\$SM_Folder\${APP_NAME}.lnk"
!ifdef WEB_SITE
Delete "$SMPROGRAMS\$SM_Folder\${APP_NAME} Website.lnk"
!endif
RmDir "$SMPROGRAMS\$SM_Folder"
!endif

!ifndef REG_START_MENU
Delete "$SMPROGRAMS\Coolcord\Level-Headed\${APP_NAME}.lnk"
!ifdef WEB_SITE
Delete "$SMPROGRAMS\Coolcord\Level-Headed\${APP_NAME} Website.lnk"
!endif
RmDir "$SMPROGRAMS\Coolcord\Level-Headed"
!endif

DeleteRegKey ${REG_ROOT} "${REG_APP_PATH}"
DeleteRegKey ${REG_ROOT} "${UNINSTALL_PATH}"
SectionEnd

######################################################################


