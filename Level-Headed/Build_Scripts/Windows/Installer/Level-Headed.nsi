############################################################################################

!define APP_NAME "Level-Headed"
!define COMP_NAME "Coolcord"
!define VERSION "0.4.0.0"
!define COPYRIGHT ""
!define DESCRIPTION "Random Level Generator"
!define LICENSE_TXT "D:\Documents\Source_Code\Level-Headed\Level-Headed\Build_Scripts\Unix\Deployed_Files\LICENSE.txt"
!define INSTALLER_NAME "D:\Documents\Source_Code\Level-Headed\Level-Headed\Build_Scripts\Unix\Deployed_Files\Level-Headed.v0.4.0-dev.Setup.exe"
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

# --------------- BEGIN AUTO-GENERATED FILE SECTION ---------------- #
SetOutPath "$INSTDIR"
File "D:\Documents\Source_Code\Level-Headed\Level-Headed\Build_Scripts\Unix\Deployed_Files\Level-Headed\Level-Headed.exe"
File "D:\Documents\Source_Code\Level-Headed\Level-Headed\Build_Scripts\Unix\Deployed_Files\Level-Headed\libb2-1.dll"
File "D:\Documents\Source_Code\Level-Headed\Level-Headed\Build_Scripts\Unix\Deployed_Files\Level-Headed\libbrotlicommon.dll"
File "D:\Documents\Source_Code\Level-Headed\Level-Headed\Build_Scripts\Unix\Deployed_Files\Level-Headed\libbrotlidec.dll"
File "D:\Documents\Source_Code\Level-Headed\Level-Headed\Build_Scripts\Unix\Deployed_Files\Level-Headed\libbz2-1.dll"
File "D:\Documents\Source_Code\Level-Headed\Level-Headed\Build_Scripts\Unix\Deployed_Files\Level-Headed\libdouble-conversion.dll"
File "D:\Documents\Source_Code\Level-Headed\Level-Headed\Build_Scripts\Unix\Deployed_Files\Level-Headed\libfreetype-6.dll"
File "D:\Documents\Source_Code\Level-Headed\Level-Headed\Build_Scripts\Unix\Deployed_Files\Level-Headed\libgcc_s_seh-1.dll"
File "D:\Documents\Source_Code\Level-Headed\Level-Headed\Build_Scripts\Unix\Deployed_Files\Level-Headed\libglib-2.0-0.dll"
File "D:\Documents\Source_Code\Level-Headed\Level-Headed\Build_Scripts\Unix\Deployed_Files\Level-Headed\libgraphite2.dll"
File "D:\Documents\Source_Code\Level-Headed\Level-Headed\Build_Scripts\Unix\Deployed_Files\Level-Headed\libharfbuzz-0.dll"
File "D:\Documents\Source_Code\Level-Headed\Level-Headed\Build_Scripts\Unix\Deployed_Files\Level-Headed\libiconv-2.dll"
File "D:\Documents\Source_Code\Level-Headed\Level-Headed\Build_Scripts\Unix\Deployed_Files\Level-Headed\libicudt73.dll"
File "D:\Documents\Source_Code\Level-Headed\Level-Headed\Build_Scripts\Unix\Deployed_Files\Level-Headed\libicuin73.dll"
File "D:\Documents\Source_Code\Level-Headed\Level-Headed\Build_Scripts\Unix\Deployed_Files\Level-Headed\libicuuc73.dll"
File "D:\Documents\Source_Code\Level-Headed\Level-Headed\Build_Scripts\Unix\Deployed_Files\Level-Headed\libintl-8.dll"
File "D:\Documents\Source_Code\Level-Headed\Level-Headed\Build_Scripts\Unix\Deployed_Files\Level-Headed\libmd4c.dll"
File "D:\Documents\Source_Code\Level-Headed\Level-Headed\Build_Scripts\Unix\Deployed_Files\Level-Headed\libpcre2-16-0.dll"
File "D:\Documents\Source_Code\Level-Headed\Level-Headed\Build_Scripts\Unix\Deployed_Files\Level-Headed\libpcre2-8-0.dll"
File "D:\Documents\Source_Code\Level-Headed\Level-Headed\Build_Scripts\Unix\Deployed_Files\Level-Headed\libpng16-16.dll"
File "D:\Documents\Source_Code\Level-Headed\Level-Headed\Build_Scripts\Unix\Deployed_Files\Level-Headed\libstdc++-6.dll"
File "D:\Documents\Source_Code\Level-Headed\Level-Headed\Build_Scripts\Unix\Deployed_Files\Level-Headed\libwinpthread-1.dll"
File "D:\Documents\Source_Code\Level-Headed\Level-Headed\Build_Scripts\Unix\Deployed_Files\Level-Headed\qt.conf"
File "D:\Documents\Source_Code\Level-Headed\Level-Headed\Build_Scripts\Unix\Deployed_Files\Level-Headed\Qt6Core.dll"
File "D:\Documents\Source_Code\Level-Headed\Level-Headed\Build_Scripts\Unix\Deployed_Files\Level-Headed\Qt6Gui.dll"
File "D:\Documents\Source_Code\Level-Headed\Level-Headed\Build_Scripts\Unix\Deployed_Files\Level-Headed\Qt6Network.dll"
File "D:\Documents\Source_Code\Level-Headed\Level-Headed\Build_Scripts\Unix\Deployed_Files\Level-Headed\Qt6Widgets.dll"
File "D:\Documents\Source_Code\Level-Headed\Level-Headed\Build_Scripts\Unix\Deployed_Files\Level-Headed\zlib1.dll"
SetOutPath "$INSTDIR\Config"
SetOutPath "$INSTDIR\Data"
SetOutPath "$INSTDIR\Data\SMB1"
File "D:\Documents\Source_Code\Level-Headed\Level-Headed\Build_Scripts\Unix\Deployed_Files\Level-Headed\Data\SMB1\Graphics.sa"
File "D:\Documents\Source_Code\Level-Headed\Level-Headed\Build_Scripts\Unix\Deployed_Files\Level-Headed\Data\SMB1\Music.sa"
File "D:\Documents\Source_Code\Level-Headed\Level-Headed\Build_Scripts\Unix\Deployed_Files\Level-Headed\Data\SMB1\ROMs.sa"
File "D:\Documents\Source_Code\Level-Headed\Level-Headed\Build_Scripts\Unix\Deployed_Files\Level-Headed\Data\SMB1\Text.sa"
SetOutPath "$INSTDIR\Levels"
SetOutPath "$INSTDIR\Levels\SMB1"
File "D:\Documents\Source_Code\Level-Headed\Level-Headed\Build_Scripts\Unix\Deployed_Files\Level-Headed\Levels\SMB1\(Tech Demo) It Plays Itself.lvls"
File "D:\Documents\Source_Code\Level-Headed\Level-Headed\Build_Scripts\Unix\Deployed_Files\Level-Headed\Levels\SMB1\(Tech Demo) Powerup Test.lvls"
File "D:\Documents\Source_Code\Level-Headed\Level-Headed\Build_Scripts\Unix\Deployed_Files\Level-Headed\Levels\SMB1\(Tech Demo) Vertical Limit Test.lvls"
File "D:\Documents\Source_Code\Level-Headed\Level-Headed\Build_Scripts\Unix\Deployed_Files\Level-Headed\Levels\SMB1\Super Mario Bros. 1 (Modified Original Levels).lvls"
SetOutPath "$INSTDIR\Plugins"
File "D:\Documents\Source_Code\Level-Headed\Level-Headed\Build_Scripts\Unix\Deployed_Files\Level-Headed\Plugins\Hexagon.dll"
File "D:\Documents\Source_Code\Level-Headed\Level-Headed\Build_Scripts\Unix\Deployed_Files\Level-Headed\Plugins\Sequential_Archive.dll"
SetOutPath "$INSTDIR\Plugins\Generators"
File "D:\Documents\Source_Code\Level-Headed\Level-Headed\Build_Scripts\Unix\Deployed_Files\Level-Headed\Plugins\Generators\SMB1_Compliance_Generator.dll"
SetOutPath "$INSTDIR\Plugins\Interpreters"
File "D:\Documents\Source_Code\Level-Headed\Level-Headed\Build_Scripts\Unix\Deployed_Files\Level-Headed\Plugins\Interpreters\SMB1_Compliance_To_SMB1.dll"
SetOutPath "$INSTDIR\Plugins\Writers"
File "D:\Documents\Source_Code\Level-Headed\Level-Headed\Build_Scripts\Unix\Deployed_Files\Level-Headed\Plugins\Writers\SMB1_Writer.dll"
SetOutPath "$INSTDIR\Qt"
SetOutPath "$INSTDIR\Qt\generic"
File "D:\Documents\Source_Code\Level-Headed\Level-Headed\Build_Scripts\Unix\Deployed_Files\Level-Headed\Qt\generic\qtuiotouchplugin.dll"
SetOutPath "$INSTDIR\Qt\imageformats"
File "D:\Documents\Source_Code\Level-Headed\Level-Headed\Build_Scripts\Unix\Deployed_Files\Level-Headed\Qt\imageformats\qgif.dll"
File "D:\Documents\Source_Code\Level-Headed\Level-Headed\Build_Scripts\Unix\Deployed_Files\Level-Headed\Qt\imageformats\qico.dll"
File "D:\Documents\Source_Code\Level-Headed\Level-Headed\Build_Scripts\Unix\Deployed_Files\Level-Headed\Qt\imageformats\qjpeg.dll"
SetOutPath "$INSTDIR\Qt\networkinformation"
File "D:\Documents\Source_Code\Level-Headed\Level-Headed\Build_Scripts\Unix\Deployed_Files\Level-Headed\Qt\networkinformation\qnetworklistmanager.dll"
SetOutPath "$INSTDIR\Qt\platforms"
File "D:\Documents\Source_Code\Level-Headed\Level-Headed\Build_Scripts\Unix\Deployed_Files\Level-Headed\Qt\platforms\qdirect2d.dll"
File "D:\Documents\Source_Code\Level-Headed\Level-Headed\Build_Scripts\Unix\Deployed_Files\Level-Headed\Qt\platforms\qminimal.dll"
File "D:\Documents\Source_Code\Level-Headed\Level-Headed\Build_Scripts\Unix\Deployed_Files\Level-Headed\Qt\platforms\qoffscreen.dll"
File "D:\Documents\Source_Code\Level-Headed\Level-Headed\Build_Scripts\Unix\Deployed_Files\Level-Headed\Qt\platforms\qwindows.dll"
SetOutPath "$INSTDIR\Qt\sqldrivers"
File "D:\Documents\Source_Code\Level-Headed\Level-Headed\Build_Scripts\Unix\Deployed_Files\Level-Headed\Qt\sqldrivers\qsqlite.dll"
File "D:\Documents\Source_Code\Level-Headed\Level-Headed\Build_Scripts\Unix\Deployed_Files\Level-Headed\Qt\sqldrivers\qsqlmysql.dll"
File "D:\Documents\Source_Code\Level-Headed\Level-Headed\Build_Scripts\Unix\Deployed_Files\Level-Headed\Qt\sqldrivers\qsqlodbc.dll"
File "D:\Documents\Source_Code\Level-Headed\Level-Headed\Build_Scripts\Unix\Deployed_Files\Level-Headed\Qt\sqldrivers\qsqlpsql.dll"
SetOutPath "$INSTDIR\Qt\styles"
File "D:\Documents\Source_Code\Level-Headed\Level-Headed\Build_Scripts\Unix\Deployed_Files\Level-Headed\Qt\styles\qwindowsvistastyle.dll"
SetOutPath "$INSTDIR\Qt\tls"
File "D:\Documents\Source_Code\Level-Headed\Level-Headed\Build_Scripts\Unix\Deployed_Files\Level-Headed\Qt\tls\qcertonlybackend.dll"
File "D:\Documents\Source_Code\Level-Headed\Level-Headed\Build_Scripts\Unix\Deployed_Files\Level-Headed\Qt\tls\qopensslbackend.dll"
File "D:\Documents\Source_Code\Level-Headed\Level-Headed\Build_Scripts\Unix\Deployed_Files\Level-Headed\Qt\tls\qschannelbackend.dll"
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

