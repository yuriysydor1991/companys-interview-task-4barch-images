#include "src/lib/libmain/BMPImage.h"

#include <cassert>
#include <cstddef>
#include <memory>
#include <vector>

#include "src/log/log.h"

namespace barchclib0
{

BMPImagePtr BMPImage::create() { return std::make_shared<BMPImage>(); }

size_t BMPImage::width() const { return mwidth; }

size_t BMPImage::height() const { return mheight; }

const barchdata& BMPImage::data() const { return mdata; }

void BMPImage::width(const size_t& nwidth) { mwidth = nwidth; }

void BMPImage::height(const size_t& nheight) { mheight = nheight; }

void BMPImage::data(const barchdata& ndata) { mdata = ndata; }

void BMPImage::data(barchdata&& ndata) { mdata = std::move(ndata); }

unsigned int BMPImage::bits_per_pixel() { return mbitspp; }

void BMPImage::bits_per_pixel(const unsigned int& nbits) { mbitspp = nbits; }

void BMPImage::filepath(const std::filesystem::path& npath) { mpath = npath; }

const std::filesystem::path& BMPImage::filepath() const { return mpath; }

PixelPtr BMPImage::pixel(const size_t& col, const size_t& row) const
{
  if (col >= width()) {
    LOGE("Invalid col index provided " << col << " (" << width() << ")");
    return {};
  }

  if (row >= height()) {
    LOGE("Invalid row index provided " << row << " (" << height() << ")");
    return {};
  }

  auto pix = std::make_shared<Pixel>();

  pix->col = col;
  pix->row = row;

  const size_t dataIndex = get_data_index(col, row);

  if (dataIndex >= mdata.size()) {
    LOGE("Insuficient data available for index " << dataIndex << " ("
                                                 << mdata.size() << ")");
    return {};
  }

  assert(mbitspp == 8U && "Currently only grayscale images are supported");

  if (mbitspp == 8U) {
    pix->y = mdata[dataIndex];
  } else {
    LOGE("Invalid pixel bits count for " << mbitspp);
    return {};
  }

  return pix;
}

size_t BMPImage::get_data_index(const size_t& col, const size_t& row) const
{
  return row * (mbitspp / 8U) * width() + col;
}

size_t BMPImage::get_data_index(const PixelPtr& pix) const
{
  return get_data_index(pix->col, pix->row);
}

void BMPImage::pixel(const PixelPtr& nval)
{
  if (nval == nullptr) {
    LOGE("Invalid pointer provided");
    return;
  }

  const size_t dataIndex = get_data_index(nval);

  if (dataIndex >= mdata.size()) {
    LOGE("Insuficient data available for index " << dataIndex << " ("
                                                 << mdata.size() << ")");
    return;
  }

  assert(mbitspp == 8U && "Currently only grayscale images are supported");

  if (mbitspp == 8U) {
    nval->y = mdata[dataIndex];
  } else {
    LOGE("Invalid pixel bits count for " << mbitspp);
    return;
  }

  if (nval->y >= 0) {
    mdata[dataIndex] = nval->y;
  } else {
    LOGE("Currently only grayscale images are supported");
    return;
  }
}

barchdata BMPImage::line(const size_t& row) const
{
  if (row >= height()) {
    LOGE("Invalid row index provided " << row << " (" << height() << ")");
    return {};
  }

  const auto startI = mdata.begin() + get_data_index(0U, row);
  const auto endI = mdata.begin() + get_data_index(width() - 1U, row);

  return barchdata(startI, endI);
}

}  // namespace barchclib0
