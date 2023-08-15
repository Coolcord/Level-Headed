cmake_minimum_required(VERSION 3.14)

project(SMB1_Compliance_To_SMB1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic -Wdeprecated -Werror")

find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets)

set(SOURCES
    SMB1_Compliance_To_SMB1.cpp
    ../../../C_Common_Code/Qt/Text_Insertion_Buffer/Text_Insertion_Buffer.cpp
    ../SMB1_Compliance_Generator/Enemy_Buffer.cpp
    ../SMB1_Compliance_Generator/Item_Buffer.cpp
    ../SMB1_Compliance_Generator/Object_Buffer.cpp
    Config_File_Handler.cpp
    Level_Generator.cpp
    ../SMB1_Compliance_Generator/SMB1_Compliance_Map.cpp
    Hacks_Handler.cpp
    Difficulty_Level_Configurations.cpp
    Configure_Settings_Form.cpp
    Tab_Base_Game.cpp
    Tab_Level_Generator.cpp
    Tab_Difficulty.cpp
    CLI_Parser.cpp
    ../../../C_Common_Code/Qt/Random/Random.cpp
    ../../../C_Common_Code/Qt/Readable_Config_File/Readable_Config_File.cpp
)

set(HEADERS
    SMB1_Compliance_To_SMB1.h
    ../../../C_Common_Code/Qt/Text_Insertion_Buffer/Text_Insertion_Buffer.h
    ../Common_SMB1_Files/Background.h
    ../Common_SMB1_Files/Background_String.h
    ../Common_SMB1_Files/Brick.h
    ../Common_SMB1_Files/Brick_String.h
    ../Common_SMB1_Files/Castle.h
    ../Common_SMB1_Files/Enemy_Item.h
    ../Common_SMB1_Files/Enemy_Item_String.h
    ../Common_SMB1_Files/Header_String.h
    ../Common_SMB1_Files/Level.h
    ../Common_SMB1_Files/Level_Attribute.h
    ../Common_SMB1_Files/Level_Attribute_String.h
    ../Common_SMB1_Files/Level_Compliment.h
    ../Common_SMB1_Files/Level_Compliment_String.h
    ../Common_SMB1_Files/Level_String.h
    ../Common_SMB1_Files/Level_Type.h
    ../Common_SMB1_Files/Level_Type_String.h
    ../Common_SMB1_Files/Object_Item.h
    ../Common_SMB1_Files/Object_Item_String.h
    ../Common_SMB1_Files/Scenery.h
    ../Common_SMB1_Files/Scenery_String.h
    ../SMB1_Compliance_Generator/Buffer_Data.h
    ../SMB1_Compliance_Generator/Enemy_Buffer.h
    ../SMB1_Compliance_Generator/Item_Buffer.h
    ../SMB1_Compliance_Generator/Object_Buffer.h
    Config_File_Handler.h
    Level_Generator.h
    Plugin_Settings.h
    Hacks_Handler.h
    SMB1_Compliance_To_SMB1_Strings.h
    Difficulty_Level_Configurations.h
    Difficulty_Level_Settings.h
    Configure_Settings_Form.h
    Tab_Base_Game.h
    Tab_Level_Generator.h
    Tab_Difficulty.h
    Tab_Interface.h
    CLI_Parser.h
    ../../../C_Common_Code/Qt/Random/Random.h
    ../../../C_Common_Code/Qt/Readable_Config_File/Readable_Config_File.h
)

set(FORMS
    Configure_Settings_Form.ui
)

add_library(${PROJECT_NAME} SHARED ${SOURCES} ${HEADERS} ${FORMS})

target_link_libraries(${PROJECT_NAME} PRIVATE
    Qt6::Core
    Qt6::Gui
    Qt6::Widgets
)
