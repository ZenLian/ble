/**
 * ObjectManagerClient
 */

#include "Manager.hpp"

#include <algorithm>
#include <gio/gio.h>
#include <vector>

using namespace ble;

Manager *Manager::get()
{
    static Manager manager;
    return &manager;
}

Manager::Manager()
{
    GDBusObjectManager *object_manager = NULL;
    GError *error = NULL;

    object_manager = g_dbus_object_manager_client_new_for_bus_sync(G_BUS_TYPE_SYSTEM,
                                                                   G_DBUS_OBJECT_MANAGER_CLIENT_FLAGS_NONE,
                                                                   "org.bluez",
                                                                   "/",
                                                                   NULL, NULL, NULL, NULL, &error);
    if (object_manager == NULL)
    {
        g_printerr("Error creating manager client: %s\n", error->message);
        g_error_free(error);
        goto out;
    }
    this->manager_ = object_manager;

out:
    // TODO: exception
    // throw std::exception();
    return;
}

void Manager::initObjects()
{
    GList *objs;

    // g_print("Object manager at %s\n", g_dbus_object_manager_get_object_path(manager_));

    std::vector<GDBusProxy *> adapter_proxies;
    std::vector<GDBusProxy *> device_proxies;

    objs = g_dbus_object_manager_get_objects(manager_);
    for (GList *obj_item = objs; obj_item != NULL; obj_item = obj_item->next)
    {
        GDBusObject *obj = (GDBusObject *)(obj_item->data);
        // g_print("%s\n", g_dbus_object_get_object_path(obj));

        GList *ifaces = g_dbus_object_get_interfaces(obj);
        for (GList *iface_item = ifaces; iface_item != NULL; iface_item = iface_item->next)
        {
            GDBusProxy *interface = G_DBUS_PROXY(iface_item->data);
            // g_print("  - %s\n", g_dbus_proxy_get_interface_name(interface));
            const gchar *name = g_dbus_proxy_get_interface_name(interface);
            if (g_str_equal(name, "org.bluez.Adapter1"))
            {
                // 保存到 Adapters
                g_object_ref(interface);
                adapter_proxies.push_back(interface);
            }
            if (g_str_equal(name, "org.bluez.Device1"))
            {
                // 保存到 Devices
                g_object_ref(interface);
                device_proxies.push_back(interface);
            }
        }
        g_list_free_full(ifaces, g_object_unref);
    }
    g_list_free_full(objs, g_object_unref);

    // TODO: addAdapters foreach adapter_proxies
    std::for_each(adapter_proxies.begin(), adapter_proxies.end(), []() -> {});
}