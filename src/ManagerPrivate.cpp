#include "ble/ManagerPrivate.hpp"

#include "ble/gdbus/ObjectManager.hpp"
#include "ble/gdbus/ObjectProxy.hpp"
#include "ble/gdbus/InterfaceProxy.hpp"

#include <functional>

using namespace ble;
using std::placeholders::_1;

ManagerPrivate::ManagerPrivate(Manager* manager)
    : _m(manager)
    , _initialized(false)
    , _objectManager(nullptr)
    , _loop(nullptr)
{
    _objectManager = new gdbus::ObjectManager("org.bluez", "/");
    _objectManager->OnObjectAdded = std::bind(&ManagerPrivate::objectAdded, this, _1);
    _objectManager->OnObjectRemoved = std::bind(&ManagerPrivate::objectRemoved, this, _1);
    _objectManager->OnInterfaceAdded = std::bind(&ManagerPrivate::interfaceAdded, this, _1);
    _objectManager->OnInterfaceRemoved = std::bind(&ManagerPrivate::interfaceRemoved, this, _1);
    manageObjects();

    _loop = g_main_loop_new(NULL, false);
}

ManagerPrivate::~ManagerPrivate()
{
    if (_loop) {
        g_main_loop_unref(_loop);
    }
    delete _objectManager;
}

void ManagerPrivate::manageObjects()
{
    std::vector<gdbus::ObjectProxyPtr> objects = _objectManager->GetManagedObjects();
    for (gdbus::ObjectProxyPtr object : objects) {
        // 按接口遍历
        for (auto interface : object->GetInterfaces()) {
            interfaceAdded(interface);
        }
    }
}

void ManagerPrivate::interfaceAdded(gdbus::InterfaceProxyPtr interface)
{
    std::string name(interface->GetInterfaceName());
    if (name == "org.bluez.Adapter1") {
        // TODO:
        addAdapter();
    }
    if (name == "org.bluez.Device1") {
        // TODO:
        addDevice();
    }
}