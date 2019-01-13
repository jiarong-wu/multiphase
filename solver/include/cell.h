/**
 * \class Cell 
 *
 * \brief The abstract base class constructs a single cell that contains geometric info.
 *
 * The information that a cell carries includes its id(id_), an array of vertex coordinate
 * (vertices_), cell center coordinate(cell_center_), cell size(H_), a map that relates 
 * direction to face center(face_centers_), another map that relates direction to neighboring
 * cell ids(neighbour_ids_). There is additional info needed if using adaptive method, including 
 * id_source_, x_source_, y_source_ for the id and xy coordinate of the cell respectively.
 * 
 * The ini_coordinate method initializes coordinate with uniform mesh. The adapt_coordinate method 
 * adapts the mesh given the source coordinate and id. The ini_neighborhood method intializes the 
 * neighboring ids.
 * 
 *
 */
#ifndef CELL_H
#define CELL_H

#include "common_definitions.h"
#include "helper_adapt.h"

using namespace CellModel;

class Cell
{	
	public:

		/** {id_} is the identification number for each control volume cell.
		   The way we enumerate cell can be shown in the following diagram ({CELL_NUMBER} = 4)
		
			|-----|-----|
			|  2  |  3  |
			|-----|-----|
			|  0  |  1  |
			|-----|-----|
	
		*/
		int id_;

		/** Info for adaptation */
		int id_source_;
		double x_source_;
		double y_source_;

		/** One cell has four vertices. {vertices} are their coordinates. */
		matrix vertices_;

    	/** cell centor coordinates */
    	vec cell_center_;

    	/** cell size, namely H_x, H_y */
    	vec H_;

    	/** Store face center coordinates */
    	map<DIRECTION, vec> face_centers_;

		/** Store neighbouring cell ids. If the neighbour is already outside the boundary, assign {OUTSIDE_CELL_ID} */
    	map<DIRECTION, int> neighbour_ids_;

		/** Constructor */
		Cell(int id);

		/** Constructor with adaptive method */
		Cell(int id, int id_source, double x_source, double y_source, double REFINE_FACTOR);

		/** Destructor */
		~Cell();

		/** Initialize {vertices_} */
		void ini_coordinates();		

		/** Initialize {vertices_} using adaptive method */
		void ad_coordinates(double REFINE_FACTOR);

		/** Initialize {neighbour_ids_} */
		void ini_neighbours();	

	private:
		/** Helper to adapt the cells */
		HelperAdapt helper_adapt_cell_;
};


#endif