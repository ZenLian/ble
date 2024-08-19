#pragma once

namespace bt
{
class Constants
{
public:
    Constants();

    static const char* DBusBase()
    {
        return "org.freedesktop.DBus";
    }

    static const char* BluezBase()
    {
        return "org.bluez";
    }

    static const char* BluezAdpter()
    {
        return "org.bluez.Adapter1";
    }

    static const char* BluezDevice()
    {
        return "org.bluez.Device1";
    }
};

} // namespace bt
