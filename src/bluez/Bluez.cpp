#include "ble/bluez/Bluez.hpp"

#include <stdexcept>

using namespace ble;

Bluez::Bluez()
    : _manager(NULL),
      _loop(NULL)
{
    GError *error = NULL;

    _manager = g_dbus_object_manager_client_new_for_bus_sync(G_BUS_TYPE_SYSTEM,
                                                             G_DBUS_OBJECT_MANAGER_CLIENT_FLAGS_NONE,
                                                             "org.bluez",
                                                             "/",
                                                             NULL, NULL, NULL, NULL, &error);
    if (_manager == NULL)
    {
        g_printerr("Error creating manager client: %s\n", error->message);
        g_error_free(error);
        throw std::runtime_error("");
    }

    // MainLoop
    _loop = g_main_loop_new(NULL, FALSE);

    // TODO: 生成所有 ObjectProxy
}

Bluez::~Bluez()
{
    if (_loop)
        g_main_loop_unref(_loop);
    if (_manager)
        g_object_unref(_manager);
}