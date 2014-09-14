#ifndef INTERPRETER_INTERFACE_H
#define INTERPRETER_INTERFACE_H

#include <QObject>
#include <QtPlugin>
#include <QString>

class Interpreter_Interface : public QObject {
public:
    bool Run();
    bool Configure_Generator();
    bool Configure_Writer();
};

Q_DECLARE_INTERFACE(Interpreter_Interface, "Interpreter_Interface")

#endif // INTERPRETER_INTERFACE_H
