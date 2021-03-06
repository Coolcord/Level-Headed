#ifndef INTERPRETER_INTERFACE_H
#define INTERPRETER_INTERFACE_H

#include <QObject>
#include <QtPlugin>
#include <QString>
#include <QWidget>

class Interpreter_Interface : public QObject {
public:
    virtual void Startup(QWidget *parent, const QString &location, const QStringList &args)=0;
    virtual void Shutdown()=0;
    virtual bool Run()=0;
    virtual bool Run_CLI()=0;
    virtual int Configure_Settings()=0;
    virtual QString Get_Seed()=0;
};

Q_DECLARE_INTERFACE(Interpreter_Interface, "Interpreter_Interface")

#endif // INTERPRETER_INTERFACE_H
