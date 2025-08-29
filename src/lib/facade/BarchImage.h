#ifndef THE_BMP_2_BARCH_IMAGE_CODER_PROJECT_BARCHIMAGE_CLASS_H
#define THE_BMP_2_BARCH_IMAGE_CODER_PROJECT_BARCHIMAGE_CLASS_H

#include <cstddef>
#include <memory>
#include <vector>

#include "ILib.h"
#include "LibraryContext.h"

namespace barchclib0
{

/**
 * @brief The barch image class.
 */
class BarchImage : public std::enable_shared_from_this<BarchImage>
{
 public:
  using BarchImagePtr = std::shared_ptr<BarchImage>;
  using barchdata = std::vector<char>;

  virtual ~BarchImage() = default;
  BarchImage() = default;

  /*
   * @brief Create instance of the BarchImage and return it's std::shared_ptr
   */
  static BarchImagePtr create();

  size_t width();
  size_t height();

  const barchdata& data();

  void width(const size_t& nwidth);
  void height(const size_t& nheight);

  void data(const barchdata& ndata);
  void data(barchdata&& ndata);

 private:
  size_t mwidth{0};
  size_t mheight{0};

  barchdata mdata;
};

using BarchImagePtr = BarchImage::BarchImagePtr;
using barchdata = BarchImage::barchdata;

}  // namespace barchclib0

#endif  // THE_BMP_2_BARCH_IMAGE_CODER_PROJECT_BARCHIMAGE_CLASS_H
