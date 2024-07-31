#pragma once

#include <gio/gio.h>

#include <vector>
#include <memory>

namespace ble
{
    class Manager
    {
    public:
        // singleton: one and only Manager
        Manager(const Manager &) = delete;
        Manager(Manager &&) = delete;
        Manager &operator=(const Manager &) = delete;
        Manager &operator=(Manager &&) = delete;
        Manager *get();

        void run();

    private:
        Manager();

        GMainLoop *loop_;
        GDBusObjectManager *manager_;

    };
}