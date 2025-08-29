#ifndef THE_BMP_2_BARCH_IMAGE_CODER_PROJECT_BARCHIMAGE_CLASS_H
#define THE_BMP_2_BARCH_IMAGE_CODER_PROJECT_BARCHIMAGE_CLASS_H

#include <cstddef>
#include <memory>
#include <vector>

#include "src/lib/facade/IBarchImage.h"

namespace barchclib0
{

/**
 * @brief The barch image class.
 */
class BarchImage : public std::enable_shared_from_this<BarchImage>,
                   virtual public IBarchImage
{
 public:
  using BarchImagePtr = std::shared_ptr<BarchImage>;

  virtual ~BarchImage() = default;
  BarchImage() = default;

  /*
   * @brief Create instance of the BarchImage and return it's std::shared_ptr
   */
  static BarchImagePtr create();

  virtual size_t width() override;
  virtual size_t height() override;

  virtual const barchdata& data() override;

  virtual void width(const size_t& nwidth) override;
  virtual void height(const size_t& nheight) override;

  virtual void data(const barchdata& ndata) override;
  virtual void data(barchdata&& ndata) override;

 private:
  size_t mwidth{0};
  size_t mheight{0};

  barchdata mdata;
};

using BarchImagePtr = BarchImage::BarchImagePtr;
using barchdata = BarchImage::barchdata;

}  // namespace barchclib0

#endif  // THE_BMP_2_BARCH_IMAGE_CODER_PROJECT_BARCHIMAGE_CLASS_H
