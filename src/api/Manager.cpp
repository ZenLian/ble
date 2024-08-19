#include "bt/api/Manager.hpp"

#include "bt/api/Adapter.hpp"
#include "bt/ManagerPrivate.hpp"

#include <algorithm>

using namespace bt;

Manager::Manager()
    : _p(new ManagerPrivate(this))
{
    printf("Manager created.\n");
}

Manager::~Manager() = default;

void Manager::Run()
{
    _p->run();
}

AdapterPtr Manager::GetDefaultAdapter() const
{
    return _p->_defaultAdapter;
}

std::vector<AdapterPtr> Manager::GetAdapters() const
{
    std::vector<AdapterPtr> result;
    for (auto it : _p->_adapters) {
        result.push_back(it.second);
    }
    return result;
}

AdapterPtr Manager::GetAdapterByAddress(const std::string& address) const
{
    for (auto it : _p->_adapters) {
        AdapterPtr adapter = it.second;
        if (adapter->address() == address) {
            return adapter;
        }
    }
    return AdapterPtr();
}

AdapterPtr Manager::GetAdapterByPath(const std::string& path) const
{
    for (auto it : _p->_adapters) {
        AdapterPtr adapter = it.second;
        if (adapter->path() == path) {
            return adapter;
        }
    }
    return AdapterPtr();
}