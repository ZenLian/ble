#pragma once

#include <memory>

namespace bt
{
class Manager;
class Adapter;
class Device;

typedef std::shared_ptr<Manager> ManagerPtr;
typedef std::shared_ptr<Adapter> AdapterPtr;
typedef std::shared_ptr<Device> DevicePtr;
} // namespace bt
