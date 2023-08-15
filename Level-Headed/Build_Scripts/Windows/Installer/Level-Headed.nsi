############################################################################################

!define APP_NAME "Level-Headed"
!define COMP_NAME "Coolcord"
!define VERSION "0.4.0.0"
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
ExecWait "TASKKILL /F /IM Level-Headed.exe"

# --------------- BEGIN AUTO-GENERATED RMDIR SECTION --------------- #
RmDir /r "$INSTDIR\Data"
RmDir /r "$INSTDIR\Levels"
RmDir /r "$INSTDIR\Plugins"
RmDir /r "$INSTDIR\Qt"
# ---------------- END AUTO-GENERATED RMDIR SECTION ---------------- #

# ------------- BEGIN AUTO-GENERATED SETOUTPATH SECTION ------------ #
SetOutPath "$INSTDIR"
SetOutPath "$INSTDIR\Config"
SetOutPath "$INSTDIR\Data"
SetOutPath "$INSTDIR\Data\SMB1"
SetOutPath "$INSTDIR\Levels"
SetOutPath "$INSTDIR\Levels\SMB1"
SetOutPath "$INSTDIR\Plugins"
SetOutPath "$INSTDIR\Plugins\Generators"
SetOutPath "$INSTDIR\Plugins\Interpreters"
SetOutPath "$INSTDIR\Plugins\Writers"
SetOutPath "$INSTDIR\Qt"
SetOutPath "$INSTDIR\Qt\generic"
SetOutPath "$INSTDIR\Qt\imageformats"
SetOutPath "$INSTDIR\Qt\networkinformation"
SetOutPath "$INSTDIR\Qt\platforms"
SetOutPath "$INSTDIR\Qt\sqldrivers"
SetOutPath "$INSTDIR\Qt\styles"
SetOutPath "$INSTDIR\Qt\tls"
# -------------- END AUTO-GENERATED SETOUTPATH SECTION ------------- #

