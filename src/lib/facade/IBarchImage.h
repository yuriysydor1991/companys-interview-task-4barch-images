#ifndef THE_BMP_2_BARCH_IMAGE_CODER_PROJECT_IBARCHIMAGE_ABSTRACT_CLASS_H
#define THE_BMP_2_BARCH_IMAGE_CODER_PROJECT_IBARCHIMAGE_ABSTRACT_CLASS_H

#include <cstddef>
#include <memory>
#include <vector>

namespace barchclib0
{

/**
 * @brief The barch image abstract class.
 */
class IBarchImage : public std::enable_shared_from_this<IBarchImage>
{
 public:
  using IBarchImagePtr = std::shared_ptr<IBarchImage>;
  using barchdata = std::vector<unsigned char>;

  virtual ~IBarchImage() = default;
  IBarchImage() = default;

  virtual size_t width() = 0;
  virtual size_t height() = 0;

  virtual const barchdata& data() = 0;

  virtual void width(const size_t& nwidth) = 0;
  virtual void height(const size_t& nheight) = 0;

  virtual void data(const barchdata& ndata) = 0;
  virtual void data(barchdata&& ndata) = 0;
};

using IBarchImagePtr = IBarchImage::IBarchImagePtr;
using barchdata = IBarchImage::barchdata;

}  // namespace barchclib0

#endif  // THE_BMP_2_BARCH_IMAGE_CODER_PROJECT_IBARCHIMAGE_ABSTRACT_CLASS_H
