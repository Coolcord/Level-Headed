cmake_minimum_required(VERSION 3.14)

project(SMB1_Compliance_Generator LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic -Wdeprecated -Werror")

find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets)

set(SOURCES
    SMB1_Compliance_Generator.cpp
    ../../../C_Common_Code/Qt/Text_Insertion_Buffer/Text_Insertion_Buffer.cpp
    ../../../C_Common_Code/Qt/Random/Random.cpp
    Enemy_Buffer.cpp
    Item_Buffer.cpp
    Level_Script_Modifier.cpp
    Level_Script_Parser.cpp
    Midpoint_Handler.cpp
    Header_Writer.cpp
    Level_Generator.cpp
    Object_Buffer.cpp
    Pipe_Pointer_Buffer.cpp
    Powerup_Distributor.cpp
    SMB1_Compliance_Object_Parser.cpp
    Standard_Overworld_Generator.cpp
    Simple_Object_Spawner.cpp
    End_Spawner.cpp
    Common_Pattern_Spawner.cpp
    Object_Spawner.cpp
    Level_Crawler.cpp
    SMB1_Compliance_Map.cpp
    Enemy_Spawner.cpp
    Bridge_Generator.cpp
    First_Page_Handler.cpp
    Island_Generator.cpp
    Underwater_Generator.cpp
    Underground_Generator.cpp
    Item_Spawner.cpp
    Required_Enemy_Spawns.cpp
    Castle_Generator.cpp
    Pipe_Exit_Generator.cpp
    Continuous_Enemies_Spawner.cpp
    Underground_Bonus_Generator.cpp
    Pipe_Intro_Generator.cpp
)

set(HEADERS
    SMB1_Compliance_Generator.h
    ../../../C_Common_Code/Qt/Text_Insertion_Buffer/Text_Insertion_Buffer.h
    ../../../C_Common_Code/Qt/Random/Random.h
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
    Block_Data.h
    Buffer_Data.h
    Enemy_Buffer.h
    Item_Buffer.h
    Level_Script_Modifier.h
    Level_Script_Parser.h
    Midpoint_Handler.h
    Header_Writer.h
    Level_Generator.h
    Object_Buffer.h
    Pipe_Pointer_Buffer.h
    Powerup_Distributor.h
    SMB1_Compliance_Object_Parser.h
    SMB1_Compliance_Parser_Arguments.h
    Standard_Overworld_Generator.h
    Simple_Object_Spawner.h
    Physics.h
    SMB1_Compliance_Generator_Interface.h
    End_Spawner.h
    Common_Pattern_Spawner.h
    Object_Spawner.h
    Level_Crawler.h
    SMB1_Compliance_Map.h
    Enemy_Spawner.h
    Bridge_Generator.h
    First_Page_Handler.h
    SMB1_Compliance_Generator_Arguments.h
    End_Pattern.h
    Island_Generator.h
    Underwater_Generator.h
    Underground_Generator.h
    Item_Spawner.h
    Required_Enemy_Spawns.h
    Extra_Enemy_Args.h
    Castle_Generator.h
    Pipe_Exit_Generator.h
    Difficulty.h
    Continuous_Enemies_Spawner.h
    Underground_Bonus_Generator.h
    Pipe_Intro_Generator.h
)

add_library(${PROJECT_NAME} SHARED ${SOURCES} ${HEADERS})

target_link_libraries(${PROJECT_NAME} PRIVATE
    Qt6::Core
    Qt6::Gui
    Qt6::Widgets
)
