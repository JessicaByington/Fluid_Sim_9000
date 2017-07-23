/********************************************************************
*	Author:						Jessica Byington
*	Date Created:				7/20/2017
*	Last Modification date:		7/20/2017
*	Filename:					Source.cpp
*
*	Overview:
*
*	Input:
*
*	Output:
*
*
********************************************************************/
#include <iostream>
using std::cin;
using std::cout;
using std::endl;

#include "Entity.h"

#include <fstream>
using std::ifstream;
using std::ofstream;

#include <sstream>
using std::stringstream;

#include <string>
using std::string;

#include <algorithm>
using std::transform;

//Main loop of program, loops through vectors and updates data in them every 1/60s
void Update(vector<Entity> & , vector< tuple< vector<int>, float> > & , vector<int>);

//Gets data to create entities, and populates vectors to store the data
void Populate(vector<Entity> & , vector< tuple< vector<int>, float> > & , vector<int> &);

//Calcuates the current water pressure in an entity
void CalculatePressure(Entity &);

//Calculates water level changes between entities
void CalculateFlow(Entity & , Entity & , float & );

// Displays input menu if no cmd args were passed
void StartMenu();

// Initializes incoming entities, returns -1 if no entity of that type was found 
int InitEntities(Entity & , string );

// Resets both consumer and producer vars in entities back to specified values
void Reset(vector<Entity> & , vector<int> );

//TEMP DATA FOR TESTING
const string FILE_PATH = "C:\\Users\\Commander\\Downloads\\demo.csv";

int main()
{
	// Init main structs
	vector<Entity> entites;
	vector< tuple< vector<int>, float> > entity_connections;
	// update producer/consumer/water pump values at end of cycle
	vector<int> update;
	
	//Get data
	Populate(entites, entity_connections, update);

	//Start main loop of program
	Update(entites, entity_connections, update);

	return 0;
}

/********************************************************************
*	Purpose:
*	Main loop of program, loops through vectors and updates data in 
*	them every 1/60s
*
*	Entry:
*
*	Exit:
*
********************************************************************/
void Update(vector<Entity> & entites, vector<tuple< vector<int>, float>> & entity_connections, vector<int> update)
{
	int size = entity_connections.size();
	for (int i = 0; i < size; i++)
	{
		int source = get<0>(entity_connections[i])[0];
		
		if (source != -1)
		{
			int dest = get<0>(entity_connections[i])[1];

			CalculateFlow(entites[source], entites[dest], get<1>(entity_connections[i]));
			CalculatePressure(entites[source]);
			CalculatePressure(entites[dest]);
		}
	}

	Reset(entites, update);
}

/********************************************************************
*	Purpose: Gets data to create entities, and populates variables to 
*		store the data
*
*	Entry: csv file location that is populated with data
*
*	Exit: vector entities, vector entity_connections will be 
*		populated
*
********************************************************************/
void Populate(vector<Entity> & entites, vector< tuple< vector<int>, float> > & entity_connections, vector<int> & update)
{
	// Used to indicated if the file opened successfully.
	// If false, user will be prompted again and will attempt to open again,
	// if true, data will be taken out of file and populate the vectors
	bool success_flag = true;
	
	// temp storage of word inbetween ',' delimiter being read from file
	string line = "";

	// default to 0 unless otherwise specified in the file
	float prev_flow = 0;

	// loop until the file is sucessfully opened
	do
	{
		// flag for first time through the loop, < 1 skip header line
		int first_line = 0;

		// reset flag for new run
		success_flag = true;

		// create file stream object
		ifstream ifs;

		// open file for reading
		ifs.open(FILE_PATH, ifstream::in);

		// check to see if file opened correctly, if unsuccessful try again.
		if (!ifs.is_open())
		{
			cout << "Failed to open file, please try again" << endl;
			success_flag = false;
		}
		else
		{
		    /* Get the whole line of text, then use list of delimiters
			 * to break up line into desired data.
			 * This is where entites and entity_pair_data is populated,
			 * loop until end of file (no more data to be read),
			 * this will read in the first line of the file.
			 */
			while (getline(ifs, line))
			{
				// reads in the first line of file and ignores it
				if (first_line != 0)
				{
					// temp struct to store entity data coming out of the file
					Entity temp_ent = Entity();

					// temp vars to hold entity index and connections that are read in
					int temp_entity_index;
					vector<int> temp_connections;
					
					// temp vars for tokenizing the current line in the file
					string token;
					vector<string> data_buffer;

					stringstream ss(line);
					//tokenize the string based on the delimiter ','
					while (getline(ss, token, ','))
					{
						data_buffer.push_back(token);
					}

					// cast from char * to int, Entity Index
					temp_ent.entity_index = atoi(data_buffer[0].c_str());
	
					// convert to string type
					string name = data_buffer[1];

					// convert value to lower case, allows for type checking
					transform(name.begin(), name.end(), name.begin(), ::tolower);

					// send data to be initialized to default values based on type
					if (!InitEntities(temp_ent, name) == 1)
					{
						cout << "Error with entity type, please check your file." << endl;
					}

					// cast from char * to int, starting level
					int starting_level = atoi(data_buffer[2].c_str());

					// check if starting flid level is different than default
					if (starting_level != temp_ent.fluid_level)
						temp_ent.fluid_level = starting_level;

					// cast from char * to int, production rate
					int prod = atof(data_buffer[3].c_str());

					// check if starting production rate is different than default
					if (prod != temp_ent.prod_rate)
						temp_ent.prod_rate = prod;

					// add index to update vector if prod rate != 0, or entity is a pump
					if ((prod != 0) || (name == "water pump"))
						update.push_back(temp_ent.entity_index);


					// looks for newline, keeps adding however many connections there are to
					// the connections vector. 
					for (int i = 6; i < data_buffer.size(); i++)
					{
						// if there was no connections, store -1
						if (data_buffer[i] != "")
							temp_connections.push_back(atoi(data_buffer[i].c_str()));
						else
							temp_connections.push_back(-1);
					}

					// add data to vectors
					// add new entity to the vector
					entites.push_back(Entity(	temp_ent.entity_index,
												temp_ent.name,
												temp_ent.max_press,
												temp_ent.min_press,
												temp_ent.fluid_level,
												temp_ent.max_cap,
												temp_ent.prod_rate,
												temp_ent.current_press));

					// temp tuple to store index data coming out the file 
					auto temp_index = make_tuple(temp_connections, prev_flow);

					// add new temp_index data to the vector
					entity_connections.push_back(temp_index);
				}
				else
					// advance the file to the next line to avoid header data
					first_line = 1;
			}
		}
	} while (!success_flag);
}

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
		entity.max_press = 200;
		entity.min_press = 0;
		entity.fluid_level = 20.0;
		entity.max_cap = 200;
		entity.prod_rate = 20.0;
		entity.current_press = 0.0;
	}
	else if (type == "pipe")
	{
		entity.name = type;
		entity.max_press = 100;
		entity.min_press = 0;
		entity.fluid_level = 0.0;
		entity.max_cap = 100;
		entity.prod_rate = 0.0;
		entity.current_press = 0.0;
	}
	else if ((type == "heat exchanger") || (type == "boiler") || (type == "steam engine") || (type == "steam turbine"))
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

