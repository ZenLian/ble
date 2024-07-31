#pragma once

#include "ble/dbus/ObjectProxy.hpp"

#include <gio/gio.h>
#include <string>
#include <vector>
#include <memory>

namespace ble
{
    class ObjectManager
    {
    public:
        ObjectManager(const std::string &name, const std::string &path);
        virtual ~ObjectManager();

        // INTERFACES
        std::vector<std::shared_ptr<ObjectProxy>> GetManagedObjects();

    private:
        GDBusObjectManager *_manager;
    };
} // namespace ble
