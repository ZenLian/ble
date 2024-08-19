#include "bt/AdapterPrivate.hpp"

#include "bt/api/Adapter.hpp"
#include "bt/api/Device.hpp"
#include "bt/gdbus/InterfaceProxy.hpp"

namespace bt
{
AdapterPrivate::AdapterPrivate(Adapter* adapter)
    : _a(adapter)
    , _proxy(nullptr)
{
}

AdapterPrivate::~AdapterPrivate()
{
}

void AdapterPrivate::loadProxy(glib::InterfaceProxyPtr proxy)
{
    _proxy = proxy;
}

void AdapterPrivate::addDevice(glib::InterfaceProxyPtr interface)
{
    DevicePtr device = std::make_shared<Device>();
    // TODO: loadProxy
    // device->loadProxy(interface)

    _devices.insert({ interface->GetObjectPath(), device });

    // 触发 DeviceAdded 信号
    if (_a->OnDeviceAdded) {
        _a->OnDeviceAdded(device);
    }
}

void AdapterPrivate::removeDevice(glib::InterfaceProxyPtr interface)
{
    // find Device
    std::string path = interface->GetObjectPath();
    auto it = _devices.find(path);
    if (it == _devices.end()) {
        return;
    }
    DevicePtr device = it->second;

    _devices.erase(path);

    // 触发 DeviceRemoved 信号
    if (_a->OnDeviceRemoved) {
        _a->OnDeviceRemoved(device);
    }
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

void AdapterPrivate::interfaceAdded(glib::InterfaceProxyPtr interface)
{
    if (interface->GetInterfaceName() == "org.bluez.Device1") {
        // TODO: addDevice
        addDevice(interface);
    }
}

void AdapterPrivate::interfaceRemoved(glib::InterfaceProxyPtr interface)
{
    if (interface->GetInterfaceName() == "org.bluez.Device1") {
        // TODO: removeDevice
        removeDevice(interface);
    }
}

void AdapterPrivate::propertiesChanged(glib::InterfaceProxyPtr interface, GVariant* changed_properties)
{
    if (interface->GetInterfaceName() != "org.bluez.Adapter1") {
        return;
    }

    g_print("[CHG] powered: %s\n", isPowered() ? "true" : "false");
}

} // namespace bt