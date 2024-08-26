#include "bt/ManagerPrivate.hpp"

#include "bt/api/Adapter.hpp"
#include "bt/api/Manager.hpp"
#include "bt/gdbus/ObjectManager.hpp"
#include "bt/gdbus/ObjectProxy.hpp"
#include "bt/gdbus/InterfaceProxy.hpp"
#include "bt/gdbus/PathUtils.hpp"
#include "bt/AdapterPrivate.hpp"

#include <gio/gio.h>
#include <functional>

using namespace bt;
using std::placeholders::_1;
using std::placeholders::_2;
using std::placeholders::_3;

ManagerPrivate::ManagerPrivate(Manager* manager)
    : _m(manager)
    , _objectManager(nullptr)
    , _loop()
    , _defaultAdapter(nullptr)
{
    printf("new ManagerPrivate...\n");
    _objectManager = new glib::ObjectManager("org.bluez", "/");
    _objectManager->OnObjectAdded = std::bind(&ManagerPrivate::objectAdded, this, _1);
    _objectManager->OnObjectRemoved = std::bind(&ManagerPrivate::objectRemoved, this, _1);
    _objectManager->OnInterfaceAdded = std::bind(&ManagerPrivate::interfaceAdded, this, _1);
    _objectManager->OnInterfaceRemoved = std::bind(&ManagerPrivate::interfaceRemoved, this, _1);
    _objectManager->OnInterfaceProxyPropertiesChanged = std::bind(&ManagerPrivate::propertiesChanged, this, _1, _2, _3);
    manageObjects();
}

ManagerPrivate::~ManagerPrivate()
{
    if (_loop.isRunning())
        _loop.quit();
    delete _objectManager;
}

void ManagerPrivate::run()
{
    printf("main loop running...\n");
    _loop.run();
}

void ManagerPrivate::manageObjects()
{
    g_print("GetManagedObjects...\n");
    std::vector<glib::ObjectProxyPtr> objects = _objectManager->GetManagedObjects();
    for (glib::ObjectProxyPtr object : objects) {
        objectAdded(object);
        // TODO: AgentManager, ProfileManager在这里创建
    }
}

void ManagerPrivate::objectAdded(glib::ObjectProxyPtr object)
{
    // 按接口遍历
    for (auto interface : object->GetInterfaces()) {
        interfaceAdded(interface);
    }
}

void ManagerPrivate::objectRemoved(glib::ObjectProxyPtr object)
{
    for (auto interface : object->GetInterfaces()) {
        interfaceRemoved(interface);
    }
}

void ManagerPrivate::interfaceAdded(glib::InterfaceProxyPtr interface)
{
    std::string name = interface->GetInterfaceName();
    if (name == "org.bluez.Adapter1") {
        // 添加 Adapter
        addAdapter(interface);
    }

    std::string path(interface->GetObjectPath());
    for (auto it : _adapters) {
        if (it.first == path || PathUtils::IsAscendantOf(it.first, path)) {
            // 交给 Adapter 处理
            it.second->_impl->interfaceAdded(interface);
        }
    }
}

void ManagerPrivate::interfaceRemoved(glib::InterfaceProxyPtr interface)
{
    std::string name = interface->GetInterfaceName();

    if (name == "org.bluez.Adapter1") {
        // 移除 Adapter
        removeAdapter(interface);
    }

    std::string path(interface->GetObjectPath());
    for (auto it : _adapters) {
        if (it.first == path || PathUtils::IsAscendantOf(it.first, path)) {
            // 交给 Adapter 处理
            it.second->_impl->interfaceRemoved(interface);
        }
    }
}

void ManagerPrivate::propertiesChanged(glib::InterfaceProxyPtr interface, GVariant* changed_properties, char** invalidated_properties)
{

    std::string path(interface->GetObjectPath());
    for (auto it : _adapters) {
        if (it.first == path || PathUtils::IsAscendantOf(it.first, path)) {
            // TODO: 交给 Adapter 处理
            it.second->_impl->propertiesChanged(interface, changed_properties);
        }
    }
}

void ManagerPrivate::addAdapter(glib::InterfaceProxyPtr interface)
{
    AdapterPtr adapter = std::make_shared<Adapter>();
    adapter->_impl->loadProxy(interface);
    _adapters.insert({ interface->GetObjectPath(), adapter });

    if (!_defaultAdapter) {
        setDefaultAdapter(adapter);
    }

    // 触发 AdapterAdded 信号
    if (_m->OnAdapterAdded) {
        _m->OnAdapterAdded(adapter);
    }
}

void ManagerPrivate::removeAdapter(glib::InterfaceProxyPtr interface)
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
    g_print("Default Adapter Set To: %s\n", adapter->address().c_str());
    _defaultAdapter = adapter;
}