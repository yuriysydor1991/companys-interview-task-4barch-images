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
      "ctests" / "CTEST_BarchReader0";
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

TEST_F(CTEST_BarchReader0, read_single_line_success)
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

TEST_F(CTEST_BarchReader0, read_single_compressed_line_success)
{
  static constexpr const size_t cwidth = 16;

  barchdata expectedData(1, 0U);
  EXPECT_TRUE(write_file(cwidth, 1, {true}, expectedData));

  auto barch = reader->read(testbarch);

  EXPECT_NE(barch, nullptr);

  EXPECT_EQ(barch->width(), cwidth);
  EXPECT_EQ(barch->height(), 1);
  EXPECT_EQ(barch->lines_table().size(), 1);
  EXPECT_TRUE(barch->lines_table()[0U]);

  const auto data = barch->data();
  EXPECT_EQ(data.size(), expectedData.size());

  for (size_t liter = 0U; liter < data.size(); ++liter) {
    EXPECT_EQ(data[liter], expectedData[liter]);
  }
}

TEST_F(CTEST_BarchReader0, read_two_white_compressed_line_success)
{
  static constexpr const size_t cwidth = 16;
  static constexpr const size_t cheight = 2;

  barchdata expectedData(2, 0U);
  EXPECT_TRUE(write_file(cwidth, cheight, {true, true}, expectedData));

  auto barch = reader->read(testbarch);

  EXPECT_NE(barch, nullptr);

  EXPECT_EQ(barch->width(), cwidth);
  EXPECT_EQ(barch->height(), cheight);
  EXPECT_EQ(barch->lines_table().size(), cheight);
  EXPECT_TRUE(barch->lines_table()[0U]);

  const auto data = barch->data();
  EXPECT_EQ(data.size(), expectedData.size());

  size_t hensurer = cheight;
  size_t eiter{0U};
  for (size_t liter = 0U; liter < barch->height(); ++liter) {
    auto line = barch->line(liter);
    EXPECT_EQ(line.size(), 1U);
    hensurer--;
    for (size_t biter = 0U; biter < line.size(); ++biter) {
      EXPECT_EQ(line[biter], expectedData[eiter++]);
    }
  }

  EXPECT_EQ(hensurer, 0);
}

TEST_F(CTEST_BarchReader0, read_two_blacks_compressed_line_success)
{
  static constexpr const size_t cwidth = 16;
  static constexpr const size_t cheight = 2;

  barchdata expectedData(2, 0B10101010);
  EXPECT_TRUE(write_file(cwidth, cheight, {true, true}, expectedData));

  auto barch = reader->read(testbarch);

  EXPECT_NE(barch, nullptr);

  EXPECT_EQ(barch->width(), cwidth);
  EXPECT_EQ(barch->height(), cheight);
  EXPECT_EQ(barch->lines_table().size(), cheight);
  EXPECT_TRUE(barch->lines_table()[0U]);

  const auto data = barch->data();
  EXPECT_EQ(data.size(), expectedData.size());

  size_t hensurer = cheight;
  size_t eiter{0U};
  for (size_t liter = 0U; liter < barch->height(); ++liter) {
    auto line = barch->line(liter);
    EXPECT_EQ(line.size(), 1U);
    EXPECT_EQ(line[0], 0B10101010);
    hensurer--;
    for (size_t biter = 0U; biter < line.size(); ++biter) {
      EXPECT_EQ(line[biter], expectedData[eiter++]);
    }
  }

  EXPECT_EQ(hensurer, 0);
}

TEST_F(CTEST_BarchReader0, read_4whites_4grayssingle_compressed_line_success)
{
  static constexpr const size_t cwidth = 8;
  static constexpr const size_t cheight = 1;

  barchdata expectedData;
  expectedData.emplace_back(0B01111111);
  expectedData.emplace_back(0B11011111);
  expectedData.emplace_back(0B11011111);
  expectedData.emplace_back(0B11011111);
  expectedData.emplace_back(0B11000000);

  EXPECT_TRUE(write_file(cwidth, 1, {true}, expectedData));

  auto barch = reader->read(testbarch);

  EXPECT_NE(barch, nullptr);

  EXPECT_EQ(barch->width(), cwidth);
  EXPECT_EQ(barch->height(), 1);
  EXPECT_EQ(barch->lines_table().size(), 1);
  EXPECT_TRUE(barch->lines_table()[0U]);

  const auto data = barch->data();
  EXPECT_EQ(data.size(), expectedData.size());

  for (size_t liter = 0U; liter < data.size(); ++liter) {
    EXPECT_EQ(data[liter], expectedData[liter]);
  }

  size_t hensurer = cheight;
  for (size_t liter = 0U; liter < barch->height(); ++liter) {
    auto line = barch->line(liter);
    EXPECT_EQ(line.size(), 5U);
    hensurer--;
  }

  EXPECT_EQ(hensurer, 0);
}

