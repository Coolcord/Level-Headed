#!/bin/bash
mkdir -pv ./build-Level-Headed-Desktop-Debug/Plugins/Generators/
mkdir -pv ./build-Level-Headed-Desktop-Debug/Plugins/Interpreters/
mkdir -pv ./build-Level-Headed-Desktop-Debug/Plugins/Writers/
cp -rvf ./SMB1/build-SMB1_Compliance_Generator-Desktop-Debug/libSMB1_Compliance_Generator.so ./build-Level-Headed-Desktop-Debug/Plugins/Generators/SMB1_Compliance_Generator.so
cp -rvf ./SMB1/build-SMB1_Compliance_To_SMB1-Desktop-Debug/libSMB1_Compliance_To_SMB1.so ./build-Level-Headed-Desktop-Debug/Plugins/Interpreters/SMB1_Compliance_To_SMB1.so
cp -rvf ./SMB1/build-SMB1_Writer-Desktop-Debug/libSMB1_Writer.so ./build-Level-Headed-Desktop-Debug/Plugins/Writers/SMB1_Writer.so
exit 0

