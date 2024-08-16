#include <gio/gio.h>

class SerialPortProfile
{
public:
    SerialPortProfile(const gchar* path)
    {
        _path = path;

        GDBusObjectSkeleton* object = g_dbus_object_skeleton_new("/my/spp");
    }

    ~SerialPortProfile()
    {
    }

    const gchar* uuid()
    {
        // Serial Port Service UUID
        return "00001101-0000-1000-8000-00805F9B34FB";
    }

private:
    const gchar* _path;
};

int main()
{
    GMainLoop* loop = NULL;
    GError* error;
    GDBusProxy* manager;

    // 创建 Profile


    // 向 ProfileManager 注册 Profile
    manager = g_dbus_proxy_new_for_bus_sync(G_BUS_TYPE_SYSTEM, G_DBUS_PROXY_FLAGS_NONE, NULL,
        "org.bluez", "/org/bluez", "org.bluez.ProfileManager1",
        NULL, &error);
    if (manager == NULL) {
        g_printerr("cannor get ProfileManager: %s\n", error->message);
        g_error_free(error);
        goto out;
    }

    g_dbus_proxy_call_sync(manager, "RegisterProfile", g_variant_new("(osa{sv})", ));

    loop = g_main_loop_new(NULL, FALSE);
    g_main_loop_run(loop);

out:
    if (loop)
        g_main_loop_unref(loop);
    if (manager)
        g_object_unref(manager);
    return 0;
}