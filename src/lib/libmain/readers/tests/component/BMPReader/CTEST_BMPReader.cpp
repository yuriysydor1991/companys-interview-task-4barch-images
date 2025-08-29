#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <fstream>
#include <iomanip>
#include <iostream>

#include "BMPReaderDataProvider.h"
#include "readers_includes.h"
#include "src/lib/facade/IBarchImage.h"
#include "src/lib/libmain/readers/BMPReader.h"

using namespace barchclib0;
using namespace barchclib0::readers;
using namespace testing;

class CTEST_BMPReader : public Test
{
 public:
  inline static std::filesystem::path iroot =
      barchclib0::readers::REAL_IMAGES_ROOT;
  inline static std::filesystem::path i1 = iroot / "test-image-1-gs.bmp";
  inline static std::filesystem::path i2 = iroot / "test-image-2-gs.bmp";

  CTEST_BMPReader() : reader{BMPReader::create()}
  {
    EXPECT_NE(reader, nullptr);
  }

  BMPReaderPtr reader;
};

TEST_F(CTEST_BMPReader, read_empty_path_failure)
{
  EXPECT_EQ(reader->read({}), nullptr);
}

TEST_F(CTEST_BMPReader, read_invalid_path_failure)
{
  EXPECT_EQ(reader->read({"/hopefully-non-existent-path"}), nullptr);
}

TEST_F(CTEST_BMPReader, read_directory_path_failure)
{
  EXPECT_EQ(reader->read({"/"}), nullptr);
}

TEST_F(CTEST_BMPReader, read_real_file_non_empty_success)
{
  EXPECT_NE(reader->read(i1), nullptr);
}

TEST_F(CTEST_BMPReader, read_real_file_success)
{
  auto bmp = reader->read(i1);

  EXPECT_NE(bmp, nullptr);

  EXPECT_GE(bmp->width(), 0);
  EXPECT_GE(bmp->height(), 0);

  const auto& expectedData =
      barchclib0::ctests::BMPReaderDataProvider::get_i1_data();
  const auto& obtainedData = bmp->data();

  //  std::fstream out("out.cpp", std::fstream::trunc | std::fstream::out);
  //  EXPECT_TRUE(out.is_open());
  //
  //  unsigned int line = 0;
  //  for (unsigned int iter = 0U; iter < bmp->data().size(); ++iter) {
  //    out << std::hex << "0x" << static_cast<unsigned int>(bmp->data()[iter])
  //        << ", ";
  //
  //    if (line++ >= 10) {
  //      out << std::endl;
  //      line = 0U;
  //    }
  //  }
  //
  //  out << std::endl;

  EXPECT_EQ(expectedData.size(), obtainedData.size());

  for (size_t iter = 0U; iter < obtainedData.size(); ++iter) {
    EXPECT_EQ(expectedData[iter], obtainedData[iter]);
  }
}

TEST_F(CTEST_BMPReader, read_real_2_file_success)
{
  auto bmp = reader->read(i2);

  EXPECT_NE(bmp, nullptr);

  EXPECT_GE(bmp->width(), 0);
  EXPECT_GE(bmp->height(), 0);

  const auto& expectedData =
      barchclib0::ctests::BMPReaderDataProvider::get_i2_data();
  const auto& obtainedData = bmp->data();

  EXPECT_EQ(expectedData.size(), obtainedData.size());

  for (size_t iter = 0U; iter < obtainedData.size(); ++iter) {
    EXPECT_EQ(expectedData[iter], obtainedData[iter]);
  }
}
