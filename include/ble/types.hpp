#pragma once

#include <memory>

namespace ble
{
class Manager;
class Adapter;
class Device;

typedef std::shared_ptr<Manager> ManagerPtr;
typedef std::shared_ptr<Adapter> AdapterPtr;
typedef std::shared_ptr<Device> DevicePtr;
} // namespace ble
