#pragma once

#include "ble/types.hpp"

#include <string>

namespace ble
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

private:
    std::unique_ptr<class AdapterPrivate> const _p;

    friend class AdapterPrivate;
    friend class ManagerPrivate;
};
} // namespace ble
