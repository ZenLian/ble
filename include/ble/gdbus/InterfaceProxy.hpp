#pragma once

#include <gio/gio.h>
#include <string>

namespace ble::gdbus
{
class InterfaceProxy
{
public:
    InterfaceProxy(GDBusProxy* proxy);
    virtual ~InterfaceProxy();

    std::string GetInterfaceName();

protected:
    GDBusProxy* _proxy;
};
}