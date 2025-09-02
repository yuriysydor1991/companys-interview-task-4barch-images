#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <fstream>
#include <iomanip>
#include <iostream>

#include "src/lib/libmain/images/BarchImage.h"
#include "src/lib/libmain/readers/BarchReader0.h"

using namespace barchclib0;
using namespace barchclib0::readers;
using namespace testing;

class CTEST_BarchReader0 : public Test
{
 public:
  inline static const std::filesystem::path testbarchdir =
      std::filesystem::temp_directory_path() / "tests" / "barch-coder" /
      "ctests" / "CTEST_BarchReader00";
  inline static const std::filesystem::path testbarch =
      testbarchdir / "test.barch";
  inline static const std::string BARCH0_STARTER = "BA000";
  inline static const size_t testReps = 10;
  inline static const unsigned char white_pixel = 255U;
  inline static const unsigned char gray_pixel = white_pixel - 1U;
  inline static constexpr const unsigned char ucharbits =
      sizeof(unsigned char) * 8;
  inline static constexpr const unsigned char leftmostone = 0B10000000;
  inline static const unsigned char zero = 0U;

  CTEST_BarchReader0() : reader{BarchReader0::create()}
  {
    EXPECT_NE(reader, nullptr);

    if (!std::filesystem::is_directory(testbarchdir)) {
      EXPECT_TRUE(std::filesystem::create_directories(testbarchdir));
    }
  }

  bool write_file(uint32_t w, uint32_t h, const linestable& lt, barchdata& d)
  {
    EXPECT_EQ(h, lt.size());

    std::ofstream f(testbarch, std::ofstream::trunc | std::ofstream::binary);

    EXPECT_TRUE(f.is_open());

    if (!f.is_open()) {
      return false;
    }

    f.write(BARCH0_STARTER.data(), BARCH0_STARTER.size());

    EXPECT_TRUE(f);

    f.write(reinterpret_cast<char*>(&w), sizeof(w));
    EXPECT_TRUE(f);
    f.write(reinterpret_cast<char*>(&h), sizeof(h));
    EXPECT_TRUE(f);

    auto byteslt = convert(lt);

    f.write(reinterpret_cast<char*>(byteslt.data()), byteslt.size());

    EXPECT_TRUE(f);

    f.write(reinterpret_cast<char*>(d.data()), d.size());

    EXPECT_TRUE(f);

    f.close();

    return true;
  }

  barchdata convert(const linestable& lt)
  {
    assert(!lt.empty());

    barchdata linesdata;

    unsigned char data = zero;
    unsigned char data_left = ucharbits;

    for (const bool& b : lt) {
      data <<= 1;
      if (b) {
        data |= 1U;
      }
      data_left--;

      if (data_left == 0) {
        linesdata.emplace_back(data);
        data = zero;
        data_left = ucharbits;
      }
    }

    if (data_left > 0 && data_left < ucharbits) {
      data <<= data_left;
      linesdata.emplace_back(data);
      data = zero;
      data_left = ucharbits;
    }

    return linesdata;
  }

  BarchReader0Ptr reader;
};

TEST_F(CTEST_BarchReader0, read_empty_path_failure)
{
  EXPECT_EQ(reader->read({}), nullptr);
}

TEST_F(CTEST_BarchReader0, read_invalid_path_failure)
{
  EXPECT_EQ(
      reader->read({"/tmp/hopefully-non-existent-path/to/barch-123.barch"}),
      nullptr);
}

TEST_F(CTEST_BarchReader0, read_single_pix_success)
{
  barchdata expectedData(testReps, gray_pixel);
  EXPECT_TRUE(write_file(testReps, 1, {false}, expectedData));

  auto barch = reader->read(testbarch);

  EXPECT_NE(barch, nullptr);

  EXPECT_EQ(barch->width(), testReps);
  EXPECT_EQ(barch->height(), 1);
  EXPECT_EQ(barch->lines_table().size(), 1);
  EXPECT_FALSE(barch->lines_table()[0U]);

  const auto data = barch->data();
  EXPECT_EQ(data.size(), expectedData.size());

  for (size_t liter = 0U; liter < data.size(); ++liter) {
    EXPECT_EQ(data[liter], expectedData[liter]);
  }
}
