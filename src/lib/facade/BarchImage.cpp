#include "src/lib/facade/BarchImage.h"

#include <memory>

#include "ILib.h"
#include "LibraryContext.h"

namespace templatelib0
{

BarchImagePtr BarchImage::create() { return std::make_shared<BarchImage>(); }

size_t BarchImage::width() { return mwidth; }

size_t BarchImage::height() { return mheight; }

const barchdata& BarchImage::data() { return mdata; }

}  // namespace templatelib0
