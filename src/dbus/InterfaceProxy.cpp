#include "ble/dbus/InterfaceProxy.hpp"

using namespace ble;

InterfaceProxy::InterfaceProxy(GDBusProxy *proxy)
    : _proxy(proxy)
{
    g_object_ref(_proxy);
}

InterfaceProxy::~InterfaceProxy()
{
    g_object_unref(_proxy);
}