#ifndef THE_BMP_2_BARCH_IMAGE_CODER_PROJECT_LIBRARYFACADE_CLASS_H
#define THE_BMP_2_BARCH_IMAGE_CODER_PROJECT_LIBRARYFACADE_CLASS_H

#include <memory>

#include "ILib.h"

namespace barchclib0
{

class LibraryFacade
{
 public:
  virtual ~LibraryFacade() = default;
  LibraryFacade() = default;

  virtual ILibPtr create();
};

}  // namespace barchclib0

#endif  // THE_BMP_2_BARCH_IMAGE_CODER_PROJECT_LIBRARYFACADE_CLASS_H
