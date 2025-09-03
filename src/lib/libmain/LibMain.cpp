#include "src/lib/libmain/LibMain.h"

#include <cassert>
#include <memory>

#include "src/log/log.h"

namespace lib0impl
{

bool LibMain::bmp_to_barch(IBarchImagePtr barch) { return false; }

bool LibMain::barch_to_bmp(IBarchImagePtr barch) { return false; }

IBarchImagePtr LibMain::read(const std::filesystem::path& imagePath)
{
  return {};
}

bool LibMain::write(IBarchImagePtr barch) { return false; }

LibMain::ILibPtr LibMain::duplicate() { return create(); }

LibMainPtr LibMain::create() { return std::make_shared<LibMain>(); }

}  // namespace lib0impl
