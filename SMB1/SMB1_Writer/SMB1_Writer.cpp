#include "SMB1_Writer.h"
#include "Level_Offset.h"
#include "Object_Writer.h"
#include "Enemy_Writer.h"
#include "Header_Writer.h"
#include "Binary_Manipulator.h"
#include "ROM_Handler.h"
#include "SMB1_Writer_Strings.h"
#include "../../Level-Headed/Common_Strings.h"
#include <QDir>
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
    this->file->close();
    delete this->file;
    delete this->levelOffset;
}

bool SMB1_Writer::Create_ROM_Directory() {
    if (this->applicationLocation.isEmpty()) return false;
    QDir romDir(this->applicationLocation + "/" + Common_Strings::DATA);
    if (!romDir.exists(Common_Strings::GAME_NAME) && !romDir.mkdir(Common_Strings::GAME_NAME)) return false;
    return true;
}

bool SMB1_Writer::Load_ROM() {
    qDebug() << "Load ROM called!";
    if (!this->Create_ROM_Directory()) return false;
    qDebug() << "ROM Directory Created!";
    ROM_Handler romHandler(this->parent, this->applicationLocation + "/" + Common_Strings::DATA + "/" + Common_Strings::GAME_NAME);
    romHandler.Clean_ROM_Directory();
    this->file = romHandler.Load_First_Local_ROM(); //TODO: Add a way to cancel, preferrably with a ref bool

    //Request for a ROM if none exist
    if (!this->file) {
        //TODO: Show a message displaying that it must be the user's first time running the plugin
        qDebug() << "Attempting to install a ROM";
        QString fileName = romHandler.Install_ROM();
        if (fileName.isEmpty()) {
            qDebug() << "Install failed!";
            return false;
        }
        return this->Load_ROM(fileName);
    }

    if (this->file) {
        this->levelOffset = new Level_Offset(this->file, romHandler.Get_ROM_Type());
        return true;
    } else {
        return false;
    }
}

bool SMB1_Writer::Load_ROM(const QString &fileName) {
    if (!this->Create_ROM_Directory()) return false;
    ROM_Handler romHandler(this->parent, this->applicationLocation + "/" + Common_Strings::DATA + "/" + Common_Strings::GAME_NAME);
    qDebug() << "Filename is: " << fileName;
    this->file = romHandler.Load_Local_ROM(fileName);
    if (this->file) {
        this->levelOffset = new Level_Offset(this->file, romHandler.Get_ROM_Type());
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

    //Read the Level
    if (!this->Read_Level_Header() || !this->Read_Objects() || !this->Read_Enemies()) {
        this->Deallocate_Buffers();
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
    return this->numEnemyBytes;
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

    this->objectWriter = new Object_Writer(this->objectsBuffer, this->headerWriter);
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

    this->enemyWriter = new Enemy_Writer(this->enemiesBuffer, this->headerWriter);
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
