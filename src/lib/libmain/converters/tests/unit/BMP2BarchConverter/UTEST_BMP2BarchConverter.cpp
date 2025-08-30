#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <string>

#include "src/lib/libmain/converters/BMP2BarchConverter.h"
#include "src/lib/libmain/images/BMPImage.h"
#include "src/lib/libmain/images/BarchImage.h"

using namespace barchclib0;
using namespace barchclib0::converters;
using namespace testing;

class UTEST_BMP2BarchConverter : public Test
{
 public:
  static constexpr const size_t testsReps = 100U;

  UTEST_BMP2BarchConverter() : conv{BMP2BarchConverter::create()}
  {
    EXPECT_NE(conv, nullptr);
  }

  BMP2BarchConverterPtr conv;
};

TEST_F(UTEST_BMP2BarchConverter, no_bmp_image_provided_failure)
{
  EXPECT_EQ(conv->convert({}), nullptr);
}

TEST_F(UTEST_BMP2BarchConverter, zero_width_bmp_image_provided_failure)
{
  auto bmp = BMPImage::create();
  EXPECT_NE(bmp, nullptr);

  EXPECT_EQ(bmp->width(), 0);

  EXPECT_EQ(conv->convert(bmp), nullptr);
}

TEST_F(UTEST_BMP2BarchConverter, zero_height_bmp_image_provided_failure)
{
  auto bmp = BMPImage::create();
  EXPECT_NE(bmp, nullptr);

  bmp->width(1);

  EXPECT_EQ(bmp->height(), 0);

  EXPECT_EQ(conv->convert(bmp), nullptr);
}

TEST_F(UTEST_BMP2BarchConverter, empty_data_bmp_image_provided_failure)
{
  auto bmp = BMPImage::create();
  EXPECT_NE(bmp, nullptr);

  bmp->width(1);
  bmp->height(1);

  EXPECT_TRUE(bmp->data().empty());

  EXPECT_EQ(conv->convert(bmp), nullptr);
}

TEST_F(UTEST_BMP2BarchConverter,
       invalid_bits_per_pixel_bmp_image_provided_exception_failure)
{
  auto bmp = BMPImage::create();
  EXPECT_NE(bmp, nullptr);

  bmp->width(1);
  bmp->height(1);

  barchdata zeros(bmp->width(), static_cast<unsigned char>(0));

  bmp->data(zeros);

  bmp->bits_per_pixel(123U);

  EXPECT_THROW({ auto unused = conv->convert(bmp); }, std::runtime_error);
}

TEST_F(UTEST_BMP2BarchConverter, single_row_arbitrary_image_convert_no_convert_success)
{
  for (unsigned char cval = 0U; cval < 255U; ++cval) {
    for (size_t titer = 1U; titer < testsReps; ++titer) {
      auto bmp = BMPImage::create();
      EXPECT_NE(bmp, nullptr);

      bmp->width(titer);
      bmp->height(1);

      barchdata zeros(bmp->width(), cval);

      bmp->data(zeros);

      auto barch = conv->convert(bmp);

      EXPECT_NE(barch, nullptr);

      auto linest = barch->lines_table();

      EXPECT_EQ(linest.size(), 1);

      EXPECT_FALSE(linest[0U]);

      auto bd = barch->data();

      EXPECT_EQ(bd.size(), zeros.size());

      for (size_t liter = 0U; liter < bd.size(); ++liter) {
        EXPECT_EQ(bd[liter], cval);
      }
    }
  }
}
