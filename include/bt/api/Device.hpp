#pragma once

#include "bt/types.hpp"

#include <string>
#include <vector>

namespace bt
{
class Device
{
public:
    Device();
    ~Device();

    std::string address();
    std::string name();
    void setName(const std::string &name);
    std::vector<std::string> uuids();
};
} // namespace bt
