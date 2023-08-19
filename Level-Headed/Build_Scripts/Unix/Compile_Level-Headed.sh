#!/bin/bash

# Possible arguments: stable, latest, local
# stable: Build from last stable version on remote Git
# latest: Build from latest version on remote Git
# local: Build from local code. Specify localSourceCodeLocation below!!!

# Setting this is only necessary if using the local argument
localSourceCodeLocation="/d/Documents/Source_Code"

# Install MinGW dependencies
if [ ${MSYSTEM} == "MINGW64" ]; then
    dependencies="git rsync mingw-w64-x86_64-gcc mingw-w64-x86_64-ninja mingw-w64-x86_64-cmake mingw-w64-x86_64-qt6-base"
    
    echo Checking dependencies for compilation...
    if ! pacman -Q $dependencies > /dev/null 2>&1; then
        echo Installing missing dependencies...
        pacman -Sy --needed --noconfirm $dependencies
    fi
fi

# Check if dependencies are installed
echo ""; echo [1/11] Preparing source code...
command -v ninja >/dev/null 2>&1 || { echo >&2 "ninja must be installed before Level-Headed can be compiled! Aborting!"; exit 1; }
command -v git >/dev/null 2>&1 || { echo >&2 "git must be installed before Level-Headed can be compiled! Aborting!"; exit 1; }
command -v gcc >/dev/null 2>&1 || { echo >&2 "gcc must be installed before Level-Headed can be compiled! Aborting!"; exit 1; }
command -v nproc >/dev/null 2>&1 || { echo >&2 "nproc must be installed before Level-Headed can be compiled! Aborting!"; exit 1; }
if [ ${MSYSTEM} == "MINGW64" ]; then
    command -v qtpaths6 >/dev/null 2>&1 || { echo >&2 "qtpaths6 must be installed before Level-Headed can be compiled! Aborting!"; exit 1; }
    command -v ldd >/dev/null 2>&1 || { echo >&2 "ldd must be installed before Level-Headed can be compiled! Aborting!"; exit 1; }
    if [ ! -f /mingw64/share/qt6/plugins/platforms/qwindows.dll ]; then
        echo "qwindows.dll could not be found! Aborting!"; exit 1;
    fi
    if [ ! -f /mingw64/share/qt6/plugins/styles/qwindowsvistastyle.dll ]; then
        echo "qwindowsvistastyle.dll could not be found! Aborting!"; exit 1;
    fi
	if [ ! -f /mingw64/share/qt6/plugins/tls/qschannelbackend.dll ]; then
        echo "qschannelbackend.dll could not be found! Aborting!"; exit 1;
    fi
fi

CPUcores=$(nproc)

# Prepare Folders And Pull Code
rm -rf Level-Headed/
mkdir Level-Headed; cd Level-Headed
mkdir source; cd source
if [ -z $1 ] || [ $1 != "local" ]; then
    git clone https://github.com/Coolcord/Level-Headed.git
    git clone https://github.com/Coolcord/Level-Headed_Data.git
    git clone https://github.com/Coolcord/Hexagon.git
    git clone https://github.com/Coolcord/Sequential_Archive.git
    git clone https://github.com/Coolcord/C_Common_Code.git
