#pragma once

#include <gio/gio.h>
namespace ble
{
    class InterfaceProxy
    {
    public:
        InterfaceProxy(GDBusProxy *proxy);
        virtual ~InterfaceProxy();

    protected:
        // 封装的 GDBusProxy
        GDBusProxy *_proxy;
    };
}