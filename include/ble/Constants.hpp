#pragma once

namespace ble
{
struct Constants
{
    const char *const DBusBase = "org.freedesktop.DBus";

    const char *const BluezBase = "org.bluez";
    const char *const BluezAdapter = "org.bluez.Adapter1";
    const char *const BluezDevice = "org.bluez.Device1";
};
    
} // namespace ble
