#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "src/lib/libmain/images/BMPImage.h"

using namespace barchclib0;
using namespace testing;

class UTEST_BMPImage : public Test
{
 public:
  UTEST_BMPImage() : bmp{BMPImage::create()} {
    EXPECT_NE(bmp, nullptr);
  }

  BMPImagePtr bmp;
};

TEST_F(UTEST_BMPImage, bmpimage_first)
{
  EXPECT_FALSE(true);
}