else
    echo Using local code...
    
    # Copy Level-Headed carefully, just in case this script is running in this folder
    echo Copying Level-Headed source code...
    
    # Prepare for Level-Headed copy
    mkdir -p Level-Headed/Level-Headed/Build_Scripts/Unix
    copy_excluding_folder() {
        cp -f "$1"/* "$2" >/dev/null 2>&1
        find "$1" -mindepth 1 -maxdepth 1 -type d -exec basename {} \; > folders.txt
        sed -i "/$3/d" folders.txt
        while IFS= read -r folder; do
            cp -rf "$1"/"$folder" "$2"
        done < folders.txt
        rm folders.txt
    }
    
    # Copy Level-Headed, but ignore "./Level-Headed/Level-Headed/Build_Scripts/Unix/Level-Headed" in case this script is running there
    copy_excluding_folder "$localSourceCodeLocation/Level-Headed" "./Level-Headed" "Level-Headed"
    copy_excluding_folder "$localSourceCodeLocation/Level-Headed/Level-Headed" "./Level-Headed/Level-Headed" "Build_Scripts"
    copy_excluding_folder "$localSourceCodeLocation/Level-Headed/Level-Headed/Build_Scripts" "./Level-Headed/Level-Headed/Build_Scripts" "Unix"
    copy_excluding_folder "$localSourceCodeLocation/Level-Headed/Level-Headed/Build_Scripts/Unix" "./Level-Headed/Level-Headed/Build_Scripts/Unix" "Level-Headed"
    
    # Copy the rest of the files
    echo Copying Level-Headed_Data source code...
    cp -rf "$localSourceCodeLocation"/Level-Headed_Data .
    echo Copying Hexagon source code...
    cp -rf "$localSourceCodeLocation"/Hexagon .
    echo Copying Sequential Archive source code...
    cp -rf "$localSourceCodeLocation"/Sequential_Archive .
    echo Copying C_Common_Code source code...
    cp -rf "$localSourceCodeLocation"/C_Common_Code .
fi

# Revert to stable code
if [ -z $1 ] || ([ $1 != "latest" ] && [ $1 != "local" ]); then
    echo Using stable code...
fi

cd Level-Headed/; git checkout tags/v0.3.10 &> /dev/null
cd ../Level-Headed_Data/; git checkout tags/v0.3.10 &> /dev/null
cd ../Hexagon/; git checkout tags/v1.1.2 &> /dev/null
cd ../Sequential_Archive/; git checkout tags/v1.1.2 &> /dev/null
cd ../C_Common_Code/; git checkout 53d15d76cb1171537becc144960721fc472505a8 &> /dev/null
cd ..
    
if [ ! -z $1 ] && [ $1 == "latest" ]; then
    echo Using latest code...
fi
    
# Build Level-Headed
echo ""; echo [2/11] Compiling Level-Headed...
cd Level-Headed/Level-Headed/
rm -rf build >/dev/null 2>&1; mkdir -p build
cd build
cmake .. -G Ninja
ninja -j "$CPUcores" || exit 1
cd ../../../

# Build the SMB1 Compliance Level Generator Plugin
echo ""; echo [3/11] Compiling the SMB1 Compliance Level Generator Plugin...
cd Level-Headed/SMB1/SMB1_Compliance_Generator/
rm -rf build >/dev/null 2>&1; mkdir -p build
cd build
cmake .. -G Ninja
ninja -j "$CPUcores" || exit 1
cd ../../../../

# Build the SMB1 Compliance to SMB1 Interpreter Plugin
echo ""; echo [4/11] Compiling the SMB1 Compliance to SMB1 Interpreter Plugin...
cd Level-Headed/SMB1/SMB1_Compliance_To_SMB1/
rm -rf build >/dev/null 2>&1; mkdir -p build
cd build
cmake .. -G Ninja
ninja -j "$CPUcores" || exit 1
cd ../../../../

# Build the SMB1 Writer Plugin
echo ""; echo [5/11] Compiling the SMB1 Writer Plugin...
cd Level-Headed/SMB1/SMB1_Writer/
rm -rf build >/dev/null 2>&1; mkdir -p build
cd build
cmake .. -G Ninja
ninja -j "$CPUcores" || exit 1
cd ../../../../

# Build the Hexagon Plugin
echo ""; echo [6/11] Compiling the Hexagon Plugin...
cd Hexagon/Hexagon/
rm -rf build >/dev/null 2>&1; mkdir -p build
cd build
cmake .. -G Ninja
ninja -j "$CPUcores" || exit 1
cd ../../../

# Build the Sequential Archive Plugin
echo ""; echo [7/11] Compiling the Sequential Archive Plugin...
cd Sequential_Archive/Sequential_Archive/
rm -rf build >/dev/null 2>&1; mkdir -p build
cd build
cmake .. -G Ninja
ninja -j "$CPUcores" || exit 1
cd ../../../

# Build SAM
echo ""; echo [8/11] Compiling SAM...
cd Sequential_Archive/Sequential_Archive_Manager/
sed -i 's/WIN32 //g' CMakeLists.txt
rm -rf build >/dev/null 2>&1; mkdir -p build
cd build
cmake .. -G Ninja
ninja -j "$CPUcores" || exit 1
cd ../../../

# Create the Runtime Environment
cd ../
mkdir -p Data/SMB1/
mkdir -p Levels/SMB1/
mkdir -p Plugins/Generators/
mkdir -p Plugins/Interpreters/
mkdir -p Plugins/Writers/

# Install Plugins and Data
if [ -z ${MSYSTEM} ]; then # assume we're on GNU/Linux or Mac
    dllExt=".so"
    exeExt=""
else # assume we're on a Unix environment running in a Windows OS
    dllExt=".dll"
    exeExt=".exe"
fi

# Pack Assets (do this before installing plugins to prevent any DLLs from being called unintentionally)
echo ""; echo [9/11] Packing assets...
mkdir -p source/Sequential_Archive/Sequential_Archive_Manager/build/Plugins/
cp source/Sequential_Archive/Sequential_Archive/build/libSequential_Archive"$dllExt" source/Sequential_Archive/Sequential_Archive_Manager/build/Plugins/Sequential_Archive"$dllExt"
source/Sequential_Archive/Sequential_Archive_Manager/build/Sequential_Archive_Manager"$exeExt" --pack source/Level-Headed_Data/Graphics Data/SMB1/Graphics.sa || exit 1
source/Sequential_Archive/Sequential_Archive_Manager/build/Sequential_Archive_Manager"$exeExt" --pack source/Level-Headed_Data/Music Data/SMB1/Music.sa || exit 1
source/Sequential_Archive/Sequential_Archive_Manager/build/Sequential_Archive_Manager"$exeExt" --pack source/Level-Headed_Data/ROMs Data/SMB1/ROMs.sa || exit 1
source/Sequential_Archive/Sequential_Archive_Manager/build/Sequential_Archive_Manager"$exeExt" --pack source/Level-Headed_Data/Text Data/SMB1/Text.sa || exit 1
source/Sequential_Archive/Sequential_Archive_Manager/build/Sequential_Archive_Manager"$exeExt" --pack 'source/Level-Headed_Data/Level_Scripts/(Tech Demo) It Plays Itself' 'Levels/SMB1/(Tech Demo) It Plays Itself.lvls' || exit 1
source/Sequential_Archive/Sequential_Archive_Manager/build/Sequential_Archive_Manager"$exeExt" --pack 'source/Level-Headed_Data/Level_Scripts/(Tech Demo) Powerup Test' 'Levels/SMB1/(Tech Demo) Powerup Test.lvls' || exit 1
source/Sequential_Archive/Sequential_Archive_Manager/build/Sequential_Archive_Manager"$exeExt" --pack 'source/Level-Headed_Data/Level_Scripts/(Tech Demo) Vertical Limit Test' 'Levels/SMB1/(Tech Demo) Vertical Limit Test.lvls' || exit 1
source/Sequential_Archive/Sequential_Archive_Manager/build/Sequential_Archive_Manager"$exeExt" --pack 'source/Level-Headed_Data/Level_Scripts/Super Mario Bros. 1 (Modified Original Levels)' 'Levels/SMB1/Super Mario Bros. 1 (Modified Original Levels).lvls' || exit 1

# Install Plugins
echo ""; echo [10/11] Installing Plugins...
cp source/Level-Headed/Level-Headed/build/Level-Headed"$exeExt" Level-Headed"$exeExt"
chmod +x Level-Headed"$exeExt"
cp source/Level-Headed/SMB1/SMB1_Compliance_Generator/build/libSMB1_Compliance_Generator"$dllExt" Plugins/Generators/SMB1_Compliance_Generator"$dllExt"
cp source/Level-Headed/SMB1/SMB1_Compliance_To_SMB1/build/libSMB1_Compliance_To_SMB1"$dllExt" Plugins/Interpreters/SMB1_Compliance_To_SMB1"$dllExt"
cp source/Level-Headed/SMB1/SMB1_Writer/build/libSMB1_Writer"$dllExt" Plugins/Writers/SMB1_Writer"$dllExt"
cp source/Hexagon/Hexagon/build/libHexagon"$dllExt" Plugins/Hexagon"$dllExt"
cp source/Sequential_Archive/Sequential_Archive/build/libSequential_Archive"$dllExt" Plugins/Sequential_Archive"$dllExt"

# Install Qt DLLs
if [ ${dllExt} == ".dll" ]; then
    qtpaths6Location=$(which qtpaths6.exe)
    qtDLLsLocation=${qtpaths6Location%/*}
    
    # Install Qt Plugins
    mkdir Qt
    echo [Paths] > qt.conf
    echo Plugins=./Qt >> qt.conf
    cp -rf /mingw64/share/qt6/plugins/* ./Qt
    
    # Install root Qt DLLs
    ldd Level-Headed"$exeExt" | awk '{print $3}' > allDLLs.txt
    ldd Plugins/Hexagon"$dllExt" | awk '{print $3}' >> allDLLs.txt
    ldd Plugins/Sequential_Archive"$dllExt" | awk '{print $3}' >> allDLLs.txt
    ldd Plugins/Generators/SMB1_Compliance_Generator"$dllExt" | awk '{print $3}' >> allDLLs.txt
    ldd Plugins/Interpreters//SMB1_Compliance_To_SMB1"$dllExt" | awk '{print $3}' >> allDLLs.txt
    ldd Plugins/Writers/SMB1_Writer"$dllExt" | awk '{print $3}' >> allDLLs.txt
    grep -v "/Windows" allDLLs.txt > nonWindows.txt
    sort nonWindows.txt | uniq > requiredDLLs.txt
    while IFS= read -r requiredDLL; do
        cp "$requiredDLL" .
    done < requiredDLLs.txt
    rm allDLLs.txt nonWindows.txt requiredDLLs.txt
fi
echo Done!

# Clean up
echo ""; echo [11/11] Cleaning up...
rm -rf source/
echo Done!

echo ""; echo "Compilation complete! Enjoy Level-Headed!"
exit 0
