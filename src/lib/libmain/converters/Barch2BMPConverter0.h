#ifndef THE_BMP_2_BARCH_IMAGE_CODER_PROJECT_BARCH2BMPCONVERTER0_CLASS_H
#define THE_BMP_2_BARCH_IMAGE_CODER_PROJECT_BARCH2BMPCONVERTER0_CLASS_H

#include <memory>

#include "src/lib/facade/IBarchImage.h"
#include "src/lib/libmain/converters/BMPAndBarchConverter0Base.h"
#include "src/lib/libmain/images/BMPImage.h"
#include "src/lib/libmain/images/BarchImage.h"

namespace barchclib0::converters
{

/**
 * @brief The Barch 2 BMP image converter class v.0.
 */
class Barch2BMPConverter0
    : public std::enable_shared_from_this<Barch2BMPConverter0>,
      virtual public BMPAndBarchConverter0Base
{
 public:
  using Barch2BMPConverter0Ptr = std::shared_ptr<Barch2BMPConverter0>;

  virtual ~Barch2BMPConverter0() = default;
  Barch2BMPConverter0() = default;

  virtual BMPImagePtr convert(BarchImagePtr barch);

  static Barch2BMPConverter0Ptr create();

 private:
  barchdata huffman_decompress(const barchdata& src, const size_t& width);

  void fill_whites(barchdata& dst, const size_t& width);
  void fill_blacks(barchdata& dst, const size_t& width);
  void copy_arbitrary(barchdata& dst, const size_t& width,
                      barchdata::const_iterator& liter,
                      barchdata::const_iterator lend, unsigned char& cc,
                      unsigned char& ccount);

  void insert_white_pixel(barchdata& dst);
  void insert_black_pixel(barchdata& dst);
  void insert_pixel(barchdata& dst, const int& color);
};

using Barch2BMPConverter0Ptr = Barch2BMPConverter0::Barch2BMPConverter0Ptr;

}  // namespace barchclib0::converters

#endif  // THE_BMP_2_BARCH_IMAGE_CODER_PROJECT_BARCH2BMPCONVERTER0_CLASS_H
