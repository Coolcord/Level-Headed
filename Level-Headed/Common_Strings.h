#ifndef COMMON_STRINGS_H
#define COMMON_STRINGS_H

#include <QString>

namespace Common_Strings {
    const static QString LEVEL_HEADED = "Level-Headed";
    const static QString PLUGINS = "Plugins";
    const static QString GENERATORS = "Generators";
    const static QString INTERPRETERS = "Interpreters";
    const static QString WRITERS = "Writers";
    const static QString DATA = "Data";
    const static QString INTERPRETER_SPLIT = "_To_";
    const static QString GENERATOR_SPLIT = "_Generator";
    const static QString WRITER_SPLIT = "_Writer";
    const static QString OK = "OK";
    const static QString SELECT_A_PLUGIN = "-- Select a Plugin --";

    #ifdef Q_OS_WIN32
    const static QString PLUGIN_EXTENSION = ".dll"; //Windows uses .dll files
    #else
    const static QString PLUGIN_EXTENSION = ".so"; //Unix uses .so files
    #endif
}

#endif // COMMON_STRINGS_H
