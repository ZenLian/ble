#pragma once

#include <memory>

namespace bt::glib
{
class ObjectProxy;
class InterfaceProxy;
class ObjectManager;

typedef std::shared_ptr<ObjectProxy> ObjectProxyPtr;
typedef std::shared_ptr<InterfaceProxy> InterfaceProxyPtr;
}