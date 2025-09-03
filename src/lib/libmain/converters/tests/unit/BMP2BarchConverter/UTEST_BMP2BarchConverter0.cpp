#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <bitset>
#include <cmath>
#include <iostream>
#include <string>

#include "src/lib/libmain/converters/BMP2BarchConverter0.h"
#include "src/lib/libmain/images/BMPImage.h"
#include "src/lib/libmain/images/BarchImage.h"

using namespace barchclib0;
using namespace barchclib0::converters;
using namespace testing;

class UTEST_BMP2BarchConverter0 : public Test
{
 public:
  static constexpr const size_t testsReps = 5U;
  static constexpr const unsigned char two_two_five = 255U;
  static constexpr const unsigned char gray_pixel = two_two_five - 1U;
  static constexpr const unsigned char white_pixel = two_two_five;

  UTEST_BMP2BarchConverter0() : conv{BMP2BarchConverter0::create()}
  {
    EXPECT_NE(conv, nullptr);
  }

  BMP2BarchConverter0Ptr conv;
};

TEST_F(UTEST_BMP2BarchConverter0, no_bmp_image_provided_failure)
{
  EXPECT_EQ(conv->convert({}), nullptr);
}

TEST_F(UTEST_BMP2BarchConverter0, zero_width_bmp_image_provided_failure)
{
  auto bmp = BMPImage::create();
  EXPECT_NE(bmp, nullptr);

  EXPECT_EQ(bmp->width(), 0);

  EXPECT_EQ(conv->convert(bmp), nullptr);
}

TEST_F(UTEST_BMP2BarchConverter0, zero_height_bmp_image_provided_failure)
{
  auto bmp = BMPImage::create();
  EXPECT_NE(bmp, nullptr);

  bmp->width(1);

  EXPECT_EQ(bmp->height(), 0);

  EXPECT_EQ(conv->convert(bmp), nullptr);
}

TEST_F(UTEST_BMP2BarchConverter0, empty_data_bmp_image_provided_failure)
{
  auto bmp = BMPImage::create();
  EXPECT_NE(bmp, nullptr);

  bmp->width(1);
  bmp->height(1);

  EXPECT_TRUE(bmp->data().empty());

  EXPECT_EQ(conv->convert(bmp), nullptr);
}

