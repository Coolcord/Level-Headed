#include "../../Common_Files/Random.h"
#include "../Common_SMB1_Files/Level_Type_String.h"
#include "Continuous_Enemies_Spawner.h"
#include "Level_Generator.h"
#include <QTextStream>
#include <assert.h>

Level_Generator::Level_Generator(QFile *file, SMB1_Compliance_Generator_Arguments *args) {
    assert(file);
    assert(args);
    this->file = file;
    this->args = args;
    this->header = new Header_Writer();
    this->object = new Object_Writer(this->args->numObjectBytes, this->args);
    this->enemy = new Enemy_Writer(this->args->numEnemyBytes);
    this->pipePointer = new Pipe_Pointer_Writer(this->object, this->enemy);
    this->requiredEnemySpawns = new Required_Enemy_Spawns(this->object, this->enemy, this->pipePointer, this->args);
    this->enemySpawner = new Enemy_Spawner(this->object, this->enemy, this->requiredEnemySpawns, this->args);
    this->continuousEnemiesSpawner = new Continuous_Enemies_Spawner(this->args, this->object, this->requiredEnemySpawns);
    this->end = new End_Spawner(this->object, this->enemy, this->args, this->requiredEnemySpawns, this->args->useAutoScroll);
    this->midpointHandler = new Midpoint_Handler(this->object, this->continuousEnemiesSpawner, this->args, this->args->levelType);
    this->firstPageHandler = new First_Page_Handler(this->object, this->args->headerBackground, this->args->startCastle, this->args->useAutoScroll);
}

Level_Generator::~Level_Generator() {
    delete this->continuousEnemiesSpawner;
    delete this->header;
    delete this->object;
    delete this->enemy;
    delete this->pipePointer;
    delete this->enemySpawner;
    delete this->requiredEnemySpawns;
    delete this->end;
    delete this->midpointHandler;
    delete this->firstPageHandler;
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
    return this->Get_Random_X(this->object->Get_Last_Object_Length());
}

void Level_Generator::Handle_Auto_Scroll_Start(int &x) {
    if (this->args->useAutoScroll) {
        if (this->object->Get_Absolute_X(x) == 0xF) ++x;
        assert(this->object->Toggle_Auto_Scroll(x));
    }
}

bool Level_Generator::Write_Buffers_To_File() {
    QTextStream stream(this->file);
    if (!this->file->seek(0)) return false;
    if (!this->header->Write_Buffer_To_File(this->file)) return false;
    stream << Level_Type::STRING_BREAK << endl;
    if (!this->object->Write_Buffer_To_File(this->file)) return false;
    stream << Level_Type::STRING_BREAK << endl;
    if (!this->enemy->Write_Buffer_To_File(this->file)) return false;
    stream << Level_Type::STRING_BREAK << endl;
    stream.flush();
    return true;
}
