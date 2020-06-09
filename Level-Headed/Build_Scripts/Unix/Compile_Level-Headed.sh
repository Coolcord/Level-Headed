#!/bin/bash

# Check if dependencies are installed
command -v qmake >/dev/null 2>&1 || { echo >&2 "qmake must be installed before Level-Headed can be compiled! Aborting!"; exit 1; }
command -v git >/dev/null 2>&1 || { echo >&2 "git must be installed before Level-Headed can be compiled! Aborting!"; exit 1; }
command -v nproc >/dev/null 2>&1 || { echo >&2 "nproc must be installed before Level-Headed can be compiled! Aborting!"; exit 1; }

CPUcores=$(nproc)

# Prepare Folders And Pull Code
rm -rf Level-Headed/
mkdir Level-Headed; cd Level-Headed
mkdir source; cd source
git clone https://github.com/Coolcord/Level-Headed.git
git clone https://github.com/Coolcord/Level-Headed_Data.git
git clone https://github.com/Coolcord/Hexagon.git
git clone https://github.com/Coolcord/Sequential_Archive.git
git clone https://github.com/Coolcord/C_Common_Code.git

# Revert to stable code
if [ -z $1 ] || [ $1 != "latest" ]; then
    echo Using stable code...
    cd Level-Headed/; git checkout tags/v0.3.8-2 &> /dev/null
    cd ../Level-Headed_Data/; git checkout tags/v0.3.8-2 &> /dev/null
    cd ../Hexagon/; git checkout b6fe2de33ebefc600f3b77bd823e2eb52d05dd06 &> /dev/null
    cd ../Sequential_Archive/; git checkout 4c64c933127554aa6dec4f26f03c786f1756764e &> /dev/null
    cd ../C_Common_Code/; git checkout f533cc1aeb8bba855749e287d43e735602210ac1 &> /dev/null
    cd ..
else
    echo Using latest code...
fi

# Build Level-Headed
cd Level-Headed/Level-Headed/
sed -i '/INCLUDEPATH += \./a QT += gui widgets' Level-Headed.pro
qmake -config release
make -j "$CPUcores" || exit 1
cd ../../

# Build the SMB1 Compliance Level Generator Plugin
cd Level-Headed/SMB1/SMB1_Compliance_Generator/
sed -i 's/.*TEMPLATE \= app.*/TEMPLATE \= lib/' SMB1_Compliance_Generator.pro
sed -i '/INCLUDEPATH += \./a CONFIG += plugin\nQT += gui widgets' SMB1_Compliance_Generator.pro
qmake -config release
make -j "$CPUcores" || exit 1
cd ../../../

# Build the SMB1 Compliance to SMB1 Interpreter Plugin
cd Level-Headed/SMB1/SMB1_Compliance_To_SMB1/
sed -i 's/.*TEMPLATE \= app.*/TEMPLATE \= lib/' SMB1_Compliance_To_SMB1.pro
sed -i '/INCLUDEPATH += \./a CONFIG += plugin\nQT += gui widgets' SMB1_Compliance_To_SMB1.pro
qmake -config release
make -j "$CPUcores" || exit 1
cd ../../../

# Build the SMB1 Writer Plugin
cd Level-Headed/SMB1/SMB1_Writer/
sed -i 's/.*TEMPLATE \= app.*/TEMPLATE \= lib/' SMB1_Writer.pro
sed -i '/INCLUDEPATH += \./a CONFIG += plugin\nQT += gui widgets' SMB1_Writer.pro
qmake -config release
make -j "$CPUcores" || exit 1
cd ../../../

# Build the Hexagon Plugin
cd Hexagon/Hexagon/
sed -i 's/.*TEMPLATE \= app.*/TEMPLATE \= lib/' Hexagon.pro
sed -i '/INCLUDEPATH += \./a CONFIG += plugin\nQT += gui widgets' Hexagon.pro
qmake -config release
make -j "$CPUcores" || exit 1
cd ../../

# Build the Sequential Archive Plugin
cd Sequential_Archive/Sequential_Archive/
sed -i 's/.*TEMPLATE \= app.*/TEMPLATE \= lib/' Sequential_Archive.pro
sed -i '/INCLUDEPATH += \./a CONFIG += plugin\nQT += gui widgets' Sequential_Archive.pro
qmake -config release
make -j "$CPUcores" || exit 1
cd ../../

# Build SAM
cd Sequential_Archive/Sequential_Archive_Manager/
sed -i '/INCLUDEPATH += \./a QT += gui widgets' Sequential_Archive_Manager.pro
qmake -config release
make -j "$CPUcores" || exit 1
cd ../../

