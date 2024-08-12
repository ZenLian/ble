#pragma once

#include "ble/gdbus/types.hpp"

#include <gio/gio.h>

#include <memory>
#include <string>
#include <vector>

namespace ble
{
namespace gdbus
{

class ObjectProxy
{
public:
    ObjectProxy(GDBusObjectProxy* proxy);
    virtual ~ObjectProxy();

    std::string GetObjectPath();
    InterfaceProxyPtr GetInterface(const std::string& name);
    std::vector<InterfaceProxyPtr> GetInterfaces();

protected:
    // 封装的 GDBusObjectProxy
    GDBusObjectProxy* _proxy;
}; // class ObjectProxy

} // namespace gdbus
} // namespace ble