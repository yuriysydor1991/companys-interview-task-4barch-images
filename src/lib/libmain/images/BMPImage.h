#ifndef THE_BMP_2_BARCH_IMAGE_CODER_PROJECT_BMPImage_CLASS_H
#define THE_BMP_2_BARCH_IMAGE_CODER_PROJECT_BMPImage_CLASS_H

#include <cstddef>
#include <memory>
#include <vector>

#include "IBarchImage.h"

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

  virtual size_t width() const override;
  virtual size_t height() const override;

  virtual const barchdata& data() override;

  virtual void width(const size_t& nwidth) override;
  virtual void height(const size_t& nheight) override;

  virtual void data(const barchdata& ndata) override;
  virtual void data(barchdata&& ndata) override;

  virtual unsigned int bits_per_pixel() override;
  virtual void bits_per_pixel(const unsigned int& nbits) override;

  virtual void filepath(const std::filesystem::path& npath) override;
  virtual const std::filesystem::path& filepath() const override;

  virtual PixelPtr pixel(const size_t& col, const size_t& row) const override;
  virtual void pixel(const PixelPtr& nval) override;

  virtual barchdata line(const size_t& row) const override;

  virtual void append_line(const barchdata& nline) override;

  virtual void clear() override;

 private:
  inline static constexpr const unsigned int default_bits_per_pix = 8U;

  size_t get_data_index(const size_t& col, const size_t& row) const;
  size_t get_data_index(const PixelPtr& pix) const;

  size_t mwidth{0};
  size_t mheight{0};

  unsigned int mbitspp{default_bits_per_pix};

  std::filesystem::path mpath;

  barchdata mdata;
};

using BMPImagePtr = BMPImage::BMPImagePtr;

}  // namespace barchclib0

#endif  // THE_BMP_2_BARCH_IMAGE_CODER_PROJECT_BMPImage_CLASS_H
