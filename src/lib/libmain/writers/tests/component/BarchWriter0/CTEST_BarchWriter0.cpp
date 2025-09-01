#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <bitset>
#include <cmath>
#include <filesystem>
#include <fstream>
#include <string>

#include "src/lib/libmain/images/BarchImage.h"
#include "src/lib/libmain/writers/BarchWriter0.h"

using namespace barchclib0;
using namespace barchclib0::writers;
using namespace testing;

class CTEST_BarchWriter0 : public Test
{
 public:
  inline static const std::filesystem::path testbarchdir =
      std::filesystem::temp_directory_path() / "tests" / "barch-coder" /
      "ctests" / "CTEST_BarchWriter0";
  inline static const std::filesystem::path testbarch =
      testbarchdir / "test.barch";

  inline static const std::string BARCH0_STARTER = "BA000";

  inline static const size_t testReps = 10;
  inline static const unsigned char white_pixel = 255U;
  inline static const unsigned char gray_pixel = white_pixel - 1U;
  inline static constexpr const unsigned char ucharbits =
      sizeof(unsigned char) * 8;
  inline static constexpr const unsigned char leftmostone = 0B10000000;

  CTEST_BarchWriter0() : writer{BarchWriter0::create()}
  {
    EXPECT_NE(writer, nullptr);

    if (!std::filesystem::is_directory(testbarchdir)) {
      EXPECT_TRUE(std::filesystem::create_directories(testbarchdir));
    }
  }

  bool check_the_file(BarchImagePtr barch)
  {
    std::ifstream f(barch->filepath(), std::ifstream::binary);

    EXPECT_TRUE(f.is_open());
    if (!f.is_open()) {
      return false;
    }

    std::string starter(BARCH0_STARTER.size(), static_cast<char>(0));

    EXPECT_NE(starter, BARCH0_STARTER);

    f.read(starter.data(), BARCH0_STARTER.size());

    EXPECT_TRUE(static_cast<bool>(f));

    EXPECT_EQ(starter, BARCH0_STARTER);

    uint32_t i32{0};

    f.read(reinterpret_cast<char*>(&i32), sizeof(uint32_t));

    EXPECT_TRUE(static_cast<bool>(f));
    EXPECT_EQ(i32, barch->width());

    f.read(reinterpret_cast<char*>(&i32), sizeof(uint32_t));

    EXPECT_TRUE(static_cast<bool>(f));
    EXPECT_EQ(i32, barch->height());

    EXPECT_TRUE(check_the_file_lines_table(barch, f));

    const auto reald = barch->data();

    barchdata filed(reald.size(), static_cast<unsigned char>(0));

    f.read(reinterpret_cast<char*>(filed.data()), filed.size());

    EXPECT_TRUE(static_cast<bool>(f));
    EXPECT_EQ(reald.size(), filed.size());

    for (size_t iter = 0U; iter < filed.size(); ++iter) {
      EXPECT_EQ(filed[iter], reald[iter]);
    }

    f.close();

    return true;
  }

  bool check_the_file_lines_table(BarchImagePtr barch, std::ifstream& f)
  {
    size_t ltbytes = std::ceil(barch->height() / 8.0);

    EXPECT_GT(ltbytes, 0);

    barchdata filelt(ltbytes, static_cast<unsigned char>(0));

    f.read(reinterpret_cast<char*>(filelt.data()), filelt.size());

    EXPECT_TRUE(static_cast<bool>(f));

    if (!f) {
      return false;
    }

    EXPECT_EQ(filelt.size(), ltbytes);

    if (filelt.size() != ltbytes) {
      return false;
    }

    linestable unpacklt;

    unpacklt.reserve(barch->height());

    for (unsigned char fltb : filelt) {
      for (unsigned int iter = 0U; iter < ucharbits && unpacklt.size() < barch->height(); ++iter) {
        unpacklt.emplace_back(static_cast<bool>(fltb & leftmostone));
        fltb <<= 1;
      }
    }

    const auto& reallt = barch->lines_table();

    EXPECT_EQ(reallt.size(), unpacklt.size());

    if (reallt.size() != unpacklt.size()) {
      return false;
    }

    bool fail{false};
    for (size_t liter = 0U; liter < unpacklt.size(); ++liter) {
      EXPECT_EQ(reallt[liter], unpacklt[liter]) << "fail index: " << liter;

      if (reallt[liter] != unpacklt[liter]) {
        fail = true;
      }
    }

    return !fail;
  }

  BarchWriter0Ptr writer;
};

TEST_F(CTEST_BarchWriter0, no_barch_image_provided_failure)
{
  EXPECT_FALSE(writer->write({}));
}

TEST_F(CTEST_BarchWriter0, no_barch_image_no_path_provided_failure)
{
  EXPECT_FALSE(writer->write({}, {}));
}

