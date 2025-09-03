#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <limits>
#include <string>

#include "src/lib/libmain/images/BMPImage.h"

using namespace barchclib0;
using namespace testing;

class UTEST_BMPImage : public Test
{
 public:
  static constexpr const size_t rndvalue = 123;
  static constexpr const size_t testsReps = 10U;
  static constexpr const size_t max_uchar_value =
      std::numeric_limits<unsigned char>::max();

  UTEST_BMPImage() : bmp{BMPImage::create()}
  {
    EXPECT_NE(bmp, nullptr);
    EXPECT_EQ(bmp->width(), 0U);
    EXPECT_EQ(bmp->height(), 0U);
    EXPECT_TRUE(bmp->data().empty());
  }

  bool compare(const barchdata& l, const barchdata& r)
  {
    EXPECT_EQ(l.size(), r.size());

    if (l.size() != r.size()) {
      return false;
    }

    for (size_t diter = 0U; diter < l.size(); diter++) {
      EXPECT_EQ(l[diter], r[diter]);

      if (l[diter] != r[diter]) {
        return false;
      }
    }

    return true;
  }

  void fill_zeros(const size_t& width, const size_t& height)
  {
    fill(width, height, static_cast<unsigned char>(0U));
  }

  void fill(const size_t& width, const size_t& height, const unsigned char& val)
  {
    bmp->clear();

    const size_t expect_size = (width + 1) * (height + 1);

    barchdata zeros(expect_size, val);

    bmp->width(width + 1);
    bmp->height(height + 1);
    bmp->data(std::move(zeros));

    EXPECT_EQ(width + 1, bmp->width());
    EXPECT_EQ(height + 1, bmp->height());
    EXPECT_FALSE(bmp->data().empty());
  }

  BMPImagePtr bmp;
};

TEST_F(UTEST_BMPImage, height_set_get_success)
{
  bmp->height(rndvalue);
  EXPECT_EQ(bmp->height(), rndvalue);
}

TEST_F(UTEST_BMPImage, width_set_get_success)
{
  bmp->width(rndvalue);
  EXPECT_EQ(bmp->width(), rndvalue);
}

TEST_F(UTEST_BMPImage, bits_per_pixel_get__8_default_success)
{
  EXPECT_EQ(bmp->bits_per_pixel(), 8U);
}

TEST_F(UTEST_BMPImage, bits_per_pixel_set_get_success)
{
  bmp->bits_per_pixel(rndvalue);
  EXPECT_EQ(bmp->bits_per_pixel(), rndvalue);
}

TEST_F(UTEST_BMPImage, filepath_empty_default_success)
{
  static const std::filesystem::path empty;

  EXPECT_EQ(bmp->filepath(), empty);
}

TEST_F(UTEST_BMPImage, filepath_set_get_success)
{
  static const std::string nopath = "/tmp/non-existent/random-image.noexists";

  bmp->filepath(nopath);

  EXPECT_EQ(bmp->filepath(), nopath);
}

TEST_F(UTEST_BMPImage, check_zeros_clear_success)
{
  bool invalid = false;
  for (size_t width = 0U; width < testsReps && !invalid; ++width) {
    for (size_t height = 0U; height < testsReps && !invalid; ++height) {
      fill_zeros(width, height);

      EXPECT_EQ(bmp->data().size(), (bmp->width() * bmp->height()));

      for (size_t w = 0U; w < bmp->width() && !invalid; w++) {
        for (size_t h = 0U; h < bmp->height() && !invalid; h++) {
          auto pixel = bmp->pixel(w, h);

          EXPECT_NE(pixel, nullptr);

          if (pixel == nullptr) {
            invalid = true;
            break;
          }

          EXPECT_EQ(pixel->y, 0);
          EXPECT_EQ(pixel->row, h);
          EXPECT_EQ(pixel->col, w);
        }
      }

      bmp->clear();

      EXPECT_TRUE(bmp->data().empty());
      EXPECT_EQ(bmp->width(), 0U);
      EXPECT_EQ(bmp->height(), 0U);
    }
  }
}

TEST_F(UTEST_BMPImage, setup_arbitrary_data_success)
{
  static_assert(max_uchar_value <= 255U);

  for (unsigned int tvalue = 0; tvalue < max_uchar_value; ++tvalue) {
    const unsigned char ctvalue = static_cast<unsigned char>(tvalue);

    for (size_t titer = 0U; titer < testsReps; ++titer) {
      barchdata arbitraryd(titer, static_cast<unsigned char>(ctvalue));

      bmp->data(arbitraryd);

      EXPECT_EQ(bmp->data().size(), arbitraryd.size());

      const bool aree = compare(bmp->data(), arbitraryd);

      EXPECT_TRUE(aree);

      if (!aree) {
        break;
      }
    }
  }
}

TEST_F(UTEST_BMPImage, setup_arbitrary_data_move_success)
{
  static_assert(max_uchar_value <= 255U);

  for (unsigned int tvalue = 0; tvalue < max_uchar_value; ++tvalue) {
    const unsigned char ctvalue = static_cast<unsigned char>(tvalue);

    for (size_t titer = 0U; titer < testsReps; ++titer) {
      barchdata arbitraryd(titer, static_cast<unsigned char>(ctvalue));
      barchdata arbitrarydc{arbitraryd};

      bmp->data(std::move(arbitraryd));

      EXPECT_TRUE(arbitraryd.empty());

      EXPECT_EQ(bmp->data().size(), arbitrarydc.size());

      const bool aree = compare(bmp->data(), arbitrarydc);

      EXPECT_TRUE(aree);

      if (!aree) {
        break;
      }
    }
  }
}

