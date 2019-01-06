#include "cell.h"

Cell::Cell(int id)
{

  // To do: Pass flags to selectively initiate class variables
  // This is important to enhance performance!
  id_ = id;
  ini_coordinates();
  ini_neighbours();
}

Cell::Cell(int id, int id_source, double x_source, double y_source)
: 
id_(id), 
id_source_(id_source),
x_source_(x_source),
y_source_(y_source)
{
  ad_coordinates();
  ini_neighbours();
}

void Cell::ini_coordinates()
{
  int index_1 = id_%LINE_CELL_NUMBER;
  int index_2 = id_/LINE_CELL_NUMBER;
  double step_x = (X_2[0] - X_1[0])/LINE_CELL_NUMBER;
  double step_y = (X_2[1] - X_1[1])/LINE_CELL_NUMBER;

  vec x_lower_left{X_1[0]+step_x*index_1, X_1[1]+step_y*index_2};

  for (int i = 0; i < VERTICES_PER_LINE; ++i)
  {
    for (int j = 0; j < VERTICES_PER_LINE; ++j)
    {
      vec v;
      v.push_back(x_lower_left[0] + step_x*j);
      v.push_back(x_lower_left[1] + step_y*i);
      vertices_.push_back(v);
    }
  }
  cell_center_.push_back(x_lower_left[0] + step_x/2);
  cell_center_.push_back(x_lower_left[1] + step_y/2);


  // Rubbish code, needed to be changed
  vec v_west;
  v_west.push_back(cell_center_[0] - step_x/2);
  v_west.push_back(cell_center_[1]);  
  face_centers_[WEST] = v_west;

  vec v_east;
  v_east.push_back(cell_center_[0] + step_x/2);
  v_east.push_back(cell_center_[1]);  
  face_centers_[EAST] = v_east;

  vec v_south;
  v_south.push_back(cell_center_[0]);
  v_south.push_back(cell_center_[1] - step_y/2);  
  face_centers_[SOUTH] = v_south;

  vec v_north;
  v_north.push_back(cell_center_[0]);
  v_north.push_back(cell_center_[1] + step_y/2);  
  face_centers_[NORTH] = v_north;    
}

