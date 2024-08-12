#include "ble/gdbus/ObjectProxy.hpp"

#include <algorithm>

using namespace ble::gdbus;

ObjectProxy::ObjectProxy(GDBusObjectProxy* proxy)
    :_proxy(g_object_ref(proxy))
{
}

ObjectProxy::~ObjectProxy()
{
    g_object_unref(_proxy);
}

std::string ObjectProxy::GetObjectPath()
{
    return std::string(g_dbus_object_get_object_path(G_DBUS_OBJECT(_proxy)));
}

InterfaceProxyPtr ObjectProxy::GetInterface(const std::string& name)
{
    GDBusInterface* interface = g_dbus_object_get_interface(G_DBUS_OBJECT(_proxy), name.c_str());
    if (interface != NULL) {
        return InterfaceProxyPtr();
    }
    InterfaceProxyPtr result = std::make_shared<InterfaceProxy>(G_DBUS_PROXY(interface));
    g_object_unref(interface);
    return result;
}

std::vector<InterfaceProxyPtr> ObjectProxy::GetInterfaces()
{
    std::vector<InterfaceProxyPtr> result;

    GList* interfaces = g_dbus_object_get_interfaces(G_DBUS_OBJECT(_proxy));
    for (GList* item = interfaces; item != NULL; item = item->next)
    {
        GDBusProxy* interface = G_DBUS_PROXY(item->data);
        // std::string name(g_dbus_proxy_get_interface_name(interface));
        // g_print("- %s\n", name.c_str());
        // _interfaces[name] = std::make_shared<InterfaceProxy>(interface);
        result.emplace_back(std::make_shared<InterfaceProxy>(interface));
    }
    g_list_free_full(interfaces, g_object_unref);
    return result;
}
