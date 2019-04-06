//-------------------------------------------------------------------
// Copyright(c) 2014 Philips Medical Systems Nederland B.V.
//-------------------------------------------------------------------

#pragma once

#include <sscfts1.h>

#define SSCFTS1_DECLARE_INTERFACE
#include "ITest.hpp"
#undef SSCFTS1_DECLARE_INTERFACE
#define SSCFTS1_DECLARE_PROXY
#include "ITest.hpp"
#undef SSCFTS1_DECLARE_PROXY

extern const std::string ITestUrn;
