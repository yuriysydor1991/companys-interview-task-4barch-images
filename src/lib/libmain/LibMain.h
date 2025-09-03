#ifndef YOUR_CPP_APP_TEMPLATE_PROJECT_LIBRARYMAIN_CLASS_H
#define YOUR_CPP_APP_TEMPLATE_PROJECT_LIBRARYMAIN_CLASS_H

#include <memory>

#include "IBarchImage.h"
#include "ILib.h"

namespace lib0impl
{

class LibMain : virtual public barchclib0::ILib
{
 public:
  using IBarchImagePtr = barchclib0::IBarchImagePtr;
  using LibMainPtr = std::shared_ptr<LibMain>;

  virtual ~LibMain() = default;
  LibMain();

  /// @brief Converts the given BMP file IBarchImage instance into the barch
  virtual bool bmp_to_barch(IBarchImagePtr barch) override;

  /// @brief Converts the given barch file IBarchImage instance into the BMP
  virtual bool barch_to_bmp(IBarchImagePtr barch) override;

  /// @brief Tries to read image by given filepath. BMP and barch only!
  virtual IBarchImagePtr read(const std::filesystem::path& imagePath) override;

  /// @brief Tries to write barch data into given file. Barch only!
  virtual bool write(IBarchImagePtr barch) override;

  virtual ILibPtr duplicate() override;

  static LibMainPtr create();
};

using IBarchImagePtr = LibMain::IBarchImagePtr;
using LibMainPtr = LibMain::LibMainPtr;

}  // namespace lib0impl

#endif  // YOUR_CPP_APP_TEMPLATE_PROJECT_LIBRARYMAIN_CLASS_H
