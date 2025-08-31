#ifndef THE_BMP_2_BARCH_IMAGE_CODER_PROJECT_BMPANDBARCHCONVERTER0BASE_CLASS_H
#define THE_BMP_2_BARCH_IMAGE_CODER_PROJECT_BMPANDBARCHCONVERTER0BASE_CLASS_H

#include "src/lib/facade/IBarchImage.h"
#include "src/lib/libmain/images/BMPImage.h"
#include "src/lib/libmain/images/BarchImage.h"

namespace barchclib0::converters
{

/**
 * @brief The barch image writer class.
 */
class BMPAndBarchConverter0Base
{
 public:
  virtual ~BMPAndBarchConverter0Base() = default;
  BMPAndBarchConverter0Base() = default;

  virtual bool supported_bits_per_color(const unsigned int& bitsc);
  virtual const unsigned int& get_min_opt_2_compress();
  virtual const unsigned int& get_batch_pixels_compress();
  virtual const unsigned int& get_supported_bits();

 protected:
  inline static constexpr const unsigned char zero = 0U;
  inline static constexpr const unsigned char two_five_five = 255U;
  inline static constexpr const unsigned char leftbit = 1 << 7;
  inline static constexpr const unsigned char ucharbits =
      sizeof(unsigned char) * 8;

  static constexpr const unsigned char coded_whites = zero;
  static constexpr const unsigned char coded_whites_bits = 1U;

  static constexpr const unsigned char coded_blacks = 0B10;
  static constexpr const unsigned char coded_blacks_bits = 2U;
  static constexpr const unsigned char coded_blacks_left =
      coded_blacks << (ucharbits - coded_blacks_bits);

  static constexpr const unsigned char coded_as_is = 0B11;
  static constexpr const unsigned char coded_as_is_bits = 2U;
  static constexpr const unsigned char coded_as_is_left =
      coded_as_is << (ucharbits - coded_as_is_bits);

 private:
  inline static const unsigned int supported_bits = 8U;
  inline static const unsigned int batch_pixels_compress = 4U;
  inline static const unsigned int min_opt_2_compress = 2U;
};

}  // namespace barchclib0::converters

#endif  // THE_BMP_2_BARCH_IMAGE_CODER_PROJECT_BMPANDBARCHCONVERTER0BASE_CLASS_H
