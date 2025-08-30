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

bool BarchWriter::write(BartchImagePtr image)
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
    
    dstfile.write(buff.data(), static_cast<std::streamsize>(buff.size()));
    
    dstfile.close();
  }
  catch (const std::exception& e) {
    LOGE("Exception during file save: " << e.what() << " for a filepath "
                                        << fpath);
    return false;
  }

  return true;
}

bool BarchWriter::write(BartchImagePtr image, barchdata& dst)
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
  
  dst.clear();
  
  const size_t reserveVal = (sizeof(uint32_t) + sizeof(uint32_t)) + image->lines_table().size() + idata.size();
  
  LOGT("Reserving the dst vector for " << reserveVal << " bytes required");
  dst.reserve(reserveVal);
  
  const std::string ws = std::to_string(static_cast<uint32_t>(image->width()));
  const std::string hs = std::to_string(static_cast<uint32_t>(image->height()));
  
  dst.insert(dst.end(), ws.begin(), ws.end());
  dst.insert(dst.end(), hs.begin(), hs.end());
  
  LOGE("!!! Insert the lines table injection into the dst buffer !!!");
  
  dst.insert(dst.end(), idata.begin(), idata.end());

  return false;
}

}  // namespace barchclib0::writers