# --------------- BEGIN AUTO-GENERATED FILE SECTION ---------------- #
File "D:\Documents\Source_Code\Level-Headed\Data\SMB1\Graphics.sa"
File "D:\Documents\Source_Code\Level-Headed\Data\SMB1\Music.sa"
File "D:\Documents\Source_Code\Level-Headed\Data\SMB1\ROMs.sa"
File "D:\Documents\Source_Code\Level-Headed\Data\SMB1\Text.sa"
File "D:\Documents\Source_Code\Level-Headed\Level-Headed.exe"
File "D:\Documents\Source_Code\Level-Headed\Levels\SMB1\(Tech Demo) It Plays Itself.lvls"
File "D:\Documents\Source_Code\Level-Headed\Levels\SMB1\(Tech Demo) Powerup Test.lvls"
File "D:\Documents\Source_Code\Level-Headed\Levels\SMB1\(Tech Demo) Vertical Limit Test.lvls"
File "D:\Documents\Source_Code\Level-Headed\Levels\SMB1\Super Mario Bros. 1 (Modified Original Levels).lvls"
File "D:\Documents\Source_Code\Level-Headed\libb2-1.dll"
File "D:\Documents\Source_Code\Level-Headed\libbrotlicommon.dll"
File "D:\Documents\Source_Code\Level-Headed\libbrotlidec.dll"
File "D:\Documents\Source_Code\Level-Headed\libbz2-1.dll"
File "D:\Documents\Source_Code\Level-Headed\libdouble-conversion.dll"
File "D:\Documents\Source_Code\Level-Headed\libfreetype-6.dll"
File "D:\Documents\Source_Code\Level-Headed\libgcc_s_seh-1.dll"
File "D:\Documents\Source_Code\Level-Headed\libglib-2.0-0.dll"
File "D:\Documents\Source_Code\Level-Headed\libgraphite2.dll"
File "D:\Documents\Source_Code\Level-Headed\libharfbuzz-0.dll"
File "D:\Documents\Source_Code\Level-Headed\libiconv-2.dll"
File "D:\Documents\Source_Code\Level-Headed\libicudt73.dll"
File "D:\Documents\Source_Code\Level-Headed\libicuin73.dll"
File "D:\Documents\Source_Code\Level-Headed\libicuuc73.dll"
File "D:\Documents\Source_Code\Level-Headed\libintl-8.dll"
File "D:\Documents\Source_Code\Level-Headed\libmd4c.dll"
File "D:\Documents\Source_Code\Level-Headed\libpcre2-16-0.dll"
File "D:\Documents\Source_Code\Level-Headed\libpcre2-8-0.dll"
File "D:\Documents\Source_Code\Level-Headed\libpng16-16.dll"
File "D:\Documents\Source_Code\Level-Headed\libstdc++-6.dll"
File "D:\Documents\Source_Code\Level-Headed\libwinpthread-1.dll"
File "D:\Documents\Source_Code\Level-Headed\Plugins\Generators\SMB1_Compliance_Generator.dll"
File "D:\Documents\Source_Code\Level-Headed\Plugins\Hexagon.dll"
File "D:\Documents\Source_Code\Level-Headed\Plugins\Interpreters\SMB1_Compliance_To_SMB1.dll"
File "D:\Documents\Source_Code\Level-Headed\Plugins\Sequential_Archive.dll"
File "D:\Documents\Source_Code\Level-Headed\Plugins\Writers\SMB1_Writer.dll"
File "D:\Documents\Source_Code\Level-Headed\Qt\generic\qtuiotouchplugin.dll"
File "D:\Documents\Source_Code\Level-Headed\Qt\imageformats\qgif.dll"
File "D:\Documents\Source_Code\Level-Headed\Qt\imageformats\qico.dll"
File "D:\Documents\Source_Code\Level-Headed\Qt\imageformats\qjpeg.dll"
File "D:\Documents\Source_Code\Level-Headed\Qt\networkinformation\qnetworklistmanager.dll"
File "D:\Documents\Source_Code\Level-Headed\Qt\platforms\qdirect2d.dll"
File "D:\Documents\Source_Code\Level-Headed\Qt\platforms\qminimal.dll"
File "D:\Documents\Source_Code\Level-Headed\Qt\platforms\qoffscreen.dll"
File "D:\Documents\Source_Code\Level-Headed\Qt\platforms\qwindows.dll"
File "D:\Documents\Source_Code\Level-Headed\Qt\sqldrivers\qsqlite.dll"
File "D:\Documents\Source_Code\Level-Headed\Qt\sqldrivers\qsqlmysql.dll"
File "D:\Documents\Source_Code\Level-Headed\Qt\sqldrivers\qsqlodbc.dll"
File "D:\Documents\Source_Code\Level-Headed\Qt\sqldrivers\qsqlpsql.dll"
File "D:\Documents\Source_Code\Level-Headed\Qt\styles\qwindowsvistastyle.dll"
File "D:\Documents\Source_Code\Level-Headed\Qt\tls\qcertonlybackend.dll"
File "D:\Documents\Source_Code\Level-Headed\Qt\tls\qopensslbackend.dll"
File "D:\Documents\Source_Code\Level-Headed\Qt\tls\qschannelbackend.dll"
File "D:\Documents\Source_Code\Level-Headed\qt.conf"
File "D:\Documents\Source_Code\Level-Headed\Qt6Core.dll"
File "D:\Documents\Source_Code\Level-Headed\Qt6Gui.dll"
File "D:\Documents\Source_Code\Level-Headed\Qt6Network.dll"
File "D:\Documents\Source_Code\Level-Headed\Qt6Widgets.dll"
File "D:\Documents\Source_Code\Level-Headed\zlib1.dll"
# ---------------- END AUTO-GENERATED FILE SECTION ----------------- #

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
Delete "$INSTDIR\Data\SMB1\Text.sa"
Delete "$INSTDIR\Levels\SMB1\(Tech Demo) It Plays Itself.lvls"
Delete "$INSTDIR\Levels\SMB1\(Tech Demo) Powerup Test.lvls"
Delete "$INSTDIR\Levels\SMB1\(Tech Demo) Vertical Limit Test.lvls"
Delete "$INSTDIR\Levels\SMB1\Super Mario Bros. 1 (Modified Original Levels).lvls"
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


