#ifndef MIDPOINT_HANDLER_H
#define MIDPOINT_HANDLER_H

class Object_Writer;

class Midpoint_Handler
{
public:
    Midpoint_Handler(Object_Writer *object);
    void Handle_Midpoint(int &x);
    bool Is_Midpoint_Written();
    int Get_Midpoint();

private:
    bool Increment_Past_Midpoint(int &x);

    Object_Writer *object;
    bool midpointWritten;
    int midpoint;
};

#endif // MIDPOINT_HANDLER_H
