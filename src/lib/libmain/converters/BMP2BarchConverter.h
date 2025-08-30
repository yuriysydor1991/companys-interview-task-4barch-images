#ifndef THE_BMP_2_BARCH_IMAGE_CODER_PROJECT_BMP2BARCHCONVERTER_CLASS_H
#define THE_BMP_2_BARCH_IMAGE_CODER_PROJECT_BMP2BARCHCONVERTER_CLASS_H

#include <memory>
#include <vector>

#include "src/lib/facade/IBarchImage.h"
#include "src/lib/libmain/BMPImage.h"
#include "src/lib/libmain/BarchImage.h"

namespace barchclib0::converters
{

/**
 * @brief The barch image writer class.
 */
class BMP2BarchConverter
    : public std::enable_shared_from_this<BMP2BarchConverter>
{
 public:
  using BMP2BarchConverterPtr = std::shared_ptr<BMP2BarchConverter>;

  virtual ~BMP2BarchConverter() = default;
  BMP2BarchConverter() = default;

  virtual BarchImagePtr convert(BMPImagePtr bmp);

 private:
  inline static const unsigned int batch_pixels_compress = 4U;
  inline static const unsigned int min_opt_2_compress = 2U;

  static bool optimal_to_compress(BMPImagePtr image, const size_t& rowIndex);

  static bool is_white(const PixelPtr& pix);

  static std::vector<bool> analyze_lines(BMPImagePtr image);

 private:
  static barchdata compress(const barchdata& line);
};

using BMP2BarchConverterPtr = BMP2BarchConverter::BMP2BarchConverterPtr;

}  // namespace barchclib0::converters

#endif  // THE_BMP_2_BARCH_IMAGE_CODER_PROJECT_BMP2BARCHCONVERTER_CLASS_H
