#include "src/lib/libmain/converters/BMP2BarchConverter.h"

#include <cassert>
#include <cmath>
#include <exception>
#include <filesystem>
#include <fstream>
#include <memory>
#include <vector>

#include "src/lib/facade/IBarchImage.h"
#include "src/log/log.h"

namespace barchclib0::converters
{

BMP2BarchConverterPtr BMP2BarchConverter::create()
{
  return std::make_shared<BMP2BarchConverter>();
}

bool BMP2BarchConverter::supported_bits_per_color(const unsigned int& bitsc)
{
  if (bitsc == supported_bits) {
    return true;
  }

  LOGE("Multicolor BGR images are not supported");

  throw std::runtime_error("Multicolor BGR images are not supported");

  return false;
}

BarchImagePtr BMP2BarchConverter::convert(BMPImagePtr bmp)
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

  LOGT("Initiating bool vector for " << bmp->height() << " images rows");

  barch->lines_table(analyze_lines(bmp));

  const auto& lines = barch->lines_table();

  assert(!lines.empty());
  assert(lines.size() == bmp->height());

  /* need to perform the compression */
  for (size_t liter = 0U; liter < bmp->height(); ++liter) {
    auto line = bmp->line(liter);

    if (liter < lines.size() && lines[liter]) {
      LOGT("Compressing line " << liter);
      line = compress(line);
    }

    barch->append_line(line);
  }

  return barch;
}

std::vector<bool> BMP2BarchConverter::analyze_lines(BMPImagePtr image)
{
  assert(image != nullptr);

  std::vector<bool> lines(image->height(), false);

  for (size_t crow = 0U; crow < image->height(); ++crow) {
    const bool crowOpt = optimal_to_compress(image, crow);
    LOGT("The image row " << (crow + 1) << " is optimal to compress: "
                          << static_cast<unsigned int>(crowOpt));
    lines[crow] = crowOpt;
  }

  return lines;
}

bool BMP2BarchConverter::optimal_to_compress(BMPImagePtr image,
                                             const size_t& rowIndex)
{
  assert(image != nullptr);

  unsigned int c4thsCount = 0U;
  unsigned int c4ths = 0U;

  for (size_t coli = 0U; coli < image->width(); ++coli) {
    auto pix = image->pixel(coli, rowIndex);

    if (is_white(pix)) {
      c4ths++;
    }

    if ((c4ths + 1) == batch_pixels_compress) {
      c4ths = 0;
      c4thsCount++;
    }
  }

  return c4thsCount >= min_opt_2_compress;
}

bool BMP2BarchConverter::is_white(const PixelPtr& pix)
{
  return (pix->y == 255) || (pix->b == 255 && pix->g == 255 && pix->r == 255);
}

barchdata BMP2BarchConverter::compress(const barchdata& line)
{
  barchdata comp;

  unsigned int cwhites = 0U;

  for (size_t liter = 0U; liter < line.size(); ++liter) {
    if (line[liter] == 255) {
      cwhites++;
    }

    if ((cwhites + 1) == min_opt_2_compress) {
      cwhites = 0U;
    }
  }

  return comp;
}

}  // namespace barchclib0::converters
