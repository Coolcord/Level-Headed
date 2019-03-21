#!/bin/bash

MODE="Debug"
SOURCE_LOCATION="/home/cord/Documents/Source_Code"

echo "Installing plugins for Level-Headed..."
mkdir -p "$SOURCE_LOCATION"/Level-Headed/build-Level-Headed-Desktop-"$MODE"/Data/SMB1/
mkdir -p "$SOURCE_LOCATION"/Level-Headed/build-Level-Headed-Desktop-"$MODE"/Plugins/Generators/
mkdir -p "$SOURCE_LOCATION"/Level-Headed/build-Level-Headed-Desktop-"$MODE"/Plugins/Interpreters/
mkdir -p "$SOURCE_LOCATION"/Level-Headed/build-Level-Headed-Desktop-"$MODE"/Plugins/Writers/
cp -rf "$SOURCE_LOCATION"/Level-Headed/SMB1/build-SMB1_Compliance_Generator-Desktop-"$MODE"/libSMB1_Compliance_Generator.so "$SOURCE_LOCATION"/Level-Headed/build-Level-Headed-Desktop-"$MODE"/Plugins/Generators/SMB1_Compliance_Generator.so
cp -rf "$SOURCE_LOCATION"/Level-Headed/SMB1/build-SMB1_Compliance_To_SMB1-Desktop-"$MODE"/libSMB1_Compliance_To_SMB1.so "$SOURCE_LOCATION"/Level-Headed/build-Level-Headed-Desktop-"$MODE"/Plugins/Interpreters/SMB1_Compliance_To_SMB1.so
cp -rf "$SOURCE_LOCATION"/Level-Headed/SMB1/build-SMB1_Writer-Desktop-"$MODE"/libSMB1_Writer.so "$SOURCE_LOCATION"/Level-Headed/build-Level-Headed-Desktop-"$MODE"/Plugins/Writers/SMB1_Writer.so
cp -rf "$SOURCE_LOCATION"/Hexagon/build-Hexagon-Desktop-"$MODE"/libHexagon.so "$SOURCE_LOCATION"/Level-Headed/build-Level-Headed-Desktop-"$MODE"/Plugins/Hexagon.so
cp -rf "$SOURCE_LOCATION"/Sequential_Archive/build-Sequential_Archive-Desktop-"$MODE"/libSequential_Archive.so "$SOURCE_LOCATION"/Level-Headed/build-Level-Headed-Desktop-"$MODE"/Plugins/Sequential_Archive.so

echo "Installing plugin for SAM..."
mkdir -p "$SOURCE_LOCATION"/Sequential_Archive/build-Sequential_Archive_Manager-Desktop-"$MODE"/Plugins/
cp -rf "$SOURCE_LOCATION"/Sequential_Archive/build-Sequential_Archive-Desktop-"$MODE"/libSequential_Archive.so "$SOURCE_LOCATION"/Sequential_Archive/build-Sequential_Archive_Manager-Desktop-"$MODE"/Plugins/Sequential_Archive.so

echo "Packing Graphics and Music..."
"$SOURCE_LOCATION"/Sequential_Archive/build-Sequential_Archive_Manager-Desktop-"$MODE"/Sequential_Archive_Manager --pack "$SOURCE_LOCATION"/Level-Headed_Data/Graphics "$SOURCE_LOCATION"/Level-Headed/build-Level-Headed-Desktop-"$MODE"/Data/SMB1/Graphics.sa
"$SOURCE_LOCATION"/Sequential_Archive/build-Sequential_Archive_Manager-Desktop-"$MODE"/Sequential_Archive_Manager --pack "$SOURCE_LOCATION"/Level-Headed_Data/Music "$SOURCE_LOCATION"/Level-Headed/build-Level-Headed-Desktop-"$MODE"/Data/SMB1/Music.sa

echo "Runtime Environment Prepared!"
exit 0

