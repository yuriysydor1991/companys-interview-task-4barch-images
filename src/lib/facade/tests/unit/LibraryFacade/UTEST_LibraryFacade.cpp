#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "LibraryFacade.h"
#include "src/lib/libmain/LibFactory.h"
#include "src/lib/libmain/LibMain.h"

using namespace barchclib0;
using namespace lib0impl;
using namespace testing;

class UTEST_LibraryFacade : public Test
{
 public:
  using LibFactory = lib0impl::LibFactory;

  UTEST_LibraryFacade() : facade{std::make_shared<LibraryFacade>()}
  {
    EXPECT_NE(facade, nullptr);
  }

  std::shared_ptr<LibraryFacade> facade;
};

TEST_F(UTEST_LibraryFacade, create_success)
{
  MockFunction<void(LibFactory&)> mockEnsurer;

  EXPECT_CALL(mockEnsurer, Call)
      .Times(1)
      .WillOnce(Invoke([](LibFactory& instance) {
        EXPECT_CALL(instance, create_default_lib()).Times(1);
      }));

  LibFactory::onMockCreate = mockEnsurer.AsStdFunction();

  auto res = facade->create();
}
