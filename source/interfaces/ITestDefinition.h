//-------------------------------------------------------------------
// Copyright(c) 2014 Philips Medical Systems Nederland B.V.
//-------------------------------------------------------------------

#pragma once

#include <remotingts1.h>

#define REMOTINGTS1_DEFINE_INTERFACE
#include "ITest.hpp"
#undef REMOTINGTS1_DEFINE_INTERFACE
#define REMOTINGTS1_DEFINE_PROXY
#include "ITest.hpp"
#undef REMOTINGTS1_DEFINE_PROXY

const std::string ITestUrn = "urn:healthcare-philips-com:service:Test:1";
