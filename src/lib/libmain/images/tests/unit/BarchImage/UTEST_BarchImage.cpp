#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <limits>
#include <string>

#include "src/lib/libmain/images/BarchImage.h"

using namespace barchclib0;
using namespace testing;

class UTEST_BarchImage : public Test
{
 public:
  static constexpr const size_t rndvalue = 123;
  static constexpr const size_t testsReps = 100U;
  static constexpr const size_t max_uchar_value =
      std::numeric_limits<unsigned char>::max();

  UTEST_BarchImage() : barch{BarchImage::create()}
  {
    EXPECT_NE(barch, nullptr);
    EXPECT_EQ(barch->width(), 0U);
    EXPECT_EQ(barch->height(), 0U);
    EXPECT_TRUE(barch->data().empty());
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
    barch->clear();

    const size_t expect_size = (width + 1) * (height + 1);

    barchdata zeros(expect_size, val);

    barch->width(width + 1);
    barch->height(height + 1);
    barch->data(std::move(zeros));

    EXPECT_EQ(width + 1, barch->width());
    EXPECT_EQ(height + 1, barch->height());
    EXPECT_FALSE(barch->data().empty());
  }

  BarchImagePtr barch;
};

TEST_F(UTEST_BarchImage, height_set_get_success)
{
  barch->height(rndvalue);
  EXPECT_EQ(barch->height(), rndvalue);
}

TEST_F(UTEST_BarchImage, width_set_get_success)
{
  barch->width(rndvalue);
  EXPECT_EQ(barch->width(), rndvalue);
}

TEST_F(UTEST_BarchImage, filepath_empty_default_success)
{
  static const std::filesystem::path empty;

  EXPECT_EQ(barch->filepath(), empty);
}

TEST_F(UTEST_BarchImage, filepath_set_get_success)
{
  static const std::string nopath = "/tmp/non-existent/random-image.noexists";

  barch->filepath(nopath);

  EXPECT_EQ(barch->filepath(), nopath);
}

TEST_F(UTEST_BarchImage, setup_arbitrary_data_success)
{
  static_assert(max_uchar_value <= 255U);

  for (unsigned int tvalue = 0; tvalue < max_uchar_value; ++tvalue) {
    const unsigned char ctvalue = static_cast<unsigned char>(tvalue);

    for (size_t titer = 0U; titer < testsReps; ++titer) {
      barchdata arbitraryd(titer, static_cast<unsigned char>(ctvalue));

      barch->data(arbitraryd);

      EXPECT_EQ(barch->data().size(), arbitraryd.size());

      const bool aree = compare(barch->data(), arbitraryd);

      EXPECT_TRUE(aree);

      if (!aree) {
        break;
      }
    }
  }
}

TEST_F(UTEST_BarchImage, setup_arbitrary_data_move_success)
{
  static_assert(max_uchar_value <= 255U);

  for (unsigned int tvalue = 0; tvalue < max_uchar_value; ++tvalue) {
    const unsigned char ctvalue = static_cast<unsigned char>(tvalue);

    for (size_t titer = 0U; titer < testsReps; ++titer) {
      barchdata arbitraryd(titer, static_cast<unsigned char>(ctvalue));
      barchdata arbitrarydc{arbitraryd};

      barch->data(std::move(arbitraryd));

      EXPECT_TRUE(arbitraryd.empty());

      EXPECT_EQ(barch->data().size(), arbitrarydc.size());

      const bool aree = compare(barch->data(), arbitrarydc);

      EXPECT_TRUE(aree);

      if (!aree) {
        break;
      }
    }
  }
}

TEST_F(UTEST_BarchImage, append_line_success)
{
  static_assert(max_uchar_value <= 255U);

  for (size_t titer = 0U; titer < testsReps; ++titer) {
    barchdata arbitraryd(rndvalue, static_cast<unsigned char>(0));

    barch->append_line(arbitraryd);

    EXPECT_EQ(barch->data().size(), (titer + 1) * rndvalue);
    EXPECT_EQ(barch->height(), (titer + 1));
  }
}

TEST_F(UTEST_BarchImage, pixel_get_runtime_errror_failure)
{
  fill_zeros(rndvalue, rndvalue);

  EXPECT_THROW({ auto p = barch->pixel(0U, 0U); }, std::runtime_error);
}

TEST_F(UTEST_BarchImage, pixel_set_runtime_errror_failure)
{
  fill_zeros(rndvalue, rndvalue);

  EXPECT_THROW(
      {
        auto p = std::make_shared<Pixel>();
        p->col = 0;
        p->row = 0;
        p->y = 1;
        barch->pixel(p);
      },
      std::runtime_error);
}