TEST_F(CTEST_BarchReader0, read_4gray_4whites_ssingle_compressed_line_success)
{
  static constexpr const size_t cwidth = 8;
  static constexpr const size_t cheight = 1;

  barchdata expectedData;
  expectedData.emplace_back(0B11111111);
  expectedData.emplace_back(0B10111111);
  expectedData.emplace_back(0B10111111);
  expectedData.emplace_back(0B10111111);
  expectedData.emplace_back(0B10000000);

  EXPECT_TRUE(write_file(cwidth, 1, {true}, expectedData));

  auto barch = reader->read(testbarch);

  EXPECT_NE(barch, nullptr);

  EXPECT_EQ(barch->width(), cwidth);
  EXPECT_EQ(barch->height(), 1);
  EXPECT_EQ(barch->lines_table().size(), 1);
  EXPECT_TRUE(barch->lines_table()[0U]);

  const auto data = barch->data();
  EXPECT_EQ(data.size(), expectedData.size());

  for (size_t liter = 0U; liter < data.size(); ++liter) {
    EXPECT_EQ(data[liter], expectedData[liter]);
  }

  size_t hensurer = cheight;
  for (size_t liter = 0U; liter < barch->height(); ++liter) {
    auto line = barch->line(liter);
    EXPECT_EQ(line.size(), 5U);
    hensurer--;
  }

  EXPECT_EQ(hensurer, 0);
}

TEST_F(CTEST_BarchReader0,
       read_4whites_4_blacks_4grayssingle_compressed_line_success)
{
  static constexpr const size_t cwidth = 12;
  static constexpr const size_t cheight = 1;

  barchdata expectedData;
  expectedData.emplace_back(0B01011111);
  expectedData.emplace_back(0B11110111);
  expectedData.emplace_back(0B11110111);
  expectedData.emplace_back(0B11110111);
  expectedData.emplace_back(0B11110000);

  EXPECT_TRUE(write_file(cwidth, 1, {true}, expectedData));

  auto barch = reader->read(testbarch);

  EXPECT_NE(barch, nullptr);

  EXPECT_EQ(barch->width(), cwidth);
  EXPECT_EQ(barch->height(), 1);
  EXPECT_EQ(barch->lines_table().size(), 1);
  EXPECT_TRUE(barch->lines_table()[0U]);

  const auto data = barch->data();
  EXPECT_EQ(data.size(), expectedData.size());

  for (size_t liter = 0U; liter < data.size(); ++liter) {
    EXPECT_EQ(data[liter], expectedData[liter]);
  }

  size_t hensurer = cheight;
  for (size_t liter = 0U; liter < barch->height(); ++liter) {
    auto line = barch->line(liter);
    EXPECT_EQ(line.size(), 5U);
    hensurer--;
  }

  EXPECT_EQ(hensurer, 0);
}

TEST_F(CTEST_BarchReader0,
       read_4_blacks_4grays_4whites_single_compressed_line_success)
{
  static constexpr const size_t cwidth = 12;
  static constexpr const size_t cheight = 1;

  barchdata expectedData;
  expectedData.emplace_back(0B10111111);
  expectedData.emplace_back(0B11101111);
  expectedData.emplace_back(0B11101111);
  expectedData.emplace_back(0B11101111);
  expectedData.emplace_back(0B11100000);

  EXPECT_TRUE(write_file(cwidth, 1, {true}, expectedData));

  auto barch = reader->read(testbarch);

  EXPECT_NE(barch, nullptr);

  EXPECT_EQ(barch->width(), cwidth);
  EXPECT_EQ(barch->height(), cheight);
  EXPECT_EQ(barch->lines_table().size(), cheight);
  EXPECT_TRUE(barch->lines_table()[0U]);

  const auto data = barch->data();
  EXPECT_EQ(data.size(), expectedData.size());

  for (size_t liter = 0U; liter < data.size(); ++liter) {
    EXPECT_EQ(data[liter], expectedData[liter]);
  }

  size_t hensurer = cheight;
  for (size_t liter = 0U; liter < barch->height(); ++liter) {
    auto line = barch->line(liter);
    EXPECT_EQ(line.size(), 5U);
    hensurer--;
  }

  EXPECT_EQ(hensurer, 0);
}

TEST_F(CTEST_BarchReader0,
       read_4_blacks_4grays_4whites_two_compressed_lines_success)
{
  static constexpr const size_t cwidth = 12;
  static constexpr const size_t cheight = 2;

  barchdata expectedData;
  expectedData.emplace_back(0B10111111);
  expectedData.emplace_back(0B11101111);
  expectedData.emplace_back(0B11101111);
  expectedData.emplace_back(0B11101111);
  expectedData.emplace_back(0B11100000);

  expectedData.emplace_back(0B10111111);
  expectedData.emplace_back(0B11101111);
  expectedData.emplace_back(0B11101111);
  expectedData.emplace_back(0B11101111);
  expectedData.emplace_back(0B11100000);

  EXPECT_TRUE(write_file(cwidth, cheight, {true, true}, expectedData));

  auto barch = reader->read(testbarch);

  EXPECT_NE(barch, nullptr);

  EXPECT_EQ(barch->width(), cwidth);
  EXPECT_EQ(barch->height(), cheight);
  EXPECT_EQ(barch->lines_table().size(), cheight);
  EXPECT_TRUE(barch->lines_table()[0U]);

  const auto data = barch->data();
  EXPECT_EQ(data.size(), expectedData.size());

  for (size_t liter = 0U; liter < data.size(); ++liter) {
    EXPECT_EQ(data[liter], expectedData[liter]);
  }

  size_t hensurer = cheight;
  for (size_t liter = 0U; liter < barch->height(); ++liter) {
    auto line = barch->line(liter);
    EXPECT_EQ(line.size(), 5U);
    hensurer--;
  }

  EXPECT_EQ(hensurer, 0);
}

