#include "bt/api/Manager.hpp"

#include "bt/api/Adapter.hpp"
#include "bt/ManagerPrivate.hpp"

#include <algorithm>

using namespace bt;

Manager::Manager()
    : _impl(new ManagerPrivate(this))
{
    printf("Manager created.\n");
}

Manager::~Manager() = default;

void Manager::run()
{
    _impl->run();
}

AdapterPtr Manager::getDefaultAdapter() const
{
    return _impl->_defaultAdapter;
}

std::vector<AdapterPtr> Manager::getAdapters() const
{
    std::vector<AdapterPtr> result;
    for (auto it : _impl->_adapters) {
        result.push_back(it.second);
    }
    return result;
}

AdapterPtr Manager::getAdapterByAddress(const std::string& address) const
{
    for (auto it : _impl->_adapters) {
        AdapterPtr adapter = it.second;
        if (adapter->address() == address) {
            return adapter;
        }
    }
    return AdapterPtr();
}

AdapterPtr Manager::getAdapterByPath(const std::string& path) const
{
    for (auto it : _impl->_adapters) {
        AdapterPtr adapter = it.second;
        if (adapter->path() == path) {
            return adapter;
        }
    }
    return AdapterPtr();
}