#include "src/lib/libmain/readers/BarchReader0.h"

#include <cassert>
#include <cmath>
#include <cstddef>
#include <exception>
#include <filesystem>
#include <fstream>
#include <memory>
#include <vector>

#include "src/lib/libmain/readers/BMP.h"
#include "src/log/log.h"

namespace barchclib0::readers
{

BarchReader0Ptr BarchReader0::create()
{
  return std::make_shared<BarchReader0>();
}

BarchImagePtr BarchReader0::read(const fs::path& imagePath)
{
  try {
    if (imagePath.empty()) {
      LOGE("Empty image path given");
      return nullptr;
    }

    if (!fs::is_regular_file(imagePath)) {
      LOGE("No valid image file: " << imagePath);
      return nullptr;
    }

    LOGD("Trying to read the file: " << imagePath);

    return read_data(imagePath);
  }
  catch (const std::exception& e) {
    LOGE("Exception " << e.what() << " during file " << imagePath << " read");
  }

  return nullptr;
}

bool BarchReader0::check_file_starter(std::ifstream& f)
{
  assert(f.is_open());

  std::string starter(BARCH0_STARTER_STR.size(), static_cast<char>(0));

  f.read(starter.data(), BARCH0_STARTER_STR.size());

  if (!static_cast<bool>(f)) {
    LOGE("Failure with file ");
    return false;
  }

  if (starter != BARCH0_STARTER_STR) {
    LOGE("Encounter inappropriate barch file starter constant: " << starter);
    return false;
  }

  return true;
}

bool BarchReader0::read_dimentions(BarchImagePtr image, std::ifstream& f)
{
  assert(image != nullptr);
  assert(f.is_open());

  uint32_t i32{0U};

  f.read(reinterpret_cast<char*>(&i32), sizeof(uint32_t));

  if (!static_cast<bool>(f)) {
    LOGE("Failure with file");
    return false;
  }

  LOGT("Image width: " << i32);
  image->width(static_cast<size_t>(i32));

  f.read(reinterpret_cast<char*>(&i32), sizeof(uint32_t));

  if (!static_cast<bool>(f)) {
    LOGE("Failure with file ");
    return false;
  }

  LOGT("Image height: " << i32);
  image->height(static_cast<size_t>(i32));

  return true;
}

BarchImagePtr BarchReader0::read_data(const fs::path& imagePath)
{
  std::ifstream f{imagePath, std::ifstream::binary};

  if (!f.is_open()) {
    LOGE("Failure during file open: " << imagePath);
    return {};
  }

  if (!check_file_starter(f)) {
    LOGE("Not valid file starter " << imagePath);
    return {};
  }

  auto image = BarchImage::create();

  assert(image != nullptr);

  if (!read_dimentions(image, f)) {
    LOGE("Fail to read file dimentions " << imagePath);
    return {};
  }

  if (!read_lines_table(image, f)) {
    LOGE("Fail to read lines table from the file " << imagePath);
    return {};
  }

  const size_t maxSize = image->width() * image->height();

  barchdata filed(maxSize, static_cast<unsigned char>(0));

  f.read(reinterpret_cast<char*>(filed.data()), filed.size());

  if (!static_cast<bool>(f)) {
    LOGE("Failure with file " << imagePath);
    return {};
  }

  std::streamsize bytesRead = f.gcount();

  LOGT("Read " << bytesRead << " bytes from " << maxSize << " max expected");

  filed.erase(filed.begin() + bytesRead, filed.end());

  f.close();

  image->data(std::move(filed));

  return image;
}

bool BarchReader0::read_lines_table(BarchImagePtr barch, std::ifstream& f)
{
  size_t ltbytes = std::ceil(barch->height() / 8.0);

  if (ltbytes == 0U) {
    LOGE("Invalid lines table value computed");
    return false;
  }

  barchdata filelt(ltbytes, static_cast<unsigned char>(0));

  f.read(reinterpret_cast<char*>(filelt.data()), filelt.size());

  if (!static_cast<bool>(f)) {
    LOGE("Failure with file");
    return {};
  }

  linestable unpacklt;

  unpacklt.reserve(barch->height());

  for (unsigned char fltb : filelt) {
    for (unsigned int iter = 0U;
         iter < ucharbits && unpacklt.size() < barch->height(); ++iter) {
      unpacklt.emplace_back(static_cast<bool>(fltb & leftmostone));
      fltb <<= 1;
    }
  }

  barch->lines_table(std::move(unpacklt));

  return static_cast<bool>(f);
}

}  // namespace barchclib0::readers
