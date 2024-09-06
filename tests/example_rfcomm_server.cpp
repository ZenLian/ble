#include <gio/gio.h>
#include <stdexcept>

class SerialPortProfile
{
public:
    SerialPortProfile(GDBusConnection* bus, const gchar* path);
    ~SerialPortProfile();

    const gchar* path() const;

    const gchar* uuid() const;

private:
    const gchar* _path;
};

static void handle_method_call(GDBusConnection* bus,
    const gchar* sender,
    const gchar* object_path,
    const gchar* interface_name,
    const gchar* method_name,
    GVariant* parameters,
    GDBusMethodInvocation* invocation,
    gpointer user_data)
{
    SerialPortProfile* profile = (SerialPortProfile*)user_data;
    g_print("method call: %s->%s:%s.%s ...\n", sender, object_path, interface_name, method_name);

    if (g_str_equal(method_name, "Release")) {
        // TODO:
        g_print("[Release]\n");
        g_dbus_method_invocation_return_value(invocation, NULL);
    }
    if (g_str_equal(method_name, "Cancel")) {
        // TODO:
        g_print("[Cancel]\n");
        g_dbus_method_invocation_return_value(invocation, NULL);
    }
    else if (g_str_equal(method_name, "NewConnection"))
    {
        gchar* s = g_variant_print(parameters, true);
        g_print("[NewConnection]: %s\n", s);
        g_free(s);

        const gchar* device;
        gint32 fd;
        g_variant_get(parameters, "(&oha{sv})", &device, &fd, NULL);
        g_print("device: %s, fd: %d\n", device, fd);
        const gchar* greeting = "hello!";
        write(fd, greeting, strlen(greeting));
        g_dbus_method_invocation_return_value(invocation, NULL);
    }
    else if (g_str_equal(method_name, "RequestDisconnection")) {
        // TODO:
        g_print("[RequestDisconnection]\n");
        g_dbus_method_invocation_return_value(invocation, NULL);
    }
}

static const GDBusInterfaceVTable interface_vtable =
{
    .method_call = handle_method_call,
    .get_property = NULL,
    .set_property = NULL
};

static const gchar* introspection_xml =
"<node>"
"  <interface name='org.bluez.Profile1'>"
"    <method name='Release'/>"
"    <method name='Cancel'/>"
"    <method name='NewConnection'>"
"      <arg type='o' name='device' direction='in'/>"
"      <arg type='h' name='fd' direction='in'/>"
"      <arg type='a{sv}' name='properties' direction='in'/>"
"    </method>"
"    <method name='RequestDisconnection'>"
"      <arg type='o' name='device' direction='in'/>"
"    </method>"
"  </interface>"
"</node>";

SerialPortProfile::SerialPortProfile(GDBusConnection* bus, const gchar* path)
{
    GError* error = NULL;
    GDBusNodeInfo* node_info;

    _path = path;

    node_info = g_dbus_node_info_new_for_xml(introspection_xml, &error);
    if (node_info == NULL) {
        g_printerr("get node info failed: %s\n", error->message);
        g_error_free(error);
        throw std::runtime_error("");
    }

    g_print("register object...\n");
    guint reg_id = g_dbus_connection_register_object(
        bus,
        path,
        node_info->interfaces[0],
        &interface_vtable,
        this,
        NULL,
        &error);
    g_assert(reg_id > 0);
    if (reg_id <= 0) {
        g_printerr("register object failed: %s\n", error->message);
        g_error_free(error);
    }
    g_print("register done.\n");

    g_dbus_node_info_unref(node_info);
}

SerialPortProfile::~SerialPortProfile()
{
}

const gchar* SerialPortProfile::path() const
{
    return _path;
}

const gchar* SerialPortProfile::uuid() const
{
    // Serial Port Service UUID
    return "00001101-0000-1000-8000-00805F9B34FB";
}

int main()
{
    GMainLoop* loop = NULL;
    GError* error = NULL;
    GDBusProxy* manager;
    SerialPortProfile* profile;
    GVariant* options;
    GVariantBuilder builder;
    GVariant* params;

    // ProfileManager
    g_print("Get ProfileManager Proxy...\n");
    manager = g_dbus_proxy_new_for_bus_sync(G_BUS_TYPE_SYSTEM, G_DBUS_PROXY_FLAGS_NONE, NULL,
        "org.bluez", "/org/bluez", "org.bluez.ProfileManager1",
        NULL, &error);

    if (manager == NULL) {
        g_printerr("get ProfileManager failed: %s\n", error->message);
        g_error_free(error);
        goto out;
    }

    // 创建 Profile
    g_print("new SerialPortProfile...\n");
    profile = new SerialPortProfile(g_dbus_proxy_get_connection(manager), "/my/spp");

    g_print("fill options...\n");
    g_variant_builder_init(&builder, G_VARIANT_TYPE("a{sv}"));
    g_print("Autoconnect...\n");
    g_variant_builder_add(&builder, "{sv}", "AutoConnect", g_variant_new("b", true));
    g_print("Channel...\n");
    g_variant_builder_add(&builder, "{sv}", "Channel", g_variant_new("q", 0));
    g_print("Role...\n");
    g_variant_builder_add(&builder, "{sv}", "Role", g_variant_new("s", "server"));
    options = g_variant_builder_end(&builder);
    g_assert(options != NULL);

    g_print("path: %s\n", profile->path());
    g_print("build params...\n");
    // params = g_variant_new("(osa{sv})", "/my/spp", profile->uuid(), options);
    params = g_variant_new_parsed("(%o, %s, "
        "{'AutoConnect': <false>,"
        "'Channel': <%q>, "
        "'Role': <'server'>})",
        profile->path(),
        profile->uuid(),
        0);
    g_print("call RegisterProfile...\n");
    g_dbus_proxy_call_sync(
        manager,
        "RegisterProfile",
        params,
        G_DBUS_CALL_FLAGS_NONE,
        -1,
        NULL,
        &error);

    if (error != NULL) {
        g_printerr("RegisterProfile failed: %s\n", error->message);
        g_error_free(error);
        goto out;
    }

    loop = g_main_loop_new(NULL, FALSE);

    g_print("running mainloop...\n");
    g_main_loop_run(loop);

out:
    if (loop)
        g_main_loop_unref(loop);
    if (manager)
        g_object_unref(manager);
    return 0;
}