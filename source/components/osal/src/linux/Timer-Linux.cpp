#include "osal/Timer.h"

using namespace std;
using namespace OSAL;
using namespace Time;

std::map<timer_t *, Timer *> Timer::_timerMap;
