cmake_minimum_required(VERSION 3.14)

project(SMB1_Writer LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic -Wdeprecated -Werror")

find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets)

set(SOURCES
    SMB1_Writer.cpp
    Bowser_Bridge_Destroyer.cpp
    Colors.cpp
    Enemy_Buffer.cpp
    Graphics_Combiner.cpp
    Graphics_Offsets.cpp
    Binary_Manipulator.cpp
    Header_Writer.cpp
    Item_Buffer.cpp
    Object_Buffer.cpp
    Palettes.cpp
    SMB1_Writer_Accessors.cpp
    ROM_Handler.cpp
    ROM_Checksum.cpp
    Midpoint_Writer.cpp
    Room_Order_Writer.cpp
    Room_ID_Handler.cpp
    Room_Address_Writer.cpp
    Level_Offset.cpp
    Enemy_Bytes_Tracker.cpp
    Hacks.cpp
    Music.cpp
    Byte_Writer.cpp
    Graphics.cpp
    Text.cpp
    Sequential_Archive_Handler.cpp
    Powerups.cpp
    Sound.cpp
    ../../../C_Common_Code/Qt/Random/Random.cpp
)

set(HEADERS
    SMB1_Writer.h
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
    Bowser_Bridge_Destroyer.h
    Color.h
    Colors.h
    Enemy_Buffer.h
    Graphics_Combiner.h
    Graphics_Offsets.h
    Item_Buffer.h
    Object_Buffer.h
    Palettes.h
    ROM_Type.h
    Binary_Manipulator.h
    Level_Offset.h
    ROM_Checksum.h
    Header_Writer.h
    SMB1_Writer_Interface.h
    ROM_Handler.h
    ROM_Filename.h
    Midpoint_Writer.h
    Room_Order_Writer.h
    Room_ID_Handler.h
    Room_Address_Writer.h
    Enemy_Bytes_Tracker.h
    Hacks.h
    Music.h
    Byte_Writer.h
    Graphics.h
    Text.h
    Sequential_Archive_Handler.h
    Powerups.h
    Unfixed_ROM_Type.h
    Sound.h
    ../../../C_Common_Code/Qt/Random/Random.h
    ../../Common_Files/Version.h
    ../Common_SMB1_Files/Fix_Strings.h
)

add_library(${PROJECT_NAME} SHARED ${SOURCES} ${HEADERS})

target_link_libraries(${PROJECT_NAME} PRIVATE
    Qt6::Core
    Qt6::Gui
    Qt6::Widgets
)
