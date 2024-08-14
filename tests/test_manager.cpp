#include "ble/ble.hpp"

#include <gio/gio.h>
#include <iostream>

using namespace ble;

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

    manager.Run();
}