TEST_F(CTEST_BarchReader0,
       read_4_blacks_4grays_8whites_two_compressed_lines_success)
{
  static constexpr const size_t cwidth = 16;
  static constexpr const size_t cheight = 2;

  barchdata expectedData;
  expectedData.emplace_back(0B10111111);
  expectedData.emplace_back(0B11101111);
  expectedData.emplace_back(0B11101111);
  expectedData.emplace_back(0B11101111);
  expectedData.emplace_back(0B11100000);

  expectedData.emplace_back(0B10111111);
  expectedData.emplace_back(0B11101111);
  expectedData.emplace_back(0B11101111);
  expectedData.emplace_back(0B11101111);
  expectedData.emplace_back(0B11100000);

  EXPECT_TRUE(write_file(cwidth, cheight, {true, true}, expectedData));

  auto barch = reader->read(testbarch);

  EXPECT_NE(barch, nullptr);

  EXPECT_EQ(barch->width(), cwidth);
  EXPECT_EQ(barch->height(), cheight);
  EXPECT_EQ(barch->lines_table().size(), cheight);
  EXPECT_TRUE(barch->lines_table()[0U]);

  const auto data = barch->data();
  EXPECT_EQ(data.size(), expectedData.size());

  for (size_t liter = 0U; liter < data.size(); ++liter) {
    EXPECT_EQ(data[liter], expectedData[liter]);
  }

  size_t hensurer = cheight;
  for (size_t liter = 0U; liter < barch->height(); ++liter) {
    auto line = barch->line(liter);
    EXPECT_EQ(line.size(), 5U);
    hensurer--;
  }

  EXPECT_EQ(hensurer, 0);
}

TEST_F(CTEST_BarchReader0,
       read_4grays_4whites_two_separate_compressed_line_success)
{
  static constexpr const size_t cwidth = 16;
  static constexpr const size_t cheight = 2;

  barchdata expectedData;
  expectedData.emplace_back(0B00000000);
  for (auto iter = 0U; iter < cwidth; ++iter) {
    expectedData.emplace_back(gray_pixel);
  }

  EXPECT_TRUE(write_file(cwidth, cheight, {true, false}, expectedData));

  auto barch = reader->read(testbarch);

  EXPECT_NE(barch, nullptr);

  EXPECT_EQ(barch->width(), cwidth);
  EXPECT_EQ(barch->height(), cheight);
  EXPECT_EQ(barch->lines_table().size(), cheight);
  EXPECT_TRUE(barch->lines_table()[0U]);

  const auto data = barch->data();
  EXPECT_EQ(data.size(), expectedData.size());

  for (size_t liter = 0U; liter < data.size(); ++liter) {
    EXPECT_EQ(data[liter], expectedData[liter]);
  }

  EXPECT_EQ(barch->line(0).size(), 1);
  EXPECT_EQ(barch->line(0)[0], 0);

  EXPECT_EQ(barch->line(1).size(), cwidth);
  for (auto iter = 0U; iter < cwidth; ++iter) {
    EXPECT_EQ(barch->line(1)[iter], gray_pixel);
  }
}

TEST_F(CTEST_BarchReader0, read_825whites_compressed_two_line_success)
{
  static constexpr const size_t cwidth = 825;
  static constexpr const size_t cheight = 2;
  static constexpr const size_t expectedLength = 28U * 2U;

  barchdata compressed(24U, 0);
  compressed.emplace_back(0B00000011);
  compressed.emplace_back(0B11111111);
  compressed.emplace_back(0B00000000);
  compressed.emplace_back(0B00000000);

  barchdata expectedData(compressed);

  expectedData.insert(expectedData.end(), compressed.begin(), compressed.end());

  EXPECT_EQ(expectedData.size(), expectedLength);

  EXPECT_TRUE(write_file(cwidth, cheight, {true, true}, expectedData));

  auto barch = reader->read(testbarch);

  EXPECT_NE(barch, nullptr);

  EXPECT_EQ(barch->width(), cwidth);
  EXPECT_EQ(barch->height(), cheight);
  EXPECT_EQ(barch->lines_table().size(), cheight);
  EXPECT_TRUE(barch->lines_table()[0U]);
  EXPECT_TRUE(barch->lines_table()[1U]);

  const auto data = barch->data();
  EXPECT_EQ(data.size(), expectedData.size());

  for (size_t liter = 0U; liter < data.size(); ++liter) {
    EXPECT_EQ(data[liter], expectedData[liter]);
  }
}
