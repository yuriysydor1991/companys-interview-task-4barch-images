#include "src/lib/libmain/BMPImage.h"

#include <cstddef>
#include <memory>
#include <vector>

namespace barchclib0
{

BMPImagePtr BMPImage::create() { return std::make_shared<BMPImage>(); }

size_t BMPImage::width() { return mwidth; }

size_t BMPImage::height() { return mheight; }

const barchdata& BMPImage::data() { return mdata; }

void BMPImage::width(const size_t& nwidth) { mwidth = nwidth; }

void BMPImage::height(const size_t& nheight) { mheight = nheight; }

void BMPImage::data(const barchdata& ndata) { mdata = ndata; }

void BMPImage::data(barchdata&& ndata) { mdata = std::move(ndata); }

}  // namespace barchclib0
