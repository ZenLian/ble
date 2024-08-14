#include "ble/AdapterPrivate.hpp"

#include "ble/gdbus/InterfaceProxy.hpp"

namespace ble
{
AdapterPrivate::AdapterPrivate(Adapter* adapter)
    : _a(adapter)
    , _proxy(nullptr)
{
}

AdapterPrivate::~AdapterPrivate()
{
}

void AdapterPrivate::loadProxy(gdbus::InterfaceProxyPtr proxy)
{
    _proxy = proxy;
}

std::string AdapterPrivate::path()
{
    return _proxy->GetObjectPath();
}

std::string AdapterPrivate::address() const
{
    GVariant* v = _proxy->GetProperty("Address");
    std::string result(g_variant_get_string(v, NULL));
    g_variant_unref(v);
    return result;
}

std::string AdapterPrivate::name() const
{
    GVariant* v = _proxy->GetProperty("Address");
    std::string result(g_variant_get_string(v, NULL));
    g_variant_unref(v);
    return result;
}

std::string AdapterPrivate::alias() const
{
    GVariant* v = _proxy->GetProperty("Alias");
    std::string result(g_variant_get_string(v, NULL));
    g_variant_unref(v);
    return result;
}

bool AdapterPrivate::isPowered() const
{
    GVariant* v = _proxy->GetProperty("Powered");
    gboolean powered = g_variant_get_boolean(v);
    g_variant_unref(v);
    return powered;
}

void AdapterPrivate::setPowered(bool powered)
{
    GVariant* value = g_variant_new("b", powered);
    _proxy->SetProperty("Powered", value);
}

bool AdapterPrivate::isDiscoverable() const
{
    GVariant* value = _proxy->GetProperty("Discoverable");
    gboolean result(g_variant_get_boolean(value));
    g_variant_unref(value);
    return result;
}

void AdapterPrivate::setDiscoverable(bool discoverable)
{
    GVariant* value = g_variant_new("b", discoverable);
    _proxy->SetProperty("Discoverable", value);
}

void AdapterPrivate::propertiesChanged(gdbus::InterfaceProxyPtr interface, GVariant* changed_properties)
{
    if (interface->GetInterfaceName() != "org.bluez.Adapter1") {
        return;
    }

    g_print("[CHG] powered: %s\n", isPowered() ? "true" : "false");
}

} // namespace ble