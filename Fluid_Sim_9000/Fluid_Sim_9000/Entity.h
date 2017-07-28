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
	Entity(	int entity_index,
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
	float fluid_level = 0.0;
	//Maxium fluid capacity allowed in entity
	int max_cap;
	//Rate at which the fluid is produced
	float prod_rate;
	//Current pressure, recalculated as fluid_level is changed
	float current_press = 0.0;
};

/********************************************************************
*	Purpose: Initializes incoming entities with default values
*
*	Entry: Ref to entity struct where data is to be stored
*
*	Exit: Populated entity struct, success = 1, error = -1 return val
*
********************************************************************/
int InitEntities(Entity & entity, string type)
{
	if ((type == "water pump") || (type == "WP"))
	{
		entity.name = type;
		entity.max_press = 100;
		entity.min_press = 0;
		entity.fluid_level = 20.0;
		entity.max_cap = 20;
		entity.prod_rate = 0.0;
		entity.current_press = 0.0;
	}
	else if ((type == "pipe") || (type == "P"))
	{
		entity.name = type;
		entity.max_press = 100;
		entity.min_press = 0;
		entity.fluid_level = 0.0;
		entity.max_cap = 100;
		entity.prod_rate = 0.0;
		entity.current_press = 0.0;
	}
	else if ((type == "heat exchanger") || (type == "boiler") || 
			 (type == "steam engine") || (type == "steam turbine") ||
			 (type == "HE") || (type == "B") || (type == "SE") ||
			 (type == "ST"))
	{
		entity.name = type;
		entity.max_press = 100;
		entity.min_press = -100;
		entity.fluid_level = 0.0;
		entity.max_cap = 200;
		entity.prod_rate = 0.0;
		entity.current_press = 0.0;
	}
	else if ((type == "tank") || (type == "T"))
	{
		entity.name = type;
		entity.max_press = 100;
		entity.min_press = 0;
		entity.fluid_level = 0.0;
		entity.max_cap = 25000;
		entity.prod_rate = 0.0;
		entity.current_press = 0.0;
	}
	else
		return -1;

	return 1;
}