#pragma once

#include "ble/types.hpp"
#include "ble/gdbus/types.hpp"

#include <gio/gio.h>

namespace ble
{
class ObjectManager;
class ObjectProxy;

class ManagerPrivate
{
public:
    explicit ManagerPrivate(Manager* manager);
    virtual ~ManagerPrivate();

    void Init();

    void manageObjects();

    /** 处理信号 */
    void objectAdded(gdbus::ObjectProxyPtr object);
    void objectRemoved(gdbus::ObjectProxyPtr object);
    void interfaceAdded(gdbus::InterfaceProxyPtr interface);
    void interfaceRemoved(gdbus::InterfaceProxyPtr interface);

    Manager* _m;
    bool _initialized;
    gdbus::ObjectManager* _objectManager;
    GMainLoop* _loop;
};
} // namespace ble
