#include "bt/api/Adapter.hpp"

#include "bt/AdapterPrivate.hpp"

namespace bt
{

Adapter::Adapter()
    : _impl(new AdapterPrivate(this))
{

}

Adapter::~Adapter() = default;

std::string Adapter::path() const
{
    return _impl->path();
}

std::string Adapter::address() const
{
    return _impl->address();
}

std::string Adapter::name() const
{
    return _impl->alias();
}

void Adapter::setName(const std::string& name)
{
    return _impl->alias(name);
}

bool Adapter::isPowered() const
{
    return _impl->powered();
}

void Adapter::setPowered(bool powered)
{
    return _impl->powered(powered);
}

std::vector<DevicePtr> Adapter::getDevices() const
{
    std::vector<DevicePtr> result;
    for (auto it : _impl->_devices) {
        result.push_back(it.second);
    }
    return result;
}

} // namespace bt