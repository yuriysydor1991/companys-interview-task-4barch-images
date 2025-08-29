#include "src/lib/libmain/writers/BarchWriter.h"

#include <cmath>
#include <exception>
#include <filesystem>
#include <fstream>
#include <memory>
#include <vector>

#include "src/lib/facade/IBarchImage.h"
#include "src/log/log.h"

namespace barchclib0::writers
{

bool BarchWriter::write(BMPImagePtr image)
{
  const auto& fpath = image->filepath();

  if (fpath.empty()) {
    LOGE("No dst file path provided for an image to save");
    return false;
  }

  barchdata buff;

  const bool bwr = write(image, buff);

  if (!bwr) {
    LOGE("Invalid buffer write result");
    return false;
  }

  if (buff.empty()) {
    LOGE("Empty file buffer obtained");
    return false;
  }

  try {
    std::ofstream dstfile(fpath, std::ofstream::binary | std::ofstream::trunc);

    if (!dstfile.is_open()) {
      LOGE("Failure to open file " << fpath);
      return false;
    }
  }
  catch (const std::exception& e) {
    LOGE("Exception during file save: " << e.what() << " for a filepath "
                                        << fpath);
    return false;
  }

  return true;
}

bool BarchWriter::write(BMPImagePtr image, barchdata& dst)
{
  if (image == nullptr) {
    LOGE("Invalid image pointer provided");
    return false;
  }

  if (image->width() == 0 || image->height() == 0) {
    LOGE("Image with invalid size provided " << image->width() << "x"
                                             << image->height());
    return false;
  }

  if (image->data().empty()) {
    LOGE("Image with invalid data buffer provided");
    return false;
  }

  LOGT("Initiating bool vector for " << image->height() << " images rows");

  std::vector<bool> lines = analyze_lines(image);

  assert(!lines.empty());

  return true;
}

std::vector<bool> BarchWriter::analyze_lines(BMPImagePtr image)
{
  assert(image != nullptr);

  std::vector<bool> lines(image->height(), false);

  for (size_t crow = 0U; crow < image->height(); ++crow) {
    const bool crowOpt = optimal_to_compress(image, crow);
    LOGT("The image row " << (crow + 1) << " is optimal to compress: "
                          << std::static_cast<unsigned int>(crowOpt));
    lines[crow] = crowOpt;
  }
}

bool BarchWriter::optimal_to_compress(BMPImagePtr image, const size_t& rowIndex)
{
  assert(image != nullptr);

  unsigned int c4thsCount = 0U;
  unsigned int c4ths = 0U;

  for (size_t coli = 0U; coli < image->cols(); ++coli) {
    auto pix = image->pixel(coli, rowIndex);

    if (is_white(pix)) {
      c4ths++;
    }

    if ((c4ths + 1) == batch_pixels_compress) {
      c4thsCount++;
    }
  }

  return c4thsCount >= min_opt_2_compress;
}

bool BarchWriter::is_white(const PixelPtr& pix)
{
  return (pix->y == 255) || (pix->b == 255 && pix->g == 255 && pix->r == 255);
}

}  // namespace barchclib0::writers
