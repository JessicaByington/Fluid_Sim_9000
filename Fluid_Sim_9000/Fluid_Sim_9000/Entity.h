#pragma once

#include <vector>
using std::vector;

#include <tuple>
using std::tuple;
using std::make_tuple;

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
	Entity(	const int max_press,
			const int min_press,
			float fluid_level,
			const int max_cap,
			float consump_rate,
			float prod_rate,
			float current_press) :
			max_press(max_press),
			min_press(min_press),
			fluid_level(fluid_level),
			max_cap(max_cap),
			consump_rate(consump_rate),
			prod_rate(prod_rate),
			current_press(current_press) {}

	//Maxium pressure
	int max_press;
	//Minimum pressure
	int min_press;
	//Current fluid level, recalculated as flow and pressure changes
	float fluid_level = 0;
	//Maxium fluid capacity allowed in entity
	int max_cap;
	//Rate at which the fluid is consumed 
	float consump_rate;
	//Rate at which the fluid is produced
	float prod_rate;
	//Current pressure, recalculated as fluid_level is changed
	float current_press;
};