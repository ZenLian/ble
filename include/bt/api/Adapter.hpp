#pragma once

#include "bt/types.hpp"

#include <string>
#include <functional>

namespace bt
{
class Adapter
{
public:
    Adapter();
    virtual ~Adapter();

    std::string path() const;

    std::string address() const;
    std::string name() const;
    void setName(const std::string& name);
    bool isPowered() const;
    void setPowered(bool powered);

    std::vector<DevicePtr> getDevices() const;

    /** 事件回调 */
    std::function<void(DevicePtr)> OnDeviceAdded;
    std::function<void(DevicePtr)> OnDeviceRemoved;

private:
    std::unique_ptr<class AdapterPrivate> const _impl;

    friend class AdapterPrivate;
    friend class ManagerPrivate;
};
} // namespace bt
