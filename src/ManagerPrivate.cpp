#include "ble/ManagerPrivate.hpp"

#include "ble/api/Adapter.hpp"
#include "ble/api/Manager.hpp"
#include "ble/gdbus/ObjectManager.hpp"
#include "ble/gdbus/ObjectProxy.hpp"
#include "ble/gdbus/InterfaceProxy.hpp"
#include "ble/gdbus/PathUtils.hpp"
#include "ble/AdapterPrivate.hpp"

#include <functional>

using namespace ble;
using std::placeholders::_1;
using std::placeholders::_2;
using std::placeholders::_3;

ManagerPrivate::ManagerPrivate(Manager* manager)
    : _m(manager)
    , _objectManager(nullptr)
    , _loop(nullptr)
    , _defaultAdapter(nullptr)
{
    printf("new ManagerPrivate...\n");
    _objectManager = new gdbus::ObjectManager("org.bluez", "/");
    _objectManager->OnObjectAdded = std::bind(&ManagerPrivate::objectAdded, this, _1);
    _objectManager->OnObjectRemoved = std::bind(&ManagerPrivate::objectRemoved, this, _1);
    _objectManager->OnInterfaceAdded = std::bind(&ManagerPrivate::interfaceAdded, this, _1);
    _objectManager->OnInterfaceRemoved = std::bind(&ManagerPrivate::interfaceRemoved, this, _1);
    _objectManager->OnInterfaceProxyPropertiesChanged = std::bind(&ManagerPrivate::propertiesChanged, this, _1, _2, _3);
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

void ManagerPrivate::run()
{
    printf("main loop running...\n");
    g_main_loop_run(_loop);
}

void ManagerPrivate::manageObjects()
{
    g_print("GetManagedObjects...\n");
    std::vector<gdbus::ObjectProxyPtr> objects = _objectManager->GetManagedObjects();
    for (gdbus::ObjectProxyPtr object : objects) {
        objectAdded(object);
        // TODO: AgentManager, ProfileManager在这里创建
    }
}

void ManagerPrivate::objectAdded(gdbus::ObjectProxyPtr object)
{
    // 按接口遍历
    for (auto interface : object->GetInterfaces()) {
        interfaceAdded(interface);
    }
}

void ManagerPrivate::objectRemoved(gdbus::ObjectProxyPtr object)
{
    for (auto interface : object->GetInterfaces()) {
        interfaceRemoved(interface);
    }
}

void ManagerPrivate::interfaceAdded(gdbus::InterfaceProxyPtr interface)
{
    std::string name = interface->GetInterfaceName();
    if (name == "org.bluez.Adapter1") {
        // TODO: 添加 Adapter
        addAdapter(interface);
    }

    std::string path(interface->GetObjectPath());
    for (auto it : _adapters) {
        if (it.first == path || PathUtils::IsAscendantOf(it.first, path)) {
            // TODO: 交给 Adapter 处理
            // it.second->_p->interfaceAdded(interface);
        }
    }
}

void ManagerPrivate::interfaceRemoved(gdbus::InterfaceProxyPtr interface)
{
    std::string name = interface->GetInterfaceName();

    if (name == "org.bluez.Adapter1") {
        // TODO: 移除 Adapter
        removeAdapter(interface);
    }

    std::string path(interface->GetObjectPath());
    for (auto it : _adapters) {
        if (it.first == path || PathUtils::IsAscendantOf(it.first, path)) {
            // TODO: 交给 Adapter 处理
            // it.second->_p->interfaceRemoved(interface);
        }
    }
}

void ManagerPrivate::propertiesChanged(gdbus::InterfaceProxyPtr interface, GVariant* changed_properties, char** invalidated_properties)
{

    std::string path(interface->GetObjectPath());
    for (auto it : _adapters) {
        if (it.first == path || PathUtils::IsAscendantOf(it.first, path)) {
            // TODO: 交给 Adapter 处理
            it.second->_p->propertiesChanged(interface, changed_properties);
        }
    }
}

void ManagerPrivate::addAdapter(gdbus::InterfaceProxyPtr interface)
{
    AdapterPtr adapter = std::make_shared<Adapter>();
    adapter->_p->loadProxy(interface);
    _adapters.insert({ interface->GetObjectPath(), adapter });

    if (!_defaultAdapter) {
        setDefaultAdapter(adapter);
    }

    // 触发 AdapterAdded 信号
    if (_m->OnAdapterAdded) {
        _m->OnAdapterAdded(adapter);
    }
}

void ManagerPrivate::removeAdapter(gdbus::InterfaceProxyPtr interface)
{
    // find Adapter
    std::string path = interface->GetObjectPath();
    auto it = _adapters.find(path);
    if (it == _adapters.end()) {
        return;
    }
    AdapterPtr adapter = it->second;

    _adapters.erase(path);

    // 重设默认 Adapter
    if (_defaultAdapter == adapter) {
        setDefaultAdapter(findUsableAdapter());
    }

    // 触发 AdapterRemoved 信号
    if (_m->OnAdapterRemoved) {
        _m->OnAdapterRemoved(adapter);
    }
}

AdapterPtr ManagerPrivate::findUsableAdapter() const
{
    for (const auto it : _adapters) {
        return it.second;
    }
    return AdapterPtr();
}

void ManagerPrivate::setDefaultAdapter(const AdapterPtr& adapter)
{
    _defaultAdapter = adapter;
}