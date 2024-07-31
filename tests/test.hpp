#pragma once

#include <cassert>

#define assertm(exp, msg) assert(((void)msg, exp))

#define EXPECT_EQ(a, b) assert((a) == (b))
#define EXPECT_TRUE(a) EXPECT_EQ(a, true)
#define EXPECT_FALSE(a) EXPECT_EQ(a, false)