#include "Entity.h"

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
	if (type == "water pump")
	{
		entity.name = type;
		entity.max_press = 100;
		entity.min_press = 0;
		entity.fluid_level = 20.0;
		entity.max_cap = 20;
		entity.prod_rate = 0.0;
		entity.current_press = 0.0;
	}
	else if ((type == "pipe") || (type == "up pipe") || 
			(type == "down pipe") || (type == "left pipe") || 
			(type == "right pipe"))
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
			(type == "steam engine") || (type == "steam turbine"))
	{
		entity.name = type;
		entity.max_press = 100;
		entity.min_press = -100;
		entity.fluid_level = 0.0;
		entity.max_cap = 200;
		entity.prod_rate = 0.0;
		entity.current_press = 0.0;
	}
	else if (type == "tank")
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

string ChangeFullName(string type)
{
	if (type == "HE")
		type = "heat exchanger";
	else if (type == "B")
		type = "boiler";
	else if (type == "SE")
		type = "steam engine";
	else if (type == "ST")
		type = "steam turbine";
	else if (type == "T")
		type = "tank";
	else if (type == "P")
		type = "pipe";
	else if (type == "UP")
		type = "up pipe";
	else if (type == "DP")
		type = "down pipe";
	else if (type == "LP")
		type = "left pipe";
	else if (type == "RP")
		type = "right pipe";
	else if (type == "WP")
		type = "water pump";

	return type;
}
