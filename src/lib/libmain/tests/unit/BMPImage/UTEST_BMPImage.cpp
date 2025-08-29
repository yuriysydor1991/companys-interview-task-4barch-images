#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "src/lib/libmain/BMPImagse.h"

using namespace barchclib0;
using namespace lib0impl;
using namespace testing;

class UTEST_BMPImage : public Test
{
 public:
  UTEST_BMPImage() : image{BMPImage::create()} { EXPECT_NE(image, nullptr); }

  BMPImagePtr image;
};

TEST_F(UTEST_BMPImage, libcall_no_context_failure) { EXPECT_FALSE(true); }
