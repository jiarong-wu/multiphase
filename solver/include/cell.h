/*! \file */
#ifndef CELL_H
#define CELL_H

#include "common_definitions.h"
#include "helper_adapt.h"

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


    // To do: Build specific structures for things like "points" and "tensors"

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
		Cell(int id, int id_source, double x_source, double y_source, double REFINE_FACTOR);

		/** Destructor */
		~Cell();

		/** Initialize {vertices_} */
		void ini_coordinates();		
		void ad_coordinates(double REFINE_FACTOR);

		/** Initialize {neighbour_ids_} */
		void ini_neighbours();	

	private:
		HelperAdapt helper_adapt_cell_;
};


#endif