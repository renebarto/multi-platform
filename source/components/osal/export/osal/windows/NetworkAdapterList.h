#pragma once

#include <memory>
#include <vector>
#include "osal/exports.h"

namespace OSAL {
namespace Network {

class Adapter;

using AdapterPtr = std::shared_ptr<Adapter>;

class AdapterList
{
public:
    AdapterList();

    bool ReScan();

    const std::vector<AdapterPtr> & GetAdapters()
    { return _adapters; }

private:
    std::vector<AdapterPtr> _adapters;
};

} // namespace Network
} // namespace OSAL