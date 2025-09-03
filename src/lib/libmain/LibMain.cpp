#include "src/lib/libmain/LibMain.h"

#include <cassert>
#include <memory>

#include "src/lib/libmain/converters/BMP2BarchConverter0.h"
#include "src/lib/libmain/converters/BMPAndBarchConverter0Base.h"
#include "src/lib/libmain/converters/Barch2BMPConverter0.h"
#include "src/lib/libmain/readers/BMPReader.h"
#include "src/lib/libmain/readers/BarchReader0.h"
#include "src/lib/libmain/writers/BarchWriter0.h"
#include "src/log/log.h"

namespace lib0impl
{

IBarchImagePtr LibMain::bmp_to_barch(IBarchImagePtr bmp)
{
  BMPImagePtr realbmp = std::dynamic_pointer_cast<BMPImage>(bmp);

  if (realbmp == nullptr) {
    LOGE("Not a bmp image provided");
    return {};
  }

  auto converter = barchclib0::converters::BMP2BarchConverter0::create();

  assert(converter != nullptr);

  auto barch = converter->convert(realbmp);

  if (barch == nullptr) {
    LOGE("Fail to convert into the barch");
    return {};
  }

  return barch;
}

IBarchImagePtr LibMain::barch_to_bmp(IBarchImagePtr barch)
{
  BarchImagePtr realb = std::dynamic_pointer_cast<BarchImage>(barch);

  if (realb == nullptr) {
    LOGE("Not a barch image pointer provided");
    return {};
  }

  auto converter = barchclib0::converters::Barch2BMPConverter0::create();

  assert(converter != nullptr);

  auto bmp = converter->convert(realb);

  if (bmp == nullptr) {
    LOGE("Fail to convert into the bmp");
    return {};
  }

  return bmp;
}

IBarchImagePtr LibMain::read(const std::filesystem::path& imagePath)
{
  if (imagePath.empty()) {
    LOGE("Empty path privided");
    return {};
  }

  try {
    if (!std::filesystem::is_regular_file(imagePath)) {
      LOGE("No file found under " << imagePath.string());
      return {};
    }
  }
  catch (std::exception& e) {
    LOGE("Exception during file check " << imagePath);
    return {};
  }

  auto reader = create_reader(imagePath);

  if (reader == nullptr) {
    LOGE("Fail to create appropriate file reader");
    return {};
  }

  auto barch = reader->unified_read(imagePath);

  if (barch == nullptr) {
    LOGE("Fail to read the file " << imagePath.string());
    return {};
  }

  return barch;
}

LibMain::IReaderPtr LibMain::create_reader(
    const std::filesystem::path& imagePath)
{
  if (barchclib0::readers::BMPReader::is_bmp(imagePath)) {
    LOGD("Seems to require a bmp reader, creating");
    return barchclib0::readers::BMPReader::create();
  }

  if (barchclib0::readers::BarchReader0::is_barch(imagePath)) {
    LOGD("Seems to require a barch reader, creating");
    return barchclib0::readers::BarchReader0::create();
  }

  LOGE("Unrecognized file format provided: " << imagePath);

  return {};
}

bool LibMain::write(IBarchImagePtr barch)
{
  if (barch == nullptr) {
    LOGE("Invalid image pointer privided");
    return false;
  }

  BarchImagePtr realb = std::dynamic_pointer_cast<BarchImage>(barch);

  if (realb == nullptr) {
    LOGE("Invalid image pointer type privided");
    return false;
  }

  auto writer = barchclib0::writers::BarchWriter0::create();

  assert(writer != nullptr);

  if (!writer->write(realb)) {
    LOGE("Fail to write barch image");
    return false;
  }

  return true;
}

LibMain::ILibPtr LibMain::duplicate() { return create(); }

LibMainPtr LibMain::create() { return std::make_shared<LibMain>(); }

}  // namespace lib0impl
