#ifndef INTERPRETER_INTERFACE_H
#define INTERPRETER_INTERFACE_H

#include <QObject>
#include <QtPlugin>
#include <QString>

class Interpreter_Interface : public QObject {
public:
    virtual bool Run()=0;
    virtual bool Configure_Generator()=0;
    virtual bool Configure_Writer()=0;
};

Q_DECLARE_INTERFACE(Interpreter_Interface, "Interpreter_Interface")

#endif // INTERPRETER_INTERFACE_H
