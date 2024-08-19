#pragma once
#include "bt/dbus/ObjectManager.hpp"
#include "bt/dbus/ObjectProxy.hpp"
#include <gio/gio.h>

namespace bt
{
class Bluez
{
public:
    Bluez();
    virtual ~Bluez();

    void Run();

private:
    void initObjects();
    void printObjects();

    ObjectManager _manager;
    ObjectProxy _rootProxy;
    GMainLoop* _loop;
};
}