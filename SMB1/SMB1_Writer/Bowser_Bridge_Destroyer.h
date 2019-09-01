#ifndef BOWSER_BRIDGE_DESTROYER_H
#define BOWSER_BRIDGE_DESTROYER_H

#include "Byte_Writer.h"

class Bowser_Bridge_Destroyer : public Byte_Writer {
public:
    Bowser_Bridge_Destroyer(QFile *file, Level_Offset *levelOffset) : Byte_Writer(file, levelOffset) {}
    ~Bowser_Bridge_Destroyer() {}
    bool Alternating();
    bool Forwards();
    bool Backwards();
    bool Chaotic();
    bool From_Center();
    bool To_Center();
    bool Grouped();

private:
    bool Groups_Of_3();
    bool Groups_Of_4();
    bool Groups_Of_6();
};

#endif // BOWSER_BRIDGE_DESTROYER_H
