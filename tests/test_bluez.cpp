#include "ble/bluez/Bluez.hpp"

using namespace ble;

int main()
{
    Bluez *bluez = new Bluez();
    bluez->Run();
}