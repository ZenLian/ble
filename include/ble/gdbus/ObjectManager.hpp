#pragma once

#include "ble/gdbus/types.hpp"

#include <functional>
#include <gio/gio.h>
#include <memory>
#include <string>
#include <vector>

namespace ble::gdbus
{
class ObjectManager
{
public:
    ObjectManager(const std::string& name, const std::string& path);
    virtual ~ObjectManager();

    // INTERFACES
    std::vector<ObjectProxyPtr> GetManagedObjects();

    using ObjectAddedCallback = std::function<void(ObjectProxyPtr)>;
    using ObjectRemovedCallback = std::function<void(ObjectProxyPtr)>;
    ObjectAddedCallback OnObjectAdded;
    ObjectRemovedCallback OnObjectRemoved;
    std::function<void(ObjectProxyPtr, InterfaceProxyPtr)> OnInterfaceAdded;
    std::function<void(ObjectProxyPtr, InterfaceProxyPtr)> OnInterfaceRemoved;

    void handleObjectAdded(GDBusObject* object);
    void handleObjectRemoved(GDBusObject* object);
    void handleInterfaceAdded(GDBusObject* object, GDBusInterface* interface);
    void handleInterfaceRemoved(GDBusObject* object, GDBusInterface* interface);
    void handleInterfaceProxyPropertiesChanged(GDBusObjectProxy* object_proxy, GDBusProxy* interface_proxy, GVariant* changed_properties,
        char** invalidated_properties);

private:
    GDBusObjectManager* _manager;
};
} // namespace ble
