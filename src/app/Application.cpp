#include "src/app/Application.h"

#include <cassert>
#include <filesystem>
#include <iostream>
#include <memory>

#include "src/log/log.h"
#include "src/qt6/Qt6Initer.h"

namespace app
{

int Application::run(std::shared_ptr<ApplicationContext> ctx)
{
  assert(ctx != nullptr);

  if (ctx == nullptr) {
    LOGE("No valid context pointer provided");
    return INVALID;
  }

  if (!check_and_change_cwd(ctx)) {
    LOGW("Ignoring failure to change CWD");
  }

  std::shared_ptr<Qt6i::Qt6Initer> qt6runner = create_qt6_initer();

  assert(qt6runner != nullptr);

  if (qt6runner->run(ctx) != 0) {
    LOGE("Qt6 controller returned invalid status");
    return INVALID;
  }

  return 0;
}

std::shared_ptr<Qt6i::Qt6Initer> Application::create_qt6_initer()
{
  return std::make_shared<Qt6i::Qt6Initer>();
}

bool Application::check_and_change_cwd(std::shared_ptr<ApplicationContext> ctx)
{
  assert(ctx != nullptr);

  if (ctx->startdir.empty()) {
    LOGD("No start dir provided");
    return true;
  }

  try {
    LOGD("Trying to change CWD to " << ctx->startdir);

    std::filesystem::current_path(ctx->startdir);
  }
  catch (const std::filesystem::filesystem_error& e) {
    std::string ed = "Can not set cwd to " + ctx->startdir + " because: ";
    ed += e.what();
    ctx->push_error(ed);
    LOGE(ed);

    return false;
  }

  return true;
}

}  // namespace app
