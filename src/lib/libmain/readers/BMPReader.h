#ifndef THE_BMP_2_BARCH_IMAGE_CODER_PROJECT_BMPREADER_CLASS_H
#define THE_BMP_2_BARCH_IMAGE_CODER_PROJECT_BMPREADER_CLASS_H

#include <cstddef>
#include <filesystem>
#include <memory>
#include <vector>

#include "src/lib/libmain/BMPImage.h"

namespace barchclib0::readers
{

namespace fs = std::filesystem;

/**
 * @brief The BMP files reader class.
 */
class BMPReader : public std::enable_shared_from_this<BMPReader>
{
 public:
  using BMPReaderPtr = std::shared_ptr<BMPReader>;

  virtual ~BMPReader() = default;
  BMPReader() = default;

  /*
   * @brief Create instance of the BMPReader and return it's std::shared_ptr
   */
  static BMPReaderPtr create();

  /*
   * @brief Read the image file under given fs path.
   *
   * @returns Returns filled BMPImage instance as a std::shared_ptr or a nullptr
   * value in case of any error.
   */
  virtual BMPImagePtr read(const fs::path& imagePath);

 private:
  BMPImagePtr read_data(const fs::path& imagePath);
};

using BMPReaderPtr = BMPReader::BMPReaderPtr;
using barchdata = IBarchImage::barchdata;

}  // namespace barchclib0::readers

#endif  // THE_BMP_2_BARCH_IMAGE_CODER_PROJECT_BMPREADER_CLASS_H
