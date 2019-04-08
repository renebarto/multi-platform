/*-----------------------------------------------------------------------------
 | Copyright: Copyright(c) 2012, Philips Medical Systems Nederland B.V.       |
 | Author:    A. Admiraal, iXR Innovation                                     |
  -----------------------------------------------------------------------------*/
#ifndef REMOTINGTS1_H
#define REMOTINGTS1_H

// Use both include guard and pragma once as pragma once is not 100% safe, but
// does inprove compilation speed.
#pragma once

#include "remoting1.h"

#include "remotingts1/bulkdata.h"
#include "remotingts1/clientnode.h"
#include "remotingts1/dispatcher.h"
#include "remotingts1/event.h"
#include "remotingts1/eventloop.h"
#include "remotingts1/formatter.h"
#include "remotingts1/functiondispatcher.h"
#include "remotingts1/functionproxy.h"
#include "remotingts1/servernode.h"

#ifdef _WIN32
# include "remotingts1/windowseventloop.h"
#endif

#include <memory>

#endif
