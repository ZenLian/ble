#pragma once

#include "bt/gdbus/types.hpp"

#include <functional>
#include <gio/gio.h>
#include <memory>
#include <string>
#include <vector>

namespace bt::glib
{
class ObjectManager
{
public:
    ObjectManager(const std::string& name, const std::string& path);
    virtual ~ObjectManager();

    // INTERFACES
    std::vector<ObjectProxyPtr> GetManagedObjects();

    std::function<void(ObjectProxyPtr)> OnObjectAdded;
    std::function<void(ObjectProxyPtr)> OnObjectRemoved;
    std::function<void(InterfaceProxyPtr)> OnInterfaceAdded;
    std::function<void(InterfaceProxyPtr)> OnInterfaceRemoved;
    std::function<void(InterfaceProxyPtr, GVariant*, char**)> OnInterfaceProxyPropertiesChanged;

    void handleObjectAdded(GDBusObject* object);
    void handleObjectRemoved(GDBusObject* object);
    void handleInterfaceAdded(GDBusInterface* interface);
    void handleInterfaceRemoved(GDBusInterface* interface);
    void handleInterfaceProxyPropertiesChanged(GDBusProxy* interface_proxy, GVariant* changed_properties,
        char** invalidated_properties);

private:
    GDBusObjectManager* _manager;
};
} // namespace bt
