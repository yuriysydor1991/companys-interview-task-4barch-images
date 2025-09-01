#include "src/lib/libmain/writers/BarchWriter0.h"

#include <errno.h>
#include <string.h>

#include <cassert>
#include <cmath>
#include <exception>
#include <filesystem>
#include <fstream>
#include <memory>
#include <vector>
#include <bitset>

#include "src/lib/facade/IBarchImage.h"
#include "src/log/log.h"

namespace barchclib0::writers
{

BarchWriter0Ptr BarchWriter0::create()
{
  return std::make_shared<BarchWriter0>();
}

bool BarchWriter0::write(BarchImagePtr image)
{
  if (image == nullptr) {
    LOGE("Invalid image pointer provided");
    return false;
  }

  return write(image, image->filepath());
}

bool BarchWriter0::write(BarchImagePtr image,
                         const std::filesystem::path& dstpath)
{
  if (image == nullptr) {
    LOGE("Invalid image pointer provided");
    return false;
  }

  if (dstpath.empty()) {
    LOGE("No dst file path provided for an image to save");
    return false;
  }

  try {
    LOGT("Trying to open file " << dstpath);
    std::ofstream dstfile(dstpath,
                          std::ofstream::binary | std::ofstream::trunc);

    if (!dstfile.is_open()) {
      LOGE("Failure to open file " << dstpath);
      return false;
    }

    if (!write(image, dstfile)) {
      LOGE("Fail to write image to the file");
      dstfile.close();
      return false;
    }

    dstfile.close();
  }
  catch (const std::exception& e) {
    LOGE("Exception during file save: " << e.what() << " for a filepath "
                                        << dstpath);
    return false;
  }

  image->filepath(dstpath);

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

  uint32_t tdim = static_cast<uint32_t>(image->width());
  dst.write(reinterpret_cast<char*>(&tdim), sizeof(uint32_t));

  tdim = static_cast<uint32_t>(image->height());
  dst.write(reinterpret_cast<char*>(&tdim), sizeof(uint32_t));

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

  return true;
}

barchdata BarchWriter0::collect_lines_data(BarchImagePtr image)
{
  assert(image != nullptr);
  assert(!image->lines_table().empty());

  barchdata linesdata;

  unsigned char data = zero;
  unsigned char data_left = ucharbits;

  for (const bool& b : image->lines_table()) {
    data <<= 1;
    if (b) {
      data |= 1U;
    }
    data_left--;

    if (data_left == 0) {
      LOGT("Emplacing: " << std::bitset<8>(data));
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

bool BarchWriter0::put_data(barchdata& data, std::ofstream& dst)
{
  dst.write(reinterpret_cast<char*>(data.data()),
            static_cast<std::streamsize>(data.size()));

  if (!dst) {
    int err = errno;
    LOGE("File contains failure: " << strerror(err));
  }

  return static_cast<bool>(dst);
}

}  // namespace barchclib0::writers
