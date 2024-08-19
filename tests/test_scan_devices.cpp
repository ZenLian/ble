#include "bt/bt.hpp"

#include <gio/gio.h>
#include <iostream>

using namespace bt;

int main()
{
    g_print("[TEST] start test.\n");
    Manager manager;

    g_print("[TEST] get adapter...\n");
    AdapterPtr adapter = manager.GetDefaultAdapter();
    adapter->setPowered(true);

    g_print("%s\n", adapter->path().c_str());
    g_print("  address: %s\n", adapter->address().c_str());
    g_print("  powered: %s\n", adapter->isPowered() ? "true" : "false");

    adapter->OnDeviceAdded = [](DevicePtr device){
        printf("[NEW] %s\n", device->address());
    };

    manager.Run();
}