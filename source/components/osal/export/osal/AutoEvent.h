#pragma once

#include <chrono>
#include <memory>
#include <string>
#include "osal/exports.h"

namespace OSAL
{

using AutoEventHandle =  void *;

class OSAL_EXPORT AutoEvent
{
public:
    using Ptr = std::shared_ptr<AutoEvent>;

    AutoEvent();
    AutoEvent(const AutoEvent & event);
    AutoEvent(AutoEventHandle handle);
    virtual ~AutoEvent();
    bool Get();
    void Set();
    void Wait();
    bool Wait(const std::chrono::nanoseconds & timeout);

    AutoEvent &operator = (const AutoEvent & other);
    AutoEvent &operator = (AutoEventHandle handle);
    operator AutoEventHandle () const;
    friend bool operator == (AutoEventHandle handle, AutoEvent & event);
    friend bool operator == (AutoEvent & event, AutoEventHandle handle);
    friend bool operator != (AutoEventHandle handle, AutoEvent & event);
    friend bool operator != (AutoEvent & event, AutoEventHandle handle);

protected:
    AutoEventHandle _semaphore;
    bool _isAssigned;
    bool TryWait();
    void Create();
    void Assign(AutoEventHandle other);
    void Destroy();
};

bool operator == (AutoEventHandle handle, AutoEvent & event);
bool operator == (AutoEvent & event, AutoEventHandle handle);
bool operator != (AutoEventHandle handle, AutoEvent & event);
bool operator != (AutoEvent & event, AutoEventHandle handle);

} // namespace OSAL