/********************************************************************
*	Purpose:
*	Calcuates the current water pressure in an entity
*
*	Entry: Entity that pressure needs to be updated as the flow has 
*		changed 
*
*	Exit: new pressure will have been calculated and updated in the
*		passed in entity
*
********************************************************************/
void CalculatePressure(Entity & entity)
{
	entity.current_press = (entity.min_press + ((entity.max_press - entity.min_press) * (entity.fluid_level / entity.max_cap)));
}

/********************************************************************
*	Purpose:
*	Calculates water level changes between entities
*
*	Entry: Entity source of flow, entity destination of flow, and the
*	previous flow calculated between the entities passed in
*
*	Exit: The new flow will be calculated and updated in 
*		entity_connections
*
********************************************************************/
void CalculateFlow(Entity & source, Entity & dest, float & prev_flow)
{
	// calculations for the inertial component of formula
	float inertial_tran = (prev_flow * 0.59);
	float limited_max_cap = (dest.max_cap * 0.1);
	float inertial_comp = 0.0;

	// check if the inertial transfer is less than 10% of the max 
	// capacity of the destination entity. abs to account for 
	// backflow (negative inertial transfer)
	if (abs(inertial_tran > limited_max_cap))
		inertial_comp = limited_max_cap;
	else
		inertial_comp = inertial_tran;

	// flow calculation
	float new_flow = (source.current_press - dest.current_press);
	prev_flow = (new_flow * 0.4 + inertial_comp);

	// recalculate water levels.
	// dest gains fluid, source loses
	if (new_flow > 0)
	{
		// bounds checking, removing from source if not empty 
		if ((source.fluid_level - prev_flow) >= 0)
			source.fluid_level -= prev_flow;
		
		// bounds checking, adding to dest if not full
		if ((dest.fluid_level + prev_flow) <= dest.max_cap)
			dest.fluid_level += prev_flow;
	}
	// back flow, dest loses fluid, source gains
	else
	{
		// bounds checking, add fluid to source if not full
		if ((source.fluid_level + prev_flow) <= source.max_cap)
			source.fluid_level += prev_flow;

		// bounds checking, adding to dest if not full
		if ((dest.fluid_level - prev_flow) >= dest.max_cap)
			dest.fluid_level -= prev_flow;
	}
}

/********************************************************************
*	Purpose: Resets both consumer and producer vars in entities back
*		to specified values. 
*
*	Entry: Entity vectors and indices of where the of values that 
*		need to be reset are
*
*	Exit: values will have been reset to defaults
*
********************************************************************/
void Reset(vector<Entity> & entities, vector<int> update)
{
	for (int i = 0; i < update.size(); i++)
	{
		// if negative number, is consumer and will be reset to 0 
		// fluid indicating it has drained. 
		if (entities[update[i]].prod_rate < 0)
			entities[update[i]].fluid_level = 0;
		else
			// assuming that it is not a consumer it will be set to 
			// the prod_rate
			entities[update[i]].fluid_level = entities[update[i]].prod_rate;
	}
}

/********************************************************************
*	Purpose:
*	If no command args passed in, prompt with menu
*
*	Entry:
*
*	Exit:
*
********************************************************************/
void StartMenu()
{

}