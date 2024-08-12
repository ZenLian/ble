#include "ble/dbus/InterfaceProxy.hpp"

using namespace ble;

DBusInterfaceProxy::DBusInterfaceProxy(GDBusProxy *proxy)
    : _proxy(proxy)
{
    g_object_ref(_proxy);
}

DBusInterfaceProxy::~DBusInterfaceProxy()
{
    g_object_unref(_proxy);
}