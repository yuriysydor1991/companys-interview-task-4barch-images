#include "src/lib/libmain/writers/BarchWriter0.h"

#include <cassert>
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

bool BarchWriter0::write(BarchImagePtr image)
{
  if (image == nullptr) {
    LOGE("Invalid image pointer provided");
    return false;
  }

  const auto& fpath = image->filepath();

  if (fpath.empty()) {
    LOGE("No dst file path provided for an image to save");
    return false;
  }

  try {
    std::ofstream dstfile(fpath, std::ofstream::binary | std::ofstream::trunc);

    if (!dstfile.is_open()) {
      LOGE("Failure to open file " << fpath);
      return false;
    }

    if (!write(image, dstfile)) {
      LOGE("Fail to write image to the buffer");
      dstfile.close();
      return false;
    }

    dstfile.close();
  }
  catch (const std::exception& e) {
    LOGE("Exception during file save: " << e.what() << " for a filepath "
                                        << fpath);
    return false;
  }

  return true;
}

bool BarchWriter0::write(BarchImagePtr image, std::ofstream& dst)
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

  const auto& idata = image->data();

  if (idata.empty()) {
    LOGE("Image with invalid data buffer provided");
    return false;
  }

  if (image->width() > max_uint32_t) {
    LOGE("Can`t express the width of " << image->width() << " with uint32_t");
    return false;
  }

  if (image->height() > max_uint32_t) {
    LOGE("Can`t express the height of " << image->height() << " with uint32_t");
    return false;
  }

  if (image->lines_table().empty()) {
    LOGE("Barch image doesn't contain the lines table");
    return false;
  }

  if (image->lines_table().size() != image->height()) {
    LOGE(
        "Barch image doesn't contain invalid lines table (height and table "
        "size missmatch)");
    return false;
  }

  dst << BARCH0_STARTER;

  dst << static_cast<uint32_t>(image->width());
  dst << static_cast<uint32_t>(image->height());

  barchdata linesdata = collect_lines_data(image);

  if (!put_data(linesdata, dst)) {
    LOGE("Fail to put the lines table into the file");
    return false;
  }

  linesdata.clear();

  linesdata = image->data();

  if (!put_data(linesdata, dst)) {
    LOGE("Fail to put the data into the file");
    return false;
  }

  return false;
}

barchdata BarchWriter0::collect_lines_data(BarchImagePtr image)
{
  assert(image != nullptr);
  assert(!image->lines_table().empty());

  barchdata linesdata;

  unsigned char data = zero;
  unsigned char data_left = ucharbits;

  for (const bool& b : image->lines_table()) {
    unsigned char bc = static_cast<unsigned char>(b);

    data |= bc;
    data_left--;

    if (data_left == 0) {
      linesdata.emplace_back(data);
      data = zero;
      data_left = ucharbits;
    }
  }

  if (data_left > 0 && data_left < ucharbits) {
    linesdata.emplace_back(data);
    data = zero;
    data_left = ucharbits;
  }

  return linesdata;
}

bool BarchWriter0::put_data(barchdata& data, std::ofstream& dst)
{
  dst.write(reinterpret_cast<char*>(data.data()), data.size());

  return !dst.badbit;
}

}  // namespace barchclib0::writers
