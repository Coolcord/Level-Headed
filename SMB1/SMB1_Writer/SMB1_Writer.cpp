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
#include "Music.h"
#include "Sound.h"
#include "Palettes.h"
#include "Powerups.h"
#include "Sequential_Archive_Handler.h"
#include "Text.h"
#include "Graphics.h"
#include "SMB1_Writer_Strings.h"
#include "../../Level-Headed/Common_Strings.h"
#include "../../Common_Files/Random.h"
#include <QDir>
#include <QMessageBox>
#include <assert.h>
#include <QDebug>

SMB1_Writer::SMB1_Writer() {
    //Set class variables
    this->applicationLocation = QString();
    this->file = nullptr;
    this->headerBuffer = nullptr;
    this->objectsBuffer = nullptr;
    this->enemiesBuffer = nullptr;
    this->enemyBytesTracker = nullptr;
    this->levelOffset = nullptr;
    this->objectWriter = nullptr;
    this->enemyWriter = nullptr;
    this->headerWriter = nullptr;
    this->roomIDHandler = nullptr;
    this->roomOrderWriter = nullptr;
    this->roomAddressWriter = nullptr;
    this->midpointWriter = nullptr;
    this->hacks = nullptr;
    this->music = nullptr;
    this->sound = nullptr;
    this->graphics = nullptr;
    this->palettes = nullptr;
    this->powerups = nullptr;
    this->sequentialArchiveHandler = nullptr;
    this->romHandler = nullptr;
    this->objectOffset = BAD_OFFSET;
    this->enemyOffset = BAD_OFFSET;
    this->numObjectBytes = 0;
    this->parent = nullptr;
}

void SMB1_Writer::Startup(QWidget *parent, const QString &location) {
    this->parent = parent;
    this->applicationLocation = location;
    this->romHandler = new ROM_Handler(this->parent, this->applicationLocation);
    this->sequentialArchiveHandler = new Sequential_Archive_Handler(location, this->romHandler->Get_Installed_ROM_Folder_Location());
    this->romHandler->Set_Sequential_Archive_Handler(this->sequentialArchiveHandler);
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
        if (!this->midpointWriter->Write_Midpoints()) {
            qDebug() << "Unable to write the midpoints to the ROM!";
        }
        this->file->close();
    }
    delete this->file;
    this->file = nullptr;
    delete this->levelOffset;
    this->levelOffset = nullptr;
    delete this->midpointWriter;
    this->midpointWriter = nullptr;
    delete this->roomOrderWriter;
    this->roomOrderWriter = nullptr;
    delete this->roomIDHandler;
    this->roomIDHandler = nullptr;
    delete this->roomAddressWriter;
    this->roomAddressWriter = nullptr;
    delete this->enemyBytesTracker;
    this->enemyBytesTracker = nullptr;
    delete this->romHandler;
    this->romHandler = nullptr;
    delete this->hacks;
    this->hacks = nullptr;
    delete this->music;
    this->music = nullptr;
    delete this->sound;
    this->sound = nullptr;
    delete this->graphics;
    this->graphics = nullptr;
    delete this->palettes;
    this->palettes = nullptr;
    delete this->powerups;
    this->powerups = nullptr;
    delete this->sequentialArchiveHandler;
    this->sequentialArchiveHandler = nullptr;
}

void SMB1_Writer::Seed_Random_Number_Generator_Instance(const QString &seed) {
    Random::Get_Instance().Seed(seed, 2);
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

bool SMB1_Writer::Load_ROM_First_Time(QString &baseRomName) {
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
        if (this->Load_ROM(fileName)) {
            baseRomName = fileName;
            return true;
        }
        return false;
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
        this->levelOffset = nullptr;
        delete this->roomIDHandler;
        this->roomIDHandler = nullptr;
        delete this->midpointWriter;
        this->midpointWriter = nullptr;
        delete this->roomOrderWriter;
        this->roomOrderWriter = nullptr;
        delete this->roomIDHandler;
        this->roomIDHandler = nullptr;
        delete this->enemyBytesTracker;
        this->enemyBytesTracker = nullptr;
        delete this->hacks;
        this->hacks = nullptr;
        delete this->music;
        this->music = nullptr;
        delete this->sound;
        this->sound = nullptr;
        delete this->graphics;
        this->graphics = nullptr;
        delete this->palettes;
        this->palettes = nullptr;
        delete this->powerups;
        this->powerups = nullptr;
        this->levelOffset = new Level_Offset(this->file, this->romHandler->Get_ROM_Type());
        this->roomIDHandler = new Room_ID_Handler(this->file, this->levelOffset);
        this->midpointWriter = new Midpoint_Writer(this->file, this->levelOffset, this->roomIDHandler);
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
        this->text = new Text(this->file, this->levelOffset);
        this->hacks = new Hacks(this->file, this->levelOffset, this->midpointWriter, this->sequentialArchiveHandler, this->text);
        this->music = new Music(this->file, this->levelOffset, this->sequentialArchiveHandler);
        this->sound = new Sound(this->file, this->levelOffset);
        this->palettes = new Palettes(this->file, this->levelOffset, this->sequentialArchiveHandler);
        this->graphics = new Graphics(this->file, this->levelOffset, this->sequentialArchiveHandler, this->text);
        this->powerups = new Powerups(this->file, this->levelOffset, this->sequentialArchiveHandler, this->graphics, this->hacks);
        this->sequentialArchiveHandler->Set_File(this->file);
        this->hacks->Set_Powerups(this->powerups);
        this->hacks->Set_Graphics(this->graphics);
        return true;
    } else {
        return false;
    }
}

bool SMB1_Writer::New_Bonus_Level(Level::Level level) {
    return this->New_Level(level, 0, 0);
}

bool SMB1_Writer::New_Level(Level::Level level, int worldNum, int levelNum) {
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
    this->roomIDHandler->Set_Current_World_Num(worldNum);
    this->roomIDHandler->Set_Current_Level_Num(levelNum);

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

bool SMB1_Writer::Write_Buffer(const qint64 offset, QByteArray *buffer) {
    assert(this->file);
    if (buffer == nullptr) return false;
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
    while (ret == 2 && buffer.data() != nullptr
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
    return (this->headerBuffer != nullptr && this->objectsBuffer != nullptr && this->enemiesBuffer != nullptr
            && this->objectOffset != BAD_OFFSET && this->enemyOffset != BAD_OFFSET
            && this->objectWriter != nullptr && this->enemyWriter != nullptr && this->headerWriter != nullptr);
}

void SMB1_Writer::Deallocate_Buffers() {
    delete this->headerBuffer;
    delete this->objectsBuffer;
    delete this->enemiesBuffer;
    delete this->objectWriter;
    delete this->enemyWriter;
    delete this->headerWriter;
    this->headerBuffer = nullptr;
    this->objectsBuffer = nullptr;
    this->enemiesBuffer = nullptr;
    this->objectWriter = nullptr;
    this->enemyWriter = nullptr;
    this->headerWriter = nullptr;
    this->objectOffset = BAD_OFFSET;
    this->enemyOffset = BAD_OFFSET;
    this->numObjectBytes = 0;
}
