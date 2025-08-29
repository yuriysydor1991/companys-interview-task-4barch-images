#ifndef THE_BMP_2_BARCH_IMAGE_CODER_PROJECT_BMP_BINARY_DATA_DECLS_CLASS_H
#define THE_BMP_2_BARCH_IMAGE_CODER_PROJECT_BMP_BINARY_DATA_DECLS_CLASS_H

#include <cstdint>

namespace barchclib0::readers
{
#pragma pack(push, 1)

struct BITMAPFILEHEADER
{
  uint16_t bfType;       // 'BM'
  uint32_t bfSize;       // file size
  uint16_t bfReserved1;  // must be 0
  uint16_t bfReserved2;  // must be 0
  uint32_t bfOffBits;    // offset to pixel data
};

struct BITMAPINFOHEADER
{
  uint32_t biSize;          // header size (40 bytes)
  int32_t biWidth;          // width in pixels
  int32_t biHeight;         // height in pixels
  uint16_t biPlanes;        // always 1
  uint16_t biBitCount;      // bits per pixel
  uint32_t biCompression;   // compression method
  uint32_t biSizeImage;     // image size (may be 0 if uncompressed)
  int32_t biXPelsPerMeter;  // horizontal resolution
  int32_t biYPelsPerMeter;  // vertical resolution
  uint32_t biClrUsed;       // colors in palette
  uint32_t biClrImportant;  // important colors
};

#pragma pack(pop)

}  // namespace barchclib0::readers

#endif  // THE_BMP_2_BARCH_IMAGE_CODER_PROJECT_BMP_BINARY_DATA_DECLS_CLASS_H
