#ifndef THE_BMP_2_BARCH_IMAGE_CODER_PROJECT_BarchReader0_CLASS_H
#define THE_BMP_2_BARCH_IMAGE_CODER_PROJECT_BarchReader0_CLASS_H

#include <cstddef>
#include <filesystem>
#include <memory>
#include <vector>

#include "src/lib/libmain/converters/BMPAndBarchConverter0Base.h"
#include "src/lib/libmain/images/BarchImage.h"

namespace barchclib0::readers
{

namespace fs = std::filesystem;

/**
 * @brief The BMP files reader class.
 */
class BarchReader0 : public std::enable_shared_from_this<BarchReader0>,
                     virtual public converters::BMPAndBarchConverter0Base
{
 public:
  using BarchReader0Ptr = std::shared_ptr<BarchReader0>;

  virtual ~BarchReader0() = default;
  BarchReader0() = default;

  /*
   * @brief Create instance of the BarchReader0 and return it's std::shared_ptr
   */
  static BarchReader0Ptr create();

  /*
   * @brief Read the image file under given fs path.
   *
   * @returns Returns filled BMPImage instance as a std::shared_ptr or a nullptr
   * value in case of any error.
   */
  virtual BarchImagePtr read(const fs::path& imagePath);

 private:
  BarchImagePtr read_data(const fs::path& imagePath);

  bool check_file_starter(std::ifstream& f);
  bool read_dimentions(BarchImagePtr image, std::ifstream& f);
  bool read_lines_table(BarchImagePtr barch, std::ifstream& f);
  bool split_lines(BarchImagePtr barch, barchdata& idata);
  bool extract_compressed_line(BarchImagePtr barch, barchdata& idata);

  inline static constexpr const unsigned char leftmostone = 0B10000000;
  inline static const std::string BARCH0_STARTER_STR = BARCH0_STARTER;
};

using BarchReader0Ptr = BarchReader0::BarchReader0Ptr;

}  // namespace barchclib0::readers

#endif  // THE_BMP_2_BARCH_IMAGE_CODER_PROJECT_BarchReader0_CLASS_H
