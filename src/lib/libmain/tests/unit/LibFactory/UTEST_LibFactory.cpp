#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "src/lib/libmain/LibFactory.h"

using namespace lib0impl;
using namespace testing;

class UTEST_LibFactory : public Test
{
 public:
  UTEST_LibFactory() : factory{LibFactory::create_factory()}
  {
    EXPECT_NE(factory, nullptr);
  }

  std::shared_ptr<LibFactory> factory;
};

TEST_F(UTEST_LibFactory, create_default_lib_success)
{
  EXPECT_NE(factory->create_default_lib(), nullptr);
}
