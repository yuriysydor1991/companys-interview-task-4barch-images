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

int BMPAndBarchConverter0Base::get_next_pack_type(
    barchdata::const_iterator& liter, barchdata::const_iterator lend,
    unsigned char& cc, unsigned char& ccount)
{
  if (ccount == zero) {
    liter++;
    if (liter >= lend) {
      LOGE("No data left");
      return -1;
    }

    cc = *liter;
    ccount = ucharbits;
  }

  if ((cc & leftbit) == zero) {
    cc <<= coded_whites_bits;
    ccount -= coded_whites_bits;
    return coded_whites;
  }

  cc <<= one;
  ccount -= one;

  if (ccount == zero) {
    LOGT("Returning coded whites");
    liter++;
    if (liter >= lend) {
      LOGE("No data left");
      return -1;
    }

    cc = *liter;
    ccount = ucharbits;
  }

  const bool secondcheck = (cc & leftbit) == zero;

  cc <<= one;
  ccount -= one;

  if (secondcheck) {
    LOGT("Returning coded blacks");
    return coded_blacks;
  }

  LOGT("Returning coded as is");

  return coded_as_is;
}

}  // namespace barchclib0::converters
