#ifndef COMMON_STRINGS_H
#define COMMON_STRINGS_H

#include <QString>

namespace Common_Strings {
    const static QString STRING_LEVEL_HEADED = "Level-Headed";
    const static QString STRING_PLUGINS = "Plugins";
    const static QString STRING_GENERATORS = "Generators";
    const static QString STRING_INTERPRETERS = "Interpreters";
    const static QString STRING_WRITERS = "Writers";
    const static QString STRING_LEVELS = "Levels";
    const static QString STRING_DATA = "Data";
    const static QString STRING_INTERPRETER_SPLIT = "_To_";
    const static QString STRING_GENERATOR_SPLIT = "_Generator";
    const static QString STRING_WRITER_SPLIT = "_Writer";
    const static QString STRING_OK = "OK";
    const static QString STRING_SELECT_A_PLUGIN = "-- Select a Plugin --";
    const static QString STRING_GENERATE_A_GAME = "Generate a Game!";
    const static QString STRING_GENERATE_ANOTHER = "Generate Another!";
    const static QString STRING_ON = "On";
    const static QString STRING_OFF = "Off";

    #ifdef Q_OS_WIN32
    const static QString STRING_PLUGIN_EXTENSION = ".dll"; //Windows uses .dll files
    const static QString STRING_NEW_LINE = "\r\n"; //Qt's endl seems to use only \n for some reason... I'll look into this later
    #else
    const static QString STRING_PLUGIN_EXTENSION = ".so"; //Unix uses .so files
    const static QString STRING_NEW_LINE = "\n";
    #endif
}

#endif // COMMON_STRINGS_H
