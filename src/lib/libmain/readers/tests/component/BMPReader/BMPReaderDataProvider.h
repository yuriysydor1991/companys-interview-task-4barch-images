#ifndef THE_BMP_2_BARCH_IMAGE_CODER_PROJECT_BMPIMAGECTESTDATAPROVIDER_CLASS_H
#define THE_BMP_2_BARCH_IMAGE_CODER_PROJECT_BMPIMAGECTESTDATAPROVIDER_CLASS_H

#include <cstddef>
#include <memory>
#include <vector>

#include "src/lib/facade/IBarchImage.h"

namespace barchclib0::ctests
{

/**
 * @brief The barch image class.
 */
class BMPReaderDataProvider
{
 public:
  virtual ~BMPReaderDataProvider() = default;
  BMPReaderDataProvider() = default;

  static const barchdata& get_i1_data();
  static const barchdata& get_i2_data();
};

}  // namespace barchclib0::ctests

#endif  // THE_BMP_2_BARCH_IMAGE_CODER_PROJECT_BMPIMAGECTESTDATAPROVIDER_CLASS_H
