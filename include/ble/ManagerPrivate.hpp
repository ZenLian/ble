#pragma once

#include "ble/types.hpp"
#include "ble/gdbus/types.hpp"

#include <gio/gio.h>
#include <map>
#include <string>

namespace ble
{
class ManagerPrivate
{
public:
    explicit ManagerPrivate(Manager* manager);
    virtual ~ManagerPrivate();

    void run();

    void manageObjects();
    void addAdapter(gdbus::InterfaceProxyPtr interface);
    void removeAdapter(gdbus::InterfaceProxyPtr interface);

    AdapterPtr findUsableAdapter() const;
    void setDefaultAdapter(const AdapterPtr& adapter);

    /** 处理信号 */
    void objectAdded(gdbus::ObjectProxyPtr object);
    void objectRemoved(gdbus::ObjectProxyPtr object);
    void interfaceAdded(gdbus::InterfaceProxyPtr interface);
    void interfaceRemoved(gdbus::InterfaceProxyPtr interface);
    void propertiesChanged(gdbus::InterfaceProxyPtr interface, GVariant* changed_properties, char** invalidated_properties);

    Manager* _m;
    gdbus::ObjectManager* _objectManager;
    GMainLoop* _loop;

    std::map<std::string, AdapterPtr> _adapters;
    AdapterPtr _defaultAdapter;
};
} // namespace ble
