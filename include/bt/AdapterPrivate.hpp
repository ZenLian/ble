#pragma once

#include "bt/types.hpp"
#include "bt/gdbus/types.hpp"
#include <gio/gio.h>
#include <vector>
#include <string>
#include <map>

namespace bt
{
class AdapterPrivate
{
public:
    explicit AdapterPrivate(Adapter* adapter);
    virtual ~AdapterPrivate();

    void loadProxy(glib::InterfaceProxyPtr proxy);

    void addDevice(glib::InterfaceProxyPtr interface);
    void removeDevice(glib::InterfaceProxyPtr interface);

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

    /** 处理 SIGNALS，由 Manager 触发 */
    void interfaceAdded(glib::InterfaceProxyPtr interface);
    void interfaceRemoved(glib::InterfaceProxyPtr interface);
    void propertiesChanged(glib::InterfaceProxyPtr interface, GVariant* changed_properties);

    Adapter* _a;

    glib::InterfaceProxyPtr _proxy;

    std::map<std::string, DevicePtr> _devices;
};

} // namespace bt
