#ifndef YOUR_CPP_APP_TEMPLATE_PROJECT_LIBRARYMAIN_CLASS_H
#define YOUR_CPP_APP_TEMPLATE_PROJECT_LIBRARYMAIN_CLASS_H

#include <memory>

#include "IBarchImage.h"
#include "ILib.h"
#include "src/lib/libmain/images/BMPImage.h"
#include "src/lib/libmain/images/BarchImage.h"
#include "src/lib/libmain/readers/IReader.h"

namespace lib0impl
{

class LibMain : virtual public barchclib0::ILib
{
 public:
  using IBarchImagePtr = barchclib0::IBarchImagePtr;
  using LibMainPtr = std::shared_ptr<LibMain>;
  using BMPImagePtr = barchclib0::BMPImagePtr;
  using BMPImage = barchclib0::BMPImage;
  using BarchImagePtr = barchclib0::BarchImagePtr;
  using BarchImage = barchclib0::BarchImage;
  using IReaderPtr = barchclib0::readers::IReaderPtr;

  virtual ~LibMain() = default;
  LibMain() = default;

  /// @brief Converts the given BMP file IBarchImage instance into the barch
  virtual IBarchImagePtr bmp_to_barch(IBarchImagePtr bmp) override;

  /// @brief Converts the given barch file IBarchImage instance into the BMP
  virtual IBarchImagePtr barch_to_bmp(IBarchImagePtr barch) override;

  /// @brief Tries to read image by given filepath. BMP and barch only!
  virtual IBarchImagePtr read(const std::filesystem::path& imagePath) override;

  /// @brief Tries to write barch data into given file. Barch only!
  virtual bool write(IBarchImagePtr barch) override;

  virtual ILibPtr duplicate() override;

  static LibMainPtr create();

 private:
  static IReaderPtr create_reader(const std::filesystem::path& imagePath);
};

using IBarchImagePtr = LibMain::IBarchImagePtr;
using LibMainPtr = LibMain::LibMainPtr;

}  // namespace lib0impl

#endif  // YOUR_CPP_APP_TEMPLATE_PROJECT_LIBRARYMAIN_CLASS_H
