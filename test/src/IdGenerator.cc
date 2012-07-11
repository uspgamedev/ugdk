#include "gtest/gtest.h"
#include <ugdk/util/idgenerator.h>

using ugdk::util::IdGenerator;

TEST(IdGenerator, Operations) {
  IdGenerator idgen(1,10,11);
  EXPECT_EQ(1, idgen.Id());
}
