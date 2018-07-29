#!/bin/bash
mkdir -pv /home/cord/Documents/Source_Code/Level-Headed/build-Level-Headed-Desktop-Debug/Data/SMB1/
mkdir -pv /home/cord/Documents/Source_Code/Level-Headed/build-Level-Headed-Desktop-Debug/Plugins/Generators/
mkdir -pv /home/cord/Documents/Source_Code/Level-Headed/build-Level-Headed-Desktop-Debug/Plugins/Interpreters/
mkdir -pv /home/cord/Documents/Source_Code/Level-Headed/build-Level-Headed-Desktop-Debug/Plugins/Writers/
cp -rvf /home/cord/Documents/Source_Code/Level-Headed/SMB1/build-SMB1_Compliance_Generator-Desktop-Debug/libSMB1_Compliance_Generator.so /home/cord/Documents/Source_Code/Level-Headed/build-Level-Headed-Desktop-Debug/Plugins/Generators/SMB1_Compliance_Generator.so
cp -rvf /home/cord/Documents/Source_Code/Level-Headed/SMB1/build-SMB1_Compliance_To_SMB1-Desktop-Debug/libSMB1_Compliance_To_SMB1.so /home/cord/Documents/Source_Code/Level-Headed/build-Level-Headed-Desktop-Debug/Plugins/Interpreters/SMB1_Compliance_To_SMB1.so
cp -rvf /home/cord/Documents/Source_Code/Level-Headed/SMB1/build-SMB1_Writer-Desktop-Debug/libSMB1_Writer.so /home/cord/Documents/Source_Code/Level-Headed/build-Level-Headed-Desktop-Debug/Plugins/Writers/SMB1_Writer.so
cp -rvf /home/cord/Documents/Source_Code/Hexagon/build-Hexagon-Desktop-Debug/libHexagon.so /home/cord/Documents/Source_Code/Level-Headed/build-Level-Headed-Desktop-Debug/Plugins/Hexagon.so
cp -rvf /home/cord/Documents/Source_Code/Sequential_Archive/build-Sequential_Archive-Desktop-Debug/libSequential_Archive.so /home/cord/Documents/Source_Code/Level-Headed/build-Level-Headed-Desktop-Debug/Plugins/Sequential_Archive.so
cp -rvf /home/cord/Documents/Source_Code/Level-Headed_Data/Graphics.sa /home/cord/Documents/Source_Code/Level-Headed/build-Level-Headed-Desktop-Debug/Data/SMB1/Graphics.sa
cp -rvf /home/cord/Documents/Source_Code/Level-Headed_Data/Music.sa /home/cord/Documents/Source_Code/Level-Headed/build-Level-Headed-Desktop-Debug/Data/SMB1/Music.sa
echo "Plugins Updated!"
exit 0