TEST_F(UTEST_BMPImage, check_zeros_all_pixels_success)
{
  bool invalid = false;
  for (size_t width = 0U; width < testsReps && !invalid; ++width) {
    for (size_t height = 0U; height < testsReps && !invalid; ++height) {
      fill_zeros(width, height);

      EXPECT_EQ(bmp->data().size(), (bmp->width() * bmp->height()));

      for (size_t w = 0U; w < bmp->width() && !invalid; w++) {
        for (size_t h = 0U; h < bmp->height() && !invalid; h++) {
          auto pixel = bmp->pixel(w, h);

          EXPECT_NE(pixel, nullptr);

          if (pixel == nullptr) {
            invalid = true;
            break;
          }

          EXPECT_EQ(pixel->y, 0);
          EXPECT_EQ(pixel->row, h);
          EXPECT_EQ(pixel->col, w);
        }
      }
    }
  }
}

TEST_F(UTEST_BMPImage, check_set_pixels_success)
{
  bool invalid = false;
  bool atleast1 = false;

  for (size_t width = 0U; width < testsReps && !invalid; ++width) {
    for (size_t height = 0U; height < testsReps && !invalid; ++height) {
      fill_zeros(width, height);

      const size_t sr = height / 2U;
      const size_t sc = width / 2U;

      auto spixel = std::make_shared<Pixel>();

      spixel->row = sr;
      spixel->col = sc;

      spixel->y = 1;

      bmp->pixel(spixel);

      for (size_t w = 0U; w < bmp->width() && !invalid; w++) {
        for (size_t h = 0U; h < bmp->height() && !invalid; h++) {
          auto pixel = bmp->pixel(w, h);

          EXPECT_NE(pixel, nullptr);

          if (pixel == nullptr) {
            invalid = true;
            break;
          }

          if (spixel->row == h && spixel->col == w) {
            EXPECT_EQ(pixel->y, spixel->y);
            atleast1 = true;
          } else {
            EXPECT_EQ(pixel->y, 0);
          }

          EXPECT_EQ(pixel->row, h);
          EXPECT_EQ(pixel->col, w);
        }
      }
    }
  }

  EXPECT_TRUE(atleast1);
}

TEST_F(UTEST_BMPImage, check_get_zero_line_correct_size_success)
{
  bool invalid = false;
  bool atleast1 = false;

  for (size_t width = 0U; width < testsReps && !invalid; ++width) {
    for (size_t height = 0U; height < testsReps && !invalid; ++height) {
      fill_zeros(width, height);

      for (size_t h = 0U; h < bmp->height() && !invalid; ++h) {
        auto line = bmp->line(h);

        EXPECT_EQ(line.size(), bmp->width());

        atleast1 = true;

        if (line.size() != width) {
          invalid = true;
        }
      }
    }
  }

  EXPECT_TRUE(atleast1);
}

TEST_F(UTEST_BMPImage, check_get_nonzero_line_success)
{
  bool invalid = false;
  bool atleast1 = false;

  for (size_t width = 0U; width < testsReps && !invalid; ++width) {
    for (size_t height = 0U; height < testsReps && !invalid; ++height) {
      fill_zeros(width, height);

      auto spixel = std::make_shared<Pixel>();

      const size_t hi = height / 2U;

      spixel->row = hi;
      spixel->y = 1;

      for (size_t cindex = 0U; cindex < bmp->width(); ++cindex) {
        spixel->col = cindex;

        bmp->pixel(spixel);
      }

      for (size_t rindex = 0U; rindex < bmp->height(); ++rindex) {
        auto line = bmp->line(rindex);

        EXPECT_EQ(line.size(), bmp->width());

        for (const auto& val : line) {
          if (rindex == hi) {
            EXPECT_EQ(spixel->y, val);
            atleast1 = true;
          } else {
            EXPECT_EQ(spixel->y, 0);
          }
        }
      }
    }
  }

  EXPECT_TRUE(atleast1);
}

TEST_F(UTEST_BMPImage, check_get_nonzero_pixel_success)
{
  bool invalid = false;
  bool atleast1 = false;

  for (size_t width = 0U; width < testsReps && !invalid; ++width) {
    for (size_t height = 0U; height < testsReps && !invalid; ++height) {
      fill_zeros(width, height);

      auto spixel = std::make_shared<Pixel>();

      const size_t hi = height / 2U;

      spixel->row = hi;
      spixel->y = 1;

      for (size_t cindex = 0U; cindex < bmp->width(); ++cindex) {
        spixel->col = cindex;

        bmp->pixel(spixel);
      }

      for (size_t w = 0U; w < bmp->width() && !invalid; w++) {
        for (size_t h = 0U; h < bmp->height() && !invalid; h++) {
          auto pixel = bmp->pixel(w, h);

          EXPECT_NE(pixel, nullptr);

          if (pixel == nullptr) {
            invalid = true;
            break;
          }

          if (spixel->row == hi) {
            EXPECT_EQ(pixel->y, spixel->y);
            atleast1 = true;
          } else {
            EXPECT_EQ(pixel->y, 0);
          }

          EXPECT_EQ(pixel->row, h);
          EXPECT_EQ(pixel->col, w);
        }
      }
    }
  }

  EXPECT_TRUE(atleast1);
}

TEST_F(UTEST_BMPImage, append_line_success)
{
  static_assert(max_uchar_value <= 255U);

  for (size_t titer = 0U; titer < testsReps; ++titer) {
    barchdata arbitraryd(rndvalue, static_cast<unsigned char>(0));

    bmp->append_line(arbitraryd);

    EXPECT_EQ(bmp->data().size(), (titer + 1) * rndvalue);
    EXPECT_EQ(bmp->height(), (titer + 1));
  }
}
