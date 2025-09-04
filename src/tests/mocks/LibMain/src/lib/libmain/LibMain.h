#ifndef YOUR_CPP_APP_TEMPLATE_PROJECT_LIBRARYMAIN_CLASS_H
#define YOUR_CPP_APP_TEMPLATE_PROJECT_LIBRARYMAIN_CLASS_H

#include <gmock/gmock.h>

#include <memory>

#include "IBarchImage.h"
#include "ILib.h"
#include "src/lib/libmain/images/BMPImage.h"
#include "src/lib/libmain/images/BarchImage.h"
#include "src/lib/libmain/readers/IReader.h"

namespace lib0impl
{

class LibMain : public barchclib0::ILib
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

  MOCK_METHOD(IBarchImagePtr, bmp_to_barch, (IBarchImagePtr bmp), (override));
  MOCK_METHOD(IBarchImagePtr, barch_to_bmp, (IBarchImagePtr barch), (override));
  MOCK_METHOD(IBarchImagePtr, read, (const std::filesystem::path& imagePath),
              (override));
  MOCK_METHOD(bool, write, (IBarchImagePtr barch), (override));
  MOCK_METHOD(ILibPtr, duplicate, (), (override));
  MOCK_METHOD(IBarchImagePtr, create_empty_bmp, (), (override));

  static LibMainPtr create() { return std::make_shared<LibMain>(); }
};

}  // namespace lib0impl

#endif  // YOUR_CPP_APP_TEMPLATE_PROJECT_LIBRARYMAIN_CLASS_H
