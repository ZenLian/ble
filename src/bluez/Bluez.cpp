#include "ble/bluez/Bluez.hpp"

#include "ble/dbus/ObjectManager.hpp"

#include <stdexcept>

using namespace ble;

Bluez::Bluez()
    : _manager("org.bluez", "/"),
      _rootProxy("/"),
      _loop(nullptr)
{
    // MainLoop
    _loop = g_main_loop_new(NULL, FALSE);

    initObjects();

    // handle signals
}

Bluez::~Bluez()
{
    if (_loop)
        g_main_loop_unref(_loop);
}

void Bluez::initObjects()
{
    std::vector<std::shared_ptr<DBusObjectProxy>> objects = _manager.GetManagedObjects();
    for (auto proxy : objects)
    {
        g_print("adding object: %s\n", proxy->GetObjectPath().c_str());
        _rootProxy.AddChild(proxy);
    }
}

void Bluez::printObjects()
{
    _rootProxy.Print();
}

void Bluez::Run()
{
    printObjects();
    g_print("running main loop...\n");
    g_main_loop_run(_loop);
}