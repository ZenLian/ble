#pragma once
#include "ble/dbus/ObjectProxy.hpp"
#include "ble/dbus/ObjectManager.hpp"
#include <gio/gio.h>

namespace ble
{
    class Bluez
    {
    public:
        Bluez();
        virtual ~Bluez();

        void Run();

    private:
        void initObjects();

        ObjectManager _manager;
        ObjectProxy _rootProxy;
        GMainLoop *_loop;
    };
}