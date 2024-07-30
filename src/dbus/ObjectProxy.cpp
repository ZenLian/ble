#include "ble/dbus/ObjectProxy.hpp"

using namespace ble;

ObjectProxy::ObjectProxy(GDBusObjectProxy *proxy)
    : _proxy(proxy)
{
    g_object_ref(_proxy);
}

ObjectProxy::~ObjectProxy()
{
    g_object_unref(_proxy);
}