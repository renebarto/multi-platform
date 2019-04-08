//-------------------------------------------------------------------
// Copyright(c) 2014 Philips Medical Systems Nederland B.V.
//-------------------------------------------------------------------

#pragma once

#include <remotingts1.h>

#define REMOTINGTS1_DECLARE_INTERFACE
#include "ITest.hpp"
#undef REMOTINGTS1_DECLARE_INTERFACE
#define REMOTINGTS1_DECLARE_PROXY
#include "ITest.hpp"
#undef REMOTINGTS1_DECLARE_PROXY

extern const std::string ITestUrn;
