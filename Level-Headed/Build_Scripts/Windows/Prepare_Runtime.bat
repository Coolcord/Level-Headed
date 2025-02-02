@ECHO OFF
COLOR 0A

SET QT=Qt_6_7_3_MinGW_64_bit
SET MODE=%1
IF %1.==. SET MODE=Debug
SET SOURCE_LOCATION=D:\Documents\Source_Code

REM Prepare Folders
MKDIR "%SOURCE_LOCATION%\Level-Headed\Level-Headed\build\Desktop_%QT%-%MODE%\Data" >NUL 2>&1
MKDIR "%SOURCE_LOCATION%\Level-Headed\Level-Headed\build\Desktop_%QT%-%MODE%\Data\SMB1" >NUL 2>&1
MKDIR "%SOURCE_LOCATION%\Level-Headed\Level-Headed\build\Desktop_%QT%-%MODE%\Plugins" >NUL 2>&1
MKDIR "%SOURCE_LOCATION%\Level-Headed\Level-Headed\build\Desktop_%QT%-%MODE%\Plugins\Generators" >NUL 2>&1
MKDIR "%SOURCE_LOCATION%\Level-Headed\Level-Headed\build\Desktop_%QT%-%MODE%\Plugins\Interpreters" >NUL 2>&1
MKDIR "%SOURCE_LOCATION%\Level-Headed\Level-Headed\build\Desktop_%QT%-%MODE%\Plugins\Writers" >NUL 2>&1
MKDIR "%SOURCE_LOCATION%\Sequential_Archive\Sequential_Archive_Manager\build\Desktop_%QT%-%MODE%\Plugins" >NUL 2>&1

REM Delete Old Plugins
ERASE "%SOURCE_LOCATION%\Level-Headed\Level-Headed\build\Desktop_%QT%-%MODE%\Plugins\Generators\SMB1_Compliance_Generator.dll" >NUL 2>&1
ERASE "%SOURCE_LOCATION%\Level-Headed\Level-Headed\build\Desktop_%QT%-%MODE%\Plugins\Interpreters\SMB1_Compliance_To_SMB1.dll" >NUL 2>&1
ERASE "%SOURCE_LOCATION%\Level-Headed\Level-Headed\build\Desktop_%QT%-%MODE%\Plugins\Writers\SMB1_Writer.dll" >NUL 2>&1
ERASE "%SOURCE_LOCATION%\Level-Headed\Level-Headed\build\Desktop_%QT%-%MODE%\Plugins\Hexagon.dll" >NUL 2>&1
ERASE "%SOURCE_LOCATION%\Level-Headed\Level-Headed\build\Desktop_%QT%-%MODE%\Plugins\Sequential_Archive.dll" >NUL 2>&1
ERASE "%SOURCE_LOCATION%\Level-Headed\Level-Headed\build\Desktop_%QT%-%MODE%\Data\SMB1\Graphics.sa" >NUL 2>&1
ERASE "%SOURCE_LOCATION%\Level-Headed\Level-Headed\build\Desktop_%QT%-%MODE%\Data\SMB1\Music.sa" >NUL 2>&1
ERASE "%SOURCE_LOCATION%\Level-Headed\Level-Headed\build\Desktop_%QT%-%MODE%\Data\SMB1\ROMs.sa" >NUL 2>&1
ERASE "%SOURCE_LOCATION%\Sequential_Archive\Sequential_Archive_Manager\build\Desktop_%QT%-%MODE%\Plugins\Sequential_Archive.dll" >NUL 2>&1

REM Install New Plugins
COPY "%SOURCE_LOCATION%\Level-Headed\SMB1\SMB1_Compliance_Generator\build\Desktop_%QT%-%MODE%\libSMB1_Compliance_Generator.dll" "%SOURCE_LOCATION%\Level-Headed\Level-Headed\build\Desktop_%QT%-%MODE%\Plugins\Generators\SMB1_Compliance_Generator.dll"
COPY "%SOURCE_LOCATION%\Level-Headed\SMB1\SMB1_Compliance_To_SMB1\build\Desktop_%QT%-%MODE%\libSMB1_Compliance_To_SMB1.dll" "%SOURCE_LOCATION%\Level-Headed\Level-Headed\build\Desktop_%QT%-%MODE%\Plugins\Interpreters\SMB1_Compliance_To_SMB1.dll"
COPY "%SOURCE_LOCATION%\Level-Headed\SMB1\SMB1_Writer\build\Desktop_%QT%-%MODE%\libSMB1_Writer.dll" "%SOURCE_LOCATION%\Level-Headed\Level-Headed\build\Desktop_%QT%-%MODE%\Plugins\Writers\SMB1_Writer.dll"
COPY "%SOURCE_LOCATION%\Hexagon\Hexagon\build\Desktop_%QT%-%MODE%\libHexagon.dll" "%SOURCE_LOCATION%\Level-Headed\Level-Headed\build\Desktop_%QT%-%MODE%\Plugins\Hexagon.dll"
COPY "%SOURCE_LOCATION%\Sequential_Archive\Sequential_Archive\build\Desktop_%QT%-%MODE%\libSequential_Archive.dll" "%SOURCE_LOCATION%\Level-Headed\Level-Headed\build\Desktop_%QT%-%MODE%\Plugins\Sequential_Archive.dll"
COPY "%SOURCE_LOCATION%\Sequential_Archive\Sequential_Archive\build\Desktop_%QT%-%MODE%\libSequential_Archive.dll" "%SOURCE_LOCATION%\Sequential_Archive\Sequential_Archive_Manager\build\Desktop_%QT%-%MODE%\Plugins\Sequential_Archive.dll"

REM Pack Sequential Archives
"%SOURCE_LOCATION%\Sequential_Archive\Sequential_Archive_Manager\build\Desktop_%QT%-%MODE%\Sequential_Archive_Manager.exe" --pack "%SOURCE_LOCATION%\Level-Headed_Data\Graphics" "%SOURCE_LOCATION%\Level-Headed\Level-Headed\build\Desktop_%QT%-%MODE%\Data\SMB1\Graphics.sa"
"%SOURCE_LOCATION%\Sequential_Archive\Sequential_Archive_Manager\build\Desktop_%QT%-%MODE%\Sequential_Archive_Manager.exe" --pack "%SOURCE_LOCATION%\Level-Headed_Data\Music" "%SOURCE_LOCATION%\Level-Headed\Level-Headed\build\Desktop_%QT%-%MODE%\Data\SMB1\Music.sa"
"%SOURCE_LOCATION%\Sequential_Archive\Sequential_Archive_Manager\build\Desktop_%QT%-%MODE%\Sequential_Archive_Manager.exe" --pack "%SOURCE_LOCATION%\Level-Headed_Data\ROMs" "%SOURCE_LOCATION%\Level-Headed\Level-Headed\build\Desktop_%QT%-%MODE%\Data\SMB1\ROMs.sa"
"%SOURCE_LOCATION%\Sequential_Archive\Sequential_Archive_Manager\build\Desktop_%QT%-%MODE%\Sequential_Archive_Manager.exe" --pack "%SOURCE_LOCATION%\Level-Headed_Data\Text" "%SOURCE_LOCATION%\Level-Headed\Level-Headed\build\Desktop_%QT%-%MODE%\Data\SMB1\Text.sa"
ECHO If there are no errors, everything should be ready!
