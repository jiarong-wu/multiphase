#include "gtest/gtest.h"
#include "cell.h"
#include "common_definitions.h"
#include "helper_adapt.h"

TEST(CellTest, BoundaryNumber) 
{
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

TEST(HelperTest, FindCellId)
{
  HelperAdapt helper;
  double x = X_2[1] - X_1[1];
  double y = X_2[1] - X_1[1];
  // If the first cell of the first line has the right id
  // Notice id starts at 0
  int id0 = helper.find_source(X_1[0], X_1[1]);
  EXPECT_EQ(id0, 0);
  // If the middle of the first line has the right id
  int id1 = helper.find_source(X_1[0]+x/2, H/2);
  EXPECT_EQ(id1, LINE_CELL_NUMBER/2 - 1);
  // If the first cell of the second line returns the right id
  int id2 = helper.find_source(X_1[0]+H/2, 2*H);
  EXPECT_EQ(id2, LINE_CELL_NUMBER);
}

// TEST(CellTest, CellId)
// {

//   x_source = 0.5;
//   y_source = 0.5;
//   id = 
//   Cell cell(int id, int id_source, double x_source, double y_source)
// }

// class CellTest : public ::testing::Test
// {
// protected:
//   void SetUp() override 
//   {

//   }
//   void TearDown() override
//   {

//   }
// };


// TEST_F(CellTest, CellId)
// {
//   Cell cell (int id, int id_source, double x_source, double y_source)
// }

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

