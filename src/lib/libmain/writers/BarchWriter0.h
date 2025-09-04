#ifndef THE_BMP_2_BARCH_IMAGE_CODER_PROJECT_BARCHIMAGEWRITER_CLASS_H
#define THE_BMP_2_BARCH_IMAGE_CODER_PROJECT_BARCHIMAGEWRITER_CLASS_H

#include <limits>
#include <memory>
#include <vector>

#include "IBarchImage.h"
#include "src/lib/libmain/converters/BMPAndBarchConverter0Base.h"
#include "src/lib/libmain/images/BarchImage.h"

namespace barchclib0::writers
{

/**
 * @brief The barch image writer class v0.
 */
class BarchWriter0 : public std::enable_shared_from_this<BarchWriter0>,
                     virtual public converters::BMPAndBarchConverter0Base
{
 public:
  using BarchWriter0Ptr = std::shared_ptr<BarchWriter0>;

  virtual ~BarchWriter0() = default;
  BarchWriter0() = default;

  virtual bool write(BarchImagePtr image);
  virtual bool write(BarchImagePtr image, const std::filesystem::path& dstpath);

  static BarchWriter0Ptr create();

 private:
  inline static constexpr const size_t max_uint32_t =
      static_cast<size_t>(std::numeric_limits<uint32_t>::max());

  bool write(BarchImagePtr image, std::ofstream& dst);

  barchdata collect_lines_data(BarchImagePtr image);

  bool put_data(barchdata& data, std::ofstream& dst);
};

using BarchWriter0Ptr = BarchWriter0::BarchWriter0Ptr;

}  // namespace barchclib0::writers

#endif  // THE_BMP_2_BARCH_IMAGE_CODER_PROJECT_BARCHIMAGEWRITER_CLASS_H
