/**
 * ObjectManagerClient
 */

#include "ble/Manager.hpp"

#include <algorithm>
#include <gio/gio.h>
#include <vector>

using namespace ble;

namespace
{
    void on_object_added(
        GDBusObjectManager *self,
        GDBusObject *object,
        gpointer user_data)
    {
        g_print("== call %s\n", __func__);
        g_print("object: %s\n", g_dbus_object_get_object_path(object));
        g_print("interfaces:\n");

        GList *interfaces = g_dbus_object_get_interfaces(object);
        for (GList *l = interfaces; l != NULL; l = l->next)
        {
            GDBusProxy *interface = G_DBUS_PROXY(l->data);
            g_print("  %s\n", g_dbus_proxy_get_interface_name(interface));
        }
        g_list_free_full(interfaces, g_object_unref);
    }

}

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

    g_signal_connect(manager_, "object-added", G_CALLBACK(on_object_added), this);

    this->loop_ = g_main_loop_new(NULL, FALSE);

out:
    // TODO: exception
    // throw std::exception();
    return;
}

void Manager::initAdapters()
{
    GList *objs;

    // g_print("Object manager at %s\n", g_dbus_object_manager_get_object_path(manager_));

    objs = g_dbus_object_manager_get_objects(manager_);
    for (GList *obj_item = objs; obj_item != NULL; obj_item = obj_item->next)
    {
        GDBusObject *obj = (GDBusObject *)(obj_item->data);
        // g_print("%s\n", g_dbus_object_get_object_path(obj));

        // 遍历 object 上的接口
        GList *ifaces = g_dbus_object_get_interfaces(obj);
        for (GList *iface_item = ifaces; iface_item != NULL; iface_item = iface_item->next)
        {
            GDBusProxy *interface = G_DBUS_PROXY(iface_item->data);
            // g_print("  - %s\n", g_dbus_proxy_get_interface_name(interface));
            const gchar *name = g_dbus_proxy_get_interface_name(interface);
            if (g_str_equal(name, "org.bluez.Adapter1"))
            {
                // 保存到 Adapters
                addAdapter(obj);
            }
        }
        g_list_free_full(ifaces, g_object_unref);
    }
    g_list_free_full(objs, g_object_unref);
}

void Manager::run()
{
    g_print("running manager mainloop...");
    g_main_loop_run(this->loop_);
}

void Manager::addAdapter(GDBusObject *adapter_proxy)
{
    auto adapter = std::make_shared<Adapter>(adapter_proxy);
    this->adapters_.push_back(adapter);
}