#include "src/lib/libmain/converters/BMPAndBarchConverter0Base.h"

#include <exception>
#include <memory>
#include <vector>

#include "src/lib/facade/IBarchImage.h"
#include "src/lib/libmain/images/BMPImage.h"
#include "src/lib/libmain/images/BarchImage.h"
#include "src/log/log.h"

namespace barchclib0::converters
{

bool BMPAndBarchConverter0Base::supported_bits_per_color(
    const unsigned int& bitsc)
{
  if (bitsc == get_supported_bits()) {
    return true;
  }

  LOGE("Multicolor BGR images are not supported");

  throw std::runtime_error("Multicolor BGR images are not supported");

  return false;
}

const unsigned int& BMPAndBarchConverter0Base::get_min_opt_2_compress()
{
  return min_opt_2_compress;
}

const unsigned int& BMPAndBarchConverter0Base::get_batch_pixels_compress()
{
  return batch_pixels_compress;
}

const unsigned int& BMPAndBarchConverter0Base::get_supported_bits()
{
  return supported_bits;
}

}  // namespace barchclib0::converters
