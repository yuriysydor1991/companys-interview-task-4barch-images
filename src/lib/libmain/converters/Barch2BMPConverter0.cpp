#include "src/lib/libmain/converters/Barch2BMPConverter0.h"

#include <exception>
#include <memory>
#include <vector>

#include "src/lib/facade/IBarchImage.h"
#include "src/lib/libmain/converters/BMPAndBarchConverter0Base.h"
#include "src/lib/libmain/images/BMPImage.h"
#include "src/lib/libmain/images/BarchImage.h"
#include "src/log/log.h"

namespace barchclib0::converters
{

BMPImagePtr Barch2BMPConverter0::convert(BarchImagePtr barch)
{
  LOGE("Method is not implemented");
  throw std::runtime_error(
      "Method Barch2BMPConverter0::convert is not implemented");
  return {};
}

Barch2BMPConverter0Ptr Barch2BMPConverter0::create()
{
  return std::make_shared<Barch2BMPConverter0>();
}

}  // namespace barchclib0::converters
