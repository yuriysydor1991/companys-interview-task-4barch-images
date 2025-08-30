#include "src/lib/libmain/BarchImage.h"

#include <cstddef>
#include <memory>
#include <vector>

#include "src/log/log.h"

namespace barchclib0
{

BarchImagePtr BarchImage::create() { return std::make_shared<BarchImage>(); }

size_t BarchImage::width() const { return mwidth; }

size_t BarchImage::height() const { return mheight; }

const barchdata& BarchImage::data() const
{
  barchdata rdata;

  rdata.reserve(width() * height());

  for (const auto& line : mdata) {
    rdata.insert(rdata.end(), line.begin(), line.end());
  }

  return rdata;
}

void BarchImage::width(const size_t& nwidth) { mwidth = nwidth; }

void BarchImage::height(const size_t& nheight) { mheight = nheight; }

void BarchImage::data(const barchdata& ndata)
{
  mdata.clear();
  mdata.emplace_back(ndata);
}

void BarchImage::data(barchdata&& ndata)
{
  mdata.clear();
  mdata.emplace_back(ndata);
  ndata.clear();
}

PixelPtr BarchImage::pixel(const size_t& col, const size_t& row) const
{
  LOGE("Not implemented");
  return nullptr;
}

void BarchImage::pixel(const PixelPtr& nval) { LOGE("Not implemented"); }

barchdata BarchImage::line(const size_t& row) const
{
  if (mdata.size() <= row) {
    LOGE("Index " << row << " is out of range for max " << mdata.size());
    return {};
  }

  return mdata[row];
}

void BarchImage::append_line(const barchdata& nline)
{
  mdata.emplace_back(nline);

  mheight++;
}

void BarchImage::filepath(const std::filesystem::path& npath) { mpath = npath; }
const std::filesystem::path& BarchImage::filepath() const { return mpath; }

void BarchImage::lines_table(const linestable& ntable) { linest = ntable; }

void BarchImage::lines_table(linestable&& ntable)
{
  linest = std::move(ntable);
}

const linestable& BarchImage::lines_table() const { return linest; }

}  // namespace barchclib0
