#ifndef SMB1_WRITER_H
#define SMB1_WRITER_H

#include "SMB1_Writer_Interface.h"
#include <QFile>
#include <QByteArray>

class Object_Writer;
class Enemy_Writer;
class Header_Writer;

class Level_Offset;

class SMB1_Writer : public SMB1_Writer_Interface {
    Q_OBJECT
    Q_PLUGIN_METADATA(IID  "SMB1_Writer_Interface")
    Q_INTERFACES(SMB1_Writer_Interface)

public:
    SMB1_Writer();
    void Shutdown();
    bool Load_ROM(const QString &romLocation);
    bool New_Level(Level::Level level);
    bool Write_Level();
    int Get_Num_Object_Bytes();
    int Get_Num_Enemy_Bytes();
    Object_Writer *Get_Object_Writer();
    Enemy_Writer *Get_Enemy_Writer();
    Header_Writer *Get_Header_Writer();


private:
    bool Are_Buffers_Allocated();
    void Deallocate_Buffers();
    bool Write_Buffer(const int offset, QByteArray *buffer);
    bool Read_Level_Header();
    bool Read_Objects();
    bool Read_Enemies();

    int numObjectBytes;
    int numEnemyBytes;
    int objectOffset;
    int enemyOffset;
    QFile *file;
    QByteArray *headerBuffer;
    QByteArray *objectsBuffer;
    QByteArray *enemiesBuffer;
    Level_Offset *levelOffset;
    Object_Writer *objectWriter;
    Enemy_Writer *enemyWriter;
    Header_Writer *headerWriter;
};

#endif // SMB1_WRITER_H
