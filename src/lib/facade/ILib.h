#ifndef YOUR_CPP_APP_TEMPLATE_PROJECT_ILIB_CLASS_H
#define YOUR_CPP_APP_TEMPLATE_PROJECT_ILIB_CLASS_H

#include <filesystem>
#include <memory>

#include "IBarchImage.h"

namespace barchclib0
{

class ILib
{
 public:
  using ILibPtr = std::shared_ptr<ILib>;

  virtual ~ILib() = default;
  ILib() = default;

  /// @brief Converts the given BMP file IBarchImage instance into the barch
  virtual IBarchImagePtr bmp_to_barch(IBarchImagePtr bmp) = 0;

  /// @brief Converts the given barch file IBarchImage instance into the BMP
  virtual IBarchImagePtr barch_to_bmp(IBarchImagePtr barch) = 0;

  /// @brief Tries to read image by given filepath. BMP and barch only!
  virtual IBarchImagePtr read(const std::filesystem::path& imagePath) = 0;

  /// @brief Tries to write barch data into given file. Barch only!
  virtual bool write(IBarchImagePtr barch) = 0;

  /// @brief duplicate the object
  virtual ILibPtr duplicate() = 0;
};

using ILibPtr = ILib::ILibPtr;

}  // namespace barchclib0

#endif  // YOUR_CPP_APP_TEMPLATE_PROJECT_ILIB_CLASS_H
