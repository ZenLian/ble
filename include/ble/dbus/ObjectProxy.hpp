#include <gio/gio.h>

#include <map>
#include <string>
#include <memory>

namespace ble
{
    class ObjectProxy
    {
    public:
        ObjectProxy(GDBusObjectProxy *proxy);
        virtual ~ObjectProxy();

    protected:
        GDBusObjectProxy *_proxy;
        std::map<std::string, std::shared_ptr<ObjectProxy>> _children;
    };
}