# Create the Runtime Environment
cd ../
mkdir -p Data/SMB1/
mkdir -p Levels/SMB1/
mkdir -p Plugins/Generators/
mkdir -p Plugins/Interpreters/
mkdir -p Plugins/Writers/

# Install Plugins and Data
releaseDirectory=source/Level-Headed/Level-Headed/release
if [ -d "$releaseDirectory" ]; then # assume we're on a Unix environment running in a Windows OS
    dllExt=".dll"
    exeExt=".exe"
    binDir="/release"
    libPrefix=""
else # assume we're on GNU/Linux or Mac
    releaseDirectory=source/Level-Headed/Level-Headed
    dllExt=".so"
    exeExt=""
    binDir=""
    libPrefix="lib"
fi
mkdir -p source/Sequential_Archive/Sequential_Archive_Manager"$binDir"/Plugins/
cp source/Level-Headed/Level-Headed"$binDir"/Level-Headed"$exeExt" Level-Headed"$exeExt"
chmod +x Level-Headed"$exeExt"
cp source/Level-Headed/SMB1/SMB1_Compliance_Generator"$binDir"/"$libPrefix"SMB1_Compliance_Generator"$dllExt" Plugins/Generators/SMB1_Compliance_Generator"$dllExt"
cp source/Level-Headed/SMB1/SMB1_Compliance_To_SMB1"$binDir"/"$libPrefix"SMB1_Compliance_To_SMB1"$dllExt" Plugins/Interpreters/SMB1_Compliance_To_SMB1"$dllExt"
cp source/Level-Headed/SMB1/SMB1_Writer"$binDir"/"$libPrefix"SMB1_Writer"$dllExt" Plugins/Writers/SMB1_Writer"$dllExt"
cp source/Hexagon/Hexagon"$binDir"/"$libPrefix"Hexagon"$dllExt" Plugins/Hexagon"$dllExt"
cp source/Sequential_Archive/Sequential_Archive"$binDir"/"$libPrefix"Sequential_Archive"$dllExt" Plugins/Sequential_Archive"$dllExt"
cp source/Sequential_Archive/Sequential_Archive"$binDir"/"$libPrefix"Sequential_Archive"$dllExt" source/Sequential_Archive/Sequential_Archive_Manager"$binDir"/Plugins/Sequential_Archive"$dllExt"
echo Packing assets...
source/Sequential_Archive/Sequential_Archive_Manager"$binDir"/Sequential_Archive_Manager"$exeExt" --pack source/Level-Headed_Data/Graphics Data/SMB1/Graphics.sa
source/Sequential_Archive/Sequential_Archive_Manager"$binDir"/Sequential_Archive_Manager"$exeExt" --pack source/Level-Headed_Data/Music Data/SMB1/Music.sa
source/Sequential_Archive/Sequential_Archive_Manager"$binDir"/Sequential_Archive_Manager"$exeExt" --pack source/Level-Headed_Data/ROMs Data/SMB1/ROMs.sa
source/Sequential_Archive/Sequential_Archive_Manager"$binDir"/Sequential_Archive_Manager"$exeExt" --pack source/Level-Headed_Data/Text Data/SMB1/Text.sa
source/Sequential_Archive/Sequential_Archive_Manager"$binDir"/Sequential_Archive_Manager"$exeExt" --pack 'source/Level-Headed_Data/Level_Scripts/(Tech Demo) It Plays Itself' 'Levels/SMB1/(Tech Demo) It Plays Itself.lvls'
source/Sequential_Archive/Sequential_Archive_Manager"$binDir"/Sequential_Archive_Manager"$exeExt" --pack 'source/Level-Headed_Data/Level_Scripts/(Tech Demo) Powerup Test' 'Levels/SMB1/(Tech Demo) Powerup Test.lvls'
source/Sequential_Archive/Sequential_Archive_Manager"$binDir"/Sequential_Archive_Manager"$exeExt" --pack 'source/Level-Headed_Data/Level_Scripts/(Tech Demo) Vertical Limit Test' 'Levels/SMB1/(Tech Demo) Vertical Limit Test.lvls'
source/Sequential_Archive/Sequential_Archive_Manager"$binDir"/Sequential_Archive_Manager"$exeExt" --pack 'source/Level-Headed_Data/Level_Scripts/Super Mario Bros. 1 (Modified Original Levels)' 'Levels/SMB1/Super Mario Bros. 1 (Modified Original Levels).lvls'

# Clean up
rm -rf source/

echo ""
echo "Compilation complete! Enjoy Level-Headed!"
exit 0

