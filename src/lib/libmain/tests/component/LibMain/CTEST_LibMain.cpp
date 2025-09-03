#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "LibraryContext.h"
#include "src/lib/libmain/LibMain.h"

using namespace barchclib0;
using namespace lib0impl;
using namespace testing;

class CTEST_LibMain : public Test
{
 public:
  CTEST_LibMain() : libmain{std::make_shared<LibMain>()}
  {
    EXPECT_NE(libmain, nullptr);
  }

  std::shared_ptr<LibMain> libmain;
};

TEST_F(CTEST_LibMain, dumm_test) { EXPECT_NE(libmain, nullptr); }

