#pragma once

#include "bt/types.hpp"
#include "bt/gdbus/types.hpp"
#include "bt/gdbus/MainLoop.hpp"

#include <map>
#include <string>

namespace bt
{
class ManagerPrivate
{
public:
    explicit ManagerPrivate(Manager* manager);
    virtual ~ManagerPrivate();

    void run();

    void manageObjects();
    void addAdapter(glib::InterfaceProxyPtr interface);
    void removeAdapter(glib::InterfaceProxyPtr interface);

    AdapterPtr findUsableAdapter() const;
    void setDefaultAdapter(const AdapterPtr& adapter);

    /** 处理信号 */
    void objectAdded(glib::ObjectProxyPtr object);
    void objectRemoved(glib::ObjectProxyPtr object);
    void interfaceAdded(glib::InterfaceProxyPtr interface);
    void interfaceRemoved(glib::InterfaceProxyPtr interface);
    void propertiesChanged(glib::InterfaceProxyPtr interface, GVariant* changed_properties, char** invalidated_properties);

    Manager* _m;
    glib::ObjectManager* _objectManager;
    glib::MainLoop _loop;

    std::map<std::string, AdapterPtr> _adapters;
    AdapterPtr _defaultAdapter;
};
} // namespace bt
