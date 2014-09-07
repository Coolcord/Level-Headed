#include "SMB1_Compliance_Generator.h"
#include <assert.h>

SMB1_Compliance_Generator::SMB1_Compliance_Generator()
{
}

bool SMB1_Compliance_Generator::Generate_Level(const QString &fileName, int numObjects, int numEnemies, Level_Type type) {
    //Create a new file for writing
    this->file = new QFile(fileName);
    assert(this->file);
    if (!this->file->open(QFile::ReadWrite | QFile::Truncate)) {
        throw "Unable to get read/write permissions";
    }


}
