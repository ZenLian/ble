#pragma once

#include <memory>

namespace ble::gdbus
{
class ObjectProxy;
class InterfaceProxy;
class ObjectManager;

typedef std::shared_ptr<ObjectProxy> ObjectProxyPtr;
typedef std::shared_ptr<InterfaceProxy> InterfaceProxyPtr;
}