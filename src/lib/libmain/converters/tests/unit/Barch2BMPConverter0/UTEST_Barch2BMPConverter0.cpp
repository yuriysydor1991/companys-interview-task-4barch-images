#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <bitset>
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
  inline static constexpr const unsigned char zero = 0U;
  static constexpr const size_t testsReps = 2U;
  static constexpr const unsigned char two_two_five = 255U;
  static constexpr const unsigned char gray_pixel = two_two_five - 1U;
  inline static constexpr const unsigned char ucharbits =
      sizeof(unsigned char) * 8;
  static constexpr const unsigned char coded_blacks = 0B10;
  static constexpr const unsigned char coded_blacks_bits = 2U;
  static constexpr const unsigned char coded_as_is = 0B11;
  static constexpr const unsigned char coded_as_is_bits = 2U;

  UTEST_Barch2BMPConverter0() : conv{Barch2BMPConverter0::create()}
  {
    EXPECT_NE(conv, nullptr);
  }

  void pack_left_bits(unsigned char& dst, unsigned char& dst_left,
                      unsigned char& data, unsigned char& data_left)
  {
    static_assert((0B1 << 7 >> 7) == 0B1);
    static_assert((0B11 << 6 >> 6) == 0B11);

    assert(dst_left > zero);
    assert(data_left > zero);

    if (dst_left > data_left) {
      dst <<= data_left;
      dst_left -= data_left;
      const unsigned char shifts = (ucharbits - data_left);
      dst |= static_cast<unsigned char>(data >> shifts);
      data = zero;
      data_left = zero;
    } else {
      dst <<= dst_left;
      const unsigned char shifts = (ucharbits - dst_left);
      dst |= static_cast<unsigned char>(data >> shifts);
      data <<= dst_left;
      data_left -= dst_left;
      dst_left = zero;
    }
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

TEST_F(UTEST_Barch2BMPConverter0, 4white_pixels_decode_success)
{
  auto barch = BarchImage::create();
  EXPECT_NE(barch, nullptr);

  barch->width(4);

  barchdata zeros(1, static_cast<unsigned char>(0));
  linestable lt(1, true);

  barch->lines_table(lt);
  barch->append_line(zeros);

  auto bmp = conv->convert(barch);

  EXPECT_NE(bmp, nullptr);

  auto bmpd = bmp->data();

  EXPECT_EQ(bmpd.size(), 4U);

  for (const auto& c : bmpd) {
    EXPECT_EQ(c, two_two_five);
  }
}

TEST_F(UTEST_Barch2BMPConverter0, 4black_pixels_decode_success)
{
  auto barch = BarchImage::create();
  EXPECT_NE(barch, nullptr);

  barch->width(4);

  barchdata whites(1, 0B10000000);
  linestable lt(1, true);

  barch->lines_table(lt);
  barch->append_line(whites);

  auto bmp = conv->convert(barch);

  EXPECT_NE(bmp, nullptr);

  auto bmpd = bmp->data();

  EXPECT_EQ(bmpd.size(), 4U);

  for (const auto& c : bmpd) {
    EXPECT_EQ(c, 0U);
  }
}

TEST_F(UTEST_Barch2BMPConverter0, 4white_pixels_arbitrary_length_decode_success)
{
  for (size_t titer = 1U; titer < testsReps; ++titer) {
    auto barch = BarchImage::create();
    EXPECT_NE(barch, nullptr);

    barch->width(4U * titer);

    barchdata zeros(std::ceil(titer / 8.0), static_cast<unsigned char>(0));
    linestable lt(1, true);

    barch->lines_table(lt);
    barch->append_line(zeros);

    auto bmp = conv->convert(barch);

    EXPECT_NE(bmp, nullptr);

    auto bmpd = bmp->data();

    EXPECT_EQ(bmpd.size(), 4U * titer);

    for (const auto& c : bmpd) {
      EXPECT_EQ(c, two_two_five);
    }
  }
}

TEST_F(UTEST_Barch2BMPConverter0, 4black_pixels_arbitrary_length_decode_success)
{
  for (size_t titer = 1U; titer < testsReps; ++titer) {
    auto barch = BarchImage::create();
    EXPECT_NE(barch, nullptr);

    barch->width(4U * titer);

    barchdata tens;
    size_t fill = 0;
    while (fill <= titer) {
      unsigned char c = 0;
      unsigned char cleft = ucharbits;

      while (cleft > 0 && fill <= titer) {
        c <<= coded_blacks_bits;
        c |= coded_blacks;
        cleft -= coded_blacks_bits;
        fill++;
      }

      if (cleft > 0) {
        c <<= cleft;
      }

      tens.emplace_back(c);
    }

    linestable lt(1, true);

    barch->lines_table(lt);
    barch->append_line(tens);

    auto bmp = conv->convert(barch);

    EXPECT_NE(bmp, nullptr);

    auto bmpd = bmp->data();

    EXPECT_EQ(bmpd.size(), 4U * titer);

    for (const auto& c : bmpd) {
      EXPECT_EQ(c, 0);
    }
  }
}

TEST_F(UTEST_Barch2BMPConverter0, 4gray_pixels_decode_success)
{
  auto barch = BarchImage::create();
  EXPECT_NE(barch, nullptr);

  static constexpr const unsigned char cwidth = 4U;

  barch->width(cwidth);

  barchdata gray;
  linestable lt(1, true);

  // 254 - 0B11111110
  gray.emplace_back(0B11111111);
  gray.emplace_back(0B10111111);
  gray.emplace_back(0B10111111);
  gray.emplace_back(0B10111111);
  gray.emplace_back(0B10111111);

  EXPECT_EQ(gray.size(), 5);

  barch->lines_table(lt);
  barch->append_line(gray);

  auto bmp = conv->convert(barch);

  EXPECT_NE(bmp, nullptr);

  auto bmpd = bmp->data();

  EXPECT_EQ(bmpd.size(), cwidth);

  for (const auto& c : bmpd) {
    EXPECT_EQ(c, gray_pixel);
  }
}

TEST_F(UTEST_Barch2BMPConverter0, 4_239gray_pixels_decode_success)
{
  auto barch = BarchImage::create();
  EXPECT_NE(barch, nullptr);

  static constexpr const unsigned char cwidth = 4U;
  static constexpr const unsigned char cgray = 239U;

  barch->width(cwidth);

  barchdata gray;
  linestable lt(1, true);

  // 239 - 11101111
  gray.emplace_back(0B11111011);
  gray.emplace_back(0B11111011);
  gray.emplace_back(0B11111011);
  gray.emplace_back(0B11111011);
  gray.emplace_back(0B11000000);

  EXPECT_EQ(gray.size(), 5);

  barch->lines_table(lt);
  barch->append_line(gray);

  auto bmp = conv->convert(barch);

  EXPECT_NE(bmp, nullptr);

  auto bmpd = bmp->data();

  EXPECT_EQ(bmpd.size(), cwidth);

  for (const auto& c : bmpd) {
    EXPECT_EQ(c, cgray);
  }
}

TEST_F(UTEST_Barch2BMPConverter0, 4_1gray_pixels_decode_success)
{
  auto barch = BarchImage::create();
  EXPECT_NE(barch, nullptr);

  static constexpr const unsigned char cwidth = 4U;
  static constexpr const unsigned char cgray = 1U;

  barch->width(cwidth);

  barchdata gray;
  linestable lt(1, true);

  // 239 - 11101111
  gray.emplace_back(0B11000000);
  gray.emplace_back(0B01000000);
  gray.emplace_back(0B01000000);
  gray.emplace_back(0B01000000);
  gray.emplace_back(0B01000000);

  EXPECT_EQ(gray.size(), 5);

  barch->lines_table(lt);
  barch->append_line(gray);

  auto bmp = conv->convert(barch);

  EXPECT_NE(bmp, nullptr);

  auto bmpd = bmp->data();

  EXPECT_EQ(bmpd.size(), cwidth);

  for (const auto& c : bmpd) {
    EXPECT_EQ(c, cgray);
  }
}

TEST_F(UTEST_Barch2BMPConverter0, 8_1gray_pixels_decode_success)
{
  auto barch = BarchImage::create();
  EXPECT_NE(barch, nullptr);

  static constexpr const unsigned char cwidth = 8U;
  static constexpr const unsigned char cgray = 1U;

  barch->width(cwidth);

  barchdata gray;
  linestable lt(1, true);

  // 239 - 11101111
  gray.emplace_back(0B11000000);
  gray.emplace_back(0B01000000);
  gray.emplace_back(0B01000000);
  gray.emplace_back(0B01000000);
  gray.emplace_back(0B01110000);
  gray.emplace_back(0B00010000);
  gray.emplace_back(0B00010000);
  gray.emplace_back(0B00010000);
  gray.emplace_back(0B00010000);

  barch->lines_table(lt);
  barch->append_line(gray);

  auto bmp = conv->convert(barch);

  EXPECT_NE(bmp, nullptr);

  auto bmpd = bmp->data();

  EXPECT_EQ(bmpd.size(), cwidth);

  for (const auto& c : bmpd) {
    EXPECT_EQ(c, cgray);
  }
}

TEST_F(UTEST_Barch2BMPConverter0, 4_whole_range_gray_pixels_decode_success)
{
  unsigned char cgray = two_two_five;

  while (--cgray > 0) {
    auto barch = BarchImage::create();
    EXPECT_NE(barch, nullptr);

    static constexpr const unsigned char cwidth = 4U;

    barch->width(cwidth);

    barchdata gray;
    linestable lt(1, true);

    unsigned char custom_data = coded_as_is;
    unsigned char custom_data_left = ucharbits - coded_as_is_bits;
    size_t fill = 0;
    unsigned char coded_expected = cgray;
    unsigned char coded_expected_left = ucharbits;

    while (gray.size() < (cwidth + 1)) {
      pack_left_bits(custom_data, custom_data_left, coded_expected,
                     coded_expected_left);

      if (coded_expected_left == 0U) {
        coded_expected = cgray;
        coded_expected_left = ucharbits;
      }

      if (custom_data_left == 0U) {
        gray.emplace_back(custom_data);
        fill++;
        custom_data = 0U;
        custom_data_left = ucharbits;
      }
    }

    EXPECT_EQ(gray.size(), 5);

    barch->lines_table(lt);
    barch->append_line(gray);

    auto bmp = conv->convert(barch);

    EXPECT_NE(bmp, nullptr);

    auto bmpd = bmp->data();

    EXPECT_EQ(bmpd.size(), cwidth);

    for (const auto& c : bmpd) {
      EXPECT_EQ(c, cgray);
    }
  }
}

TEST_F(UTEST_Barch2BMPConverter0, 4white_and_4_1gray_pixels_decode_success)
{
  auto barch = BarchImage::create();
  EXPECT_NE(barch, nullptr);

  static constexpr const unsigned char cwidth = 8U;
  static constexpr const unsigned char cgray = 1U;

  barch->width(cwidth);

  barchdata gray;
  linestable lt(1, true);

  gray.emplace_back(0B01100000);
  gray.emplace_back(0B00100000);
  gray.emplace_back(0B00100000);
  gray.emplace_back(0B00100000);
  gray.emplace_back(0B00100000);

  EXPECT_EQ(gray.size(), 5);

  barch->lines_table(lt);
  barch->append_line(gray);

  auto bmp = conv->convert(barch);

  EXPECT_NE(bmp, nullptr);

  auto bmpd = bmp->data();

  EXPECT_EQ(bmpd.size(), cwidth);

  for (size_t bmpter = 0U; bmpter < bmpd.size(); ++bmpter) {
    auto c = bmpd[bmpter];
    if (bmpter >= 4) {
      EXPECT_EQ(c, cgray);
    } else {
      EXPECT_EQ(c, two_two_five);
    }
  }
}

TEST_F(UTEST_Barch2BMPConverter0, 4black_and_4_1gray_pixels_decode_success)
{
  auto barch = BarchImage::create();
  EXPECT_NE(barch, nullptr);

  static constexpr const unsigned char cwidth = 8U;
  static constexpr const unsigned char cgray = 1U;

  barch->width(cwidth);

  barchdata gray;
  linestable lt(1, true);

  gray.emplace_back(0B10110000);
  gray.emplace_back(0B00010000);
  gray.emplace_back(0B00010000);
  gray.emplace_back(0B00010000);
  gray.emplace_back(0B00010000);

  EXPECT_EQ(gray.size(), 5);

  barch->lines_table(lt);
  barch->append_line(gray);

  auto bmp = conv->convert(barch);

  EXPECT_NE(bmp, nullptr);

  auto bmpd = bmp->data();

  EXPECT_EQ(bmpd.size(), cwidth);

  for (size_t bmpter = 0U; bmpter < bmpd.size(); ++bmpter) {
    auto c = bmpd[bmpter];
    if (bmpter >= 4) {
      EXPECT_EQ(c, cgray);
    } else {
      EXPECT_EQ(c, 0U);
    }
  }
}

TEST_F(UTEST_Barch2BMPConverter0, 8white_and_4_1gray_pixels_decode_success)
{
  auto barch = BarchImage::create();
  EXPECT_NE(barch, nullptr);

  static constexpr const unsigned char cwidth = 12U;
  static constexpr const unsigned char cgray = 1U;

  barch->width(cwidth);

  barchdata gray;
  linestable lt(1, true);

  gray.emplace_back(0B00110000);
  gray.emplace_back(0B00010000);
  gray.emplace_back(0B00010000);
  gray.emplace_back(0B00010000);
  gray.emplace_back(0B00010000);

  EXPECT_EQ(gray.size(), 5);

  barch->lines_table(lt);
  barch->append_line(gray);

  auto bmp = conv->convert(barch);

  EXPECT_NE(bmp, nullptr);

  auto bmpd = bmp->data();

  EXPECT_EQ(bmpd.size(), cwidth);

  for (size_t bmpter = 0U; bmpter < bmpd.size(); ++bmpter) {
    auto c = bmpd[bmpter];
    if (bmpter >= 8) {
      EXPECT_EQ(c, cgray);
    } else {
      EXPECT_EQ(c, two_two_five);
    }
  }
}

TEST_F(UTEST_Barch2BMPConverter0, 8black_and_4_1gray_pixels_decode_success)
{
  auto barch = BarchImage::create();
  EXPECT_NE(barch, nullptr);

  static constexpr const unsigned char cwidth = 12U;
  static constexpr const unsigned char cgray = 1U;

  barch->width(cwidth);

  barchdata gray;
  linestable lt(1, true);

  gray.emplace_back(0B10101100);
  gray.emplace_back(0B00000100);
  gray.emplace_back(0B00000100);
  gray.emplace_back(0B00000100);
  gray.emplace_back(0B00000100);

  EXPECT_EQ(gray.size(), 5);

  barch->lines_table(lt);
  barch->append_line(gray);

  auto bmp = conv->convert(barch);

  EXPECT_NE(bmp, nullptr);

  auto bmpd = bmp->data();

  EXPECT_EQ(bmpd.size(), cwidth);

  for (size_t bmpter = 0U; bmpter < bmpd.size(); ++bmpter) {
    auto c = bmpd[bmpter];
    if (bmpter >= 8) {
      EXPECT_EQ(c, cgray);
    } else {
      EXPECT_EQ(c, 0U);
    }
  }
}

TEST_F(UTEST_Barch2BMPConverter0,
       4white_4black_and_4_1gray_pixels_decode_success)
{
  auto barch = BarchImage::create();
  EXPECT_NE(barch, nullptr);

  static constexpr const unsigned char cwidth = 12U;
  static constexpr const unsigned char cgray = 1U;

  barch->width(cwidth);

  barchdata gray;
  linestable lt(1, true);

  gray.emplace_back(0B01011000);
  gray.emplace_back(0B00001000);
  gray.emplace_back(0B00001000);
  gray.emplace_back(0B00001000);
  gray.emplace_back(0B00001000);

  EXPECT_EQ(gray.size(), 5);

  barch->lines_table(lt);
  barch->append_line(gray);

  auto bmp = conv->convert(barch);

  EXPECT_NE(bmp, nullptr);

  auto bmpd = bmp->data();

  EXPECT_EQ(bmpd.size(), cwidth);

  for (size_t bmpter = 0U; bmpter < bmpd.size(); ++bmpter) {
    auto c = bmpd[bmpter];
    if (bmpter >= 4 && bmpter < 8) {
      EXPECT_EQ(c, 0);
    } else if (bmpter >= 8) {
      EXPECT_EQ(c, cgray);
    } else {
      EXPECT_EQ(c, two_two_five);
    }
  }
}

TEST_F(UTEST_Barch2BMPConverter0,
       4blacks_4whites_and_4_1gray_pixels_decode_success)
{
  auto barch = BarchImage::create();
  EXPECT_NE(barch, nullptr);

  static constexpr const unsigned char cwidth = 12U;
  static constexpr const unsigned char cgray = 1U;

  barch->width(cwidth);

  barchdata gray;
  linestable lt(1, true);

  gray.emplace_back(0B10011000);
  gray.emplace_back(0B00001000);
  gray.emplace_back(0B00001000);
  gray.emplace_back(0B00001000);
  gray.emplace_back(0B00001000);

  EXPECT_EQ(gray.size(), 5);

  barch->lines_table(lt);
  barch->append_line(gray);

  auto bmp = conv->convert(barch);

  EXPECT_NE(bmp, nullptr);

  auto bmpd = bmp->data();

  EXPECT_EQ(bmpd.size(), cwidth);

  for (size_t bmpter = 0U; bmpter < bmpd.size(); ++bmpter) {
    auto c = bmpd[bmpter];
    if (bmpter >= 4 && bmpter < 8) {
      EXPECT_EQ(c, two_two_five);
    } else if (bmpter >= 8) {
      EXPECT_EQ(c, cgray);
    } else {
      EXPECT_EQ(c, 0);
    }
  }
}

TEST_F(UTEST_Barch2BMPConverter0, 4blacks_4_1gray_4whites_pixels_decode_success)
{
  auto barch = BarchImage::create();
  EXPECT_NE(barch, nullptr);

  static constexpr const unsigned char cwidth = 12U;
  static constexpr const unsigned char cgray = 1U;

  barch->width(cwidth);

  barchdata gray;
  linestable lt(1, true);

  gray.emplace_back(0B10110000);
  gray.emplace_back(0B00010000);
  gray.emplace_back(0B00010000);
  gray.emplace_back(0B00010000);
  gray.emplace_back(0B00010000);

  EXPECT_EQ(gray.size(), 5);

  barch->lines_table(lt);
  barch->append_line(gray);

  auto bmp = conv->convert(barch);

  EXPECT_NE(bmp, nullptr);

  auto bmpd = bmp->data();

  EXPECT_EQ(bmpd.size(), cwidth);

  for (size_t bmpter = 0U; bmpter < bmpd.size(); ++bmpter) {
    auto c = bmpd[bmpter];
    if (bmpter >= 4 && bmpter < 8) {
      EXPECT_EQ(c, cgray);
    } else if (bmpter >= 8) {
      EXPECT_EQ(c, two_two_five);
    } else {
      EXPECT_EQ(c, 0);
    }
  }
}

TEST_F(UTEST_Barch2BMPConverter0, 4whites_4_1gray_4blacks_pixels_decode_success)
{
  auto barch = BarchImage::create();
  EXPECT_NE(barch, nullptr);

  static constexpr const unsigned char cwidth = 12U;
  static constexpr const unsigned char cgray = 1U;

  barch->width(cwidth);

  barchdata gray;
  linestable lt(1, true);

  gray.emplace_back(0B01100000);
  gray.emplace_back(0B00100000);
  gray.emplace_back(0B00100000);
  gray.emplace_back(0B00100000);
  gray.emplace_back(0B00110000);

  EXPECT_EQ(gray.size(), 5);

  barch->lines_table(lt);
  barch->append_line(gray);

  auto bmp = conv->convert(barch);

  EXPECT_NE(bmp, nullptr);

  auto bmpd = bmp->data();

  EXPECT_EQ(bmpd.size(), cwidth);

  for (size_t bmpter = 0U; bmpter < bmpd.size(); ++bmpter) {
    auto c = bmpd[bmpter];
    if (bmpter >= 4 && bmpter < 8) {
      EXPECT_EQ(c, cgray);
    } else if (bmpter >= 8) {
      EXPECT_EQ(c, 0);
    } else {
      EXPECT_EQ(c, two_two_five);
    }
  }
}