# -------------- BEGIN AUTO-GENERATED DELETE SECTION --------------- #
Delete "$INSTDIR\Levels\SMB1\Super Mario Bros. 1 (Modified Original Levels).lvls"
Delete "$INSTDIR\Plugins\Interpreters\SMB1_Compliance_To_SMB1.dll"
Delete "$INSTDIR\Plugins\Generators\SMB1_Compliance_Generator.dll"
Delete "$INSTDIR\Levels\SMB1\(Tech Demo) Vertical Limit Test.lvls"
Delete "$INSTDIR\Qt\networkinformation\qnetworklistmanager.dll"
Delete "$INSTDIR\Levels\SMB1\(Tech Demo) It Plays Itself.lvls"
Delete "$INSTDIR\Levels\SMB1\(Tech Demo) Powerup Test.lvls"
Delete "$INSTDIR\Qt\styles\qwindowsvistastyle.dll"
Delete "$INSTDIR\Qt\generic\qtuiotouchplugin.dll"
Delete "$INSTDIR\Plugins\Writers\SMB1_Writer.dll"
Delete "$INSTDIR\Plugins\Sequential_Archive.dll"
Delete "$INSTDIR\Qt\tls\qschannelbackend.dll"
Delete "$INSTDIR\Qt\tls\qcertonlybackend.dll"
Delete "$INSTDIR\Qt\sqldrivers\qsqlmysql.dll"
Delete "$INSTDIR\Qt\platforms\qoffscreen.dll"
Delete "$INSTDIR\Qt\tls\qopensslbackend.dll"
Delete "$INSTDIR\Qt\sqldrivers\qsqlpsql.dll"
Delete "$INSTDIR\Qt\sqldrivers\qsqlodbc.dll"
Delete "$INSTDIR\Qt\platforms\qdirect2d.dll"
Delete "$INSTDIR\Qt\sqldrivers\qsqlite.dll"
Delete "$INSTDIR\Qt\platforms\qwindows.dll"
Delete "$INSTDIR\Qt\platforms\qminimal.dll"
Delete "$INSTDIR\Qt\imageformats\qjpeg.dll"
Delete "$INSTDIR\libdouble-conversion.dll"
Delete "$INSTDIR\Qt\imageformats\qico.dll"
Delete "$INSTDIR\Qt\imageformats\qgif.dll"
Delete "$INSTDIR\Data\SMB1\Graphics.sa"
Delete "$INSTDIR\libwinpthread-1.dll"
Delete "$INSTDIR\libbrotlicommon.dll"
Delete "$INSTDIR\Plugins\Hexagon.dll"
Delete "$INSTDIR\libgcc_s_seh-1.dll"
Delete "$INSTDIR\Data\SMB1\Music.sa"
Delete "$INSTDIR\libpcre2-16-0.dll"
Delete "$INSTDIR\libharfbuzz-0.dll"
Delete "$INSTDIR\libglib-2.0-0.dll"
Delete "$INSTDIR\libfreetype-6.dll"
Delete "$INSTDIR\Data\SMB1\Text.sa"
Delete "$INSTDIR\Data\SMB1\ROMs.sa"
Delete "$INSTDIR\libpcre2-8-0.dll"
Delete "$INSTDIR\libgraphite2.dll"
Delete "$INSTDIR\libbrotlidec.dll"
Delete "$INSTDIR\libstdc++-6.dll"
Delete "$INSTDIR\libpng16-16.dll"
Delete "$INSTDIR\libicuuc73.dll"
Delete "$INSTDIR\libicuin73.dll"
Delete "$INSTDIR\libicudt73.dll"
Delete "$INSTDIR\libiconv-2.dll"
Delete "$INSTDIR\Qt6Widgets.dll"
Delete "$INSTDIR\Qt6Network.dll"
Delete "$INSTDIR\libintl-8.dll"
Delete "$INSTDIR\libbz2-1.dll"
Delete "$INSTDIR\libmd4c.dll"
Delete "$INSTDIR\libb2-1.dll"
Delete "$INSTDIR\Qt6Core.dll"
Delete "$INSTDIR\Qt6Gui.dll"
Delete "$INSTDIR\zlib1.dll"
Delete "$INSTDIR\qt.conf"
# --------------- END AUTO-GENERATED DELETE SECTION ---------------- #
Delete "$INSTDIR\Config\*.cfg"
Delete "$INSTDIR\*.dll"

# ------------- BEGIN AUTO-GENERATED UNINSTALL SECTION ------------- #
RmDir /r "$INSTDIR\Qt\networkinformation"
RmDir /r "$INSTDIR\Plugins\Interpreters"
RmDir /r "$INSTDIR\Plugins\Generators"
RmDir /r "$INSTDIR\Qt\imageformats"
RmDir /r "$INSTDIR\Plugins\Writers"
RmDir /r "$INSTDIR\Qt\sqldrivers"
RmDir /r "$INSTDIR\Qt\platforms"
RmDir /r "$INSTDIR\Levels\SMB1"
RmDir /r "$INSTDIR\Qt\generic"
RmDir /r "$INSTDIR\Qt\styles"
RmDir /r "$INSTDIR\Data\SMB1"
RmDir /r "$INSTDIR\Plugins"
RmDir /r "$INSTDIR\Qt\tls"
RmDir /r "$INSTDIR\Levels"
RmDir /r "$INSTDIR\Config"
RmDir /r "$INSTDIR\Data"
RmDir /r "$INSTDIR\Qt"
# -------------- END AUTO-GENERATED UNINSTALL SECTION -------------- #
 
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


