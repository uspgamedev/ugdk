
#include "gtest/gtest.h"
#include <ugdk/util/idgenerator.h>

using ugdk::util::IDGenerator;

TEST(IDGenerator, StraightGeneration) {
  IDGenerator idgen(1,10,-1);
  
  for (int i = 1; i <= 10; i++) {
    EXPECT_EQ(i, idgen.GenerateID());
  }

  for (int i = 1; i <= 10; i++) {
    EXPECT_EQ(i, idgen.ReleaseID(i));
  }
}

TEST(IDGenerator, ScrambledGeneration) {
  IDGenerator idgen(-5, -1, 1);

  int id[5];
  for (int i = 0; i < 4; ++i) {
    EXPECT_NE(1, (id[i+1] = idgen.GenerateID()));
    EXPECT_NE(1, (id[i] = idgen.GenerateID()));
    EXPECT_EQ(id[i+1], idgen.ReleaseID(id[i+1]));
  }
}

TEST(IDGenerator, NoMoreIDs) {
  IDGenerator idgen(1,1,-1);

  idgen.GenerateID();
  EXPECT_EQ(-1, idgen.GenerateID());
  EXPECT_EQ(-1, idgen.GenerateID()); // just to make sure
}

TEST(IDGenerator, ReleaseInvalidID) {
  IDGenerator idgen(0,9000,-1);

  EXPECT_EQ(-1, idgen.ReleaseID(-1));
  EXPECT_EQ(-1, idgen.ReleaseID(-20));
  EXPECT_EQ(-1, idgen.ReleaseID(9001));
}

TEST(IDGenerator, MultipleRelease) {
  IDGenerator idgen(1,1,-1);

  int multiply_released_ID = idgen.GenerateID();
  idgen.ReleaseID(multiply_released_ID);
  EXPECT_EQ(-1, idgen.ReleaseID(multiply_released_ID));
}

