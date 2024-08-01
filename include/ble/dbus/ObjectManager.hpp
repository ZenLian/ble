#pragma once

#include "ble/dbus/ObjectProxy.hpp"

#include <functional>
#include <gio/gio.h>
#include <memory>
#include <string>
#include <vector>

namespace ble
{
    class ObjectManager
    {
    public:
        ObjectManager(const std::string &name, const std::string &path);
        virtual ~ObjectManager();

        // INTERFACES
        std::vector<std::shared_ptr<ObjectProxy>> GetManagedObjects();

        using ObjectAddedCallback = std::function<void(std::shared_ptr<ObjectProxy>)>;
        using ObjectRemovedCallback = std::function<void(std::shared_ptr<ObjectProxy>)>;
        ObjectAddedCallback OnObjectAdded;
        ObjectRemovedCallback OnObjectRemoved;

        void handleObjectAdded(GDBusObject *object);
        void handleObjectRemoved(GDBusObject *object);

    private:
        GDBusObjectManager *_manager;
    };
} // namespace ble
