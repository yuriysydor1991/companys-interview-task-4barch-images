#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <cmath>
#include <string>

#include "src/lib/libmain/converters/Barch2BMPConverter0.h"
#include "src/lib/libmain/images/BarchImage.h"

using namespace barchclib0;
using namespace barchclib0::converters;
using namespace testing;

class UTEST_Barch2BMPConverter0 : public Test
{
 public:
  static constexpr const size_t testsReps = 5U;
  static constexpr const unsigned char two_two_five = 255U;

  UTEST_Barch2BMPConverter0() : conv{Barch2BMPConverter0::create()}
  {
    EXPECT_NE(conv, nullptr);
  }

  Barch2BMPConverter0Ptr conv;
};

TEST_F(UTEST_Barch2BMPConverter0, no_barch_image_provided_failure)
{
  EXPECT_EQ(conv->convert({}), nullptr);
}

TEST_F(UTEST_Barch2BMPConverter0, zero_width_barch_image_provided_failure)
{
  auto barch = BarchImage::create();
  EXPECT_NE(barch, nullptr);

  EXPECT_EQ(barch->width(), 0);

  EXPECT_EQ(conv->convert(barch), nullptr);
}

TEST_F(UTEST_Barch2BMPConverter0, zero_height_barch_image_provided_failure)
{
  auto barch = BarchImage::create();
  EXPECT_NE(barch, nullptr);

  barch->width(1);

  EXPECT_EQ(barch->height(), 0);

  EXPECT_EQ(conv->convert(barch), nullptr);
}

TEST_F(UTEST_Barch2BMPConverter0, empty_data_barch_image_provided_failure)
{
  auto barch = BarchImage::create();
  EXPECT_NE(barch, nullptr);

  barch->width(1);
  barch->height(1);

  EXPECT_TRUE(barch->data().empty());

  EXPECT_EQ(conv->convert(barch), nullptr);
}

TEST_F(UTEST_Barch2BMPConverter0,
       invalid_bits_per_pixel_barch_image_provided_exception_failure)
{
  auto barch = BarchImage::create();
  EXPECT_NE(barch, nullptr);

  barch->width(1);
  barch->height(1);

  barchdata zeros(barch->width(), static_cast<unsigned char>(0));

  barch->data(zeros);

  barch->bits_per_pixel(123U);

  EXPECT_THROW({ auto unused = conv->convert(barch); }, std::runtime_error);
}
