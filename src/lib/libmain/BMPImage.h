#ifndef THE_BMP_2_BARCH_IMAGE_CODER_PROJECT_BMPImage_CLASS_H
#define THE_BMP_2_BARCH_IMAGE_CODER_PROJECT_BMPImage_CLASS_H

#include <cstddef>
#include <memory>
#include <vector>

#include "src/lib/facade/IBarchImage.h"

namespace barchclib0
{

/**
 * @brief The barch image class.
 */
class BMPImage : public std::enable_shared_from_this<BMPImage>,
                 virtual public IBarchImage
{
 public:
  using BMPImagePtr = std::shared_ptr<BMPImage>;

  virtual ~BMPImage() = default;
  BMPImage() = default;

  /*
   * @brief Create instance of the BMPImage and return it's std::shared_ptr
   */
  static BMPImagePtr create();

  virtual size_t width() override;
  virtual size_t height() override;

  virtual const barchdata& data() override;

  virtual void width(const size_t& nwidth) override;
  virtual void height(const size_t& nheight) override;

  virtual void data(const barchdata& ndata) override;
  virtual void data(barchdata&& ndata) override;

  virtual unsigned int bits_per_pixel();
  virtual void bits_per_pixel(const unsigned int& nbits);

 private:
  size_t mwidth{0};
  size_t mheight{0};

  unsigned int mbitspp{0U};

  barchdata mdata;
};

using BMPImagePtr = BMPImage::BMPImagePtr;

}  // namespace barchclib0

#endif  // THE_BMP_2_BARCH_IMAGE_CODER_PROJECT_BMPImage_CLASS_H
