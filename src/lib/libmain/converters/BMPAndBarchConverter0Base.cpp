#include "src/lib/libmain/converters/BMPAndBarchConverter0Base.h"

#include <bitset>
#include <cassert>
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

void BMPAndBarchConverter0Base::pack_right_bits(unsigned char& dst,
                                                unsigned char& dst_left,
                                                unsigned char& data,
                                                unsigned char& data_left)
{
  static_assert((0B1 << 7 >> 7) == 0B1);
  static_assert((0B11 << 6 >> 6) == 0B11);

  assert(dst_left > zero);
  assert(data_left > zero);

  LOGT("Trying to pack " << std::bitset<ucharbits>(data) << " with data left "
                         << static_cast<unsigned int>(data_left) << " into "
                         << std::bitset<ucharbits>(dst) << " and dst left "
                         << static_cast<unsigned int>(dst_left));

  if (dst_left > data_left) {
    dst <<= data_left;
    dst_left -= data_left;
    const unsigned char shifts = (ucharbits - data_left);
    dst |= static_cast<unsigned char>(
        static_cast<unsigned char>(data << shifts) >> shifts);
    data >>= data_left;
    data_left = zero;
  } else {
    dst <<= dst_left;
    const unsigned char shifts = (ucharbits - dst_left);
    dst |= static_cast<unsigned char>(
        static_cast<unsigned char>(data << shifts) >> shifts);
    data >>= dst_left;
    data_left -= dst_left;
    dst_left = zero;
  }

  LOGT("Result: " << std::bitset<ucharbits>(dst) << "("
                  << static_cast<unsigned int>(dst) << " and dst left "
                  << static_cast<unsigned int>(dst_left) << " with data "
                  << std::bitset<ucharbits>(data) << " and bits left "
                  << static_cast<unsigned int>(data_left));
}

void BMPAndBarchConverter0Base::pack_left_bits(unsigned char& dst,
                                               unsigned char& dst_left,
                                               unsigned char& data,
                                               unsigned char& data_left)
{
  static_assert((0B1 << 7 >> 7) == 0B1);
  static_assert((0B11 << 6 >> 6) == 0B11);

  assert(dst_left > zero);
  assert(data_left > zero);

  LOGT("Trying to pack " << std::bitset<ucharbits>(data) << " with data left "
                         << static_cast<unsigned int>(data_left) << " into "
                         << std::bitset<ucharbits>(dst) << " and dst left "
                         << static_cast<unsigned int>(dst_left));

  if (dst_left > data_left) {
    dst <<= data_left;
    dst_left -= data_left;
    const unsigned char shifts = (ucharbits - data_left);
    dst |= static_cast<unsigned char>(data >> shifts);
    data = zero;
    data_left = zero;
  } else {
    dst <<= dst_left;
    const unsigned char shifts = (ucharbits - dst_left);
    dst |= static_cast<unsigned char>(data >> shifts);
    data <<= dst_left;
    data_left -= dst_left;
    dst_left = zero;
  }

  LOGT("Result: " << std::bitset<ucharbits>(dst) << "("
                  << static_cast<unsigned int>(dst) << ") and dst left "
                  << static_cast<unsigned int>(dst_left) << " with data "
                  << std::bitset<ucharbits>(data) << " and bits left "
                  << static_cast<unsigned int>(data_left));
}

}  // namespace barchclib0::converters
