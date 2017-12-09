#include "SMB1_Writer.h"
#include "Level_Offset.h"
#include "Midpoint_Writer.h"
#include "Object_Writer.h"
#include "Enemy_Writer.h"
#include "Header_Writer.h"
#include "Enemy_Bytes_Tracker.h"
#include "Binary_Manipulator.h"
#include "ROM_Handler.h"
#include "Room_ID_Handler.h"
#include "Room_Order_Writer.h"
#include "Room_Address_Writer.h"
#include "Hacks.h"
#include "SMB1_Writer_Strings.h"
#include "../../Level-Headed/Common_Strings.h"
#include <QDir>
#include <QMessageBox>
#include <assert.h>
#include <QDebug>

SMB1_Writer::SMB1_Writer() {
    //Set class variables
    this->applicationLocation = QString();
    this->headerBuffer = NULL;
    this->objectsBuffer = NULL;
    this->enemiesBuffer = NULL;
    this->enemyBytesTracker = NULL;
    this->levelOffset = NULL;
    this->objectWriter = NULL;
    this->enemyWriter = NULL;
    this->headerWriter = NULL;
    this->roomIDHandler = NULL;
    this->roomOrderWriter = NULL;
    this->roomAddressWriter = NULL;
    this->midpointWriter = NULL;
    this->hacks = NULL;
    this->romHandler = NULL;
    this->objectOffset = BAD_OFFSET;
    this->enemyOffset = BAD_OFFSET;
    this->numObjectBytes = 0;
    this->parent = NULL;
}

void SMB1_Writer::Startup(QWidget *parent, const QString &location) {
    assert(parent);
    this->parent = parent;
    this->applicationLocation = location;
    this->romHandler = new ROM_Handler(this->parent, this->applicationLocation);
}

void SMB1_Writer::Shutdown() {
    if (this->file) {
        if (this->Are_Buffers_Allocated()) {
            if (!this->Write_Level()) { //try to flush the buffers
                this->Deallocate_Buffers(); //deallocate memory manually
            }
        }
        assert(!this->Are_Buffers_Allocated()); //make sure memory leaks never happen
        if (!this->roomOrderWriter->Write_Room_Order_Table()) {
            qDebug() << "Unable to write the room order table to the ROM!";
        }
        if (!this->roomAddressWriter->Write_Room_Address_Tables()) {
            qDebug() << "Unable to write the room address tables to the ROM!";
        }
        //TODO: Fix midpoints on Famicom version
        if (!this->midpointWriter->Write_Midpoints()) {
            qDebug() << "Unable to write the midpoints to the ROM!";
        }
        this->file->close();
    }
    delete this->file;
    this->file = NULL;
    delete this->levelOffset;
    this->levelOffset = NULL;
    delete this->midpointWriter;
    this->midpointWriter = NULL;
    delete this->roomOrderWriter;
    this->roomOrderWriter = NULL;
    delete this->roomIDHandler;
    this->roomIDHandler = NULL;
    delete this->roomAddressWriter;
    this->roomAddressWriter = NULL;
    delete this->enemyBytesTracker;
    this->enemyBytesTracker = NULL;
    delete this->romHandler;
    this->romHandler = NULL;
    delete this->hacks;
    this->hacks = NULL;
}

QStringList SMB1_Writer::Get_Installed_ROMs() {
    return this->romHandler->Get_Installed_ROMs();
}

QString SMB1_Writer::Install_ROM() {
    return this->romHandler->Install_ROM();
}

bool SMB1_Writer::Create_ROM_Directory() {
    if (this->applicationLocation.isEmpty()) return false;
    QDir romDir(this->applicationLocation + "/" + Common_Strings::STRING_DATA);
    if (!romDir.exists(Common_Strings::STRING_GAME_NAME) && !romDir.mkdir(Common_Strings::STRING_GAME_NAME)) return false;
    return true;
}

bool SMB1_Writer::Load_ROM() {
    if (!this->Create_ROM_Directory()) {
        QMessageBox::critical(this->parent, Common_Strings::STRING_LEVEL_HEADED, Common_Strings::STRING_LEVEL_HEADED +
                         " does not have proper read/write permissions. Cannot continue!");
        return false;
    }
    this->romHandler->Clean_ROM_Directory();
    bool cancel = false;
    this->file = this->romHandler->Load_First_Local_ROM(cancel);
    //Request for a ROM if none exist
    if (!cancel && !this->file) {
        QMessageBox::information(this->parent, Common_Strings::STRING_LEVEL_HEADED,
                                 "It looks like this is your first time using the SMB1 Writer Plugin. In order to continue, please provide a clean SMB1 ROM to use as a base game.",
                                 Common_Strings::STRING_OK);
        QString fileName = this->romHandler->Install_ROM();
        if (fileName.isEmpty()) {
            return false;
        }
        return this->Load_ROM(fileName);
    }
    return this->Load_ROM_Offsets(cancel);
}

