#include "ble/dbus/ObjectManager.hpp"

#include <stdexcept>

using namespace ble;

ObjectManager::ObjectManager(const std::string &name, const std::string &path)
{
    GError *error = NULL;

    _manager = g_dbus_object_manager_client_new_for_bus_sync(G_BUS_TYPE_SYSTEM,
                                                             G_DBUS_OBJECT_MANAGER_CLIENT_FLAGS_NONE,
                                                             name.c_str(),
                                                             path.c_str(),
                                                             NULL, NULL, NULL, NULL, &error);
    if (_manager == NULL)
    {
        g_printerr("Failed to creat ObjectManagerClient: %s\n", error->message);
        g_error_free(error);
        throw std::runtime_error("");
    }
}

ObjectManager::~ObjectManager()
{
    g_object_unref(_manager);
}

std::vector<std::shared_ptr<ObjectProxy>> ObjectManager::GetManagedObjects()
{
    std::vector<std::shared_ptr<ObjectProxy>> result;
    g_print("GetManagedObjects at %s\n", g_dbus_object_manager_get_object_path(_manager));

    GList *objs;
    objs = g_dbus_object_manager_get_objects(_manager);
    for (GList *obj_item = objs; obj_item != NULL; obj_item = obj_item->next)
    {
        GDBusObject *obj = (GDBusObject *)(obj_item->data);
        g_print("adding object: %s\n", g_dbus_object_get_object_path(obj));
        result.emplace_back(std::make_shared<ObjectProxy>(G_DBUS_OBJECT_PROXY(obj)));
    }
    g_list_free_full(objs, g_object_unref);
    return result;
}