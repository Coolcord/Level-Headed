#!/bin/bash

qt=Qt_6_5_2_MinGW_64_bit
mode=Debug
sourceLocation=/d/Documents/Source_Code

echo Installing Qt DLLs...
qtpaths6Location=$(which qtpaths6.exe)
qtDLLsLocation=${qtpaths6Location%/*}

# Install Qt Plugins
cd "$sourceLocation"/Sequential_Archive/build-Sequential_Archive_Manager-Desktop_"$qt"-"$mode"
mkdir -p Qt/platforms
echo [Paths] > qt.conf
echo Plugins=./Qt >> qt.conf
cp /mingw64/share/qt6/plugins/platforms/qwindows.dll Qt/platforms

# Install root Qt DLLs
ldd Sequential_Archive_Manager.exe | awk '{print $3}' > allDLLs.txt
ldd Plugins/Sequential_Archive.dll | awk '{print $3}' >> allDLLs.txt
grep -v "/Windows" allDLLs.txt > nonWindows.txt
sort nonWindows.txt | uniq > requiredDLLs.txt
while IFS= read -r requiredDLL; do
	cp "$requiredDLL" .
done < requiredDLLs.txt
rm allDLLs.txt nonWindows.txt requiredDLLs.txt

exit 0
