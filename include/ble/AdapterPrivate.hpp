#pragma once

#include "ble/types.hpp"
#include "ble/gdbus/types.hpp"
#include <gio/gio.h>
#include <vector>
#include <string>

namespace ble
{
class AdapterPrivate
{
public:
    explicit AdapterPrivate(Adapter* adapter);
    virtual ~AdapterPrivate();

    void loadProxy(gdbus::InterfaceProxyPtr proxy);

    std::string path();

    /** PROPERTIES */
    std::string address() const;
    std::string name() const;
    std::string alias() const;
    // void setAlias(const std::string& alias);
    bool isPowered() const;
    void setPowered(bool powered);
    bool isDiscoverable() const;
    void setDiscoverable(bool discoverable);
    // uint32_t discoverableTimeout() const;
    // void setDiscoverableTimeout();
    // bool isPairable() const;
    // void setPairable(bool pairable);
    // bool isDiscovering() const;
    // std::vector<std::string> uuids();

    /** SIGNALS */
    void propertiesChanged(gdbus::InterfaceProxyPtr interface, GVariant* changed_properties);

    Adapter* _a;

    gdbus::InterfaceProxyPtr _proxy;
    // gdbus::Properties* _propertiesProxy;
};

} // namespace ble
