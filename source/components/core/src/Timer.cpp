#include "core/Timer.h"

#include <iomanip>
#include <sstream>
#include <osal/osal.h>

using namespace std;
using namespace Core;

Timer::Timer()
    : _interval()
    , _initialRepeatCount()
    , _currentRepeatCount()
    , _callback()
{
}

Timer::~Timer()
{
}

void Timer::StopTimer()
{

}

