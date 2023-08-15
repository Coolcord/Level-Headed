#!/bin/bash

# Make sure a version number was provided
if [ -z $1 ]; then
    echo "No version argument provided! The first argument must be the version!"
    echo ""
    echo "$0 <version>"
    echo ""
    echo "E.g.: $0 v0.4.0"
    exit 1
fi

# Extract the source code location
if [ ! -f ./Compile_Level-Headed.sh ]; then
    echo "Compile_Level-Headed.sh could not be found! Aborting!"; exit 1
fi
eval "$(cat ./Compile_Level-Headed.sh | grep 'localSourceCodeLocation=\"')"
if [ ! -d "$localSourceCodeLocation" ]; then
    echo "Source code not found at \"$localSourceCodeLocation\". Aborting!"; exit 1
fi
if [ ! -f "$localSourceCodeLocation"/Level-Headed/Common_Files/Version.h ]; then
    echo "Version.h could not be found! Aborting!"; exit 1
fi
if [ ! -f "$localSourceCodeLocation"/Level-Headed/Level-Headed/Build_Scripts/Windows/Installer/Level-Headed.nsi ]; then
    echo "Level-Headed.nsi could not be found! Aborting!"; exit 1
fi

# Add v to the version if it doesn't exist
version="$1"
if [[ "${original}" != v* ]]; then
    modified="v${original}"
fi
versionNumber="${version:1}"

# Split the parts into significant, major, minor, and patch versions
IFS=. read -r significant major minor <<< "$versionNumber"
IFS=- read -r minor patch <<< "$minor"
if [ -z $patch ]; then
    patch="0"
fi
if [[ $version == *-dev ]]; then
    patch="0"
fi
installerVersion="$significant.$major.$minor.$patch"

# Update the Level-Headed version
sed -i "s/    const static QString VERSION_NUMBER = .*/    const static QString VERSION_NUMBER = \"$versionNumber\"\;/g" "$localSourceCodeLocation"/Level-Headed/Common_Files/Version.h

# Update the Installer version
sed -i "s/!define VERSION \".*/!define VERSION \"$installerVersion\"/g" "$localSourceCodeLocation"/Level-Headed/Level-Headed/Build_Scripts/Windows/Installer/Level-Headed.nsi

# Update the Readme version

# Compile Level-Headed
sh ./Compile_Level-Headed.sh local || exit 1

# Compile the Level-Headed installer for users who want an installer

# Zip up Level-Headed archive for users who don't want an installer

exit 0
