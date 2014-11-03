#include "SMB1_Writer.h"
#include "Level_Offset.h"
#include "Midpoint_Writer.h"
#include "Object_Writer.h"
#include "Enemy_Writer.h"
#include "Header_Writer.h"
#include "Binary_Manipulator.h"
#include "ROM_Handler.h"
#include "Room_ID_Handler.h"
#include "Room_Order_Writer.h"
#include "Room_Address_Writer.h"
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
    this->levelOffset = NULL;
    this->objectWriter = NULL;
    this->enemyWriter = NULL;
    this->headerWriter = NULL;
    this->roomIDHandler = NULL;
    this->roomOrderWriter = NULL;
    this->roomAddressWriter = NULL;
    this->objectOffset = BAD_OFFSET;
    this->enemyOffset = BAD_OFFSET;
    this->numObjectBytes = 0;
    this->numEnemyBytes = 0;
    this->parent = NULL;
}

void SMB1_Writer::Startup(QWidget *parent, QString location) {
    assert(parent);
    this->parent = parent;
    this->applicationLocation = location;
}

void SMB1_Writer::Shutdown() {
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
    if (!this->midpointWriter->Write_Midpoints()) {
        qDebug() << "Unable to write the midpoints to the ROM!";
    }
    this->file->close();
    delete this->file;
    delete this->levelOffset;
    delete this->midpointWriter;
    delete this->roomOrderWriter;
    delete this->roomIDHandler;
    delete this->roomAddressWriter;
}

bool SMB1_Writer::Create_ROM_Directory() {
    if (this->applicationLocation.isEmpty()) return false;
    QDir romDir(this->applicationLocation + "/" + Common_Strings::DATA);
    if (!romDir.exists(Common_Strings::GAME_NAME) && !romDir.mkdir(Common_Strings::GAME_NAME)) return false;
    return true;
}

bool SMB1_Writer::Load_ROM() {
    if (!this->Create_ROM_Directory()) {
        QMessageBox::critical(this->parent, Common_Strings::LEVEL_HEADED, Common_Strings::LEVEL_HEADED +
                         " does not have proper read/write permissions. Cannot continue!");
        return false;
    }
    ROM_Handler romHandler(this->parent, this->applicationLocation + "/" + Common_Strings::DATA + "/" + Common_Strings::GAME_NAME);
    romHandler.Clean_ROM_Directory();
    bool cancel = false;
    this->file = romHandler.Load_First_Local_ROM(cancel);
    //Request for a ROM if none exist
    if (!cancel && !this->file) {
        QMessageBox::information(this->parent, Common_Strings::LEVEL_HEADED,
                                 "It looks like this is your first time using the SMB1 Writer Plugin. In order to continue, please provide a clean SMB1 ROM to use as a base game.",
                                 Common_Strings::OK);
        QString fileName = romHandler.Install_ROM();
        if (fileName.isEmpty()) {
            return false;
        }
        return this->Load_ROM(fileName);
    }
    return this->Load_ROM_Offsets(cancel, romHandler);
}

bool SMB1_Writer::Load_ROM(const QString &fileName) {
    if (!this->Create_ROM_Directory()) return false;
    ROM_Handler romHandler(this->parent, this->applicationLocation + "/" + Common_Strings::DATA + "/" + Common_Strings::GAME_NAME);
    qDebug() << "Filename is: " << fileName;
    bool cancel = false;
    this->file = romHandler.Load_Local_ROM(fileName, cancel);
    return this->Load_ROM_Offsets(cancel, romHandler);
}

bool SMB1_Writer::Load_ROM_Offsets(bool cancel, const ROM_Handler &romHandler) {
    if (!cancel && this->file) {
        this->levelOffset = new Level_Offset(this->file, romHandler.Get_ROM_Type());
        this->roomIDHandler = new Room_ID_Handler();
        this->midpointWriter = new Midpoint_Writer(this->file, this->levelOffset, this->roomIDHandler);
        if (!this->midpointWriter->Read_Midpoints()) return false;
        this->roomOrderWriter = new Room_Order_Writer(this->file, this->levelOffset, this->roomIDHandler);
        this->roomIDHandler->Set_Room_Order_Writer(this->roomOrderWriter);
        if (!this->roomOrderWriter->Read_Room_Order_Table()) return false;
        this->roomAddressWriter = new Room_Address_Writer(this->file, this->levelOffset);
        this->roomIDHandler->Set_Room_Address_Writer(this->roomAddressWriter);
        return this->roomAddressWriter->Read_Room_Address_Tables();
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

    //Read the Level
    if (!this->Read_Level_Header() || !this->Read_Objects() || !this->Read_Enemies()) {
        this->Deallocate_Buffers();
        return false;
    }

    this->roomIDHandler->Set_Current_Level(level);
    return true;
}

bool SMB1_Writer::Write_Level() {
    if (!this->file) return false; //the ROM needs to be loaded first

    unsigned char id = 0;
    assert(this->roomIDHandler->Change_Current_Level_Attribute(Level_Attribute::CASTLE));
    assert(this->roomIDHandler->Get_Room_ID_From_Level(this->roomIDHandler->Get_Current_Level(), id));
    qDebug() << "New Room ID: " << id;
    this->Deallocate_Buffers();
    return true;

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
    return this->numEnemyBytes;
}

bool SMB1_Writer::Set_Number_Of_Worlds(int value) {
    if (!this->roomOrderWriter) return false;
    return this->roomOrderWriter->Set_Number_Of_Worlds(value);
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
    assert(!this->enemyWriter);
    if (this->enemyOffset == BAD_OFFSET || !this->enemiesBuffer || this->enemyWriter) return false;
    if (this->enemyOffset == 0) return true; //nothing to read
    if (!this->file->seek(this->enemyOffset)) return false;

    //Read the enemies from the level
    this->numEnemyBytes = 0;
    for (QByteArray coordinate = this->file->peek(1); !coordinate.isEmpty() &&
         static_cast<unsigned char>(coordinate.data()[0]) != 0xFF; coordinate = this->file->peek(1)) {
        if (Binary_Manipulator::Get_Second_Digit_From_Hex(static_cast<unsigned char>(coordinate[0])) == 0xE) { //pipe pointer
            QByteArray buffer(3, ' ');
            if (this->file->read(buffer.data(), 3) != 3) return false; //something went wrong...
            this->enemiesBuffer->append(buffer);
            this->numEnemyBytes += 3;
        } else { //typical enemy
            QByteArray buffer(2, ' ');
            if (this->file->read(buffer.data(), 2) != 2) return false; //something went wrong...
            this->enemiesBuffer->append(buffer);
            this->numEnemyBytes += 2;
        }
    }

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
    this->numEnemyBytes = 0;
    this->numObjectBytes = 0;
}
