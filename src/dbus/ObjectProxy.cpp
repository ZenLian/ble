#include "ble/dbus/ObjectProxy.hpp"

#include "ble/dbus/PathUtils.hpp"

#include <algorithm>

using namespace ble;

ObjectProxy::ObjectProxy(const std::string &path)
    : _path(path)
{
    // 空结点，没有对应 _proxy，但可以添加子结点
}

ObjectProxy::ObjectProxy(GDBusObjectProxy *proxy)
{
    InitProxy(proxy);
}

ObjectProxy::~ObjectProxy()
{
    if (_proxy != nullptr)
        g_object_unref(_proxy);
}

void ObjectProxy::InitProxy(GDBusObjectProxy *proxy)
{
    _proxy = proxy;
    g_object_ref(_proxy);

    _path = g_dbus_object_get_object_path(G_DBUS_OBJECT(_proxy));
    g_print("[InitProxy] path: %s\n", _path.c_str());

    // 获取所有接口
    g_print("[InitProxy] getting interfaces...\n");
    GList *interfaces = g_dbus_object_get_interfaces(G_DBUS_OBJECT(_proxy));
    for (GList *item = interfaces; item != NULL; item = item->next)
    {
        GDBusProxy *interface = G_DBUS_PROXY(item->data);
        std::string name(g_dbus_proxy_get_interface_name(interface));
        g_print("- %s\n", name.c_str());
        _interfaces[name] = std::make_shared<InterfaceProxy>(interface);
    }
    g_list_free_full(interfaces, g_object_unref);
    g_print("[InitProxy] over.\n");
}

void ObjectProxy::AddChild(std::shared_ptr<ObjectProxy> proxy)
{
    std::string path(proxy->_path);
    // 不属于本结点，不予处理
    if (!PathUtils::IsDescendantOf(path, _path))
    {
        g_print("skip %s\n", path.c_str());
        return;
    }

    // 已存在子结点，重新初始化 proxy
    if (_children.find(path) != _children.end())
    {
        g_print("InitProxy for %s\n", path.c_str());
        _children[path]->InitProxy(proxy->_proxy);
        return;
    }

    // 是直接子结点，创建子结点并用 proxy 初始化
    if (PathUtils::IsChildOf(path, this->_path))
    {
        g_print("new child %s\n", path.c_str());
        _children[path] = proxy;
        onChildCreated(path);
        return;
    }

    // 不是直接子结点，递归给子结点处理
    // 寻找可以处理这个路径的子结点
    auto iter = std::find_if(_children.begin(), _children.end(),
                             [&path](const std::pair<std::string, std::shared_ptr<ObjectProxy>> &child_data) -> bool
                             {
                                 return PathUtils::IsDescendantOf(path, child_data.first);
                             });
    // 递归给子结点处理
    if (iter != _children.end())
    {
        g_print("forwarding to child: %s\n", iter->first.c_str());
        iter->second->AddChild(proxy);
    }
    // 子结点还不存在，先创建空的子结点，再递归给子结点处理
    else
    {
        std::string child_path = PathUtils::NextChild(_path, path);
        g_print("creating empty child and forward: %s\n", child_path.c_str());
        auto child = std::make_shared<ObjectProxy>(child_path);
        _children[child_path] = child;
        onChildCreated(child_path);
        child->AddChild(proxy);
    }
}

void ObjectProxy::onChildCreated(const std::string &path)
{
    g_print("[NEW] %s\n", path.c_str());
}