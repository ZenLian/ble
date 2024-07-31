#pragma once

#include <gio/gio.h>

#include <vector>

namespace ble
{
    class Adapter
    {
    public:
        Adapter(GDBusProxy *proxy);
        virtual ~Adapter();

    private:
        GDBusProxy *proxy_;
    };

}