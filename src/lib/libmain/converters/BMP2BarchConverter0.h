#ifndef THE_BMP_2_BARCH_IMAGE_CODER_PROJECT_BMP2BARCHCONVERTER0_CLASS_H
#define THE_BMP_2_BARCH_IMAGE_CODER_PROJECT_BMP2BARCHCONVERTER0_CLASS_H

#include <memory>
#include <vector>

#include "src/lib/facade/IBarchImage.h"
#include "src/lib/libmain/converters/BMPAndBarchConverter0Base.h"
#include "src/lib/libmain/images/BMPImage.h"
#include "src/lib/libmain/images/BarchImage.h"

namespace barchclib0::converters
{

/**
 * @brief The BMP 2 Barch image converter class v.0.
 */
class BMP2BarchConverter0
    : public std::enable_shared_from_this<BMP2BarchConverter0>,
      virtual public BMPAndBarchConverter0Base
{
 public:
  using BMP2BarchConverter0Ptr = std::shared_ptr<BMP2BarchConverter0>;

  virtual ~BMP2BarchConverter0() = default;
  BMP2BarchConverter0() = default;

  virtual BarchImagePtr convert(BMPImagePtr bmp);

  static BMP2BarchConverter0Ptr create();

 private:
  bool optimal_to_compress(BMPImagePtr image, const size_t& rowIndex);

  static bool is_white(const PixelPtr& pix);
  static bool is_black(const PixelPtr& pix);
  static bool is_white(const unsigned char& val);
  static bool is_black(const unsigned char& val);

  static bool all_blacks(barchdata::const_iterator begin,
                         barchdata::const_iterator end);
  static bool all_whites(barchdata::const_iterator begin,
                         barchdata::const_iterator end);

  std::vector<bool> analyze_lines(BMPImagePtr image);

  barchdata huffman_compress(const barchdata& line);

  barchdata get_encoded(barchdata::const_iterator begin,
                        barchdata::const_iterator end, unsigned char& dst,
                        unsigned char& dst_left);
  barchdata get_encoded_as_is(barchdata::const_iterator begin,
                              barchdata::const_iterator end, unsigned char& dst,
                              unsigned char& dst_left);
  static barchdata get_encoded_blacks(unsigned char& dst,
                                      unsigned char& dst_left);
  static barchdata get_encoded_whites(unsigned char& dst,
                                      unsigned char& dst_left);
};

using BMP2BarchConverter0Ptr = BMP2BarchConverter0::BMP2BarchConverter0Ptr;

}  // namespace barchclib0::converters

#endif  // THE_BMP_2_BARCH_IMAGE_CODER_PROJECT_BMP2BARCHCONVERTER0_CLASS_H
