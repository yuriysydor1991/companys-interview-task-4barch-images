#include "src/lib/libmain/converters/Barch2BMPConverter0.h"

#include <exception>
#include <memory>
#include <vector>

#include "src/lib/facade/IBarchImage.h"
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

  const auto& dstdata = bmp->data();

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

  barchdata rt;

  rt.reserve(width);

  for (barchdata::const_iterator liter = src.cbegin(); liter < src.cend();
       ++liter) {
    unsigned char cc = *liter;
    unsigned char ccount = ucharbits;

    if ((cc & coded_blacks_left) == coded_blacks_left) {
      // fill blacks
      fill_blacks(rt, width);
      cc <<= coded_blacks_bits;
      ccount -= coded_blacks_bits;
    } else if ((cc & coded_whites) == coded_whites) {
      // fill whites
      fill_whites(rt, width);
      cc <<= coded_whites_bits;
      ccount -= coded_whites_bits;
    } else if ((cc & leftbit) == zero && (cc << 1 & leftbit) == leftbit) {
      // fill arbitrary pixels
      copy_arbitrary(rt, width, liter, src.cend(), cc, ccount);
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
                                         barchdata::const_iterator liter,
                                         barchdata::const_iterator lend,
                                         unsigned char& cc,
                                         unsigned char ccount)
{
}

void Barch2BMPConverter0::insert_white_pixel(barchdata& dst)
{
  insert_pixel(dst, two_five_five);
}

void Barch2BMPConverter0::insert_black_pixel(barchdata& dst)
{
  insert_pixel(dst, zero);
}

void Barch2BMPConverter0::insert_pixel(barchdata& dst, const int& color)
{
  dst.emplace_back(color);
}

}  // namespace barchclib0::converters
