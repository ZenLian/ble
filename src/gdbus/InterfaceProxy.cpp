#include "ble/gdbus/InterfaceProxy.hpp"

using namespace ble::gdbus;

InterfaceProxy::InterfaceProxy(GDBusProxy* proxy)
    : _proxy(proxy)
{
    g_object_ref(_proxy);
}

InterfaceProxy::~InterfaceProxy()
{
    g_object_unref(_proxy);
}

std::string InterfaceProxy::GetInterfaceName()
{
    return std::string(g_dbus_proxy_get_interface_name(_proxy));
}