bool SMB1_Writer::Load_ROM(const QString &fileName) {
    if (!this->Create_ROM_Directory()) return false;
    bool cancel = false;
    this->file = this->romHandler->Load_Local_ROM(fileName, cancel);
    return this->Load_ROM_Offsets(cancel);
}

bool SMB1_Writer::Load_ROM_Offsets(bool cancel) {
    if (!cancel && this->file) {
        delete this->levelOffset;
        this->levelOffset = NULL;
        delete this->roomIDHandler;
        this->roomIDHandler = NULL;
        delete this->midpointWriter;
        this->midpointWriter = NULL;
        delete this->roomOrderWriter;
        this->roomOrderWriter = NULL;
        delete this->roomIDHandler;
        this->roomIDHandler = NULL;
        delete this->enemyBytesTracker;
        this->enemyBytesTracker = NULL;
        delete this->hacks;
        this->hacks = NULL;
        this->levelOffset = new Level_Offset(this->file, this->romHandler->Get_ROM_Type());
        this->roomIDHandler = new Room_ID_Handler(this->file, this->levelOffset);
        this->midpointWriter = new Midpoint_Writer(this->file, this->levelOffset, this->roomIDHandler);
        if (!this->midpointWriter->Read_Midpoints()) return false;
        this->roomOrderWriter = new Room_Order_Writer(this->file, this->levelOffset, this->roomIDHandler);
        this->roomIDHandler->Set_Room_Order_Writer(this->roomOrderWriter);
        if (!this->roomOrderWriter->Read_Room_Order_Table()) return false;
        this->roomAddressWriter = new Room_Address_Writer(this->file, this->levelOffset);
        this->roomIDHandler->Set_Room_Address_Writer(this->roomAddressWriter);
        this->levelOffset->Set_Extras(this->roomIDHandler, this->roomAddressWriter);
        if (!this->roomAddressWriter->Read_Room_Address_Tables()) return false;
        this->enemyBytesTracker = new Enemy_Bytes_Tracker(this->file, this->levelOffset);
        this->roomIDHandler->Set_Enemy_Bytes_Tracker(this->enemyBytesTracker);
        if (!this->enemyBytesTracker->Calculate_Enemy_Bytes_In_All_Levels()) return false;
        this->hacks = new Hacks(this->file, this->levelOffset);
        return true;
    } else {
        return false;
    }
}

bool SMB1_Writer::New_Level(Level::Level level) {
    if (!this->file) return false; //the ROM needs to be loaded first

    //Make sure that the buffers are empty
    if (this->Are_Buffers_Allocated()) return false;

    //Allocate Memory
    this->objectOffset = this->levelOffset->Get_Level_Object_Offset(level);
    this->enemyOffset = this->levelOffset->Get_Level_Enemy_Offset(level);
    this->headerBuffer = new QByteArray();
    this->objectsBuffer = new QByteArray();
    this->enemiesBuffer = new QByteArray();

    Level::Level previousLevel = this->roomIDHandler->Get_Current_Level();
    this->roomIDHandler->Set_Current_Level(level);

    //Read the Level
    if (!this->Read_Level_Header() || !this->Read_Objects() || !this->Read_Enemies()) {
        this->Deallocate_Buffers();
        this->roomIDHandler->Set_Current_Level(previousLevel);
        return false;
    }

    return true;
}

bool SMB1_Writer::Write_Level() {
    if (!this->file) return false; //the ROM needs to be loaded first

    //Make sure the offsets have been set
    if (this->objectOffset == BAD_OFFSET) return false;
    if (this->enemyOffset == BAD_OFFSET) return false;

    //Fill the object and enemy buffers if they aren't already full
    if (!this->objectWriter->Fill_Buffer()) return false;
    if (!this->enemyWriter->Fill_Buffer()) return false;

    //Write Header
    if (!this->Write_Buffer(this->objectOffset-2, this->headerBuffer)) return false;

    //Write Objects
    if (!this->Write_Buffer(this->objectOffset, this->objectsBuffer)) return false;

    //Write Enemies
    if (!this->Write_Buffer(this->enemyOffset, this->enemiesBuffer)) return false;

    //Deallocate memory
    this->Deallocate_Buffers();
    return true;
}

int SMB1_Writer::Get_Num_Object_Bytes() {
    return this->numObjectBytes;
}

