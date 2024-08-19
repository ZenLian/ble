#pragma once

#include "bt/gdbus/types.hpp"

#include <gio/gio.h>

#include <memory>
#include <string>
#include <vector>

namespace bt
{
namespace glib
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

} // namespace glib
} // namespace bt