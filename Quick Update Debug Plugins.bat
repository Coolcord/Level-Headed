@ECHO OFF
COLOR 0A
ERASE F:\Documents\Source_Code\Level-Headed\build-Level-Headed-Desktop_Qt_5_9_3_MinGW_32bit-Debug\debug\Plugins\Generators\SMB1_Compliance_Generator.dll
ERASE F:\Documents\Source_Code\Level-Headed\build-Level-Headed-Desktop_Qt_5_9_3_MinGW_32bit-Debug\debug\Plugins\Interpreters\SMB1_Compliance_To_SMB1.dll
ERASE F:\Documents\Source_Code\Level-Headed\build-Level-Headed-Desktop_Qt_5_9_3_MinGW_32bit-Debug\debug\Plugins\Writers\SMB1_Writer.dll
ERASE F:\Documents\Source_Code\Sequential_Archive\build-Sequential_Archive_Manager-Desktop_Qt_5_9_3_MinGW_32bit-Debug\debug\Sequential_Archive.dll
COPY F:\Documents\Source_Code\Level-Headed\SMB1\build-SMB1_Compliance_Generator-Desktop_Qt_5_9_3_MinGW_32bit-Debug\debug\SMB1_Compliance_Generator.dll F:\Documents\Source_Code\Level-Headed\build-Level-Headed-Desktop_Qt_5_9_3_MinGW_32bit-Debug\debug\Plugins\Generators\SMB1_Compliance_Generator.dll
COPY F:\Documents\Source_Code\Level-Headed\SMB1\build-SMB1_Compliance_To_SMB1-Desktop_Qt_5_9_3_MinGW_32bit-Debug\debug\SMB1_Compliance_To_SMB1.dll F:\Documents\Source_Code\Level-Headed\build-Level-Headed-Desktop_Qt_5_9_3_MinGW_32bit-Debug\debug\Plugins\Interpreters\SMB1_Compliance_To_SMB1.dll
COPY F:\Documents\Source_Code\Level-Headed\SMB1\build-SMB1_Writer-Desktop_Qt_5_9_3_MinGW_32bit-Debug\debug\SMB1_Writer.dll F:\Documents\Source_Code\Level-Headed\build-Level-Headed-Desktop_Qt_5_9_3_MinGW_32bit-Debug\debug\Plugins\Writers\SMB1_Writer.dll
COPY F:\Documents\Source_Code\Sequential_Archive\build-Sequential_Archive-Desktop_Qt_5_9_3_MinGW_32bit-Debug\debug\Sequential_Archive.dll F:\Documents\Source_Code\Sequential_Archive\build-Sequential_Archive_Manager-Desktop_Qt_5_9_3_MinGW_32bit-Debug\debug\Sequential_Archive.dll
EXIT
