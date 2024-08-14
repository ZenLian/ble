#include "ble/api/Adapter.hpp"

#include "ble/AdapterPrivate.hpp"

namespace ble
{

Adapter::Adapter()
    : _p(new AdapterPrivate(this))
{

}

Adapter::~Adapter() = default;

std::string Adapter::path()
{
    return _p->path();
}

std::string Adapter::address()
{
    return _p->address();
}

std::string Adapter::name()
{
    return _p->name();
}

bool Adapter::isPowered()
{
    return _p->isPowered();
}

void Adapter::setPowered(bool powered)
{
    return _p->setPowered(powered);
}

} // namespace ble