#!/bin/bash
mkdir -pv ./build-Level-Headed-Desktop-Debug/Plugins/Generators/
mkdir -pv ./build-Level-Headed-Desktop-Debug/Plugins/Interpreters/
mkdir -pv ./build-Level-Headed-Desktop-Debug/Plugins/Writers/
cp -rvf ./SMB1/build-SMB1_Compliance_Generator-Desktop-Release/libSMB1_Compliance_Generator.so ./build-Level-Headed-Desktop-Release/Plugins/Generators/SMB1_Compliance_Generator.so
cp -rvf ./SMB1/build-SMB1_Compliance_To_SMB1-Desktop-Release/libSMB1_Compliance_To_SMB1.so ./build-Level-Headed-Desktop-Release/Plugins/Interpreters/SMB1_Compliance_To_SMB1.so
cp -rvf ./SMB1/build-SMB1_Writer-Desktop-Release/libSMB1_Writer.so ./build-Level-Headed-Desktop-Release/Plugins/Writers/SMB1_Writer.so
echo "Plugins Updated!"
exit 0

