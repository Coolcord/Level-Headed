#include "Level_Generator.h"
#include "../../../C_Common_Code/Qt/Random/Random.h"
#include "../Common_SMB1_Files/Level_Type_String.h"
#include "Continuous_Enemies_Spawner.h"
#include <QTextStream>
#include <assert.h>

Level_Generator::Level_Generator(QFile *file, SMB1_Compliance_Generator_Arguments *args) {
    assert(file);
    assert(args);
    this->file = file;
    this->args = args;
    this->header = new Header_Writer();
    this->objects = new Object_Buffer(this->args->numObjectBytes, this->args);
    this->enemies = new Enemy_Buffer(this->args->numEnemyBytes);
    this->pipePointer = new Pipe_Pointer_Buffer(this->objects, this->enemies);
    this->requiredEnemySpawns = new Required_Enemy_Spawns(this->objects, this->enemies, this->pipePointer, this->args);
    this->levelCrawler = new Level_Crawler(this->objects, this->requiredEnemySpawns);
    this->enemySpawner = new Enemy_Spawner(this->objects, this->enemies, this->levelCrawler, this->requiredEnemySpawns, this->args);
    this->continuousEnemiesSpawner = new Continuous_Enemies_Spawner(this->args, this->objects, this->requiredEnemySpawns);
    this->end = new End_Spawner(this->objects, this->enemies, this->args, this->requiredEnemySpawns, this->args->useAutoScroll);
    this->powerupDistributor = new Powerup_Distributor(this->levelCrawler, this->objects, this->args, true); //must come after end is allocated
    this->midpointHandler = new Midpoint_Handler(this->objects, this->continuousEnemiesSpawner, this->args, this->args->levelType);
    this->firstPageHandler = new First_Page_Handler(this->objects, this->args->headerBackground, this->args->startCastle, this->args->useAutoScroll);
}

Level_Generator::~Level_Generator() {
    delete this->continuousEnemiesSpawner;
    delete this->header;
    delete this->objects;
    delete this->enemies;
    delete this->pipePointer;
    delete this->enemySpawner;
    delete this->requiredEnemySpawns;
    delete this->end;
    delete this->midpointHandler;
    delete this->firstPageHandler;
    delete this->levelCrawler;
    delete this->powerupDistributor;
}

int Level_Generator::Get_Random_X() {
    //Aim for a lower value... but allow higher values to be possible
    int random = Random::Get_Instance().Get_Num(19);
    if (random < 3) {
        return Random::Get_Instance().Get_Num(0x05);
    } else if (random < 10) {
        return Random::Get_Instance().Get_Num(0x06)+1;
    } else if (random < 16) {
        return Random::Get_Instance().Get_Num(0x07)+1;
    } else if (random < 19) {
        return Random::Get_Instance().Get_Num(0x09)+1;
    } else if (random < 20) {
        return Random::Get_Instance().Get_Num(0x0E)+1;
    } else {
        assert(false);
        return 0;
    }
}

int Level_Generator::Get_Random_X(int min, bool firstPage) {
    int x = this->Get_Random_X();
    x += min;
    if (!firstPage && x > 0x10) x = 0x10;
    else if (firstPage && x > 0x1F) x = 0x1F;
    return x;
}

int Level_Generator::Get_Safe_Random_X() {
    return this->Get_Random_X(this->objects->Get_Last_Object_Length());
}

void Level_Generator::Handle_Auto_Scroll_Start(int &x) {
    if (this->args->useAutoScroll) {
        if (this->objects->Get_Page_Relative_Absolute_X(x) == 0xF) ++x;
        assert(this->objects->Toggle_Auto_Scroll(x));
    }
}

bool Level_Generator::Write_Buffers_To_File() {
    this->args->doesLevelExceedVerticalObjectLimit = this->objects->Did_Level_Exceed_Vertical_Object_Limit();
    QTextStream stream(this->file);
    if (!this->file->seek(0)) return false;
    if (!this->header->Write_Buffer_To_File(this->file)) return false;
    stream << Level_Type::STRING_BREAK << endl;
    if (!this->objects->Write_Buffer_To_File(this->file)) return false;
    stream << Level_Type::STRING_BREAK << endl;
    if (!this->enemies->Write_Buffer_To_File(this->file)) return false;
    stream << Level_Type::STRING_BREAK << endl;
    stream.flush();
    return true;
}

void Level_Generator::Remove_Unsafe_Cannons_From_Level() {
    assert(this->objects);
    if (this->args->levelCompliment != Level_Compliment::BULLET_BILL_TURRETS) return; //no cannons can exist on the level
    int midpointPageX = this->midpointHandler->Get_Midpoint()*16;
    int minInvalidMidpointX = midpointPageX, maxInvalidMidpointX = midpointPageX+15;

    this->objects->Seek_To_First_Item();
    while (!this->objects->At_End()) {
        Buffer_Data *data = this->objects->Get_Current_For_Modification();
        switch (data->objectItem) {
        default:
            break;
        case Object_Item::BULLET_BILL_TURRET:
            if (data->y == 0x9 || data->y == 0xA) { //check if the cannon's height is in range of Mario standing on the ground
                if (data->absoluteX < 16) { //remove the cannon if it is on the first page of the level
                    data->objectItem = Object_Item::VERTICAL_BLOCKS;
                } else { //remove the cannon if it is on the midpoint page
                    if (data->absoluteX >= minInvalidMidpointX && data->absoluteX <= maxInvalidMidpointX) data->objectItem = Object_Item::VERTICAL_BLOCKS;
                }
            }
            break;
        }
        this->objects->Seek_To_Next();
    }
}
