#ifndef LEVEL_SCRIPT_MODIFIER_H
#define LEVEL_SCRIPT_MODIFIER_H

#include "../Common_SMB1_Files/Level_Attribute.h"
#include "SMB1_Compliance_Parser_Arguments.h"

class Level_Script_Modifier {
public:
    Level_Script_Modifier() {}
    ~Level_Script_Modifier() {}
    bool Perform_Enemy_Chaotic_Swap(Enemy_Buffer *enemyBuffer, Level_Attribute::Level_Attribute levelAttribute, bool allowHammerBros, bool allowLakitus, bool allowContinousEnemySpawners);
    bool Redistribute_Enemies(SMB1_Compliance_Generator_Arguments &args, SMB1_Compliance_Parser_Arguments &parserArgs);
    bool Redistribute_Powerups(Object_Buffer *objectBuffer);

private:
    bool Random_Enemy(Buffer_Data *data, bool underwater, bool allowHammerBros, bool allowLakitus, bool allowContinousEnemySpawners);
    void Reset_Enemy_Settings(Buffer_Data *data);
};

#endif // LEVEL_SCRIPT_MODIFIER_H
