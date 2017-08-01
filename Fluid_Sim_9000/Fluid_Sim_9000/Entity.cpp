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
	if ((type == "water pump") || (type == "WP"))
	{
		entity.name = "water pump";
		entity.max_press = 100;
		entity.min_press = 0;
		entity.fluid_level = 20.0;
		entity.max_cap = 20;
		entity.prod_rate = 0.0;
		entity.current_press = 0.0;
	}
	else if ((type == "pipe") || (type == "P"))
	{
		entity.name = "pipe";
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
		if (type == "HE")
			entity.name = "heat exchanger";
		else if (type == "B")
			entity.name = "boiler";
		else if (type == "SE")
			entity.name = "steam engine";
		else if (type == "ST")
			entity.name = "steam turbine";
		else
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
