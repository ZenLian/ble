#include <gio/gio.h>

void manage_objects(GDBusObjectManager *manager)
{
    GList *objs;

    g_print("Object manager at %s\n", g_dbus_object_manager_get_object_path(manager));

    objs = g_dbus_object_manager_get_objects(manager);
    for (GList *obj_item = objs; obj_item != NULL; obj_item = obj_item->next)
    {
        GDBusObject *obj = (GDBusObject *)(obj_item->data);
        g_print("%s\n", g_dbus_object_get_object_path(obj));

        GList *ifaces = g_dbus_object_get_interfaces(obj);
        for (GList *iface_item = ifaces; iface_item != NULL; iface_item = iface_item->next)
        {
            GDBusProxy *interface = G_DBUS_PROXY(iface_item->data);
            g_print("  - %s\n", g_dbus_proxy_get_interface_name(interface));
        }
        if (ifaces)
            g_list_free_full(ifaces, g_object_unref);
    }
    if (objs)
        g_list_free_full(objs, g_object_unref);
}

void on_object_added(
    GDBusObjectManager *self,
    GDBusObject *object,
    gpointer user_data)
{
    g_print("== call %s\n", __func__);
    g_print("object: %s\n", g_dbus_object_get_object_path(object));
    g_print("interfaces:\n");

    GList *interfaces = g_dbus_object_get_interfaces(object);
    g_print("size: %u\n", g_list_length(interfaces));
    for (GList *l = interfaces; l != NULL; l = l->next)
    {
        GDBusProxy *interface = G_DBUS_PROXY(l->data);
        g_print("  %s\n", g_dbus_proxy_get_interface_name(interface));
    }
    g_list_free_full(interfaces, g_object_unref);
}

void on_object_removed(
    GDBusObjectManager *self,
    GDBusObject *object,
    gpointer user_data)
{
    g_print("== call %s\n", __func__);
    g_print("object removed: %s\n", g_dbus_object_get_object_path(object));
}

void on_interface_added(
    GDBusObjectManager *manager,
    GDBusObject *object,
    GDBusInterface *interface,
    gpointer user_data)
{
    g_print("== call %s\n", __func__);
    // const gchar *iface_name = g_dbus_proxy_get_interface_name(G_DBUS_PROXY(interface));
    // g_print("Interface Added: %s\n", iface_name);
}

void on_interface_removed(
    GDBusObjectManager *self,
    GDBusObject *object,
    GDBusInterface *interface,
    gpointer user_data)
{
    g_print("== call %s\n", __func__);
}

void on_interface_proxy_properties_changed(
    GDBusObjectManagerClient *manager,
    GDBusObjectProxy *object_proxy,
    GDBusProxy *interface_proxy,
    GVariant *changed_properties,
    char **invalidated_properties,
    gpointer user_data)
{
    g_print("== call %s\n", __func__);
    g_print("object: %s\n", g_dbus_object_get_object_path(G_DBUS_OBJECT(object_proxy)));
    g_print("interface: %s\n", g_dbus_proxy_get_interface_name(interface_proxy));
    g_print("changed properties:\n");

    GVariantIter iter;
    const gchar *key;
    GVariant *value;
    g_variant_iter_init(&iter, changed_properties);
    while (g_variant_iter_next(&iter, "{&sv}", &key, &value))
    {
        gchar *s = g_variant_print(value, TRUE);
        g_print("  %s -> %s\n", key, s);
        g_variant_unref(value);
        g_free(s);
    }
}

int main()
{
    GError *error = NULL;
    GDBusObjectManager *manager = NULL;
    GMainLoop *loop = NULL;

    loop = g_main_loop_new(NULL, FALSE);

    manager = g_dbus_object_manager_client_new_for_bus_sync(G_BUS_TYPE_SYSTEM,
                                                            G_DBUS_OBJECT_MANAGER_CLIENT_FLAGS_NONE,
                                                            "org.bluez",
                                                            "/",
                                                            NULL, NULL, NULL, NULL, &error);
    if (manager == NULL)
    {
        g_printerr("Error creating manager client: %s\n", error->message);
        g_error_free(error);
        goto out;
    }

    manage_objects(manager);

    g_signal_connect(manager, "object-added", G_CALLBACK(on_object_added), NULL);
    g_signal_connect(manager, "object-removed", G_CALLBACK(on_object_removed), NULL);
    g_signal_connect(manager, "interface-added", G_CALLBACK(on_interface_added), NULL);
    g_signal_connect(manager, "interface-removed", G_CALLBACK(on_interface_removed), NULL);
    g_signal_connect(manager, "interface-proxy-properties-changed", G_CALLBACK(on_interface_proxy_properties_changed), NULL);

    g_print("running mainloop...\n");
    g_main_loop_run(loop);

out:
    if (manager)
        g_object_unref(manager);
    if (loop)
        g_main_loop_unref(loop);
    return 0;
}