#pragma once

#include <memory>
#include <string>
#include "database/exports.h"

namespace Database
{

class DATABASE_EXPORT RecordSet
{
public:
    typedef std::shared_ptr<RecordSet> Ptr;

    RecordSet() {}
    virtual ~RecordSet() {}

};

} // namespace Database
