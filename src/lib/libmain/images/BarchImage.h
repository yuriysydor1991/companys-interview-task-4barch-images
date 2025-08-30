#ifndef THE_BMP_2_BARCH_IMAGE_CODER_PROJECT_BARCHIMAGE_CLASS_H
#define THE_BMP_2_BARCH_IMAGE_CODER_PROJECT_BARCHIMAGE_CLASS_H

#include <cstddef>
#include <filesystem>
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
  using barchscans = std::vector<barchdata>;
  using linestable = std::vector<bool>;

  virtual ~BarchImage() = default;
  BarchImage() = default;

  /*
   * @brief Create instance of the BarchImage and return it's std::shared_ptr
   */
  static BarchImagePtr create();

  virtual size_t width() const override;
  virtual size_t height() const override;

  virtual const barchdata& data() const override;

  virtual void width(const size_t& nwidth) override;
  virtual void height(const size_t& nheight) override;

  virtual void data(const barchdata& ndata) override;
  virtual void data(barchdata&& ndata) override;

  virtual void filepath(const std::filesystem::path& npath) override;
  virtual const std::filesystem::path& filepath() const override;

  virtual PixelPtr pixel(const size_t& col, const size_t& row) const override;
  virtual void pixel(const PixelPtr& nval) override;

  virtual barchdata line(const size_t& row) const override;

  virtual void append_line(const barchdata& nline) override;

  virtual void lines_table(const linestable& ntable);
  virtual void lines_table(linestable&& ntable);
  virtual const linestable& lines_table() const;
  
  virtual void clear() override;

 private:
  size_t mwidth{0};
  size_t mheight{0};

  barchscans mdata;

  /// @brief The compressed lines table. Vector index corresponds to line index
  /// in the image.
  linestable linest;

  std::filesystem::path mpath;
};

using BarchImagePtr = BarchImage::BarchImagePtr;
using barchscans = BarchImage::barchscans;
using linestable = BarchImage::linestable;

}  // namespace barchclib0

#endif  // THE_BMP_2_BARCH_IMAGE_CODER_PROJECT_BARCHIMAGE_CLASS_H
