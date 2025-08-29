#include "src/lib/libmain/BarchImage.h"

#include <cstddef>
#include <memory>
#include <vector>

namespace barchclib0
{

BarchImagePtr BarchImage::create() { return std::make_shared<BarchImage>(); }

size_t BarchImage::width() { return mwidth; }

size_t BarchImage::height() { return mheight; }

const barchdata& BarchImage::data() { return mdata; }

void BarchImage::width(const size_t& nwidth) { mwidth = nwidth; }

void BarchImage::height(const size_t& nheight) { mheight = nheight; }

void BarchImage::data(const barchdata& ndata) { mdata = ndata; }

void BarchImage::data(barchdata&& ndata) { mdata = std::move(ndata); }

}  // namespace barchclib0
