#ifndef THE_BMP_2_BARCH_IMAGE_CODER_PROJECT_BARCHIMAGEWRITER_CLASS_H
#define THE_BMP_2_BARCH_IMAGE_CODER_PROJECT_BARCHIMAGEWRITER_CLASS_H

#include <memory>
#include <vector>

#include "src/lib/facade/IBarchImage.h"
#include "src/lib/libmain/BMPImage.h"

namespace barchclib0::writers
{

/**
 * @brief The barch image writer class.
 */
class BarchWriter : public std::enable_shared_from_this<IBarchImage>
{
 public:
  inline static const unsigned int batch_pixels_compress = 4U;
  inline static const unsigned int min_opt_2_compress = 2U;

  using BarchWriterPtr = std::shared_ptr<BarchWriter>;

  virtual ~BarchWriter() = default;
  BarchWriter() = default;

  virtual bool write(BMPImagePtr image);
  virtual bool write(BMPImagePtr image, barchdata& dst);

 private:
  static bool optimal_to_compress(BMPImagePtr image, const size_t& rowIndex);

  static bool is_white(const PixelPtr& pix);

  static std::vector<bool> analyze_lines(BMPImagePtr image);
};

using BarchWriterPtr = BarchWriter::BarchWriterPtr;

}  // namespace barchclib0::writers

#endif  // THE_BMP_2_BARCH_IMAGE_CODER_PROJECT_BARCHIMAGEWRITER_CLASS_H
