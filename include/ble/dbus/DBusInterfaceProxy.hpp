#pragma once

#include <gio/gio.h>
namespace ble
{
    class DBusInterfaceProxy
    {
    public:
        DBusInterfaceProxy(GDBusProxy *proxy);
        virtual ~DBusInterfaceProxy();

    protected:
        // 封装的 GDBusProxy
        GDBusProxy *_proxy;
    };
}