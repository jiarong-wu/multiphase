#include "cell.h"

Cell::Cell(int id)
:
id_(id), 
id_source_(0),
x_source_(0),
y_source_(0)
{
  // To do: Pass flags to selectively initiate class variables
  // This is important to enhance performance!
  HelperAdapt helper_adapt_cell_;
  ini_coordinates();
  ini_neighbours();
}

Cell::Cell(int id, int id_source, double x_source, double y_source, double REFINE_FACTOR)
: 
id_(id), 
id_source_(id_source),
x_source_(x_source),
y_source_(y_source)
{
  HelperAdapt helper_adapt_cell_;
  ad_coordinates(REFINE_FACTOR);
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

// compute _step function write to x (or y) steps of all the cells that are to the left (or below)
// and it returns by value the source cell size


void Cell::ad_coordinates(double REFINE_FACTOR)   
{
  int index_1 = id_%LINE_CELL_NUMBER;
  int index_2 = id_/LINE_CELL_NUMBER;
  int index_1_source = id_source_%LINE_CELL_NUMBER;
  int index_2_source = id_source_/LINE_CELL_NUMBER;
  // This can later be an input parameter, it has to be a number bigger than 1
  // We could even specify different law for refinement
  // Use four coordinate to fully determine the cell position and size
  double x_left, x_right, y_lower, y_higher;

  // Calculating x coordinate
  vec x_step;
  int m, m0;
  double x_distance;
  // If the current cell is on the right of the source cell
  if (index_1 < index_1_source)
  {
    m = index_1;
    m0 = index_1_source;
    x_distance = x_source_ - X_1[0];
    x_step = helper_adapt_cell_.compute_step(m, m0, x_distance, REFINE_FACTOR);
    // After getting x_step[m], computing the x coordinate
    x_left = X_1[0];
    for (int i=0; i < m; ++i)
      x_left += x_step[i]; 
    x_right = x_left + x_step[m];
    cell_center_.push_back((x_left + x_right)/2.);
    H_.push_back(x_right - x_left);
  }
  // If the current cell is on the right of the source cell
  // The majority of the code is the same, only to use different m, m0 and x_distance
  else if (index_1 > index_1_source)
  {
    m = index_1 - index_1_source;
    m0 = LINE_CELL_NUMBER - 1 - index_1_source;
    x_distance = X_2[0] - x_source_;
    x_step = helper_adapt_cell_.compute_step(m, m0, x_distance, REFINE_FACTOR);
    // Don't forget that now we add from x_source_ instead of X_1[0]
    x_left = x_source_ + x_step[0]/2;
    for (int i=1; i < m; ++i)
      x_left += x_step[i]; 
    x_right = x_left + x_step[m];
    cell_center_.push_back((x_left + x_right)/2.);
    H_.push_back(x_right - x_left);
  }

  // If the current cell is the source cell
  // This is a bit tricky: due to the current algorithm, we need to first calculate 
  // the size from the left and from the right, perform an average and then nudge the
  // center a little bit, leaving cell_center_ shifted from x_source_ a little
  else
  {
    m = index_1;
    m0 = index_1_source;
    x_distance = x_source_ - X_1[0];  
    x_step = helper_adapt_cell_.compute_step(m, m0, x_distance, REFINE_FACTOR);
    x_left = x_source_ - x_step[m]/2;
    x_step.clear();
    m = index_1 - index_1_source;
    m0 = LINE_CELL_NUMBER - 1 -index_1_source;
    x_distance = X_2[0] - x_source_;
    x_step = helper_adapt_cell_.compute_step(m, m0, x_distance, REFINE_FACTOR);
    x_right = x_source_ + x_step[0]/2;
    cell_center_.push_back((x_left + x_right)/2.);
    H_.push_back(x_right - x_left);
  }

  // Calculating Y coordinate
  vec y_step;
  int n0,n;
  double y_distance;

  if (index_2 < index_2_source)
  {
    n = index_2;
    n0 = index_2_source;
    y_distance = y_source_ - X_1[1];
    y_step = helper_adapt_cell_.compute_step(n, n0, y_distance, REFINE_FACTOR);
    y_lower = X_1[1];
    for (int i=0; i < n; ++i)
      y_lower += y_step[i]; 
    y_higher = y_lower + y_step[n];
    cell_center_.push_back((y_lower + y_higher)/2.);
    H_.push_back(y_higher - y_lower);
  }

  else if (index_2 > index_2_source)
  {
    n = index_2 - index_2_source;
    n0 = LINE_CELL_NUMBER - 1 - index_2_source;
    y_distance = X_2[1] - y_source_;
    y_step = helper_adapt_cell_.compute_step(n, n0, y_distance, REFINE_FACTOR);
    y_lower = y_source_;
    for (int i=0; i < n; ++i)
      y_lower += y_step[i]; 
    y_higher = y_lower + y_step[n];
    cell_center_.push_back((y_lower + y_higher)/2.);
    H_.push_back(y_higher - y_lower);
  }

  // If the current cell is the source cell
  else
  {
    n = index_2;
    n0 = index_2_source;
    y_distance = y_source_ - X_1[1]; 
    y_step = helper_adapt_cell_.compute_step(n, n0, y_distance, REFINE_FACTOR);
    y_lower = y_source_ - y_step[n]/2;
    y_step.clear();
    n = index_2 - index_2_source;
    n0 = LINE_CELL_NUMBER - 1 - index_2_source;
    y_distance = X_2[1] - y_source_;
    y_step = helper_adapt_cell_.compute_step(n, n0, y_distance, REFINE_FACTOR);
    y_higher = y_source_ + y_step[0]/2;
    cell_center_.push_back((y_lower + y_higher)/2.);
    H_.push_back(y_higher - y_lower);
  }

  vec v_west{x_left, cell_center_[1]}; 
  face_centers_[WEST] = v_west;

  vec v_east{x_right, cell_center_[1]};
  face_centers_[EAST] = v_east;

  vec v_south{cell_center_[0], y_lower};
  face_centers_[SOUTH] = v_south;

  vec v_north{cell_center_[0], y_higher}; 
  face_centers_[NORTH] = v_north; 

  // Computing the vertices 
  vertices_.push_back({x_left, y_lower});
  vertices_.push_back({x_right, y_lower});
  vertices_.push_back({x_left, y_higher});
  vertices_.push_back({x_right, y_higher});
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

Cell::~Cell()
{

}