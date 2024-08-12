#pragma once

#include "ble/dbus/ObjectProxy.hpp"

#include <functional>
#include <gio/gio.h>
#include <memory>
#include <string>
#include <vector>

namespace ble
{
    class DBusObjectManager
    {
    public:
        DBusObjectManager(const std::string &name, const std::string &path);
        virtual ~DBusObjectManager();

        // INTERFACES
        std::vector<std::shared_ptr<DBusObjectProxy>> GetManagedObjects();

        using ObjectAddedCallback = std::function<void(std::shared_ptr<DBusObjectProxy>)>;
        using ObjectRemovedCallback = std::function<void(std::shared_ptr<DBusObjectProxy>)>;
        ObjectAddedCallback OnObjectAdded;
        ObjectRemovedCallback OnObjectRemoved;

        void handleObjectAdded(GDBusObject *object);
        void handleObjectRemoved(GDBusObject *object);

    private:
        GDBusObjectManager *_manager;
    };
} // namespace ble