TEST_F(UTEST_BMP2BarchConverter0,
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

TEST_F(UTEST_BMP2BarchConverter0,
       single_row_arbitrary_image_convert_no_convert_success)
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

TEST_F(UTEST_BMP2BarchConverter0,
       multi_row_arbitrary_image_convert_no_convert_success)
{
  for (unsigned char cval = 0U; cval < 255U; ++cval) {
    for (size_t width = 1U; width < testsReps; ++width) {
      for (size_t height = 1U; height < testsReps; ++height) {
        auto bmp = BMPImage::create();
        EXPECT_NE(bmp, nullptr);

        bmp->width(width);
        bmp->height(height);

        barchdata zeros(bmp->width() * bmp->height(), cval);

        bmp->data(zeros);

        auto barch = conv->convert(bmp);

        EXPECT_NE(barch, nullptr);

        auto linest = barch->lines_table();

        EXPECT_EQ(linest.size(), bmp->height());

        for (const auto& v : linest) {
          EXPECT_FALSE(v);
        }

        auto bd = barch->data();

        EXPECT_EQ(bd.size(), zeros.size());

        for (size_t liter = 0U; liter < bd.size(); ++liter) {
          EXPECT_EQ(bd[liter], cval);
        }

        for (size_t liter = 0U; liter < barch->height(); ++liter) {
          auto line = barch->line(liter);
          EXPECT_EQ(line.size(), bmp->width());
          for (size_t citer = 0U; citer < line.size(); ++citer) {
            EXPECT_EQ(line[citer], cval);
          }
        }
      }
    }
  }
}

TEST_F(UTEST_BMP2BarchConverter0,
       single_row_8_white_pixels_convert_image_success)
{
  auto bmp = BMPImage::create();
  EXPECT_NE(bmp, nullptr);

  bmp->width(8);
  bmp->height(1);

  barchdata whites(bmp->width(), two_two_five);

  bmp->data(whites);

  auto barch = conv->convert(bmp);

  EXPECT_NE(barch, nullptr);

  auto linest = barch->lines_table();

  EXPECT_EQ(linest.size(), 1);

  EXPECT_TRUE(linest[0U]);

  auto bd = barch->data();

  EXPECT_FALSE(bd.empty());
  EXPECT_EQ(bd.size(), 1U);
  EXPECT_EQ(bd[0], 0);
}

TEST_F(UTEST_BMP2BarchConverter0,
       single_row_4_whites_4_blacks_convert_image_success)
{
  auto bmp = BMPImage::create();
  EXPECT_NE(bmp, nullptr);

  bmp->width(8);
  bmp->height(1);

  barchdata whites(4, two_two_five);
  barchdata blacks(4, 0U);
  barchdata mixedp;

  mixedp.insert(mixedp.end(), whites.begin(), whites.end());
  mixedp.insert(mixedp.end(), blacks.begin(), blacks.end());

  bmp->data(mixedp);

  auto barch = conv->convert(bmp);

  EXPECT_NE(barch, nullptr);

  auto linest = barch->lines_table();

  EXPECT_EQ(linest.size(), 1);

  EXPECT_TRUE(linest[0U]);

  auto bd = barch->data();

  EXPECT_FALSE(bd.empty());
  EXPECT_EQ(bd.size(), 1U);
  EXPECT_EQ(bd[0], 0B01000000);
}

TEST_F(UTEST_BMP2BarchConverter0,
       single_row_4_blacks_4_whites_convert_image_success)
{
  auto bmp = BMPImage::create();
  EXPECT_NE(bmp, nullptr);

  bmp->width(8);
  bmp->height(1);

  barchdata whites(4, two_two_five);
  barchdata blacks(4, 0U);
  barchdata mixedp;

  mixedp.insert(mixedp.end(), blacks.begin(), blacks.end());
  mixedp.insert(mixedp.end(), whites.begin(), whites.end());

  bmp->data(mixedp);

  auto barch = conv->convert(bmp);

  EXPECT_NE(barch, nullptr);

  auto linest = barch->lines_table();

  EXPECT_EQ(linest.size(), 1);

  EXPECT_TRUE(linest[0U]);

  auto bd = barch->data();

  EXPECT_FALSE(bd.empty());
  EXPECT_EQ(bd.size(), 1U);
  EXPECT_EQ(bd[0], 0B10000000);
}

TEST_F(UTEST_BMP2BarchConverter0, single_row_8_blacks_convert_image_success)
{
  auto bmp = BMPImage::create();
  EXPECT_NE(bmp, nullptr);

  bmp->width(8);
  bmp->height(1);

  barchdata blacks(8U, 0U);

  bmp->data(blacks);

  auto barch = conv->convert(bmp);

  EXPECT_NE(barch, nullptr);

  auto linest = barch->lines_table();

  EXPECT_EQ(linest.size(), 1);

  EXPECT_TRUE(linest[0U]);

  auto bd = barch->data();

  EXPECT_FALSE(bd.empty());
  EXPECT_EQ(bd.size(), 1U);
  EXPECT_EQ(bd[0], 0B10100000);
}

TEST_F(UTEST_BMP2BarchConverter0, single_row_16_blacks_convert_image_success)
{
  auto bmp = BMPImage::create();
  EXPECT_NE(bmp, nullptr);

  bmp->width(16U);
  bmp->height(1U);

  barchdata blacks(16U, 0U);

  bmp->data(blacks);

  auto barch = conv->convert(bmp);

  EXPECT_NE(barch, nullptr);

  auto linest = barch->lines_table();

  EXPECT_EQ(linest.size(), 1);

  EXPECT_TRUE(linest[0U]);

  auto bd = barch->data();

  EXPECT_FALSE(bd.empty());
  EXPECT_EQ(bd.size(), 1U);
  EXPECT_EQ(bd[0], 0B10101010);
}

TEST_F(UTEST_BMP2BarchConverter0, single_row_15_blacks_convert_image_success)
{
  auto bmp = BMPImage::create();
  EXPECT_NE(bmp, nullptr);

  bmp->width(15U);
  bmp->height(1U);

  barchdata blacks(15U, 0U);

  bmp->data(blacks);

  auto barch = conv->convert(bmp);

  EXPECT_NE(barch, nullptr);

  auto linest = barch->lines_table();

  EXPECT_EQ(linest.size(), 1);

  EXPECT_TRUE(linest[0U]);

  auto bd = barch->data();

  EXPECT_FALSE(bd.empty());
  EXPECT_EQ(bd.size(), 4U);
  EXPECT_EQ(bd[0], 0B10101011);
  EXPECT_EQ(bd[1], 0B0);
  EXPECT_EQ(bd[2], 0B0);
  EXPECT_EQ(bd[3], 0B0);
}

TEST_F(UTEST_BMP2BarchConverter0, single_row_14_blacks_convert_image_success)
{
  auto bmp = BMPImage::create();
  EXPECT_NE(bmp, nullptr);

  bmp->width(14U);
  bmp->height(1U);

  barchdata blacks(14U, 0U);

  bmp->data(blacks);

  auto barch = conv->convert(bmp);

  EXPECT_NE(barch, nullptr);

  auto linest = barch->lines_table();

  EXPECT_EQ(linest.size(), 1);

  EXPECT_TRUE(linest[0U]);

  auto bd = barch->data();

  EXPECT_FALSE(bd.empty());
  EXPECT_EQ(bd.size(), 4U);
  EXPECT_EQ(bd[0], 0B10101011);
  EXPECT_EQ(bd[1], 0B0);
  EXPECT_EQ(bd[2], 0B0);
  EXPECT_EQ(bd[3], 0B0);
}

TEST_F(UTEST_BMP2BarchConverter0, single_row_13_blacks_convert_image_success)
{
  auto bmp = BMPImage::create();
  EXPECT_NE(bmp, nullptr);

  bmp->width(13U);
  bmp->height(1U);

  barchdata blacks(13U, 0U);

  bmp->data(blacks);

  auto barch = conv->convert(bmp);

  EXPECT_NE(barch, nullptr);

  auto linest = barch->lines_table();

  EXPECT_EQ(linest.size(), 1);

  EXPECT_TRUE(linest[0U]);

  auto bd = barch->data();

  EXPECT_FALSE(bd.empty());
  EXPECT_EQ(bd.size(), 4U);
  EXPECT_EQ(bd[0], 0B10101011);
  EXPECT_EQ(bd[1], 0B0);
  EXPECT_EQ(bd[3], 0B0);
}

TEST_F(UTEST_BMP2BarchConverter0,
       single_row_4_whites_5_blacks_convert_image_success)
{
  auto bmp = BMPImage::create();
  EXPECT_NE(bmp, nullptr);

  bmp->width(9);
  bmp->height(1);

  barchdata whites(4, two_two_five);
  barchdata blacks(5, 0U);
  barchdata mixedp;

  mixedp.insert(mixedp.end(), whites.begin(), whites.end());
  mixedp.insert(mixedp.end(), blacks.begin(), blacks.end());

  bmp->data(mixedp);

  auto barch = conv->convert(bmp);

  EXPECT_NE(barch, nullptr);

  auto linest = barch->lines_table();

  EXPECT_EQ(linest.size(), 1);

  EXPECT_TRUE(linest[0U]);

  auto bd = barch->data();

  EXPECT_FALSE(bd.empty());
  EXPECT_EQ(bd.size(), 5U);
  EXPECT_EQ(bd[0], 0B01011000);
  EXPECT_EQ(bd[1], 0B00000000);
  EXPECT_EQ(bd[2], 0B00000000);
  EXPECT_EQ(bd[3], 0B00000000);
  EXPECT_EQ(bd[4], 0B00000000);
}

TEST_F(UTEST_BMP2BarchConverter0,
       single_row_even_whites_arbitrary_size_convert_image_success)
{
  auto bmp = BMPImage::create();
  EXPECT_NE(bmp, nullptr);

  for (size_t iter = 8U; iter < (8U + testsReps * 4); iter += 4U) {
    bmp->width(iter);
    bmp->height(1);

    barchdata whites(iter, two_two_five);

    bmp->data(std::move(whites));

    auto barch = conv->convert(bmp);

    EXPECT_NE(barch, nullptr);

    auto linest = barch->lines_table();

    EXPECT_EQ(linest.size(), 1);

    EXPECT_TRUE(linest[0U]);

    auto bd = barch->data();

    EXPECT_FALSE(bd.empty());
    const size_t expected_cnt = std::ceil(iter / 4U / 8.f);
    EXPECT_EQ(bd.size(), expected_cnt);
    for (unsigned int biter = 0U; biter < expected_cnt; ++biter) {
      EXPECT_EQ(bd[biter], 0B0);
    }
  }
}

TEST_F(UTEST_BMP2BarchConverter0,
       single_row_even_blacks_arbitrary_size_convert_image_success)
{
  auto bmp = BMPImage::create();
  EXPECT_NE(bmp, nullptr);

  for (size_t iter = 32U; iter < (32U + testsReps * 32U); iter += 32U) {
    bmp->width(iter);
    bmp->height(1);

    barchdata blacks(iter, 0U);

    bmp->data(std::move(blacks));

    auto barch = conv->convert(bmp);

    EXPECT_NE(barch, nullptr);

    auto linest = barch->lines_table();

    EXPECT_EQ(linest.size(), 1);

    EXPECT_TRUE(linest[0U]);

    auto bd = barch->data();

    EXPECT_FALSE(bd.empty());
    const size_t expected_cnt = std::ceil(iter / 2U / 8.f);
    EXPECT_EQ(bd.size(), expected_cnt);
    for (unsigned int biter = 0U; biter < expected_cnt; ++biter) {
      EXPECT_EQ(bd[biter], 0B10101010);
    }
  }
}

TEST_F(UTEST_BMP2BarchConverter0,
       single_row_even_grays_arbitrary_size_no_convert_image_success)
{
  auto bmp = BMPImage::create();
  EXPECT_NE(bmp, nullptr);

  for (size_t iter = 32U; iter < (32U + testsReps * 32U); iter += 32U) {
    bmp->width(iter);
    bmp->height(1);

    barchdata grays(iter, two_two_five - 1U);

    bmp->data(std::move(grays));

    auto barch = conv->convert(bmp);

    EXPECT_NE(barch, nullptr);

    auto linest = barch->lines_table();

    EXPECT_EQ(linest.size(), 1);

    EXPECT_FALSE(linest[0U]);

    auto bd = barch->data();

    EXPECT_FALSE(bd.empty());
    EXPECT_EQ(bd.size(), iter);
    for (unsigned int biter = 0U; biter < bd.size(); ++biter) {
      EXPECT_EQ(bd[biter], two_two_five - 1U);
    }
  }
}

TEST_F(UTEST_BMP2BarchConverter0,
       single_row_even_grays_arbitrary_dems_no_convert_image_success)
{
  auto bmp = BMPImage::create();
  EXPECT_NE(bmp, nullptr);

  for (size_t iter = 32U; iter < (32U + testsReps * 32U); iter += 32U) {
    bmp->width(iter);
    bmp->height(iter);

    barchdata grays(bmp->width() * bmp->height(), two_two_five - 1U);

    bmp->data(std::move(grays));

    auto barch = conv->convert(bmp);

    EXPECT_NE(barch, nullptr);

    auto linest = barch->lines_table();

    EXPECT_EQ(linest.size(), iter);

    for (const auto& l : linest) {
      EXPECT_FALSE(l);
    }

    auto bd = barch->data();

    EXPECT_FALSE(bd.empty());
    EXPECT_EQ(bd.size(), iter * iter);

    for (unsigned int biter = 0U; biter < iter * iter; ++biter) {
      EXPECT_EQ(bd[biter], two_two_five - 1U);
    }
  }
}

TEST_F(UTEST_BMP2BarchConverter0,
       single_row_even_whites_arbitrary_dims_convert_image_success)
{
  auto bmp = BMPImage::create();
  EXPECT_NE(bmp, nullptr);

  for (size_t iter = 8U; iter < (8U + testsReps * 4); iter += 4U) {
    bmp->width(iter);
    bmp->height(iter);

    barchdata whites(bmp->width() * bmp->height(), two_two_five);

    bmp->data(std::move(whites));

    auto barch = conv->convert(bmp);

    EXPECT_NE(barch, nullptr);

    auto linest = barch->lines_table();

    EXPECT_EQ(linest.size(), iter);

    for (const auto& l : linest) {
      EXPECT_TRUE(l);
    }

    auto bd = barch->data();

    EXPECT_FALSE(bd.empty());
    const size_t expected_cnt = std::ceil(iter / 4U / 8.f) * iter;
    EXPECT_EQ(bd.size(), expected_cnt);
    for (unsigned int biter = 0U; biter < expected_cnt; ++biter) {
      EXPECT_EQ(bd[biter], 0B0);
    }
  }
}

TEST_F(UTEST_BMP2BarchConverter0,
       single_row_even_blacks_arbitrary_dims_convert_image_success)
{
  auto bmp = BMPImage::create();
  EXPECT_NE(bmp, nullptr);

  for (size_t iter = 32U; iter < (32U + testsReps * 32U); iter += 32U) {
    bmp->width(iter);
    bmp->height(iter);

    barchdata blacks(iter * iter, 0U);

    bmp->data(std::move(blacks));

    auto barch = conv->convert(bmp);

    EXPECT_NE(barch, nullptr);

    auto linest = barch->lines_table();

    EXPECT_EQ(linest.size(), iter);

    for (const auto& l : linest) {
      EXPECT_TRUE(l);
    }

    auto bd = barch->data();

    EXPECT_FALSE(bd.empty());
    const size_t expected_cnt = std::ceil(iter / 2U / 8.f) * iter;
    EXPECT_EQ(bd.size(), expected_cnt);
    for (unsigned int biter = 0U; biter < expected_cnt; ++biter) {
      EXPECT_EQ(bd[biter], 0B10101010);
    }
  }
}

TEST_F(UTEST_BMP2BarchConverter0,
       single_row_8_white_and_4gray_pixels_convert_image_success)
{
  auto bmp = BMPImage::create();
  EXPECT_NE(bmp, nullptr);

  bmp->width(12);
  bmp->height(1);

  barchdata white(8U, two_two_five);
  barchdata gray(4U, gray_pixel);

  barchdata mix(white);
  mix.insert(mix.end(), gray.begin(), gray.end());

  bmp->data(mix);

  auto barch = conv->convert(bmp);

  EXPECT_NE(barch, nullptr);

  auto linest = barch->lines_table();

  EXPECT_EQ(linest.size(), 1);

  EXPECT_TRUE(linest[0U]);

  auto bd = barch->data();

  EXPECT_FALSE(bd.empty());
  EXPECT_EQ(bd.size(), 5U);
  EXPECT_EQ(bd[0], 0B00111111);
  EXPECT_EQ(bd[1], 0B11101111);
  EXPECT_EQ(bd[2], 0B11101111);
  EXPECT_EQ(bd[3], 0B11101111);
  EXPECT_EQ(bd[4], 0B11100000);
}

TEST_F(UTEST_BMP2BarchConverter0,
       single_row_8_white_and_4_1gray_pixels_convert_image_success)
{
  auto bmp = BMPImage::create();
  EXPECT_NE(bmp, nullptr);

  bmp->width(12);
  bmp->height(1);

  barchdata white(8U, two_two_five);
  barchdata gray(4U, 1U);

  barchdata mix(white);
  mix.insert(mix.end(), gray.begin(), gray.end());

  bmp->data(mix);

  auto barch = conv->convert(bmp);

  EXPECT_NE(barch, nullptr);

  auto linest = barch->lines_table();

  EXPECT_EQ(linest.size(), 1);

  EXPECT_TRUE(linest[0U]);

  auto bd = barch->data();

  EXPECT_FALSE(bd.empty());
  EXPECT_EQ(bd.size(), 5U);
  EXPECT_EQ(bd[0], 0B00110000);
  EXPECT_EQ(bd[1], 0B00010000);
  EXPECT_EQ(bd[2], 0B00010000);
  EXPECT_EQ(bd[3], 0B00010000);
  EXPECT_EQ(bd[4], 0B00010000);
}

TEST_F(UTEST_BMP2BarchConverter0,
       single_row_8_white_and_4_240gray_pixels_convert_image_success)
{
  auto bmp = BMPImage::create();
  EXPECT_NE(bmp, nullptr);

  bmp->width(12);
  bmp->height(1);

  barchdata white(8U, two_two_five);
  barchdata gray(4U, 240U);

  barchdata mix(white);
  mix.insert(mix.end(), gray.begin(), gray.end());

  bmp->data(mix);

  auto barch = conv->convert(bmp);

  EXPECT_NE(barch, nullptr);

  auto linest = barch->lines_table();

  EXPECT_EQ(linest.size(), 1);

  EXPECT_TRUE(linest[0U]);

  auto bd = barch->data();

  EXPECT_FALSE(bd.empty());
  EXPECT_EQ(bd.size(), 5U);
  EXPECT_EQ(bd[0], 0B00111111);
  EXPECT_EQ(bd[1], 0B00001111);
  EXPECT_EQ(bd[2], 0B00001111);
  EXPECT_EQ(bd[3], 0B00001111);
  EXPECT_EQ(bd[4], 0B00000000);
}

TEST_F(UTEST_BMP2BarchConverter0,
       single_row_4_240gray_and_8_white_pixels_convert_image_success)
{
  auto bmp = BMPImage::create();
  EXPECT_NE(bmp, nullptr);

  bmp->width(12);
  bmp->height(1);

  barchdata white(8U, two_two_five);
  barchdata gray(4U, 240U);

  barchdata mix(gray);
  mix.insert(mix.end(), white.begin(), white.end());

  bmp->data(mix);

  auto barch = conv->convert(bmp);

  EXPECT_NE(barch, nullptr);

  auto linest = barch->lines_table();

  EXPECT_EQ(linest.size(), 1);

  EXPECT_TRUE(linest[0U]);

  auto bd = barch->data();

  EXPECT_FALSE(bd.empty());
  EXPECT_EQ(bd.size(), 5U);
  EXPECT_EQ(bd[0], 0B11111100);
  EXPECT_EQ(bd[1], 0B00111100);
  EXPECT_EQ(bd[2], 0B00111100);
  EXPECT_EQ(bd[3], 0B00111100);
  EXPECT_EQ(bd[4], 0B00000000);
}

TEST_F(UTEST_BMP2BarchConverter0,
       single_row_4_240gray_and_9_white_pixels_convert_image_success)
{
  auto bmp = BMPImage::create();
  EXPECT_NE(bmp, nullptr);

  bmp->width(13);
  bmp->height(1);

  barchdata white(9U, two_two_five);
  barchdata gray(4U, 240U);

  barchdata mix(gray);
  mix.insert(mix.end(), white.begin(), white.end());

  bmp->data(mix);

  auto barch = conv->convert(bmp);

  EXPECT_NE(barch, nullptr);

  auto linest = barch->lines_table();

  EXPECT_EQ(linest.size(), 1);

  EXPECT_TRUE(linest[0U]);

  auto bd = barch->data();

  EXPECT_FALSE(bd.empty());
  EXPECT_EQ(bd.size(), 9U);
  EXPECT_EQ(bd[0], 0B11111100);
  EXPECT_EQ(bd[1], 0B00111100);
  EXPECT_EQ(bd[2], 0B00111100);
  EXPECT_EQ(bd[3], 0B00111100);
  EXPECT_EQ(bd[4], 0B00001111);
  EXPECT_EQ(bd[5], 0B11111100);
  EXPECT_EQ(bd[6], 0B00000000);
  EXPECT_EQ(bd[7], 0B00000000);
  EXPECT_EQ(bd[8], 0B00000000);
}

TEST_F(UTEST_BMP2BarchConverter0,
       single_row_4_240gray_and_10_white_pixels_convert_image_success)
{
  auto bmp = BMPImage::create();
  EXPECT_NE(bmp, nullptr);

  bmp->width(14);
  bmp->height(1);

  barchdata white(10U, two_two_five);
  barchdata gray(4U, 240U);

  barchdata mix(gray);
  mix.insert(mix.end(), white.begin(), white.end());

  bmp->data(mix);

  auto barch = conv->convert(bmp);

  EXPECT_NE(barch, nullptr);

  auto linest = barch->lines_table();

  EXPECT_EQ(linest.size(), 1);

  EXPECT_TRUE(linest[0U]);

  auto bd = barch->data();

  EXPECT_FALSE(bd.empty());
  EXPECT_EQ(bd.size(), 9U);
  EXPECT_EQ(bd[0], 0B11111100);
  EXPECT_EQ(bd[1], 0B00111100);
  EXPECT_EQ(bd[2], 0B00111100);
  EXPECT_EQ(bd[3], 0B00111100);
  EXPECT_EQ(bd[4], 0B00001111);
  EXPECT_EQ(bd[5], 0B11111111);
  EXPECT_EQ(bd[6], 0B11111100);
  EXPECT_EQ(bd[7], 0B00000000);
  EXPECT_EQ(bd[8], 0B00000000);
}

TEST_F(UTEST_BMP2BarchConverter0,
       16whites_4_1gray_4blacks_pixels_encode_success)
{
  auto bmp = BMPImage::create();
  EXPECT_NE(bmp, nullptr);

  bmp->width(28U);
  bmp->height(1);

  barchdata mixed(18U, white_pixel);

  mixed.push_back(0U);
  mixed.push_back(0U);
  mixed.push_back(0U);

  for (auto iter = 0U; iter < 7U; ++iter) {
    mixed.push_back(white_pixel);
  }

  EXPECT_EQ(bmp->width(), mixed.size());

  bmp->data(mixed);

  auto barch = conv->convert(bmp);

  EXPECT_NE(barch, nullptr);

  auto linest = barch->lines_table();

  EXPECT_EQ(linest.size(), 1);

  EXPECT_TRUE(linest[0U]);

  const auto& bd = barch->data();

  EXPECT_FALSE(bd.empty());

  barchdata expected;

  expected.emplace_back(0B00001111);  // 16 whites //custom started
  expected.emplace_back(0B11111111);  // white
  expected.emplace_back(0B11111100);  // white // black started
  expected.emplace_back(0B00000000);  // black
  expected.emplace_back(0B00000011);  // black // custom started
  expected.emplace_back(0B00000000);  // black
  expected.emplace_back(0B11111111);  // white
  expected.emplace_back(0B11111111);  // white
  expected.emplace_back(0B11111111);  // white
  expected.emplace_back(0B00000000);  // whites

  EXPECT_EQ(bd.size(), expected.size());

  for (size_t iter = 0U; iter < bd.size() && iter < expected.size(); ++iter) {
    std::cout << "bd[" << iter << "] -> " << std::bitset<8>(bd[iter])
              << " ?= expected[" << iter << "] -> "
              << std::bitset<8>(expected[iter]) << std::endl;
    EXPECT_EQ(bd[iter], expected[iter]);
  }
}
