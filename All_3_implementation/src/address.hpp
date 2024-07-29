#include<iostream>
#include<string>
using namespace std;

#define SERVICE_ID_1 0x0011
#define INSTANCE_ID_1 0x1111
#define METHOD_ID_1 0x0100

#define SERVICE_ID_2 0x0012
#define INSTANCE_ID_2 0x2222
#define EVENT_ID_2 0x0200

#define EVENT_GROUP_ID 0x1000

#define SERVICE_ID_3 0x0013
#define INSTANCE_ID_3 0x3333
#define SET_METHOD_ID 0x0004
#define GET_METHOD_ID 0x0005
#define EVENT_ID_3 0x0300
#define LEN 4

class Tool
{
    public:
        string name;
        int taken;
        Tool(string name)
        {
            this->name=name;
            taken=0;
        }
        int status()
        {
            return taken;
        }

};

string Inventory[LEN]={"Screwdriver","Drill","Hammer","Chisel"};
