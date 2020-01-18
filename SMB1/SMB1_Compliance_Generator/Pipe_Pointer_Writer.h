#ifndef PIPE_POINTER_WRITER_H
#define PIPE_POINTER_WRITER_H

class Enemy_Writer;
class Object_Writer;

class Pipe_Pointer_Writer {
public:
    Pipe_Pointer_Writer(Object_Writer *objects, Enemy_Writer *enemies);
    ~Pipe_Pointer_Writer() {}
    bool Pipe_Pointer(int x, int room, int page);
    bool Enterable_Pipe(int x, int y, int height, int room, int page);
    bool Reverse_L_Pipe(int x, int room, int page);
    bool Tall_Reverse_L_Pipe(int x, int yPlacement, int room, int page);
    bool Underwater_Sideways_Pipe(int x, int y, int room, int page);
    bool Brick_With_Vine(int x, int y, int room, int page);

private:
    Pipe_Pointer_Writer(const Pipe_Pointer_Writer&);
    Pipe_Pointer_Writer& operator=(const Pipe_Pointer_Writer&);
    bool Is_Safe_To_Write_Pipe_Pointer();

    Enemy_Writer *enemies;
    Object_Writer *objects;
};

#endif // PIPE_POINTER_WRITER_H
