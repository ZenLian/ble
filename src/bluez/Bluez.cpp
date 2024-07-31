#include "ble/bluez/Bluez.hpp"

#include <stdexcept>

using namespace ble;

Bluez::Bluez()
    : _manager(nullptr),
      _rootProxy(nullptr),
      _loop(nullptr)
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
        throw std::runtime_error("Error creating manager client");
    }

    // MainLoop
    _loop = g_main_loop_new(NULL, FALSE);

    _rootProxy = new ObjectProxy("/");
    initObjects();
}

Bluez::~Bluez()
{
    if (_loop)
        g_main_loop_unref(_loop);
    if (_rootProxy)
        delete _rootProxy;
    if (_manager)
        g_object_unref(_manager);
}

void Bluez::initObjects()
{
    GList *objs;

    g_print("Object manager at %s\n", g_dbus_object_manager_get_object_path(_manager));

    objs = g_dbus_object_manager_get_objects(_manager);
    for (GList *obj_item = objs; obj_item != NULL; obj_item = obj_item->next)
    {
        GDBusObject *obj = (GDBusObject *)(obj_item->data);
        g_print("adding object: %s\n", g_dbus_object_get_object_path(obj));
        _rootProxy->AddChild(G_DBUS_OBJECT_PROXY(obj));
    }
    g_list_free_full(objs, g_object_unref);
}

void Bluez::Run()
{
    g_print("running main loop...\n");
    g_main_loop_run(_loop);
}