//-------------------------------------------------------------------
// Copyright(c) 2014 Philips Medical Systems Nederland B.V.
//-------------------------------------------------------------------

#pragma once

#include <sscfts1.h>

#define SSCFTS1_DEFINE_INTERFACE
#include "ITest.hpp"
#undef SSCFTS1_DEFINE_INTERFACE
#define SSCFTS1_DEFINE_PROXY
#include "ITest.hpp"
#undef SSCFTS1_DEFINE_PROXY

const std::string ITestUrn = "urn:healthcare-philips-com:service:Test:1";
