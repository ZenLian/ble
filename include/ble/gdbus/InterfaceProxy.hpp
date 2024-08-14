#pragma once

#include "ble/gdbus/types.hpp"

#include <gio/gio.h>
#include <string>

namespace ble::gdbus
{
class InterfaceProxy
{
public:
    InterfaceProxy(GDBusProxy* proxy);
    virtual ~InterfaceProxy();

    std::string GetInterfaceName() const;
    std::string GetObjectPath() const;

    GVariant* GetProperty(const std::string& property_name) const;
    void SetProperty(const std::string& property_name, GVariant* value);

protected:
    GDBusProxy* _proxy;
};
}