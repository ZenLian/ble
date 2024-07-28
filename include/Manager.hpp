#include <gio/gio.h>

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

    private:
        Manager();
        void initObjects();

        GDBusObjectManager *manager_;
    };
}