#pragma once

#include "ble/dbus/InterfaceProxy.hpp"

#include <gio/gio.h>

#include <map>
#include <string>
#include <memory>
#include <mutex>

namespace ble
{
    class ObjectProxy
    {
    public:
        ObjectProxy(const std::string &path);
        ObjectProxy(GDBusObjectProxy *proxy);
        virtual ~ObjectProxy();

        void InitProxy(GDBusObjectProxy *proxy);
        void AddChild(std::shared_ptr<ObjectProxy> proxy);

        std::string GetObjectPath();

    protected:
        void onChildCreated(const std::string &path);

        // 封装的 GDBusObjectProxy
        GDBusObjectProxy *_proxy;
        // Object 路径
        std::string _path;
        // 包含的接口
        std::map<std::string, std::shared_ptr<InterfaceProxy>> _interfaces;
        // 子节点
        std::map<std::string, std::shared_ptr<ObjectProxy>> _children;
        std::mutex _children_mutex;
    };
}