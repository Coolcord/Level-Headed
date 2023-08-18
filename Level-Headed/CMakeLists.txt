cmake_minimum_required(VERSION 3.14)

project(Level-Headed LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic -Wdeprecated -Werror")

find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets Network)

set(SOURCES
    ../../C_Common_Code/Qt/Git_Update_Checker/Git_Update_Checker.cpp
    ../../C_Common_Code/Qt/Qt_App_Theme_Fixes/Fusion_Fixes.cpp
    ../../C_Common_Code/Qt/Readable_Config_File/Readable_Config_File.cpp
    ../../C_Common_Code/Qt/Random/Random.cpp
    Main_Window.cpp
    Main.cpp
    Plugin_Handler.cpp
    CLI_Passthrough.cpp
    Update_Checker.cpp
    Update_Dialog.cpp
)

set(HEADERS
    Main_Window.h
    ../../C_Common_Code/Qt/Git_Update_Checker/Git_Update_Checker.h
    ../../C_Common_Code/Qt/Qt_App_Theme_Fixes/Fusion_Fixes.h
    ../../C_Common_Code/Qt/Readable_Config_File/Readable_Config_File.h
    ../../C_Common_Code/Qt/Random/Random.h
    ../Common_Files/Version.h
    Plugin_Handler.h
    Common_Strings.h
    Interpreter_Interface.h
    CLI_Passthrough.h
    Update_Checker.h
    Update_Dialog.h
)

set(FORMS
    Main_Window.ui
    Update_Dialog.ui
)

set(RC_FILE Level-Headed.rc)

# Remove WIN32 if you want the console window
add_executable(${PROJECT_NAME} WIN32 ${SOURCES} ${HEADERS} ${FORMS} ${RC_FILE})

target_link_libraries(${PROJECT_NAME} PRIVATE
    Qt6::Core
    Qt6::Gui
    Qt6::Widgets
    Qt6::Network
)

include(GNUInstallDirs)
install(TARGETS ${PROJECT_NAME}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
