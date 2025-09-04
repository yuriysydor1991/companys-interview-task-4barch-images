#include "LibraryFacade.h"

#include <cassert>
#include <iostream>
#include <memory>

#include "src/lib/libmain/LibFactory.h"

namespace barchclib0
{

ILibPtr LibraryFacade::create()
{
  auto f = lib0impl::LibFactory::create_factory();

  assert(f != nullptr);

  return f->create_default_lib();
}

}  // namespace barchclib0
