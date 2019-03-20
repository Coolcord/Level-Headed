#!/bin/bash

# Check if dependencies are installed
command -v qmake >/dev/null 2>&1 || { echo >&2 "qmake must be installed before Level-Headed can be compiled! Aborting!"; exit 1; }
command -v git >/dev/null 2>&1 || { echo >&2 "git must be installed before Level-Headed can be compiled! Aborting!"; exit 1; }
command -v nproc >/dev/null 2>&1 || { echo >&2 "nproc must be installed before Level-Headed can be compiled! Aborting!"; exit 1; }

CPUcores=$(nproc)

# Prepare Folders And Pull Code
rm -rf Level-Headed
mkdir Level-Headed
cd Level-Headed
mkdir source
cd source
git clone https://github.com/Coolcord/Level-Headed.git
git clone https://github.com/Coolcord/Level-Headed_Data.git
git clone https://github.com/Coolcord/Hexagon.git
git clone https://github.com/Coolcord/Sequential_Archive.git
git clone https://github.com/Coolcord/C_Common_Code.git

# Build Level-Headed
cd Level-Headed/Level-Headed/
sed -i '/INCLUDEPATH += \./a QT += gui widgets' Level-Headed.pro
qmake -config release
make -j $CPUcores || exit 1
cd ../../

# Build the SMB1 Compliance Level Generator Plugin
cd Level-Headed/SMB1/SMB1_Compliance_Generator/
sed -i 's/.*TEMPLATE \= app.*/TEMPLATE \= lib/' SMB1_Compliance_Generator.pro
sed -i '/INCLUDEPATH += \./a CONFIG += plugin\nQT += gui widgets' SMB1_Compliance_Generator.pro
qmake -config release
make -j $CPUcores || exit 1
cd ../../../

# Build the SMB1 Compliance to SMB1 Interpreter Plugin
cd Level-Headed/SMB1/SMB1_Compliance_To_SMB1/
sed -i 's/.*TEMPLATE \= app.*/TEMPLATE \= lib/' SMB1_Compliance_To_SMB1.pro
sed -i '/INCLUDEPATH += \./a CONFIG += plugin\nQT += gui widgets' SMB1_Compliance_To_SMB1.pro
qmake -config release
make -j $CPUcores || exit 1
cd ../../../

# Build the SMB1 Writer Plugin
cd Level-Headed/SMB1/SMB1_Writer/
sed -i 's/.*TEMPLATE \= app.*/TEMPLATE \= lib/' SMB1_Writer.pro
sed -i '/INCLUDEPATH += \./a CONFIG += plugin\nQT += gui widgets' SMB1_Writer.pro
qmake -config release
make -j $CPUcores || exit 1
cd ../../../

# Build the Hexagon Plugin
cd Hexagon/Hexagon/
sed -i 's/.*TEMPLATE \= app.*/TEMPLATE \= lib/' Hexagon.pro
sed -i '/INCLUDEPATH += \./a CONFIG += plugin\nQT += gui widgets' Hexagon.pro
qmake -config release
make -j $CPUcores || exit 1
cd ../../

# Build the Sequential Archive Plugin
cd Sequential_Archive/Sequential_Archive/
sed -i 's/.*TEMPLATE \= app.*/TEMPLATE \= lib/' Sequential_Archive.pro
sed -i '/INCLUDEPATH += \./a CONFIG += plugin\nQT += gui widgets' Sequential_Archive.pro
qmake -config release
make -j $CPUcores || exit 1
cd ../../

# Build SAM
cd Sequential_Archive/Sequential_Archive_Manager/
sed -i '/INCLUDEPATH += \./a QT += gui widgets' Sequential_Archive_Manager.pro
qmake -config release
make -j $CPUcores || exit 1
cd ../../

# Create the Runtime Environment
cd ../
mkdir -p Data/SMB1/
mkdir -p Levels/SMB1/
mkdir -p Plugins/Generators/
mkdir -p Plugins/Interpreters/
mkdir -p Plugins/Writers/
mkdir -p source/Sequential_Archive/Sequential_Archive_Manager/Plugins/

# Install Plugins and Data
releaseDirectory=source/Level-Headed/Level-Headed/release
if [ -d "$releaseDirectory" ]; then # assume we're on a Unix environment running in a Windows OS
    cp source/Level-Headed/Level-Headed/release/Level-Headed.exe Level-Headed.exe
    chmod +x Level-Headed.exe
    cp source/Level-Headed/SMB1/SMB1_Compliance_Generator/release/SMB1_Compliance_Generator.dll Plugins/Generators/SMB1_Compliance_Generator.dll
    cp source/Level-Headed/SMB1/SMB1_Compliance_To_SMB1/release/SMB1_Compliance_To_SMB1.dll Plugins/Interpreters/SMB1_Compliance_To_SMB1.dll
    cp source/Level-Headed/SMB1/SMB1_Writer/release/SMB1_Writer.dll Plugins/Writers/SMB1_Writer.dll
    cp source/Hexagon/Hexagon/release/Hexagon.dll Plugins/Hexagon.dll
    cp source/Sequential_Archive/Sequential_Archive/release/Sequential_Archive.dll Plugins/Sequential_Archive.dll
    cp source/Sequential_Archive/Sequential_Archive/release/Sequential_Archive.dll source/Sequential_Archive/Sequential_Archive_Manager/Plugins/Sequential_Archive.dll
    source/Sequential_Archive/Sequential_Archive_Manager/release/Sequential_Archive_Manager.exe --pack source/Level-Headed_Data/Graphics Data/SMB1/Graphics.sa
    source/Sequential_Archive/Sequential_Archive_Manager/release/Sequential_Archive_Manager.exe --pack source/Level-Headed_Data/Music Data/SMB1/Music.sa
else # assume we're on GNU/Linux or Mac
    cp source/Level-Headed/Level-Headed/Level-Headed Level-Headed
    chmod +x Level-Headed
    cp source/Level-Headed/SMB1/SMB1_Compliance_Generator/libSMB1_Compliance_Generator.so Plugins/Generators/SMB1_Compliance_Generator.so
    cp source/Level-Headed/SMB1/SMB1_Compliance_To_SMB1/libSMB1_Compliance_To_SMB1.so Plugins/Interpreters/SMB1_Compliance_To_SMB1.so
    cp source/Level-Headed/SMB1/SMB1_Writer/libSMB1_Writer.so Plugins/Writers/SMB1_Writer.so
    cp source/Hexagon/Hexagon/libHexagon.so Plugins/Hexagon.so
    cp source/Sequential_Archive/Sequential_Archive/libSequential_Archive.so Plugins/Sequential_Archive.so
    cp source/Sequential_Archive/Sequential_Archive/libSequential_Archive.so source/Sequential_Archive/Sequential_Archive_Manager/Plugins/Sequential_Archive.so
    source/Sequential_Archive/Sequential_Archive_Manager/Sequential_Archive_Manager --pack source/Level-Headed_Data/Graphics Data/SMB1/Graphics.sa
    source/Sequential_Archive/Sequential_Archive_Manager/Sequential_Archive_Manager --pack source/Level-Headed_Data/Music Data/SMB1/Music.sa
fi

# Clean up
rm -rf source/

echo ""
echo "Compilation complete! Enjoy Level-Headed!"
exit 0

