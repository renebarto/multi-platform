#pragma once

#include <memory>
#include <vector>

namespace Network
{

class Adapter;
using AdapterPtr = std::shared_ptr<Adapter>;

class AdapterList
{
public:
    AdapterList();

    void ReScan();

private:
    std::vector<AdapterPtr> _adapters;
};

} // namespace Network
