#include "bt/bluez/Bluez.hpp"

using namespace bt;

int main()
{
    Bluez *bluez = new Bluez();
    bluez->Run();
}