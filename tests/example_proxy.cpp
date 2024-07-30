#include <gio/gio.h>

void get_managed_objects(GDBusProxy *manager)
{
    GError *error = NULL;
    GVariant *result = NULL;
    GVariant *objects = NULL;

    g_print("callinng GetManagedObjects...\n");
    result = g_dbus_proxy_call_sync(manager, "GetManagedObjects", NULL, G_DBUS_CALL_FLAGS_NONE,
                                    1000, NULL, &error);
    if (error)
    {
        g_printerr("Error creating manager proxy: %s\n", error->message);
        g_error_free(error);
        return;
    }

    g_print("result is of type: %s\n", g_variant_get_type_string(result));
    g_variant_get(result, "(*)", &objects);
    g_variant_unref(result);

    g_print("objects is of type: %s\n", g_variant_get_type_string(objects));

    GVariantIter iter;
    const gchar *path;
    GVariant *interfaces;
    g_variant_iter_init(&iter, objects);
    while (g_variant_iter_loop(&iter, "{o*}", &path, &interfaces))
    {
        g_print("+ %s\n", path);

        GVariantIter iter2;
        const gchar *interface;
        GVariant *properties;
        g_variant_iter_init(&iter2, interfaces);
        while (g_variant_iter_loop(&iter2, "{s*}", &interface, &properties))
        {
            g_print("  - %s\n", interface);
        }
    }

    g_print("iteration done\n");
out:
    if (objects)
        g_variant_unref(objects);
}

static void
on_properties_changed(GDBusProxy *proxy,
                      GVariant *changed_properties,
                      const gchar *const *invalidated_properties,
                      gpointer user_data)
{
    /* Note that we are guaranteed that changed_properties and
     * invalidated_properties are never NULL
     */

    if (g_variant_n_children(changed_properties) > 0)
    {
        GVariantIter *iter;
        const gchar *key;
        GVariant *value;

        g_print(" *** Properties Changed:\n");
        g_variant_get(changed_properties,
                      "a{sv}",
                      &iter);
        while (g_variant_iter_loop(iter, "{&sv}", &key, &value))
        {
            gchar *value_str;
            value_str = g_variant_print(value, TRUE);
            g_print("      %s -> %s\n", key, value_str);
            g_free(value_str);
        }
        g_variant_iter_free(iter);
    }

    if (g_strv_length((GStrv)invalidated_properties) > 0)
    {
        guint n;
        g_print(" *** Properties Invalidated:\n");
        for (n = 0; invalidated_properties[n] != NULL; n++)
        {
            const gchar *key = invalidated_properties[n];
            g_print("      %s\n", key);
        }
    }
}

static void
on_signal(GDBusProxy *proxy,
          gchar *sender_name,
          gchar *signal_name,
          GVariant *parameters,
          gpointer user_data)
{
    gchar *parameters_str;

    parameters_str = g_variant_print(parameters, TRUE);
    g_print(" *** Received Signal: %s: %s\n",
            signal_name,
            parameters_str);
    g_free(parameters_str);
}

int main()
{
    GError *error = NULL;
    GDBusProxy *manager = NULL;
    GMainLoop *loop = NULL;

    loop = g_main_loop_new(NULL, FALSE);

    manager = g_dbus_proxy_new_for_bus_sync(G_BUS_TYPE_SYSTEM, G_DBUS_PROXY_FLAGS_NONE, NULL,
                                            "org.bluez", "/", "org.freedesktop.DBus.ObjectManager",
                                            NULL, &error);

    if (manager == NULL)
    {
        g_printerr("Error creating manager proxy: %s\n", error->message);
        g_error_free(error);
        goto out;
    }

    g_signal_connect(manager,
                     "g-properties-changed",
                     G_CALLBACK(on_properties_changed),
                     NULL);
    g_signal_connect(manager,
                     "g-signal",
                     G_CALLBACK(on_signal),
                     NULL);

    get_managed_objects(manager);

    g_main_loop_run(loop);

out:
    if (manager)
        g_object_unref(manager);
    if (loop)
        g_main_loop_unref(loop);
}