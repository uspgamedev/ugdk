#include "gtest/gtest.h"
#include <ugdk/util/idgenerator.h>

using ugdk::util::IdGenerator;

TEST(IdGenerator, Operations) {
  IdGenerator idgen(1,10,11);
  
  for (int i = 1; i < 11; i++) {
    EXPECT_EQ(i, idgen.Id());
  }

  EXPECT_EQ(11, idgen.Id());

  for (int i = 1; i < 11; i++) {
    EXPECT_EQ(i, idgen.RemoveId(i));
  }

  EXPECT_EQ(11, idgen.RemoveId(1));
}
