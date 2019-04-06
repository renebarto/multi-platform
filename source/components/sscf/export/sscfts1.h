/*-----------------------------------------------------------------------------
 | Copyright: Copyright(c) 2012, Philips Medical Systems Nederland B.V.       |
 | Author:    A. Admiraal, iXR Innovation                                     |
  -----------------------------------------------------------------------------*/
#ifndef SSCFTS1_H
#define SSCFTS1_H

// Use both include guard and pragma once as pragma once is not 100% safe, but
// does inprove compilation speed.
#pragma once

#include "sscf1.h"

#include "sscfts1/bulkdata.h"
#include "sscfts1/clientnode.h"
#include "sscfts1/dispatcher.h"
#include "sscfts1/event.h"
#include "sscfts1/eventloop.h"
#include "sscfts1/formatter.h"
#include "sscfts1/functiondispatcher.h"
#include "sscfts1/functionproxy.h"
#include "sscfts1/servernode.h"

#ifdef _WIN32
# include "sscfts1/windowseventloop.h"
#endif

#include <memory>

#endif
