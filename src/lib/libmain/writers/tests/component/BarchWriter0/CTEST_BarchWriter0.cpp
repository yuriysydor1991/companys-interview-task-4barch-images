#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <bitset>
#include <cmath>
#include <string>

#include "src/lib/libmain/images/BarchImage.h"
#include "src/lib/libmain/writers/BarchWriter0.h"

using namespace barchclib0;
using namespace barchclib0::writers;
using namespace testing;

class CTEST_BarchWriter0 : public Test
{
 public:
  CTEST_BarchWriter0() : writer{BarchWriter0::create()}
  {
    EXPECT_NE(writer, nullptr);
  }

  BarchWriter0Ptr writer;
};

TEST_F(CTEST_BarchWriter0, no_barch_image_provided_failure)
{
  EXPECT_FALSE(writer->write({}));
}
