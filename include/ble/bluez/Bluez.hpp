#include <gio/gio.h>

namespace ble
{
    class Bluez
    {
    public:
        Bluez();
        virtual ~Bluez();

    private:
        GDBusObjectManager *_manager;
        GMainLoop *_loop;
    };
}