TEST_F(CTEST_BarchWriter0, empty_barch_image_no_path_provided_failure)
{
  auto barch = BarchImage::create();

  EXPECT_FALSE(writer->write(barch, {}));
}

TEST_F(CTEST_BarchWriter0, single_pixel_image_success)
{
  auto barch = BarchImage::create();

  barch->width(1);
  barch->height(1);
  barch->lines_table({false});
  barch->data({0});
  barch->filepath(testbarch);

  EXPECT_TRUE(writer->write(barch));

  EXPECT_TRUE(check_the_file(barch));
}

TEST_F(CTEST_BarchWriter0, arbitrary_size_image_success)
{
  auto barch = BarchImage::create();

  barch->width(testReps);
  barch->height(testReps);
  barch->lines_table(linestable(barch->height(), false));
  barch->data(barchdata((barch->width() * barch->height()),
                        static_cast<unsigned char>(gray_pixel)));
  barch->filepath(testbarch);

  EXPECT_TRUE(writer->write(barch));

  EXPECT_TRUE(check_the_file(barch));
}

TEST_F(CTEST_BarchWriter0, single_line_arbitrary_size_image_success)
{
  auto barch = BarchImage::create();

  barch->width(testReps);
  barch->height(1);
  barch->lines_table(linestable(barch->height(), false));
  barch->data(barchdata((barch->width() * barch->height()),
                        static_cast<unsigned char>(gray_pixel)));
  barch->filepath(testbarch);

  EXPECT_TRUE(writer->write(barch));

  EXPECT_TRUE(check_the_file(barch));
}

TEST_F(CTEST_BarchWriter0, single_width_arbitrary_height_image_success)
{
  auto barch = BarchImage::create();

  barch->width(1);
  barch->height(testReps);
  barch->lines_table(linestable(barch->height(), false));
  barch->data(barchdata((barch->width() * barch->height()),
                        static_cast<unsigned char>(gray_pixel)));
  barch->filepath(testbarch);

  EXPECT_TRUE(writer->write(barch));

  EXPECT_TRUE(check_the_file(barch));
}

TEST_F(CTEST_BarchWriter0, arbitrary_size_compressed_image_success)
{
  auto barch = BarchImage::create();

  barch->width(testReps);
  barch->height(testReps);

  linestable lt(barch->height(), false);

  lt[lt.size() - 1U] = true;

  barch->lines_table(lt);

  barch->data(barchdata((barch->width() * barch->height()),
                        static_cast<unsigned char>(gray_pixel)));

  barch->filepath(testbarch);

  EXPECT_TRUE(writer->write(barch));

  EXPECT_TRUE(check_the_file(barch));
}

TEST_F(CTEST_BarchWriter0, arbitrary_size_compressed_image_success2)
{
  auto barch = BarchImage::create();

  barch->width(testReps);
  barch->height(testReps);

  linestable lt(barch->height(), false);

  lt[lt.size() - 2U] = true;

  barch->lines_table(lt);

  barch->data(barchdata((barch->width() * barch->height()),
                        static_cast<unsigned char>(gray_pixel)));

  barch->filepath(testbarch);

  EXPECT_TRUE(writer->write(barch));

  EXPECT_TRUE(check_the_file(barch));
}

TEST_F(CTEST_BarchWriter0, arbitrary_size_compressed_image_success3)
{
  auto barch = BarchImage::create();

  barch->width(testReps);
  barch->height(testReps);

  linestable lt(barch->height(), false);

  lt[lt.size() - 3U] = true;

  barch->lines_table(lt);

  barch->data(barchdata((barch->width() * barch->height()),
                        static_cast<unsigned char>(gray_pixel)));

  barch->filepath(testbarch);

  EXPECT_TRUE(writer->write(barch));

  EXPECT_TRUE(check_the_file(barch));
}

TEST_F(CTEST_BarchWriter0, arbitrary_size_random_row_compressed_image_success3)
{
  for (size_t titer = 0U; titer < testReps; ++titer) {
    auto barch = BarchImage::create();

    barch->width(testReps);
    barch->height(testReps);

    linestable lt(barch->height(), false);

    lt[titer] = true;

    barch->lines_table(lt);

    barch->data(barchdata((barch->width() * barch->height()),
                          static_cast<unsigned char>(gray_pixel)));

    barch->filepath(testbarch);

    EXPECT_TRUE(writer->write(barch));

    EXPECT_TRUE(check_the_file(barch));
  }
}

TEST_F(CTEST_BarchWriter0, single_height_compressed_image_with_data_success)
{
  auto barch = BarchImage::create();

  barch->width(testReps);
  barch->height(1);
  barch->lines_table(linestable(barch->height(), true));
  barch->data(barchdata(std::ceil(testReps / 4U),
                        static_cast<unsigned char>(0)));
  barch->filepath(testbarch);

  EXPECT_TRUE(writer->write(barch));

  EXPECT_TRUE(check_the_file(barch));
}
