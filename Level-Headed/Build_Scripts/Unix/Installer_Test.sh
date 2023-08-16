#!/bin/bash
cd ./Deployed_Files
localSourceCodeLocation="/d/Documents/Source_Code"


# Prepare Installer for Compilation
echo ""; echo "Compiling Level-Headed Installer..."
startRmdirSectionMarker="# --------------- BEGIN AUTO-GENERATED RMDIR SECTION --------------- #"
endRmdirSectionMarker="# ---------------- END AUTO-GENERATED RMDIR SECTION ---------------- #"
startSetOutPathSectionMarker="# ------------- BEGIN AUTO-GENERATED SETOUTPATH SECTION ------------ #"
endSetOutPathSectionMarker="# -------------- END AUTO-GENERATED SETOUTPATH SECTION ------------- #"
startFileSectionMarker="# --------------- BEGIN AUTO-GENERATED FILE SECTION ---------------- #"
endFileSectionMarker="# ---------------- END AUTO-GENERATED FILE SECTION ----------------- #"

startDeleteSectionMarker="# -------------- BEGIN AUTO-GENERATED DELETE SECTION --------------- #"
endDeleteSectionMarker="# --------------- END AUTO-GENERATED DELETE SECTION ---------------- #"
startUninstallSectionMarker="# ------------- BEGIN AUTO-GENERATED UNINSTALL SECTION ------------- #"
endUninstallSectionMarker="# -------------- END AUTO-GENERATED UNINSTALL SECTION -------------- #"

installerFile="$localSourceCodeLocation"/Level-Headed/Level-Headed/Build_Scripts/Windows/Installer/Level-Headed.nsi
rm rmdirSection.txt setOutPathSection.txt fileSection.txt deleteSectionUnsorted.txt deleteSection.txt uninstallSectionUnsorted.txt uninstallSection.txt > /dev/null 2>&1

# Prepare RMDIR Section
find ./Level-Headed -maxdepth 1 -type d -print > directories.txt
sed -i 's|/|\\|g' directories.txt
sed -i 's|\.\\Level-Headed|\$INSTDIR|g' directories.txt
sed -i '/\\Config/d' directories.txt # don't delete config directories
while IFS= read -r directory; do
    echo RmDir /r \""$directory"\" >> rmdirSection.txt
done < directories.txt
sed -i '/\"$INSTDIR\"/d' directories.txt # don't delete the base directory
sed -i '/^RmDir \/r "$INSTDIR"$/d' rmdirSection.txt

# Prepare SetOutputPath Section
find ./Level-Headed -type d -print > directories.txt
sed -i 's|/|\\|g' directories.txt
sed -i 's|\.\\Level-Headed|\$INSTDIR|g' directories.txt
while IFS= read -r directory; do
    echo SetOutPath \""$directory"\" >> setOutPathSection.txt
done < directories.txt

# Prepare File Section
find ./Level-Headed -type f -print > files.txt
sed -i "s|\.\/|$localSourceCodeLocation\/|g" files.txt
while IFS= read -r file; do
    echo File \""$file"\" >> fileSection.txt
done < files.txt

# Prepare Delete Section
find ./Level-Headed -type f -print > files.txt
sed -i 's|/|\\|g' files.txt
sed -i 's|\.\\Level-Headed|$INSTDIR|g' files.txt
while IFS= read -r file; do
    echo Delete \""$file"\" >> deleteSectionUnsorted.txt
done < files.txt
sed -i '/Level-Headed\.exe/d' deleteSectionUnsorted.txt # don't delete the Level-Headed exe for now
awk '{ print length($0) " " $0; }' deleteSectionUnsorted.txt | sort -n -r | cut -d ' ' -f 2- > deleteSection.txt # sort from longest to shortest

# Prepare Uninstall Section
find ./Level-Headed -type d -print > directories.txt
sed -i 's|/|\\|g' directories.txt
sed -i 's|\.\\Level-Headed|\$INSTDIR|g' directories.txt
while IFS= read -r directory; do
    echo RmDir /r \""$directory"\" >> uninstallSectionUnsorted.txt
done < directories.txt
sed -i '/\"$INSTDIR\"/d' directories.txt # don't delete the base directory
sed -i '/^RmDir \/r "$INSTDIR"$/d' uninstallSectionUnsorted.txt
awk '{ print length($0) " " $0; }' uninstallSectionUnsorted.txt | sort -n -r | cut -d ' ' -f 2- > uninstallSection.txt # sort from longest to shortest

convert_unix_paths_to_windows() {
    sed -i 's|/a/|A:/|g; s|/b/|B:/|g; s|/c/|C:/|g; s|/d/|D:/|g; s|/e/|E:/|g; s|/f/|F:/|g; s|/g/|G:/|g; s|/h/|H:/|g; s|/i/|I:/|g; s|/j/|J:/|g; s|/k/|K:/|g; s|/l/|L:/|g; s|/m/|M:/|g; s|/n/|N:/|g; s|/o/|O:/|g; s|/p/|P:/|g; s|/q/|Q:/|g; s|/r/|R:/|g; s|/s/|S:/|g; s|/t/|T:/|g; s|/u/|U:/|g; s|/v/|V:/|g; s|/w/|W:/|g; s|/x/|X:/|g; s|/y/|Y:/|g; s|/z/|Z:/|g; s|/|\\|g' "$1"
}

convert_unix_paths_to_windows "fileSection.txt"
convert_unix_paths_to_windows "deleteSection.txt"

update_section() {
    # Find the line numbers of the markers
    startLine=$(grep -n "$1" "$installerFile" | cut -d ':' -f 1)
    endLine=$(grep -n "$2" "$installerFile" | cut -d ':' -f 1)

    # Check if markers were found
    if [ -z "$startLine" ] || [ -z "$endLine" ]; then
      echo "Markers not found in $installerFile"
      exit 1
    fi

    # Calculate the line range to delete
    deleteRange="$((startLine+1)),${endLine}d"
    
    # Restore the end section marker
    echo $2 >> $3

    # Delete lines between markers and insert contents of fileSection.txt
    sed -i "$deleteRange" "$installerFile"
    sed -i "${startLine}r $3" "$installerFile"
}

update_section "$startRmdirSectionMarker" "$endRmdirSectionMarker" "rmdirSection.txt"
update_section "$startSetOutPathSectionMarker" "$endSetOutPathSectionMarker" "setOutPathSection.txt"
update_section "$startFileSectionMarker" "$endFileSectionMarker" "fileSection.txt"
update_section "$startDeleteSectionMarker" "$endDeleteSectionMarker" "deleteSection.txt"
update_section "$startUninstallSectionMarker" "$endUninstallSectionMarker" "uninstallSection.txt"
rm rmdirSection.txt setOutPathSection.txt fileSection.txt deleteSectionUnsorted.txt deleteSection.txt uninstallSectionUnsorted.txt uninstallSection.txt directories.txt files.txt

# TODO: Handle uninstaller sections (Delete files and RMDIR)
# TODO: Handle installer name
# TODO: Handle License

# Compile the Level-Headed installer for users who want an installer
# TODO: Write this...
echo "Done!"
