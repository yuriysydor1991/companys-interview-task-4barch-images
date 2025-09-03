#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <bitset>
#include <iostream>

#include "LibMain_includes.h"
#include "LibraryContext.h"
#include "src/lib/libmain/LibMain.h"

using namespace barchclib0;
using namespace lib0impl;
using namespace testing;

class CTEST_LibMain : public Test
{
 public:
  inline static const std::filesystem::path images_root =
      ct_libmain::REAL_IMAGES_ROOT;

  inline static std::filesystem::path i1 = images_root / "test-image-1-gs.bmp";
  inline static std::filesystem::path i2 = images_root / "test-image-2-gs.bmp";

  CTEST_LibMain() : controller{LibMain::create()}
  {
    EXPECT_NE(controller, nullptr);
  }

  LibMainPtr controller;
};

TEST_F(CTEST_LibMain, read_bmp_1_success)
{
  IBarchImagePtr bmp1 = controller->read(i1);

  EXPECT_NE(bmp1, nullptr);

  EXPECT_EQ(bmp1->width(), 825U);
  EXPECT_EQ(bmp1->height(), 1200U);
  EXPECT_EQ(bmp1->bits_per_pixel(), 8U);
  EXPECT_FALSE(bmp1->data().empty());
  EXPECT_EQ(bmp1->data().size(), 990000);
}

TEST_F(CTEST_LibMain, read_bmp_2_success)
{
  IBarchImagePtr bmp2 = controller->read(i2);

  EXPECT_NE(bmp2, nullptr);

  EXPECT_EQ(bmp2->width(), 825U);
  EXPECT_EQ(bmp2->height(), 1200U);
  EXPECT_EQ(bmp2->bits_per_pixel(), 8U);
  EXPECT_FALSE(bmp2->data().empty());
  EXPECT_EQ(bmp2->data().size(), 990000);
}

TEST_F(CTEST_LibMain, convert_bmp_1_success)
{
  IBarchImagePtr bmp1 = controller->read(i1);

  EXPECT_NE(bmp1, nullptr);

  IBarchImagePtr barch1 = controller->bmp_to_barch(bmp1);

  EXPECT_NE(barch1, nullptr);

  EXPECT_EQ(barch1->width(), 825U);
  EXPECT_EQ(barch1->height(), 1200U);
  EXPECT_EQ(barch1->bits_per_pixel(), 8U);
  EXPECT_FALSE(barch1->data().empty());
  EXPECT_EQ(barch1->data().size(), 161111);
}

TEST_F(CTEST_LibMain, convert_bmp_2_success)
{
  IBarchImagePtr bmp2 = controller->read(i2);

  EXPECT_NE(bmp2, nullptr);

  IBarchImagePtr barch2 = controller->bmp_to_barch(bmp2);

  EXPECT_NE(barch2, nullptr);

  EXPECT_EQ(barch2->width(), 825U);
  EXPECT_EQ(barch2->height(), 1200U);
  EXPECT_EQ(barch2->bits_per_pixel(), 8U);
  EXPECT_FALSE(barch2->data().empty());
  EXPECT_EQ(barch2->data().size(), 394358);
}

TEST_F(CTEST_LibMain, restore_bmp_1_success)
{
  IBarchImagePtr bmp1 = controller->read(i1);

  EXPECT_NE(bmp1, nullptr);

  IBarchImagePtr barch1 = controller->bmp_to_barch(bmp1);

  EXPECT_NE(barch1, nullptr);

  IBarchImagePtr restored = controller->barch_to_bmp(barch1);

  EXPECT_EQ(restored->width(), bmp1->width());
  EXPECT_EQ(restored->height(), bmp1->height());
  EXPECT_EQ(restored->bits_per_pixel(), bmp1->bits_per_pixel());
  EXPECT_FALSE(restored->data().empty());
  EXPECT_EQ(restored->data().size(), bmp1->data().size());

  const auto& d1 = bmp1->data();
  const auto& d2 = restored->data();

  EXPECT_EQ(d1.size(), d2.size());

  size_t failures{0U};

  for (size_t biter = 0U; biter < d1.size(); ++biter) {
    EXPECT_EQ(d1[biter], d2[biter]) << "Expect bytes to match at " << biter;

    if (d1[biter] != d2[biter]) {
      failures++;
    }
  }

  EXPECT_EQ(failures, 0U) << "Expecting no failures";

  size_t liter = 0U;
  for (; liter < bmp1->height(); ++liter) {
    auto origl = bmp1->line(liter);
    auto restoredl = restored->line(liter);

    EXPECT_EQ(origl.size(), restoredl.size());

    bool fail = false;
    for (size_t biter = 0U; biter < origl.size() && biter < restoredl.size();
         ++biter) {
      EXPECT_EQ(origl[biter], restoredl[biter])
          << "Expect " << liter << " lines bytes to match at " << biter;

      if (origl[biter] != restoredl[biter]) {
        fail = true;
      }
    }

    if (fail) {
      auto bline = barch1->line(liter);

      for (unsigned int lbiter = 0U; lbiter < bline.size(); ++lbiter) {
        std::cout << "barch line[" << lbiter << "] -> "
                  << std::bitset<8>(bline[lbiter]) << std::endl;
      }
    }
  }
}

TEST_F(CTEST_LibMain, restore_bmp_2_success)
{
  IBarchImagePtr bmp2 = controller->read(i2);

  EXPECT_NE(bmp2, nullptr);

  IBarchImagePtr barch2 = controller->bmp_to_barch(bmp2);

  EXPECT_NE(barch2, nullptr);

  IBarchImagePtr restored = controller->barch_to_bmp(barch2);

  EXPECT_EQ(restored->width(), bmp2->width());
  EXPECT_EQ(restored->height(), bmp2->height());
  EXPECT_EQ(restored->bits_per_pixel(), bmp2->bits_per_pixel());
  EXPECT_FALSE(restored->data().empty());
  EXPECT_EQ(restored->data().size(), bmp2->data().size());

  const auto& d1 = bmp2->data();
  const auto& d2 = restored->data();

  EXPECT_EQ(d1.size(), d2.size());

  size_t failures{0U};

  for (size_t biter = 0U; biter < d1.size(); ++biter) {
    EXPECT_EQ(d1[biter], d2[biter]) << "Expect bytes to match at " << biter;

    if (d1[biter] != d2[biter]) {
      failures++;
    }
  }

  EXPECT_EQ(failures, 0U) << "Expecting no failures";

  size_t liter = 0U;
  for (; liter < bmp2->height(); ++liter) {
    auto origl = bmp2->line(liter);
    auto restoredl = restored->line(liter);

    EXPECT_EQ(origl.size(), restoredl.size());

    bool fail = false;
    for (size_t biter = 0U; biter < origl.size() && biter < restoredl.size();
         ++biter) {
      EXPECT_EQ(origl[biter], restoredl[biter])
          << "Expect " << liter << " lines bytes to match at " << biter;

      if (origl[biter] != restoredl[biter]) {
        fail = true;
      }
    }

    if (fail) {
      auto bline = barch2->line(liter);

      for (unsigned int lbiter = 0U; lbiter < bline.size(); ++lbiter) {
        std::cout << "barch line[" << lbiter << "] -> "
                  << std::bitset<8>(bline[lbiter]) << std::endl;
      }
    }
  }
}
