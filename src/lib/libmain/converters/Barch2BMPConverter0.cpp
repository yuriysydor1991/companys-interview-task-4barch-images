#include "src/lib/libmain/converters/Barch2BMPConverter0.h"

#include <bitset>
#include <cassert>
#include <exception>
#include <memory>
#include <vector>

#include "IBarchImage.h"
#include "src/lib/libmain/converters/BMPAndBarchConverter0Base.h"
#include "src/lib/libmain/images/BMPImage.h"
#include "src/lib/libmain/images/BarchImage.h"
#include "src/log/log.h"

namespace barchclib0::converters
{

BMPImagePtr Barch2BMPConverter0::convert(BarchImagePtr barch)
{
  if (barch == nullptr) {
    LOGE("Invalid image pointer provided");
    return {};
  }

  if (barch->width() == 0 || barch->height() == 0) {
    LOGE("Image with invalid size provided " << barch->width() << "x"
                                             << barch->height());
    return {};
  }

  if (barch->data().empty()) {
    LOGE("Image with invalid data buffer provided");
    return {};
  }

  if (!supported_bits_per_color(barch->bits_per_pixel())) {
    LOGE("Multicolor BGR images are not supported");
    return {};
  }

  const auto& linestable = barch->lines_table();

  if (linestable.size() != barch->height()) {
    LOGE("Lines table (" << linestable.size() << ") missmatches image rows ("
                         << barch->height() << ")");
    return {};
  }

  auto bmp = BMPImage::create();

  bmp->width(barch->width());
  bmp->bits_per_pixel(barch->bits_per_pixel());

  for (size_t liter = 0U; liter < barch->height() && liter < linestable.size();
       ++liter) {
    auto scanline = barch->line(liter);

    if (linestable[liter]) {
      scanline = huffman_decompress(scanline, barch->width());
    }

    bmp->append_line(scanline);
  }

  return bmp;
}

Barch2BMPConverter0Ptr Barch2BMPConverter0::create()
{
  return std::make_shared<Barch2BMPConverter0>();
}

barchdata Barch2BMPConverter0::huffman_decompress(const barchdata& src,
                                                  const size_t& width)
{
  static_assert(leftbit == 0B10000000);
  static_assert(static_cast<unsigned char>(
                    static_cast<unsigned char>(0B11000000) << 2) == zero);
  static_assert(static_cast<unsigned char>(
                    static_cast<unsigned char>(
                        static_cast<unsigned char>(0B11000000) << 2) >>
                    2) == zero);

  barchdata rt;

  rt.reserve(width);

  for (barchdata::const_iterator liter = src.cbegin();
       liter < src.cend() && rt.size() < width; ++liter) {
    unsigned char cc = *liter;
    unsigned char ccount = ucharbits;

    while (ccount > 0 && rt.size() < width && liter < src.cend()) {
      LOGT("checking " << std::bitset<ucharbits>(cc) << ") with left "
                       << static_cast<unsigned int>(ccount));
      const int itype = get_next_pack_type(liter, src.end(), cc, ccount);

      if (itype < 0) {
        LOGT("End of data reached");
        return rt;
      }

      const unsigned char type = static_cast<unsigned char>(itype);

      if (type == coded_whites) {
        fill_whites(rt, width);
      } else if (type == coded_blacks) {
        fill_blacks(rt, width);
      } else if (type == coded_as_is) {
        copy_arbitrary(rt, width, liter, src.cend(), cc, ccount);
      }

      LOGT("result size " << rt.size());
    }
  }

  return rt;
}

void Barch2BMPConverter0::fill_whites(barchdata& dst, const size_t& width)
{
  unsigned int wcount = zero;

  while (wcount++ < get_batch_pixels_compress() && dst.size() < width) {
    insert_white_pixel(dst);
  }
}

void Barch2BMPConverter0::fill_blacks(barchdata& dst, const size_t& width)
{
  unsigned int bcount = zero;

  while (bcount++ < get_batch_pixels_compress() && dst.size() < width) {
    insert_black_pixel(dst);
  }
}

void Barch2BMPConverter0::copy_arbitrary(barchdata& dst, const size_t& width,
                                         barchdata::const_iterator& liter,
                                         barchdata::const_iterator lend,
                                         unsigned char& cc,
                                         unsigned char& ccount)
{
  LOGT("Trying to fill arbitrary pixels");

  assert(liter < lend);

  unsigned char bytes_inserted = zero;

  if (ccount == zero) {
    liter++;
    if (liter >= lend) {
      LOGW("No data left");
      return;
    }
    cc = *liter;
    ccount = ucharbits;
  }

  unsigned char data = zero;
  unsigned char data_left = ucharbits;

  while (liter < lend && dst.size() < width &&
         bytes_inserted < get_batch_pixels_compress()) {
    pack_left_bits(data, data_left, cc, ccount);

    if (data_left == zero) {
      dst.emplace_back(data);
      data = zero;
      data_left = ucharbits;
      bytes_inserted++;
    }

    if (ccount == zero) {
      liter++;
      cc = *liter;
      ccount = ucharbits;
    }
  }
}

void Barch2BMPConverter0::insert_white_pixel(barchdata& dst)
{
  LOGT("fill whites");

  insert_pixel(dst, two_five_five);
}

void Barch2BMPConverter0::insert_black_pixel(barchdata& dst)
{
  LOGT("fill blacks");

  insert_pixel(dst, zero);
}

void Barch2BMPConverter0::insert_pixel(barchdata& dst, const int& color)
{
  dst.emplace_back(color);
}

}  // namespace barchclib0::converters
