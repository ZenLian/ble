#include "bt/gdbus/ObjectManager.hpp"

#include "bt/gdbus/ObjectProxy.hpp"
#include "bt/gdbus/InterfaceProxy.hpp"

#include <stdexcept>

using namespace bt::glib;

namespace
{
void on_object_added(
    GDBusObjectManager* self,
    GDBusObject* object,
    gpointer user_data)
{
    ObjectManager* manager = static_cast<ObjectManager*>(user_data);
    manager->handleObjectAdded(object);
}

void on_object_removed(
    GDBusObjectManager* self,
    GDBusObject* object,
    gpointer user_data)
{
    ObjectManager* manager = static_cast<ObjectManager*>(user_data);
    manager->handleObjectRemoved(object);
}

void on_interface_added(
    GDBusObjectManager* self,
    GDBusObject* object,
    GDBusInterface* interface,
    gpointer user_data
)
{
    ObjectManager* manager = static_cast<ObjectManager*>(user_data);
    manager->handleInterfaceAdded(interface);
}

void on_interface_removed(
    GDBusObjectManager* self,
    GDBusObject* object,
    GDBusInterface* interface,
    gpointer user_data
)
{
    ObjectManager* manager = static_cast<ObjectManager*>(user_data);
    manager->handleInterfaceRemoved(interface);
}

void on_interface_proxy_properties_changed(
    GDBusObjectManagerClient* self,
    GDBusObjectProxy* object_proxy,
    GDBusProxy* interface_proxy,
    GVariant* changed_properties,
    char** invalidated_properties,
    gpointer user_data)
{
    ObjectManager* manager = static_cast<ObjectManager*>(user_data);
    manager->handleInterfaceProxyPropertiesChanged(interface_proxy, changed_properties, invalidated_properties);
}

}

ObjectManager::ObjectManager(const std::string& name, const std::string& path)
{
    GError* error = NULL;

    g_print("Creating ObjectManager...\n");
    _manager = g_dbus_object_manager_client_new_for_bus_sync(
        G_BUS_TYPE_SYSTEM,
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

    g_print("Connecting g_signals...\n");
    g_signal_connect(_manager, "object-added", G_CALLBACK(on_object_added), this);
    g_signal_connect(_manager, "object-removed", G_CALLBACK(on_object_removed), this);
    g_signal_connect(_manager, "interface-added", G_CALLBACK(on_interface_added), this);
    g_signal_connect(_manager, "interface-removed", G_CALLBACK(on_interface_removed), this);
    g_signal_connect(_manager, "interface-proxy-properties-changed", G_CALLBACK(on_interface_proxy_properties_changed), this);

}

ObjectManager::~ObjectManager()
{
    g_object_unref(_manager);
}

std::vector<ObjectProxyPtr> ObjectManager::GetManagedObjects()
{
    std::vector<ObjectProxyPtr> result;
    g_print("GetManagedObjects at %s\n", g_dbus_object_manager_get_object_path(_manager));

    GList* objs;
    objs = g_dbus_object_manager_get_objects(_manager);
    for (GList* obj_item = objs; obj_item != NULL; obj_item = obj_item->next)
    {
        GDBusObject* obj = (GDBusObject*)(obj_item->data);
        // g_print("adding object: %s\n", g_dbus_object_get_object_path(obj));
        result.emplace_back(std::make_shared<ObjectProxy>(G_DBUS_OBJECT_PROXY(obj)));
    }
    g_list_free_full(objs, g_object_unref);
    return result;
}

void ObjectManager::handleObjectAdded(GDBusObject* object)
{
    g_print("[NEW] %s\n", g_dbus_object_get_object_path(object));
    if (OnObjectAdded) {
        OnObjectAdded(std::make_shared<ObjectProxy>(G_DBUS_OBJECT_PROXY(object)));
    }
}

void ObjectManager::handleObjectRemoved(GDBusObject* object)
{
    g_print("[DEL] %s\n", g_dbus_object_get_object_path(object));
    if (OnObjectRemoved) {
        OnObjectRemoved(std::make_shared<ObjectProxy>(G_DBUS_OBJECT_PROXY(object)));
    }
}

void ObjectManager::handleInterfaceAdded(GDBusInterface* interface)
{
    if (OnInterfaceAdded) {
        OnInterfaceAdded(std::make_shared<InterfaceProxy>(G_DBUS_PROXY(interface)));
    }
}

void ObjectManager::handleInterfaceRemoved(GDBusInterface* interface)
{
    if (OnInterfaceRemoved) {
        OnInterfaceRemoved(std::make_shared<InterfaceProxy>(G_DBUS_PROXY(interface)));
    }
}

void ObjectManager::handleInterfaceProxyPropertiesChanged(GDBusProxy* interface_proxy, GVariant* changed_properties,
    char** invalidated_properties)
{
    if (OnInterfaceProxyPropertiesChanged) {
        OnInterfaceProxyPropertiesChanged(std::make_shared<InterfaceProxy>(interface_proxy), changed_properties, invalidated_properties);
    }
}