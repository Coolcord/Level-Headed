@ECHO OFF
COLOR 0A
ERASE E:\Level-Headed\build-Level-Headed-Desktop_Qt_5_3_MinGW_32bit-Debug\debug\Plugins\Generators\SMB1_Compliance_Generator.dll
ERASE E:\Level-Headed\build-Level-Headed-Desktop_Qt_5_3_MinGW_32bit-Debug\debug\Plugins\Interpreters\SMB1_Compliance_To_SMB1.dll
ERASE E:\Level-Headed\build-Level-Headed-Desktop_Qt_5_3_MinGW_32bit-Debug\debug\Plugins\Writers\SMB1_Writer.dll
COPY E:\Level-Headed\SMB1\build-SMB1_Compliance_Generator-Desktop_Qt_5_3_MinGW_32bit-Debug\debug\SMB1_Compliance_Generator.dll E:\Level-Headed\build-Level-Headed-Desktop_Qt_5_3_MinGW_32bit-Debug\debug\Plugins\Generators\SMB1_Compliance_Generator.dll
COPY E:\Level-Headed\SMB1\build-SMB1_Compliance_To_SMB1-Desktop_Qt_5_3_MinGW_32bit-Debug\debug\SMB1_Compliance_To_SMB1.dll E:\Level-Headed\build-Level-Headed-Desktop_Qt_5_3_MinGW_32bit-Debug\debug\Plugins\Interpreters\SMB1_Compliance_To_SMB1.dll
COPY E:\Level-Headed\SMB1\build-SMB1_Writer-Desktop_Qt_5_3_MinGW_32bit-Debug\debug\SMB1_Writer.dll E:\Level-Headed\build-Level-Headed-Desktop_Qt_5_3_MinGW_32bit-Debug\debug\Plugins\Writers\SMB1_Writer.dll
EXIT