int SMB1_Writer::Get_Num_Enemy_Bytes() {
    assert(this->enemyBytesTracker);
    assert(this->roomIDHandler);
    return this->enemyBytesTracker->Get_Enemy_Byte_Count_In_Level(this->roomIDHandler->Get_Current_Level());
}

bool SMB1_Writer::Set_Number_Of_Worlds(int value) {
    if (!this->roomOrderWriter) return false;
    return this->roomOrderWriter->Set_Number_Of_Worlds(value);
}

QString SMB1_Writer::Get_Output_ROM_Location() {
    if (this->romHandler) return this->romHandler->Get_Ouput_ROM_Location();
    else return QString();
}

bool SMB1_Writer::Set_Output_ROM_Location(const QString &location) {
    if (this->romHandler) {
        this->romHandler->Set_Output_ROM_Location(location);
        return true;
    } else {
        return false;
    }
}

bool SMB1_Writer::Write_Buffer(const int offset, QByteArray *buffer) {
    assert(this->file);
    if (buffer == NULL) return false;
    if (!this->file->seek(offset)) return false;
    return this->file->write(buffer->data(), buffer->length()) == buffer->length();
}

bool SMB1_Writer::Read_Level_Header() {
    assert(this->file);
    if (!this->headerBuffer || this->headerWriter) return false;
    if (!this->file->seek(objectOffset-2)) return false;
    QByteArray buffer(2, ' ');
    if (this->file->read(buffer.data(), 2) != 2) return false;
    this->headerBuffer->clear();
    this->headerBuffer->append(buffer);
    assert(this->headerBuffer->size() == 2);
    this->headerWriter = new Header_Writer(this->headerBuffer);
    return true;
}

bool SMB1_Writer::Read_Objects() {
    assert(this->file);
    assert(this->headerBuffer);
    assert(this->headerWriter);
    assert(!this->objectWriter);
    assert(this->objectsBuffer->isEmpty());
    if (this->objectOffset == BAD_OFFSET || !this->objectsBuffer || this->objectWriter) return false;
    if (!this->file->seek(this->objectOffset)) return false;

    QByteArray buffer(2, ' ');
    this->numObjectBytes = 0;
    qint64 ret = this->file->read(buffer.data(), 2);
    while (ret == 2 && buffer.data() != NULL
           && static_cast<unsigned char>(buffer.data()[0]) != 0xFD) {
        this->objectsBuffer->append(buffer);
        this->numObjectBytes += 2;
        ret = this->file->read(buffer.data(), 2);
    }

    this->objectWriter = new Object_Writer(this->objectsBuffer, this->headerWriter, this->roomIDHandler);
    return true;
}

bool SMB1_Writer::Read_Enemies() {
    assert(this->file);
    assert(this->headerBuffer);
    assert(this->headerWriter);
    assert(this->enemyBytesTracker);
    assert(this->roomIDHandler);
    assert(!this->enemyWriter);
    assert(this->enemiesBuffer->isEmpty());
    if (this->enemyOffset == BAD_OFFSET || !this->enemiesBuffer || this->enemyWriter) return false;
    if (this->enemyOffset == 0) return true; //nothing to read
    if (!this->file->seek(this->enemyOffset)) return false;

    //Read the enemies from the level
    int bufferSize = this->enemyBytesTracker->Get_Enemy_Byte_Count_In_Level(this->roomIDHandler->Get_Current_Level());
    this->enemiesBuffer->append(this->file->read(bufferSize));
    if (this->enemiesBuffer->size() != bufferSize) return false;

    this->enemyWriter = new Enemy_Writer(this->enemiesBuffer, this->headerWriter, this->roomIDHandler);
    return true;
}

bool SMB1_Writer::Are_Buffers_Allocated() {
    return (this->headerBuffer != NULL && this->objectsBuffer != NULL && this->enemiesBuffer != NULL
            && this->objectOffset != BAD_OFFSET && this->enemyOffset != BAD_OFFSET
            && this->objectWriter != NULL && this->enemyWriter != NULL && this->headerWriter != NULL);
}

void SMB1_Writer::Deallocate_Buffers() {
    delete this->headerBuffer;
    delete this->objectsBuffer;
    delete this->enemiesBuffer;
    delete this->objectWriter;
    delete this->enemyWriter;
    delete this->headerWriter;
    this->headerBuffer = NULL;
    this->objectsBuffer = NULL;
    this->enemiesBuffer = NULL;
    this->objectWriter = NULL;
    this->enemyWriter = NULL;
    this->headerWriter = NULL;
    this->objectOffset = BAD_OFFSET;
    this->enemyOffset = BAD_OFFSET;
    this->numObjectBytes = 0;
}
