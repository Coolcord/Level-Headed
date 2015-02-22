@ECHO OFF
COLOR 0A
ERASE E:\Level-Headed\build-Level-Headed-Desktop_Qt_5_4_0_MinGW_32bit-Release\release\Plugins\Generators\SMB1_Compliance_Generator.dll
ERASE E:\Level-Headed\build-Level-Headed-Desktop_Qt_5_4_0_MinGW_32bit-Release\release\Plugins\Interpreters\SMB1_Compliance_To_SMB1.dll
ERASE E:\Level-Headed\build-Level-Headed-Desktop_Qt_5_4_0_MinGW_32bit-Release\release\Plugins\Writers\SMB1_Writer.dll
COPY E:\Level-Headed\SMB1\build-SMB1_Compliance_Generator-Desktop_Qt_5_4_0_MinGW_32bit-Release\release\SMB1_Compliance_Generator.dll E:\Level-Headed\build-Level-Headed-Desktop_Qt_5_4_0_MinGW_32bit-Release\release\Plugins\Generators\SMB1_Compliance_Generator.dll
COPY E:\Level-Headed\SMB1\build-SMB1_Compliance_To_SMB1-Desktop_Qt_5_4_0_MinGW_32bit-Release\release\SMB1_Compliance_To_SMB1.dll E:\Level-Headed\build-Level-Headed-Desktop_Qt_5_4_0_MinGW_32bit-Release\release\Plugins\Interpreters\SMB1_Compliance_To_SMB1.dll
COPY E:\Level-Headed\SMB1\build-SMB1_Writer-Desktop_Qt_5_4_0_MinGW_32bit-Release\release\SMB1_Writer.dll E:\Level-Headed\build-Level-Headed-Desktop_Qt_5_4_0_MinGW_32bit-Release\release\Plugins\Writers\SMB1_Writer.dll
EXIT
