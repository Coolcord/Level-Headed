#include "Simple_Object_Spawner.h"
#include <assert.h>

Simple_Object_Spawner::Simple_Object_Spawner(Object_Writer *object) : Spawner() {
    assert(object);
    this->object = object;
}

bool Simple_Object_Spawner::Spawn_Simple_Object() {

}
