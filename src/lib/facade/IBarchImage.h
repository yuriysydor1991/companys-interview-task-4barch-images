#ifndef THE_BMP_2_BARCH_IMAGE_CODER_PROJECT_IBARCHIMAGE_ABSTRACT_CLASS_H
#define THE_BMP_2_BARCH_IMAGE_CODER_PROJECT_IBARCHIMAGE_ABSTRACT_CLASS_H

#include <cstddef>
#include <filesystem>
#include <memory>
#include <vector>

#include "Pixel.h"

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

  virtual size_t width() const = 0;
  virtual size_t height() const = 0;

  virtual const barchdata& data() = 0;

  virtual void width(const size_t& nwidth) = 0;
  virtual void height(const size_t& nheight) = 0;

  virtual void data(const barchdata& ndata) = 0;
  virtual void data(barchdata&& ndata) = 0;

  virtual void filepath(const std::filesystem::path& npath) = 0;
  virtual const std::filesystem::path& filepath() const = 0;

  virtual PixelPtr pixel(const size_t& col, const size_t& row) const = 0;
  virtual void pixel(const PixelPtr& nval) = 0;

  virtual barchdata line(const size_t& row) const = 0;

  virtual void append_line(const barchdata& nline) = 0;
  
  virtual void clear() = 0;
};

using IBarchImagePtr = IBarchImage::IBarchImagePtr;
using barchdata = IBarchImage::barchdata;

}  // namespace barchclib0

#endif  // THE_BMP_2_BARCH_IMAGE_CODER_PROJECT_IBARCHIMAGE_ABSTRACT_CLASS_H
