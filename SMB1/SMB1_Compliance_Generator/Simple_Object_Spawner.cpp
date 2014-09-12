#include "Simple_Object_Spawner.h"
#include <assert.h>

Simple_Object_Spawner::Simple_Object_Spawner(Object_Writer *object) : Spawner() {
    assert(object);
    this->object = object;
}

bool Simple_Object_Spawner::Spawn_Simple_Object() {
    switch (qrand() % this->PROBABILITY_TOTAL_SIMPLE) {
        default:
            return this->Common_Objects();
    }
}

bool Simple_Object_Spawner::Common_Objects() {
    switch (qrand() % this->PROBABILITY_COMMON) {
    return this->object->Horizontal_Bricks(x, y, length);
    return this->object->Horizontal_Blocks(x, y, length);
    return this->object->Horizontal_Coints(x, y, length);
    return this->object->Pipe(x, y, height);
    return this->object->Hole(x, length, false);
    
    default: 
    }
}
