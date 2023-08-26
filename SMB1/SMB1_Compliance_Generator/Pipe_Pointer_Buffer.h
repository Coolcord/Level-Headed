#ifndef PIPE_POINTER_BUFFER_H
#define PIPE_POINTER_BUFFER_H

#include "../Common_SMB1_Files/Level.h"

class Enemy_Buffer;
class Object_Buffer;
class Required_Enemy_Spawns;

class Pipe_Pointer_Buffer {
public:
    Pipe_Pointer_Buffer(Object_Buffer *objects, Enemy_Buffer *enemies, Required_Enemy_Spawns *requiredEnemySpawns);
    ~Pipe_Pointer_Buffer() {}
    bool Pipe_Pointer(int x, Level::Level level, int world, int page);
    bool Enterable_Pipe(int x, int y, int height, Level::Level level, int world, int page);
    bool Tall_Reverse_L_Pipe(int x, int yPlacement, Level::Level level, int world, int page);
    bool Underwater_Sideways_Pipe(int x, int y, Level::Level level, int world, int page);
    bool Brick_With_Vine(int x, int y, Level::Level level, int world, int page);

private:
    Pipe_Pointer_Buffer(const Pipe_Pointer_Buffer&);
    Pipe_Pointer_Buffer& operator=(const Pipe_Pointer_Buffer&);
    bool Is_Safe_To_Write_Pipe_Pointer();

    Enemy_Buffer *enemies;
    Object_Buffer *objects;
    Required_Enemy_Spawns *requiredEnemySpawns;
};

#endif // PIPE_POINTER_BUFFER_H
