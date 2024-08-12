#pragma once

#include "ble/types.hpp"

#include <gio/gio.h>

namespace ble
{
class DBusObjectManager;
class DBusObjectProxy;

class ManagerPrivate
{
public:
    explicit ManagerPrivate(Manager *manager);
    virtual ~ManagerPrivate();

    void Init();

    void manageObjects();

    /** 处理信号 */
    void objectAdded(DBusObjectProxyPtr object);
    void objectRemoved(DBusObjectProxyPtr object);

    Manager *_m;
    bool _initialized;
    DBusObjectManager *_objectManager;
    GMainLoop *_loop;
};
} // namespace ble
