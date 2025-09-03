#include "src/lib/libmain/readers/BMPReader.h"

#include <cassert>
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

BMPReaderPtr BMPReader::create() { return std::make_shared<BMPReader>(); }

IBarchImagePtr BMPReader::unified_read(const fs::path& imagePath)
{
  return read(imagePath);
}

bool BMPReader::is_bmp(const fs::path& imagePath)
{
  static const std::string bmpe = ".bmp";

  LOGT("Checking: " << imagePath.extension());

  return imagePath.extension() == bmpe;
}

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

    LOGD("Trying to read the file: " << imagePath);

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

  BITMAPFILEHEADER fileHeader;
  BITMAPINFOHEADER infoHeader;

  fimage.read(reinterpret_cast<char*>(&fileHeader), sizeof(fileHeader));
  fimage.read(reinterpret_cast<char*>(&infoHeader), sizeof(infoHeader));

  if (fileHeader.bfType != 0x4D42) {
    LOGE("Not a BMP file " << imagePath);
    return nullptr;
  }

  const std::streamsize width = infoHeader.biWidth;
  const std::streamsize height = infoHeader.biHeight;

  if (width <= 0) {
    LOGE("Invalid width specified (0)");
    return nullptr;
  }

  if (height <= 0) {
    LOGE("Invalid height specified (0)");
    return nullptr;
  }

  LOGT("Declared image size: " << width << "x" << height);
  LOGT("Bits per pixel " << infoHeader.biBitCount);

  LOGT("Offset to " << fileHeader.bfOffBits);

  fimage.seekg(fileHeader.bfOffBits, std::ios::beg);

  const std::streamsize rowSize =
      ((infoHeader.biBitCount * infoHeader.biWidth + 31) / 32) * 4;

  LOGT("Row size: " << rowSize << " bytes");

  const std::streamsize expectedSize = rowSize * std::abs(infoHeader.biHeight);

  LOGT("Expected size: " << expectedSize << " bytes");

  barchdata fdata(static_cast<barchdata::size_type>(expectedSize),
                  static_cast<barchdata::value_type>(0));

  fimage.read(reinterpret_cast<char*>(fdata.data()), expectedSize);

  const std::streamsize readSize = fimage.gcount();

  fimage.close();

  if (readSize != expectedSize) {
    LOGE("Expected to fetch " << expectedSize << " bytes but got " << readSize
                              << " bytes instead");
    return nullptr;
  }

  auto image = BMPImage::create();

  assert(image != nullptr);

  LOGT("Filling image with " << width << "x" << height << " (" << readSize
                             << " bytes) of data");

  image->width(static_cast<size_t>(width));
  image->height(static_cast<size_t>(height));
  image->data(std::move(fdata));
  image->bits_per_pixel(infoHeader.biBitCount);

  return image;
}

}  // namespace barchclib0::readers
