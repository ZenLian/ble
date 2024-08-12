#include "ble/ManagerPrivate.hpp"

#include "ble/dbus/ObjectManager.hpp"
#include "ble/constants.hpp"

#include <functional>

using namespace ble;
using std::placeholders::_1;

ManagerPrivate::ManagerPrivate(Manager *manager)
    : _m(manager)
    , _initialized(false)
    , _objectManager(nullptr)
    , _loop(nullptr)
{
    _objectManager = new DBusObjectManager("org.bluez", "/");
    _objectManager->OnObjectAdded = std::bind(&ManagerPrivate::objectAdded, this, _1);
    _objectManager->OnObjectRemoved = std::bind(&ManagerPrivate::objectRemoved, this, _1);
    manageObjects();
}

ManagerPrivate::~ManagerPrivate()
{
    if (_loop) {
        g_main_loop_unref(loop);
    }
    delete _objectManager;
}

void ManagerPrivate::manageObjects()
{
    std::vector<DBusObjectProxyPtr> objects = _objectManager->GetManagedObjects();
    for (DBusObjectProxyPtr object : objects) {
        // TODO: 按接口遍历。
        // 也许可以考虑不用 GDBus 封装的 ObjectManager，而是自己调用、监听 OM 接口，但这样就需要自己创建 InterfaceProxy。
    }
}

void ManagerPrivate::Init()
{

}