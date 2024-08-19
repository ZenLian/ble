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

    std::string path();

    std::string address();
    std::string name();
    bool isPowered();
    void setPowered(bool powered);

    /** 事件回调 */
    std::function<void(DevicePtr)> OnDeviceAdded;
    std::function<void(DevicePtr)> OnDeviceRemoved;


private:
    std::unique_ptr<class AdapterPrivate> const _p;

    friend class AdapterPrivate;
    friend class ManagerPrivate;
};
} // namespace bt
