#include "src/lib/libmain/readers/BarchReader0.h"

#include <bitset>
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
    LOGE("Failure with file");
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

  std::streamsize bytesRead = f.gcount();

  LOGT("Read " << bytesRead << " bytes from " << maxSize << " max expected");

  filed.erase(filed.begin() + bytesRead, filed.end());

  f.close();

  if (!split_lines(image, filed)) {
    LOGE("Fail to split lines");
    return {};
  }

  return image;
}

bool BarchReader0::split_lines(BarchImagePtr barch, barchdata& idata)
{
  assert(barch != nullptr);

  if (idata.empty()) {
    LOGE("Empty data provided");
    return false;
  }

  const auto lt = barch->lines_table();

  for (size_t lti = 0U; lti < lt.size() && !idata.empty(); ++lti) {
    if (lt[lti]) {
      if (!extract_compressed_line(barch, idata)) {
        LOGE("Fail to extract the compressed line");
        return false;
      }

      continue;
    }

    auto idb = idata.begin();
    auto ide = idata.begin() + barch->width();

    barchdata scanline(idb, ide);

    idata.erase(idb, ide);

    barch->height(barch->height() - 1U);
    barch->append_line(scanline);
  }

  return true;
}

bool BarchReader0::extract_compressed_line(BarchImagePtr barch,
                                           barchdata& idata)
{
  static_assert(leftbit == 0B10000000);

  LOGT("Trying to extract the compressed line with " << barch->width()
                                                     << " max width");

  size_t line_size{zero};
  auto biter = idata.begin();

  while (biter < idata.end() && line_size < barch->width()) {
    unsigned char cc = *biter;
    unsigned char ccleft = ucharbits;

    LOGT("Taking next byte " << std::bitset<8>(cc) << " and line size is "
                             << line_size);

    while (ccleft > 0 && line_size < barch->width() && biter < idata.end()) {
      if (cc & leftbit) {
        if ((static_cast<unsigned char>(cc << 1) & leftbit) == leftbit) {
          LOGT("Located grays " << get_batch_pixels_compress()
                                << " bytes encoded");
          // 4 grays
          line_size += get_batch_pixels_compress();
          // skip all other bits
          unsigned int skips{0U};
          while (skips++ < get_batch_pixels_compress() && biter < idata.end()) {
            biter++;
          }
          cc = *biter;
          cc <<= ucharbits - ccleft;
        } else {
          // 4 blacks
          LOGT("Located " << get_batch_pixels_compress()
                          << " black bytes encoded");
          line_size += get_batch_pixels_compress();
          cc <<= coded_blacks_bits;
          ccleft -= coded_blacks_bits;
        }
      } else {
        // 4 whites
        LOGT("Located " << get_batch_pixels_compress()
                        << " white bytes encoded");
        line_size += get_batch_pixels_compress();
        cc <<= coded_whites_bits;
        ccleft -= coded_whites_bits;
      }
    }

    ++biter;
  }

  barchdata line(idata.begin(), biter);
  idata.erase(idata.begin(), biter);

  barch->height(barch->height() - 1U);
  barch->append_line(line);

  return true;
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
