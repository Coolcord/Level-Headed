#!/bin/bash
mkdir -pv /home/cord/Documents/Source_Code/Level-Headed/build-Level-Headed-Desktop-Debug/Plugins/Generators/
mkdir -pv /home/cord/Documents/Source_Code/Level-Headed/build-Level-Headed-Desktop-Debug/Plugins/Interpreters/
mkdir -pv /home/cord/Documents/Source_Code/Level-Headed/build-Level-Headed-Desktop-Debug/Plugins/Writers/
cp -rvf /home/cord/Documents/Source_Code/Level-Headed/SMB1/build-SMB1_Compliance_Generator-Desktop-Debug/libSMB1_Compliance_Generator.so /home/cord/Documents/Source_Code/Level-Headed/build-Level-Headed-Desktop-Debug/Plugins/Generators/SMB1_Compliance_Generator.so
cp -rvf /home/cord/Documents/Source_Code/Level-Headed/SMB1/build-SMB1_Compliance_To_SMB1-Desktop-Debug/libSMB1_Compliance_To_SMB1.so /home/cord/Documents/Source_Code/Level-Headed/build-Level-Headed-Desktop-Debug/Plugins/Interpreters/SMB1_Compliance_To_SMB1.so
cp -rvf /home/cord/Documents/Source_Code/Level-Headed/SMB1/build-SMB1_Writer-Desktop-Debug/libSMB1_Writer.so /home/cord/Documents/Source_Code/Level-Headed/build-Level-Headed-Desktop-Debug/Plugins/Writers/SMB1_Writer.so
echo "Plugins Updated!"
exit 0

