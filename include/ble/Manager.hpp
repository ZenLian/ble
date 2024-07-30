#include "Adapter.hpp"

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
        void addAdapter(GDBusObject *adapter_proxy);

    private:
        Manager();
        void initAdapters();

        GMainLoop *loop_;
        GDBusObjectManager *manager_;
        std::vector<std::shared_ptr<Adapter>> adapters_;
    };
}