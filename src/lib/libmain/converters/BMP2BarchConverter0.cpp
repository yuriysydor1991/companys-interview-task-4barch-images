#include "src/lib/libmain/converters/BMP2BarchConverter0.h"

#include <bitset>
#include <cassert>
#include <cmath>
#include <exception>
#include <filesystem>
#include <fstream>
#include <memory>
#include <vector>

#include "IBarchImage.h"
#include "src/log/log.h"

namespace barchclib0::converters
{

BMP2BarchConverter0Ptr BMP2BarchConverter0::create()
{
  return std::make_shared<BMP2BarchConverter0>();
}

BarchImagePtr BMP2BarchConverter0::convert(BMPImagePtr bmp)
{
  if (bmp == nullptr) {
    LOGE("Invalid image pointer provided");
    return {};
  }

  if (bmp->width() == 0 || bmp->height() == 0) {
    LOGE("Image with invalid size provided " << bmp->width() << "x"
                                             << bmp->height());
    return {};
  }

  if (bmp->data().empty()) {
    LOGE("Image with invalid data buffer provided");
    return {};
  }

  if (!supported_bits_per_color(bmp->bits_per_pixel())) {
    LOGE("Multicolor BGR images are not supported");
    return {};
  }

  auto barch = BarchImage::create();

  assert(barch != nullptr);

  barch->width(bmp->width());

  LOGT("Initiating bool vector for " << bmp->height() << " images rows");

  barch->lines_table(analyze_lines(bmp));

  const auto& lines = barch->lines_table();

  assert(!lines.empty());
  assert(lines.size() == bmp->height());

  /* need to perform the compression */
  for (size_t liter = zero; liter < bmp->height(); ++liter) {
    auto line = bmp->line(liter);

    if (liter < lines.size() && lines[liter]) {
      LOGT("Compressing line " << liter);
      line = huffman_compress(line);
    }

    barch->append_line(line);
  }

  return barch;
}

std::vector<bool> BMP2BarchConverter0::analyze_lines(BMPImagePtr image)
{
  assert(image != nullptr);

  std::vector<bool> lines(image->height(), false);

  for (size_t crow = zero; crow < image->height(); ++crow) {
    const bool crowOpt = optimal_to_compress(image, crow);
    LOGT("The image row " << (crow + 1) << " is optimal to compress: "
                          << static_cast<unsigned int>(crowOpt));
    lines[crow] = crowOpt;
  }

  return lines;
}

bool BMP2BarchConverter0::optimal_to_compress(BMPImagePtr image,
                                              const size_t& rowIndex)
{
  assert(image != nullptr);

  unsigned int c4thsWCount = zero;
  unsigned int c4thsW = zero;
  unsigned int c4thsBCount = zero;
  unsigned int c4thsB = zero;

  for (size_t coli = zero; coli < image->width(); ++coli) {
    auto pix = image->pixel(coli, rowIndex);

    if (is_white(pix)) {
      c4thsW++;
      c4thsB = zero;
    } else if (is_black(pix)) {
      c4thsB++;
      c4thsW = zero;
    }

    if (c4thsW >= get_batch_pixels_compress()) {
      c4thsW = zero;
      c4thsWCount++;
    }

    if (c4thsB >= get_batch_pixels_compress()) {
      c4thsB = zero;
      c4thsBCount++;
    }
  }

  return (c4thsWCount + c4thsBCount) >= get_min_opt_2_compress();
}

bool BMP2BarchConverter0::is_white(const PixelPtr& pix)
{
  return (pix->y == two_five_five) ||
         (pix->b == two_five_five && pix->g == two_five_five &&
          pix->r == two_five_five);
}

bool BMP2BarchConverter0::is_black(const PixelPtr& pix)
{
  return (pix->y == zero) ||
         (pix->b == zero && pix->g == zero && pix->r == zero);
}

bool BMP2BarchConverter0::is_white(const unsigned char& val)
{
  return val == two_five_five;
}

bool BMP2BarchConverter0::is_black(const unsigned char& val)
{
  return val == zero;
}

bool BMP2BarchConverter0::all_blacks(barchdata::const_iterator begin,
                                     barchdata::const_iterator end)
{
  while (begin < end) {
    if (!is_black(*begin)) {
      return false;
    }

    begin++;
  }

  return true;
}

bool BMP2BarchConverter0::all_whites(barchdata::const_iterator begin,
                                     barchdata::const_iterator end)
{
  while (begin < end) {
    if (!is_white(*begin)) {
      return false;
    }

    begin++;
  }

  return true;
}

barchdata BMP2BarchConverter0::huffman_compress(const barchdata& line)
{
  barchdata comp;

  comp.reserve(line.size());

  barchdata::const_iterator liter = line.cbegin();
  unsigned char dst = zero;
  unsigned char dst_left = ucharbits;

  while (liter < line.cend()) {
    barchdata::const_iterator enditer =
        std::distance(liter, line.cend()) >= get_batch_pixels_compress()
            ? (liter + get_batch_pixels_compress())
            : line.cend();

    auto ccodedline = get_encoded(liter, enditer, dst, dst_left);

    if (!ccodedline.empty()) {
      comp.insert(comp.end(), ccodedline.begin(), ccodedline.end());
    }

    if (dst_left == zero) {
      LOGT("Packing byte " << std::bitset<ucharbits>(dst)
                           << " with no data left");
      comp.push_back(dst);
      dst = zero;
      dst_left = ucharbits;
    }

    liter = enditer;
  }

  if (dst_left > zero && dst_left < ucharbits) {
    dst <<= dst_left;
    LOGT("Packing last byte " << std::bitset<ucharbits>(dst) << " with "
                              << static_cast<unsigned int>(dst_left)
                              << " trailing bits");
    comp.push_back(dst);
  }

  return comp;
}

barchdata BMP2BarchConverter0::get_encoded(barchdata::const_iterator begin,
                                           barchdata::const_iterator end,
                                           unsigned char& dst,
                                           unsigned char& dst_left)
{
  LOGT("Iters distance: " << std::distance(begin, end));

  assert(std::distance(begin, end) <= get_batch_pixels_compress());

  if (std::distance(begin, end) < get_batch_pixels_compress()) {
    LOGT("Compressing as is for unsuficient data "
         << std::distance(begin, end));
    return get_encoded_as_is(begin, end, dst, dst_left);
  } else if (all_whites(begin, end)) {
    return get_encoded_whites(dst, dst_left);
  } else if (all_blacks(begin, end)) {
    return get_encoded_blacks(dst, dst_left);
  } else {
    return get_encoded_as_is(begin, end, dst, dst_left);
  }

  return {};
}

barchdata BMP2BarchConverter0::get_encoded_as_is(
    barchdata::const_iterator begin, barchdata::const_iterator end,
    unsigned char& dst, unsigned char& dst_left)
{
  LOGT("Coding as is");

  static const unsigned char bitsRequired =
      get_batch_pixels_compress() * ucharbits;

  barchdata rt;

  unsigned char data = coded_as_is_left;
  unsigned char data_left = coded_as_is_bits;

  while (data_left > 0) {
    pack_left_bits(dst, dst_left, data, data_left);

    if (dst_left == zero) {
      rt.push_back(dst);
      dst = zero;
      dst_left = ucharbits;
    }
  }

  data = *begin;
  data_left = ucharbits;
  unsigned char bitspacked = zero;

  while (begin < end) {
    pack_left_bits(dst, dst_left, data, data_left);

    if (data_left == zero) {
      bitspacked += ucharbits;
      begin++;
      if (begin < end) {
        LOGT("No data left");
        data_left = ucharbits;
        data = *begin;
      }
    }

    if (dst_left == zero) {
      rt.push_back(dst);
      dst = zero;
      dst_left = ucharbits;
    }

    LOGT("already packed bits: " << static_cast<unsigned>(bitspacked));
  }

  if (dst_left > zero && dst_left < ucharbits && bitspacked < bitsRequired) {
    bitspacked += dst_left;
    dst <<= dst_left;
    LOGT("Packing last byte " << std::bitset<ucharbits>(dst) << " with "
                              << static_cast<unsigned int>(dst_left)
                              << " trailing bits");
    rt.push_back(dst);
    dst = zero;
    dst_left = ucharbits;
  }

  while (bitspacked < bitsRequired) {
    LOGT("Packing trailing zero byte");
    rt.push_back(zero);
    bitspacked += ucharbits;
    dst = zero;
    dst_left = ucharbits;
    LOGT("already packed bits: " << static_cast<unsigned>(bitspacked));
  }

  LOGT("total packed bits: " << static_cast<unsigned>(bitspacked) << "/"
                             << static_cast<unsigned>(bitsRequired));

  return rt;
}

barchdata BMP2BarchConverter0::get_encoded_blacks(unsigned char& dst,
                                                  unsigned char& dst_left)
{
  assert(dst_left > zero);

  LOGT("Coding as blacks");

  unsigned char data = coded_blacks_left;
  unsigned char data_left = coded_blacks_bits;

  barchdata rt;

  while (data_left > 0) {
    pack_left_bits(dst, dst_left, data, data_left);

    if (dst_left == zero) {
      rt.push_back(dst);
      dst = 0;
      dst_left = ucharbits;
    }
  }

  return rt;
}

barchdata BMP2BarchConverter0::get_encoded_whites(unsigned char& dst,
                                                  unsigned char& dst_left)
{
  assert(dst_left > zero);

  LOGT("Coding as whites");

  unsigned char data = coded_whites;
  unsigned char data_left = coded_whites_bits;

  pack_left_bits(dst, dst_left, data, data_left);

  return {};
}

}  // namespace barchclib0::converters
