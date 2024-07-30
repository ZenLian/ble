#include "ble/Adapter.hpp"

using namespace ble;

Adapter::Adapter(GDBusProxy *proxy)
    : proxy_(proxy)
{
    g_object_ref(proxy);
}

Adapter::~Adapter()
{
    g_object_unref(proxy_);
}