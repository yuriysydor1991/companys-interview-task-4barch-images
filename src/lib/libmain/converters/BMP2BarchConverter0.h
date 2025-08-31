#ifndef THE_BMP_2_BARCH_IMAGE_CODER_PROJECT_BMP2BARCHCONVERTER0_CLASS_H
#define THE_BMP_2_BARCH_IMAGE_CODER_PROJECT_BMP2BARCHCONVERTER0_CLASS_H

#include <memory>
#include <vector>

#include "src/lib/facade/IBarchImage.h"
#include "src/lib/libmain/images/BMPImage.h"
#include "src/lib/libmain/images/BarchImage.h"

namespace barchclib0::converters
{

/**
 * @brief The barch image writer class.
 */
class BMP2BarchConverter0
    : public std::enable_shared_from_this<BMP2BarchConverter0>
{
 public:
  using BMP2BarchConverter0Ptr = std::shared_ptr<BMP2BarchConverter0>;

  virtual ~BMP2BarchConverter0() = default;
  BMP2BarchConverter0() = default;

  virtual BarchImagePtr convert(BMPImagePtr bmp);

  static BMP2BarchConverter0Ptr create();

 protected:
  virtual bool supported_bits_per_color(const unsigned int& bitsc);

 private:
  inline static constexpr const unsigned char zero = 0U;
  inline static constexpr const unsigned char two_five_five = 255U;
  inline static const unsigned int supported_bits = 8U;
  inline static const unsigned int batch_pixels_compress = 4U;
  inline static const unsigned int min_opt_2_compress = 2U;
  static constexpr const unsigned char ucharbits = sizeof(unsigned char) * 8;

  static bool optimal_to_compress(BMPImagePtr image, const size_t& rowIndex);

  static bool is_white(const PixelPtr& pix);
  static bool is_black(const PixelPtr& pix);
  static bool is_white(const unsigned char& val);
  static bool is_black(const unsigned char& val);

  static bool all_blacks(barchdata::const_iterator begin,
                         barchdata::const_iterator end);
  static bool all_whites(barchdata::const_iterator begin,
                         barchdata::const_iterator end);

  static std::vector<bool> analyze_lines(BMPImagePtr image);

  static barchdata huffman_compress(const barchdata& line);

  static barchdata get_encoded(barchdata::const_iterator begin,
                               barchdata::const_iterator end,
                               unsigned char& dst, unsigned char& dst_left);
  static barchdata get_encoded_as_is(barchdata::const_iterator begin,
                                     barchdata::const_iterator end,
                                     unsigned char& dst,
                                     unsigned char& dst_left);
  static barchdata get_encoded_blacks(unsigned char& dst,
                                      unsigned char& dst_left);
  static barchdata get_encoded_whites(unsigned char& dst,
                                      unsigned char& dst_left);
  static void pack_bits(unsigned char& dst, unsigned char& dst_left,
                        unsigned char& data, unsigned char& data_left);
};

using BMP2BarchConverter0Ptr = BMP2BarchConverter0::BMP2BarchConverter0Ptr;

}  // namespace barchclib0::converters

#endif  // THE_BMP_2_BARCH_IMAGE_CODER_PROJECT_BMP2BARCHCONVERTER0_CLASS_H
