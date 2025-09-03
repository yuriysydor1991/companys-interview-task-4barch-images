#ifndef THE_BMP_2_BARCH_IMAGE_CODER_PROJECT_IREADER_ABSTRACT_CLASS_H
#define THE_BMP_2_BARCH_IMAGE_CODER_PROJECT_IREADER_ABSTRACT_CLASS_H

#include <filesystem>
#include <memory>

#include "src/lib/facade/IBarchImage.h"

namespace barchclib0::readers
{

namespace fs = std::filesystem;

/**
 * @brief The BMP files reader class.
 */
class IReader
{
 public:
  using IReaderPtr = std::shared_ptr<IReader>;

  virtual ~IReader() = default;
  IReader() = default;

  /*
   * @brief Interface to read the image file under given fs path.
   *
   * @returns Returns filled IBarchImage instance as a std::shared_ptr or a
   * nullptr value in case of any error.
   */
  virtual IBarchImagePtr unified_read(const fs::path& imagePath) = 0;
};

using IReaderPtr = IReader::IReaderPtr;

}  // namespace barchclib0::readers

#endif  // THE_BMP_2_BARCH_IMAGE_CODER_PROJECT_IREADER_ABSTRACT_CLASS_H
