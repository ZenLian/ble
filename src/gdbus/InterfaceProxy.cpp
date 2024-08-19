#include "bt/gdbus/InterfaceProxy.hpp"

using namespace bt::glib;

InterfaceProxy::InterfaceProxy(GDBusProxy* proxy)
    : _proxy(proxy)
{
    g_object_ref(_proxy);
}

InterfaceProxy::~InterfaceProxy()
{
    g_object_unref(_proxy);
}

std::string InterfaceProxy::GetInterfaceName() const
{
    return std::string(g_dbus_proxy_get_interface_name(_proxy));
}

std::string InterfaceProxy::GetObjectPath() const
{
    return std::string(g_dbus_proxy_get_object_path(_proxy));
}

GVariant* InterfaceProxy::GetProperty(const std::string& property_name) const
{
    return g_dbus_proxy_get_cached_property(_proxy, property_name.c_str());
}

void InterfaceProxy::SetProperty(const std::string& property_name, GVariant* value)
{
    GDBusObject* object = g_dbus_interface_get_object(G_DBUS_INTERFACE(_proxy));
    GDBusInterface* propertiesProxy = g_dbus_object_get_interface(object, "org.freedesktop.DBus.Properties");
    GError* error = NULL;

    g_dbus_proxy_call_sync(G_DBUS_PROXY(propertiesProxy), "Set",
        g_variant_new("(ssv)", g_dbus_proxy_get_interface_name(_proxy), property_name.c_str(), value),
        G_DBUS_CALL_FLAGS_NONE,
        -1,
        NULL,
        &error);

    if (error != NULL) {
        g_print("ERROR: cannot set property: %s\n", property_name.c_str());
        g_error_free(error);
    }

    g_object_unref(propertiesProxy);
}
