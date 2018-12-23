#include "gtest/gtest.h"
#include "cell.h"
#include "common_definitions.h"

TEST(CellTest, BoundaryNumber) {
  int counter = 0;
  // Iterate over cells
  for (int id = 0; id < CELL_NUMBER; ++id)
  {
    Cell cell(id);
    // Check out the syntax of C++ STL map if you are not familiar
    // It's like the dictionary in Python
    map<DIRECTION, int>::iterator it;
    for (it = cell.neighbour_ids_.begin(); it != cell.neighbour_ids_.end(); it++)
    {
      // Purely for test purpose
      if (it->second == OUTSIDE_CELL_ID)
        counter++;   
    }
  }
  EXPECT_EQ(counter, 4*LINE_CELL_NUMBER);
}

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

