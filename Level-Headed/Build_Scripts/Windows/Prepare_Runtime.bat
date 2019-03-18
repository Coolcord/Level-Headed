@ECHO OFF
COLOR 0A

SET QT=Qt_5_11_1_MinGW_32bit
SET MODE=Debug
SET SOURCE_LOCATION=F:\Documents\Source_Code

ERASE %SOURCE_LOCATION%\Level-Headed\build-Level-Headed-Desktop_%QT%-%MODE%\%MODE%\Plugins\Generators\SMB1_Compliance_Generator.dll
ERASE %SOURCE_LOCATION%\Level-Headed\build-Level-Headed-Desktop_%QT%-%MODE%\%MODE%\Plugins\Interpreters\SMB1_Compliance_To_SMB1.dll
ERASE %SOURCE_LOCATION%\Level-Headed\build-Level-Headed-Desktop_%QT%-%MODE%\%MODE%\Plugins\Writers\SMB1_Writer.dll
ERASE %SOURCE_LOCATION%\Level-Headed\build-Level-Headed-Desktop_%QT%-%MODE%\%MODE%\Plugins\Hexagon.dll
ERASE %SOURCE_LOCATION%\Level-Headed\build-Level-Headed-Desktop_%QT%-%MODE%\%MODE%\Plugins\Sequential_Archive.dll
ERASE %SOURCE_LOCATION%\Level-Headed\build-Level-Headed-Desktop_%QT%-%MODE%\%MODE%\Data\SMB1\Graphics.sa
ERASE %SOURCE_LOCATION%\Level-Headed\build-Level-Headed-Desktop_%QT%-%MODE%\%MODE%\Data\SMB1\Music.sa
COPY %SOURCE_LOCATION%\Level-Headed\SMB1\build-SMB1_Compliance_Generator-Desktop_%QT%-%MODE%\%MODE%\SMB1_Compliance_Generator.dll F:\Documents\Source_Code\Level-Headed\build-Level-Headed-Desktop_%QT%-%MODE%\%MODE%\Plugins\Generators\SMB1_Compliance_Generator.dll
COPY %SOURCE_LOCATION%\Level-Headed\SMB1\build-SMB1_Compliance_To_SMB1-Desktop_%QT%-%MODE%\%MODE%\SMB1_Compliance_To_SMB1.dll F:\Documents\Source_Code\Level-Headed\build-Level-Headed-Desktop_%QT%-%MODE%\%MODE%\Plugins\Interpreters\SMB1_Compliance_To_SMB1.dll
COPY %SOURCE_LOCATION%\Level-Headed\SMB1\build-SMB1_Writer-Desktop_%QT%-%MODE%\%MODE%\SMB1_Writer.dll %SOURCE_LOCATION%\Level-Headed\build-Level-Headed-Desktop_%QT%-%MODE%\%MODE%\Plugins\Writers\SMB1_Writer.dll
COPY %SOURCE_LOCATION%\Hexagon\build-Hexagon-Desktop_%QT%-%MODE%\%MODE%\Hexagon.dll %SOURCE_LOCATION%\Level-Headed\build-Level-Headed-Desktop_%QT%-%MODE%\%MODE%\Plugins\Hexagon.dll
COPY %SOURCE_LOCATION%\Sequential_Archive\build-Sequential_Archive-Desktop_%QT%-%MODE%\%MODE%\Sequential_Archive.dll %SOURCE_LOCATION%\Level-Headed\build-Level-Headed-Desktop_%QT%-%MODE%\%MODE%\Plugins\Sequential_Archive.dll
COPY %SOURCE_LOCATION%\Level-Headed_Data\Graphics.sa %SOURCE_LOCATION%\Level-Headed\build-Level-Headed-Desktop_%QT%-%MODE%\%MODE%\Data\SMB1\Graphics.sa
COPY %SOURCE_LOCATION%\Level-Headed_Data\Music.sa %SOURCE_LOCATION%\Level-Headed\build-Level-Headed-Desktop_%QT%-%MODE%\%MODE%\Data\SMB1\Music.sa
EXIT
