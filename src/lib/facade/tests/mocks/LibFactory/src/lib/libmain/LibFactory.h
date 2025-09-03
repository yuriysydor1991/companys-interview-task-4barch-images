#ifndef YOUR_CPP_APP_TEMPLATE_PROJECT_LIBFACTORY_CLASS_H
#define YOUR_CPP_APP_TEMPLATE_PROJECT_LIBFACTORY_CLASS_H

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <functional>
#include <memory>
#include <set>

#include "src/lib/facade/ILib.h"

namespace lib0impl
{

class LibFactorySynthParent
{
 public:
  using ILib = barchclib0::ILib;

  virtual ~LibFactorySynthParent() = default;
  LibFactorySynthParent() = default;

  virtual std::shared_ptr<ILib> create_default_lib() = 0;
};

class LibFactory : public LibFactorySynthParent
{
 public:
  virtual ~LibFactory() = default;

  inline static std::function<void(LibFactory&)> onMockCreate;

  LibFactory()
  {
    if (onMockCreate != nullptr) {
      onMockCreate(*this);
    }
  }

  MOCK_METHOD(std::shared_ptr<ILib>, create_default_lib, (), (override));

  inline static std::shared_ptr<LibFactory> create_factory()
  {
    return std::make_shared<LibFactory>();
  }
};

}  // namespace lib0impl

#endif  // YOUR_CPP_APP_TEMPLATE_PROJECT_LIBFACTORY_CLASS_H
