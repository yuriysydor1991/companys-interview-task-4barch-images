#include "src/lib/libmain/readers/BMPReader.h"

#include <cassert>
#include <cstddef>
#include <exception>
#include <filesystem>
#include <fstream>
#include <memory>
#include <vector>

#include "src/log/log.h"

namespace barchclib0::readers
{

BMPReaderPtr BMPReader::create() { return std::make_shared<BMPReader>(); }

BMPImagePtr BMPReader::read(const fs::path& imagePath)
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

    return read_data(imagePath);
  }
  catch (const std::exception& e) {
    LOGE("Exception " << e.what() << " during file " << imagePath << " read");
  }

  return nullptr;
}

BMPImagePtr BMPReader::read_data(const fs::path& imagePath)
{
  std::ifstream fimage{imagePath, std::ifstream::binary};

  if (!fimage.is_open()) {
    LOGE("Failure during file open: " << imagePath);
    return nullptr;
  }

  uint32_t width{0U};

  fimage >> width;

  if (width == 0) {
    LOGE("Invalid width specified (0)");
    return nullptr;
  }

  uint32_t height{0U};

  fimage >> height;

  if (height == 0) {
    LOGE("Invalid height specified (0)");
    return nullptr;
  }

  const std::streamsize expectedSize = static_cast<std::streamsize>(width) *
                                       static_cast<std::streamsize>(height);

  barchdata fdata(static_cast<barchdata::size_type>(expectedSize),
                  static_cast<barchdata::value_type>(0));

  fimage.read(reinterpret_cast<char*>(fdata.data()), expectedSize);

  const std::streamsize readSize = fimage.gcount();

  if (readSize != expectedSize) {
    LOGE("Expected to fetch " << expectedSize << " bytes but got " << readSize
                              << " bytes instead");
    return nullptr;
  }

  fimage.close();

  auto image = BMPImage::create();

  assert(image != nullptr);

  LOGT("Filling image with " << width << "x" << height << " (" << readSize
                             << " bytes) of data");

  image->width(static_cast<size_t>(width));
  image->height(static_cast<size_t>(height));
  image->data(std::move(fdata));

  return image;
}

}  // namespace barchclib0::readers
