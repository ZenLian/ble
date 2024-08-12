#include "ble/api/Manager.hpp"

#include "ble/ManagerPrivate.hpp"

using namespace ble;

Manager::Manager()
    : _p(new ManagerPrivate(this))
{
    
}

Manager::~Manager() = default;

void Manager::Init()
{
    _p->Init();
}

AdapterPtr Manager::GetDefaultAdapter() const
{
    return _p->_defaultAdapter;
}

std::vector<AdapterPtr> Manager::GetAdapters() const
{
    return _p->_adapters;
}

AdapterPtr Manager::GetAdapterByAddress(const std::string &address) const
{
    for (AdapterPtr adapter : _p->_adapters) {
        if (adapter->address() == address) {
            return adapter;
        }
    }
    return AdapterPtr();
}

AdapterPtr Manager::GetAdapterByPath(const std::string &path) const
{
    for (AdapterPtr adapter : _p->_adapters) {
        if (adapter->path() == path) {
            return adapter;
        }
    }
    return AdapterPtr();
}