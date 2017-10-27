#ifndef UTIL_STATUS_TEST_MACROS_H_
#define UTIL_STATUS_TEST_MACROS_H_

#include "util/status.h"
#include "third_party/googletest/googletest/include/gtest/gtest.h"

#define EXPECT_OK(status) \
  EXPECT_TRUE((status).ok())

#define EXPECT_NOT_OK(status) \
  EXPECT_FALSE((status).ok())

#define EXPECT_STATUS(expr, msg, l, c) \
  do {                                 \
    const Status _status = (expr);    \
    EXPECT_EQ(msg, _status.message()); \
    EXPECT_EQ(l, _status.line());      \
    EXPECT_EQ(c, _status.column());    \
  } while(false)

#endif  // UTIL_STATUS_TEST_MACROS_H_
