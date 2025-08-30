#ifndef THE_BMP_2_BARCH_IMAGE_CODER_PROJECT_BARCHIMAGEWRITER_CLASS_H
#define THE_BMP_2_BARCH_IMAGE_CODER_PROJECT_BARCHIMAGEWRITER_CLASS_H

#include <memory>
#include <vector>
#include <limits>

#include "src/lib/facade/IBarchImage.h"
#include "src/lib/libmain/BartchImage.h"

namespace barchclib0::writers
{

/**
 * @brief The barch image writer class.
 */
class BarchWriter : public std::enable_shared_from_this<BarchWriter>
{
 public:
  using BarchWriterPtr = std::shared_ptr<BarchWriter>;

  virtual ~BarchWriter() = default;
  BarchWriter() = default;

  virtual bool write(BartchImagePtr image);
  virtual bool write(BartchImagePtr image, barchdata& dst);
  
private:
  inline static constexpr const size_t max_uint32_t = static_cast<size_t>(std::numeric_limits<uint32_t>::max());
};

using BarchWriterPtr = BarchWriter::BarchWriterPtr;

}  // namespace barchclib0::writers

#endif  // THE_BMP_2_BARCH_IMAGE_CODER_PROJECT_BARCHIMAGEWRITER_CLASS_H