void Cell::ad_coordinates()   
{
  int index_1 = id_%LINE_CELL_NUMBER;
  int index_2 = id_/LINE_CELL_NUMBER;
  int index_1_source = id_source_%LINE_CELL_NUMBER;
  int index_2_source = id_source_/LINE_CELL_NUMBER;
  // This can later be an input parameter, it has to be a number bigger than 1
  // We could even specify different law for refinement
  int REFINE_FACTOR = 1.2;
  // Use four coordinate to fully determine the cell position and size
  double x_left, x_right, y_lower, y_higher;

  // Calculate x coordinate
  double denominator = 0;
  vec x_step;
  // To make reuse of code easier, pass index & index_source to m & m0,
  // x0 is the center cell x_step as well as the boundary cell x_step
  int m0,m;
  double x0, x_distance;
  // If the current cell is on the left of the source cell
  if (index_1 < index_1_source)
  {
    m = index_1;
    m0 = index_1_source;
    x_distance = x_source_ - X_1[0];

    // If index_1_source is an odd number
    if (m0%2 ==1)
    {
      for (int i=0; i < m0/2; ++i)
        denominator += 2*pow(REFINE_FACTOR, i);
      denominator += -0.5 + pow(REFINE_FACTOR, m0/2+1);
      x0 = x_distance/denominator;
      // Calculating each cell width, put them in an array
      for (int i=0; i < m0; ++i)
      {
        int power = (m0 - 1)/2 - abs(m - (m0+1)/2);
        x_step.push_back(pow(REFINE_FACTOR, power)*x0);
      }
    }
    // If index_1_source_ is an even number 
    else
    {
      for (int i=0; i < m0/2; ++i)
        denominator += 2*pow(REFINE_FACTOR, i);
      denominator += -0.5;
      x0 = x_distance/denominator;
      for (int i=0; i < m0; ++i)
      {
        int power = (m0/2 - 1 - abs(m - m0/2));
        x_step.push_back(pow(REFINE_FACTOR, power)*x0);
      }
    }

    // After getting x_step[m], computing the x coordinate
    double x_left = X_1[0];
    for (int i=0; i < m-1; ++i)
      x_left += x_step[i]; 
    double x_right = x_left + x_step[m-1];
    cell_center_.push_back((x_left + x_right)/2.);

  }
  // If the current cell is on the right of the source cell
  // The majority of the code is the same, only to use different m, m0 and x_distance
  else if (index_1 > index_1_source)
  {
    m = index_1 - index_1_source + 1;
    m0 = LINE_CELL_NUMBER + 1;
    x_distance = X_2[0] - x_source_;

    if (m0%2 ==1)
    {
      for (int i=0; i < m0/2; ++i)
        denominator += 2*pow(REFINE_FACTOR, i);
      denominator += -0.5 + pow(REFINE_FACTOR, m0/2+1);
      x0 = x_distance/denominator;
      // Calculating each cell width, put them in an array
      for (int i=0; i < m0; ++i)
      {
        int power = (m0 - 1)/2 - abs(m - (m0+1)/2);
        x_step.push_back(pow(REFINE_FACTOR, power)*x0);
      }
    }
    // If index_1_source_ is an even number 
    else
    {
      for (int i=0; i < m0/2; ++i)
        denominator += 2*pow(REFINE_FACTOR, i);
      denominator += -0.5;
      x0 = x_distance/denominator;
      for (int i=0; i < m0; ++i)
      {
        int power = (m0/2 - 1 - abs(m - m0/2));
        x_step.push_back(pow(REFINE_FACTOR, power)*x0);
      }
    }

    // Don't forget that now we add from x_source_ instead of X_1[0]
    double x_left = x_source_;
    for (int i=0; i < m-1; ++i)
      x_left += x_step[i]; 
    double x_right = x_left + x_step[m-1];
    cell_center_.push_back((x_left + x_right)/2.);
  }

  // If the current cell is the source cell
  else
  {
    m0 = index_1_source;
    x_distance = x_source_ - X_1[0];
    
    // If index_1_source is an odd number
    if (m0%2 ==1)
    {
      for (int i=0; i < m0/2; ++i)
        denominator += 2*pow(REFINE_FACTOR, i);
      denominator += -0.5 + pow(REFINE_FACTOR, m0/2+1);
      x0 = x_distance/denominator;
    }
    else
    {
      for (int i=0; i < m0/2; ++i)
        denominator += 2*pow(REFINE_FACTOR, i);
      denominator += -0.5;
      x0 = x_distance/denominator;
    }

    cell_center_.push_back(x_source_);
    x_left = x_source_ - x0/2;
    double x_right = x_left + x0;
  }

  // Calculating Y coordinate
  vec y_step;

  int n0,n;
  double y0, y_distance;

  if (index_2 < index_2_source)
  {
    n = index_2;
    y0 = index_2_source;
    y_distance = y_source_ - X_1[1];

    if (n0%2 ==1)
    {
      for (int i=0; i < n0/2; ++i)
        denominator += 2*pow(REFINE_FACTOR, i);
      denominator += -0.5 + pow(REFINE_FACTOR, n0/2+1);
      y0 = y_distance/denominator;
      for (int i=0; i < n0; ++i)
      {
        int power = (n0 - 1)/2 - abs(n - (n0+1)/2);
        y_step.push_back(pow(REFINE_FACTOR, power)*y0);
      }
    }
    else
    {
      for (int i=0; i < n0/2; ++i)
        denominator += 2*pow(REFINE_FACTOR, i);
      denominator += -0.5;
      y0 = y_distance/denominator;
      for (int i=0; i < n0; ++i)
      {
        int power = (n0/2 - 1 - abs(n - n0/2));
        y_step.push_back(pow(REFINE_FACTOR, power)*y0);
      }
    }

    double y_lower = X_1[1];
    for (int i=0; i < n-1; ++i)
      y_lower += y_step[i]; 
    double y_higher = y_lower + y_step[n-1];
    cell_center_.push_back((y_lower + y_higher)/2.);
  }

  else if (index_2 > index_2_source)
  {
    n = index_2 - index_2_source + 1;
    n0 = LINE_CELL_NUMBER + 1;
    y_distance = X_2[1] - y_source_;

    if (n0%2 ==1)
    {
      for (int i=0; i < n0/2; ++i)
        denominator += 2*pow(REFINE_FACTOR, i);
      denominator += -0.5 + pow(REFINE_FACTOR, n0/2+1);
      y0 = y_distance/denominator;
      // Calculating each cell width, put them in an array
      for (int i=0; i < n0; ++i)
      {
        int power = (n0 - 1)/2 - abs(n - (n0+1)/2);
        y_step.push_back(pow(REFINE_FACTOR, power)*y0);
      }
    }
    // If index_1_source_ is an even number 
    else
    {
      for (int i=0; i < n0/2; ++i)
        denominator += 2*pow(REFINE_FACTOR, i);
      denominator += -0.5;
      y0 = y_distance/denominator;
      for (int i=0; i < n0; ++i)
      {
        int power = (n0/2 - 1 - abs(n - n0/2));
        y_step.push_back(pow(REFINE_FACTOR, power)*y0);
      }
    }

    // Don't forget that now we add from x_source_ instead of X_1[0]
    double y_lower = y_source_;
    for (int i=0; i < n-1; ++i)
      y_lower += y_step[i]; 
    double y_higher = y_lower + y_step[n-1];
    cell_center_.push_back((y_lower + y_higher)/2.);
  }

  // If the current cell is the source cell
  else
  {
    n0 = index_2_source;
    y_distance = y_source_ - X_1[1];
    
    // If index_1_source is an odd number
    if (n0%2 ==1)
    {
      for (int i=0; i < n0/2; ++i)
        denominator += 2*pow(REFINE_FACTOR, i);
      denominator += -0.5 + pow(REFINE_FACTOR, n0/2+1);
      y0 = y_distance/denominator;
    }
    else
    {
      for (int i=0; i < n0/2; ++i)
        denominator += 2*pow(REFINE_FACTOR, i);
      denominator += -0.5;
      y0 = y_distance/denominator;
    }

    cell_center_.push_back(y_source_);
    y_lower = y_source_ - y0/2;
    double y_higher = y_lower + y0;
  }

  vec v_west{x_left, cell_center_[1]}; 
  face_centers_[WEST] = v_west;

  vec v_east{x_right, cell_center_[1]};
  face_centers_[EAST] = v_east;

  vec v_south{cell_center_[0], y_lower};
  face_centers_[SOUTH] = v_south;

  vec v_north{cell_center_[0], y_higher}; 
  face_centers_[NORTH] = v_north;    
}

void Cell::ini_neighbours()
{

  // Western neighbouring cell
  if (id_%LINE_CELL_NUMBER == 0)
    neighbour_ids_[WEST] = OUTSIDE_CELL_ID; 
  else
    neighbour_ids_[WEST] = id_ - 1;

  // Eastern neighbouring cell
  if ((id_+1)%LINE_CELL_NUMBER == 0)
    neighbour_ids_[EAST] = OUTSIDE_CELL_ID;
  else
    neighbour_ids_[EAST] = id_ + 1;

  // Southern neighbouring cell
  if (id_/LINE_CELL_NUMBER == 0)
    neighbour_ids_[SOUTH] = OUTSIDE_CELL_ID;
  else
    neighbour_ids_[SOUTH] = id_ - LINE_CELL_NUMBER;

  // Northern neighbouring cell
  if (id_/LINE_CELL_NUMBER + 1 == LINE_CELL_NUMBER)
    neighbour_ids_[NORTH] = OUTSIDE_CELL_ID;
  else
    neighbour_ids_[NORTH] = id_ + LINE_CELL_NUMBER;


}

