#include "gtest/gtest.h"
#include "cell.h"
#include "common_definitions.h"
#include "helper_adapt.h"
#include <numeric>


class HelperAdaptTest : public testing::Test
{
protected:
  void SetUp() override 
  {
      HelperAdapt helper;
  }
};
/* Test for HelperAdapt::find_source */
TEST_F(HelperAdaptTest, FindCellId)
{
  double x = X_2[0] - X_1[0];
  double y = X_2[1] - X_1[1];
  // If the first cell of the first line has the right id
  // Notice id starts at 0
  int id0 = HelperAdaptTest::helper.find_source(X_1[0], X_1[1]);
  EXPECT_EQ(id0, 0);
  // If the middle of the first line has the right id
  int id1 = helper.find_source(X_1[0]+x/2, H/2);
  EXPECT_EQ(id1, LINE_CELL_NUMBER/2 - 1);
  // If the first cell of the second line returns the right id
  int id2 = helper.find_source(X_1[0]+H/2, 2*H);
  EXPECT_EQ(id2, LINE_CELL_NUMBER);
}

/* Test1 for HelperAdapt::compute_step */
// 1. When setting the REFINE_FACTOR to one, and the source is in the center
// the result should be the same with the original
// 2. When setting the REFINE_FACTOR to one, the whole line add up to the correct length
TEST_F(HelperAdaptTest, ComputeStep1)
{
  vec x_step1, x_step2;
  x_step1 = helper.compute_step(20, 31, (X_2[0] - X_1[0])/2 - H/2, 1);
  double x = x_step1[20];
  EXPECT_EQ(x, H);

  x_step2 = helper.compute_step(63, 63, (X_2[0] - X_1[0]) - H/2, 1);
  double length;
  for(vec::iterator it = x_step2.begin(); it != x_step2.end(); ++it)
    length += *it;
  EXPECT_EQ(length, X_2[0] - X_1[0]);
}

/* Test2 for HelperAdapt::compute_step */
// The ratio btw cells is indeed the assigned value REFINE_RATIO
TEST_F(HelperAdaptTest, ComputeStep2)
{
  vec x_step1;
  x_step1 = helper.compute_step(20, 31, (X_2[0] - X_1[0])/2 - H/2, 1.2);
  EXPECT_EQ(x_step1[20], x_step1[19]/1.2);
}

/* Test3 for HelperAdapt::compute_step */
// If the first cell is the source cell
TEST(HelperAdaptTest, ComputeStep3)
{
  HelperAdapt helper;
  vec x_step1;
  x_step1 = helper.compute_step(0, 0, H/2, 1.2);
  EXPECT_EQ(x_step1[0], H);
}

/* Test4 for HelperAdapt::compute_step. */
// A case where the number is even = 4
// A case where the number is odd = 5
TEST(HelperAdaptTest, ComputeStep4_Even)
{
  HelperAdapt helper;
  vec x_step2;
  x_step2 = helper.compute_step(4, 4, 4.5*H, 1.2);
  EXPECT_FLOAT_EQ(x_step2[0], 4.5/5.38*H); 
  EXPECT_FLOAT_EQ(x_step2[1], 1.2*4.5/5.38*H); 
  EXPECT_FLOAT_EQ(x_step2[2], 1.44*4.5/5.38*H);
  EXPECT_FLOAT_EQ(x_step2[3], 1.2*4.5/5.38*H);  
  EXPECT_FLOAT_EQ(x_step2[4], 4.5/5.38*H);
}

TEST_F(HelperAdaptTest, ComputeStep4_Odd)
{
  vec x_step3;
  x_step3 = helper.compute_step(5, 5, 5.5*H, 1.2);
  EXPECT_FLOAT_EQ(x_step3[0], 5.5/6.78*H);  
  EXPECT_FLOAT_EQ(x_step3[1], 1.2*5.5/6.78*H);  
  EXPECT_FLOAT_EQ(x_step3[2], 1.44*5.5/6.78*H); 
  EXPECT_FLOAT_EQ(x_step3[3], 1.44*5.5/6.78*H); 
  EXPECT_FLOAT_EQ(x_step3[4], 1.2*5.5/6.78*H); 
  EXPECT_FLOAT_EQ(x_step3[5], 5.5/6.78*H);
}


TEST(CellTest, BoundaryNumber) 
{
  int counter = 0;
  // Iterate over cells
  for (int id = 0; id < CELL_NUMBER; ++id)
  {
    // Cell *cell = new Cell(0);
    Cell cell(id);
    map<DIRECTION, int>::iterator it;
    for (it = cell.neighbour_ids_.begin(); it != cell.neighbour_ids_.end(); it++)
    {
      if (it->second == OUTSIDE_CELL_ID)
        counter++;  
    }
  }
  EXPECT_EQ(counter, 4*LINE_CELL_NUMBER);
}

/* Test1 for Cell::ad_coordinates() */
// For the first line of cells, is the total length equal to added cell size
TEST(CellTest, AdaptiveMesh1)
{
  double length = 0;
  for (int id = 0; id < LINE_CELL_NUMBER; ++id)
  {
    Cell cell(id, 32, (X_2[0] - X_1[0])/2 - H/2, H/2, 1);
    length += cell.H_[0];
  }
  EXPECT_EQ(length, X_2[0] - X_1[0]);

  length = 0;
  for (int id = 0; id < LINE_CELL_NUMBER; ++id)
  {
    Cell cell(id, 63, (X_2[0] - X_1[0])- H/2, H/2, 1);
    length += cell.H_[0];
  }
  EXPECT_EQ(length, X_2[0] - X_1[0]);
}






// TEST(CellTest, CellId)
// {
//   x_source = (X_2[0] - X_1[0])/2 + X_1[0];
//   y_source = H/2;
//   id = 31;
//   id_source = LINE_CELL_NUMBER/2 - 1;
//   Cell cell(int id, int id_source, double x_source, double y_source);
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
