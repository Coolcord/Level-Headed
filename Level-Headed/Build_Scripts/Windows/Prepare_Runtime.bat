@ECHO OFF
COLOR 0A

SET QT=Qt_5_12_2_MinGW_32_bit
SET MODE=%1
IF %1.==. SET MODE=Debug
SET SOURCE_LOCATION=F:\Documents\Source_Code

REM Prepare Folders
MKDIR "%SOURCE_LOCATION%\Level-Headed\build-Level-Headed-Desktop_%QT%-%MODE%\%MODE%\Data" >NUL 2>&1
MKDIR "%SOURCE_LOCATION%\Level-Headed\build-Level-Headed-Desktop_%QT%-%MODE%\%MODE%\Data\SMB1" >NUL 2>&1
MKDIR "%SOURCE_LOCATION%\Level-Headed\build-Level-Headed-Desktop_%QT%-%MODE%\%MODE%\Plugins" >NUL 2>&1
MKDIR "%SOURCE_LOCATION%\Level-Headed\build-Level-Headed-Desktop_%QT%-%MODE%\%MODE%\Plugins\Generators" >NUL 2>&1
MKDIR "%SOURCE_LOCATION%\Level-Headed\build-Level-Headed-Desktop_%QT%-%MODE%\%MODE%\Plugins\Interpreters" >NUL 2>&1
MKDIR "%SOURCE_LOCATION%\Level-Headed\build-Level-Headed-Desktop_%QT%-%MODE%\%MODE%\Plugins\Writers" >NUL 2>&1
MKDIR "%SOURCE_LOCATION%\Sequential_Archive\build-Sequential_Archive_Manager-Desktop_%QT%-%MODE%\%MODE%\Plugins" >NUL 2>&1

REM Delete Old Plugins
ERASE "%SOURCE_LOCATION%\Level-Headed\build-Level-Headed-Desktop_%QT%-%MODE%\%MODE%\Plugins\Generators\SMB1_Compliance_Generator.dll" >NUL 2>&1
ERASE "%SOURCE_LOCATION%\Level-Headed\build-Level-Headed-Desktop_%QT%-%MODE%\%MODE%\Plugins\Interpreters\SMB1_Compliance_To_SMB1.dll" >NUL 2>&1
ERASE "%SOURCE_LOCATION%\Level-Headed\build-Level-Headed-Desktop_%QT%-%MODE%\%MODE%\Plugins\Writers\SMB1_Writer.dll" >NUL 2>&1
ERASE "%SOURCE_LOCATION%\Level-Headed\build-Level-Headed-Desktop_%QT%-%MODE%\%MODE%\Plugins\Hexagon.dll" >NUL 2>&1
ERASE "%SOURCE_LOCATION%\Level-Headed\build-Level-Headed-Desktop_%QT%-%MODE%\%MODE%\Plugins\Sequential_Archive.dll" >NUL 2>&1
ERASE "%SOURCE_LOCATION%\Level-Headed\build-Level-Headed-Desktop_%QT%-%MODE%\%MODE%\Data\SMB1\Graphics.sa" >NUL 2>&1
ERASE "%SOURCE_LOCATION%\Level-Headed\build-Level-Headed-Desktop_%QT%-%MODE%\%MODE%\Data\SMB1\Music.sa" >NUL 2>&1
ERASE "%SOURCE_LOCATION%\Level-Headed\build-Level-Headed-Desktop_%QT%-%MODE%\%MODE%\Data\SMB1\ROMs.sa" >NUL 2>&1
ERASE "%SOURCE_LOCATION%\Sequential_Archive\build-Sequential_Archive_Manager-Desktop_%QT%-%MODE%\%MODE%\Plugins\Sequential_Archive.dll" >NUL 2>&1

REM Install New Plugins
COPY "%SOURCE_LOCATION%\Level-Headed\SMB1\build-SMB1_Compliance_Generator-Desktop_%QT%-%MODE%\%MODE%\SMB1_Compliance_Generator.dll" "%SOURCE_LOCATION%\Level-Headed\build-Level-Headed-Desktop_%QT%-%MODE%\%MODE%\Plugins\Generators\SMB1_Compliance_Generator.dll"
COPY "%SOURCE_LOCATION%\Level-Headed\SMB1\build-SMB1_Compliance_To_SMB1-Desktop_%QT%-%MODE%\%MODE%\SMB1_Compliance_To_SMB1.dll" "%SOURCE_LOCATION%\Level-Headed\build-Level-Headed-Desktop_%QT%-%MODE%\%MODE%\Plugins\Interpreters\SMB1_Compliance_To_SMB1.dll"
COPY "%SOURCE_LOCATION%\Level-Headed\SMB1\build-SMB1_Writer-Desktop_%QT%-%MODE%\%MODE%\SMB1_Writer.dll" "%SOURCE_LOCATION%\Level-Headed\build-Level-Headed-Desktop_%QT%-%MODE%\%MODE%\Plugins\Writers\SMB1_Writer.dll"
COPY "%SOURCE_LOCATION%\Hexagon\build-Hexagon-Desktop_%QT%-%MODE%\%MODE%\Hexagon.dll" "%SOURCE_LOCATION%\Level-Headed\build-Level-Headed-Desktop_%QT%-%MODE%\%MODE%\Plugins\Hexagon.dll"
COPY "%SOURCE_LOCATION%\Sequential_Archive\build-Sequential_Archive-Desktop_%QT%-%MODE%\%MODE%\Sequential_Archive.dll" "%SOURCE_LOCATION%\Level-Headed\build-Level-Headed-Desktop_%QT%-%MODE%\%MODE%\Plugins\Sequential_Archive.dll"
COPY "%SOURCE_LOCATION%\Sequential_Archive\build-Sequential_Archive-Desktop_%QT%-%MODE%\%MODE%\Sequential_Archive.dll" "%SOURCE_LOCATION%\Sequential_Archive\build-Sequential_Archive_Manager-Desktop_%QT%-%MODE%\%MODE%\Plugins\Sequential_Archive.dll"

REM Pack Sequential Archives
"%SOURCE_LOCATION%\Sequential_Archive\build-Sequential_Archive_Manager-Desktop_%QT%-%MODE%\%MODE%\Sequential_Archive_Manager.exe" --pack "%SOURCE_LOCATION%\Level-Headed_Data\Graphics" "%SOURCE_LOCATION%\Level-Headed\build-Level-Headed-Desktop_%QT%-%MODE%\%MODE%\Data\SMB1\Graphics.sa"
"%SOURCE_LOCATION%\Sequential_Archive\build-Sequential_Archive_Manager-Desktop_%QT%-%MODE%\%MODE%\Sequential_Archive_Manager.exe" --pack "%SOURCE_LOCATION%\Level-Headed_Data\Music" "%SOURCE_LOCATION%\Level-Headed\build-Level-Headed-Desktop_%QT%-%MODE%\%MODE%\Data\SMB1\Music.sa"
"%SOURCE_LOCATION%\Sequential_Archive\build-Sequential_Archive_Manager-Desktop_%QT%-%MODE%\%MODE%\Sequential_Archive_Manager.exe" --pack "%SOURCE_LOCATION%\Level-Headed_Data\ROMs" "%SOURCE_LOCATION%\Level-Headed\build-Level-Headed-Desktop_%QT%-%MODE%\%MODE%\Data\SMB1\ROMs.sa"
ECHO If there are no errors, everything should be ready!
