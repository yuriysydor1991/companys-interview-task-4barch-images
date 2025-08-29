#ifndef THE_BMP_2_BARCH_IMAGE_CODER_PROJECT_PIXEL_STRUCTURE_H
#define THE_BMP_2_BARCH_IMAGE_CODER_PROJECT_PIXEL_STRUCTURE_H

#include <cstddef>
#include <memory>

namespace barchclib0
{

/**
 * @brief The single pixel info class.
 */
struct Pixel
{
  int b{-1};
  int g{-1};
  int r{-1};
  int a{-1};

  /// @brief Grayscale value
  int y{-1};

  size_t row{0U};
  size_t col{0U};
};

using PixelPtr = std::shared_ptr<Pixel>;

}  // namespace barchclib0

#endif  // THE_BMP_2_BARCH_IMAGE_CODER_PROJECT_PIXEL_STRUCTURE_H
