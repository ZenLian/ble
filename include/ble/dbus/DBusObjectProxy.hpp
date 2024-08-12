#pragma once

#include "ble/dbus/DBusInterfaceProxy.hpp"

#include <gio/gio.h>

#include <map>
#include <memory>
#include <mutex>
#include <string>

namespace ble
{
    class DBusObjectProxy
    {
    public:
        DBusObjectProxy(const std::string &path);
        DBusObjectProxy(GDBusObjectProxy *proxy);
        virtual ~DBusObjectProxy();

        void AddChild(std::shared_ptr<DBusObjectProxy> proxy);

        std::string GetObjectPath();

        void Print();

    protected:
        void loadProxy(GDBusObjectProxy *proxy);
        void unloadProxy();
        void onChildCreated(const std::string &path);

        // 封装的 GDBusObjectProxy
        GDBusObjectProxy *_proxy;
        // Object 路径
        std::string _path;
        // 包含的接口
        std::map<std::string, std::shared_ptr<DBusInterfaceProxy>> _interfaces;
        // 子节点
        std::map<std::string, std::shared_ptr<DBusObjectProxy>> _children;
        std::mutex _children_mutex;
    };

    typedef std::shared_ptr<DBusObjectProxy> DBusObjectProxyPtr;
}