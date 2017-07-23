#pragma once

#include <string>
using std::string;

#include <vector>
using std::vector;

#include <list>
using std::list;

#include <tuple>
using std::tuple;
using std::make_tuple;
using std::get;

/********************************************************************
*	Struct: Entity
*
*	Purpose:
*		Baseline data struct for all fluid entities to be created
*		from.
*
********************************************************************/
struct Entity
{
	//default ctor
	Entity() {}

	//ctor
	Entity( int entity_index,
			string name,
			int max_press,
			int min_press,
			float fluid_level,
			int max_cap,
			float prod_rate,
			float current_press) :
			entity_index(entity_index),
			name(name),
			max_press(max_press),
			min_press(min_press),
			fluid_level(fluid_level),
			max_cap(max_cap),
			prod_rate(prod_rate),
			current_press(current_press) {}

	// entity index
	int entity_index;
	//name of type of entity
	string name;
	//Maxium pressure
	int max_press;
	//Minimum pressure
	int min_press;
	//Current fluid level, recalculated as flow and pressure changes
	float fluid_level = 0;
	//Maxium fluid capacity allowed in entity
	int max_cap;
	//Rate at which the fluid is produced
	float prod_rate;
	//Current pressure, recalculated as fluid_level is changed
	float current_press;
};