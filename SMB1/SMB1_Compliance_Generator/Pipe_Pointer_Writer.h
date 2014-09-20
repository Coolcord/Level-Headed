#ifndef PIPE_POINTER_WRITER_H
#define PIPE_POINTER_WRITER_H

class Enemy_Writer;
class Object_Writer;

class Pipe_Pointer_Writer
{
public:
    Pipe_Pointer_Writer(Object_Writer *object, Enemy_Writer *enemy);
    bool Pipe_Pointer(int x, int room, int page);
    bool Enterable_Pipe(int x, int y, int height, int room, int page);
    bool Reverse_L_Pipe(int x, int room, int page);
    bool Tall_Reverse_L_Pipe(int x, int yPlacement, int room, int page);
    bool Underwater_Sideways_Pipe(int x, int y, int room, int page);
    bool Brick_With_Vine(int x, int y, int room, int page);

private:
    bool Is_Safe_To_Write_Pipe_Pointer();

    Enemy_Writer *enemy;
    Object_Writer *object;
};

#endif // PIPE_POINTER_WRITER_H
