/********************************************************************
*	Author:						Jessica Byington
*	Date Created:				7/20/2017
*	Last Modification date:		7/27/2017
*	Filename:					Source.cpp
*
*	Overview: This program is used to simulate the fluid mechanics in
*		various entities in Factorio that will be specified by the 
*		user in a file. 
*
*	Input: A file will be provided by the user with specified 
*		parameters, order, and connections to other entities.
*
*	Output: A csv file with calculated fluid flow between all entites
*		that will be output every cycle (user specified seconds * 60)
*
*
********************************************************************/
#include <iostream>
using std::cin;
using std::cout;
using std::endl;
using std::fixed;

#include <fstream>
using std::ifstream;
using std::ofstream;

#include <sstream>
using std::stringstream;

#include <string>
using std::string;

#include <algorithm>
using std::transform;

#include <list>
using std::list;

#include <math.h>
using std::roundf;

#include "Entity.h"

/*******************************************************************/
/* Function Declarations */
//Main loop of program, loops through vectors and updates data in them every 1/60s
void Update(vector<Entity> & , vector< tuple< vector<int>, float> > & , vector<int>, string , int );

//Gets data to create entities, and populates vectors to store the data
void Populate(string, vector<Entity> & , vector< tuple< vector<int>, float> > & , vector<int> &);

//Calcuates the current water pressure in an entity
void CalculatePressure(Entity &);

//Calculates water level changes between entities
void CalculateFlow(Entity & , Entity & , float & );

// Displays input menu if no cmd args were passed
int StartMenu(string &, string &);

// Initializes incoming entities, returns -1 if no entity of that type was found 
int InitEntities(Entity & , string );

// Resets both consumer and producer vars in entities back to specified values
void Reset(vector<Entity> & , vector<int> );

// Outputs a csv file of calculated data (To print out header for csv data)
void PrintToFile(string, int, vector<Entity>);

// Outputs a csv file of calculated data (Overloaded to print fluid level data)
void PrintToFile(string, vector<Entity>, int );

/*******************************************************************/
/* Constants */
// default value for water pump
const float WATER_PUMP_LEVEL = 20;

//TEMP DATA FOR TESTING
const string FILE_PATH = "C:\\Users\\Commander\\Downloads\\demo.csv";

/*******************************************************************/
/* Function Defiitions */
int main()
{
	// Init main structs
	vector<Entity> entities;
	vector< tuple< vector<int>, float> > entity_connections;
	
	// update producer/consumer/water pump values at end of cycle
	vector<int> update;

	// Values to get from user
	// get number of cycles
	int num_cycles = 0;
	// get input file location
	string input_path = "";
	// get output file location
	string output_path = "";

	// get user input
	num_cycles = StartMenu(input_path, output_path);

	// convert from seconds to cycles
	num_cycles *= 60;

	// Get data from input file
	Populate(input_path, entities, entity_connections, update);

	// print headers for the entities in the output file
	PrintToFile(output_path, num_cycles, entities);
	PrintToFile(output_path, entities, 0);

	// run loop for number of cycles specified by user
	for (int i = 0; i < num_cycles; i++)
	{
		//Start main loop of program
		Update(entities, entity_connections, update, output_path, i + 1);
	}

	return 0;
}

/********************************************************************
*	Purpose: Main loop of program, loops through vectors and updates 
*		data in them every cycle
*
*	Entry: Populated vector of entities and entity connections 
*
*	Exit: Press and flow calculated and updated in each entity and
*		in each connection
*
********************************************************************/
void Update(vector<Entity> & entities, vector<tuple< vector<int>, float>> & entity_connections, vector<int> update, string output_file, int cycle)
{
	// store size, removing some overhead of calling ".size()" every loop
	int size = entity_connections.size();

	// run calculation on each pair in the connections vector
	for (int i = 0; i < size; i++)
	{
		// get source index
		int source = get<0>(entity_connections[i])[0];
		
		// if source index is -1, then it has no connections and does not 
		// anywhere to flow to. 
		if (source != -1)
		{
			// TO DO: Make entity_conection index to j, for multiple inputs
			// get dest index 
			int dest = get<0>(entity_connections[i])[1];

			// preform calculations 
			CalculatePressure(entities[source]);
			CalculatePressure(entities[dest]);
			CalculateFlow(entities[source], entities[dest], get<1>(entity_connections[i]));
		}
	}

	// resets values to default
	Reset(entities, update);

	// print out fluid level per entity for this cycle
	PrintToFile(output_file, entities, cycle);
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
void Populate(string input_file, vector<Entity> & entities, vector< tuple< vector<int>, float> > & entity_connections, vector<int> & update)
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
		ifs.open(input_file, ifstream::in);

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
			 * This is where entities and entity_pair_data is populated,
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
					entities.push_back(Entity(	temp_ent.entity_index,
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

		// file no longer in use, close
		ifs.close();

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
/*int InitEntities(Entity & entity, string type)
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
}*/

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
	float new_flow = ((source.current_press - dest.current_press) * 0.4);
	prev_flow = (new_flow + inertial_comp);

	// recalculate water levels.
	// dest gains fluid, source loses
	if (prev_flow >= 0)
	{
		// check to see if there is enough fluid to transfer. 
		if (source.fluid_level >= abs(prev_flow))
		{
			// bounds checking, removing from source if not empty 
			if ((source.fluid_level - prev_flow) >= 0)
				source.fluid_level = (source.fluid_level - prev_flow);

			// bounds checking, adding to dest if not full
			if ((dest.fluid_level + prev_flow) <= dest.max_cap)
				dest.fluid_level = (dest.fluid_level + prev_flow);
		}
		// give max amount of fluid in the source entity to fill to max capacity 
		else if (dest.fluid_level < dest.max_cap)
		{ 
			// Calc the amount needed to fill entity to capacity 
			float amount_needed = (dest.max_cap - dest.fluid_level);

			// give all of the source fluid to the dest as long as it is within the max cap
			if (amount_needed >= source.fluid_level)
			{
				// add all of the fluids from the source, emptying the source
				dest.fluid_level = (dest.fluid_level + source.fluid_level);
				source.fluid_level = (source.fluid_level - source.fluid_level);
			}
			// give a portion of the sources fluids to the dest
			else if (amount_needed < source.fluid_level)
			{
				// add only amount needed to dest, only removing amount needed from source
				dest.fluid_level = (dest.fluid_level + amount_needed);
				source.fluid_level = (source.fluid_level - amount_needed);
			}

		}

	}
	// back flow, dest loses fluid, source gains
	else
	{
		// check to see if there is enough fluid to transfer. 
		if (dest.fluid_level >= abs(prev_flow))
		{
			// bounds checking, addign fluid to source if not full
			if ((source.fluid_level + abs(prev_flow)) <= source.max_cap)
				source.fluid_level = (source.fluid_level + abs(prev_flow));

			// bounds checking, removing from dest if not empty
			if ((dest.fluid_level - abs(prev_flow)) >= 0)
				dest.fluid_level = (dest.fluid_level - abs(prev_flow));
		}
		// give max amount of fluid in the dest entity to fill to max capacity 
		else if (source.fluid_level < source.max_cap)
		{
			// Calc the amount needed to fill entity to capacity 
			float amount_needed = (source.max_cap - source.fluid_level);

			// give all of the dest fluid to the source as long as it is within the max cap
			if (amount_needed >= source.fluid_level)
			{
				// add all of the fluids from the dest, emptying the dest
				source.fluid_level = (source.fluid_level + dest.fluid_level);
				dest.fluid_level = (dest.fluid_level - dest.fluid_level);
			}
			// give a portion of the dest fluids to the source
			else if (amount_needed < dest.fluid_level)
			{
				// add only amount needed to source, only removing amount needed from dest
				source.fluid_level = (source.fluid_level + amount_needed);
				dest.fluid_level = (dest.fluid_level - amount_needed);
			}

		}
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
		// Check if entity has a prod_rate
		if (entities[update[i]].prod_rate != 0)
		{
			// calculate the change in fluid level
			float prod_calc = (entities[update[i]].fluid_level + (entities[update[i]].prod_rate / 60));

			// make sure new fluid level will be within the bounds of the entity
			if ((entities[update[i]].max_cap >= prod_calc) && (prod_calc >= 0))
			{
				// change the fluid level in the entity
				entities[update[i]].fluid_level = prod_calc;
			}
		}
		// water pump fluid level will always be 20
		else if (entities[update[i]].name == "water pump")
			entities[update[i]].fluid_level = WATER_PUMP_LEVEL;
		// nothing to do, exit if
		else
			break;
	}
}

/********************************************************************
*	Purpose: If no command args passed in, prompt with menu
*
*	Entry: none
*
*	Exit: retrieved input/output file locations from user and number
*		of desired cycles.
*
********************************************************************/
int StartMenu(string & input_path, string & output_path)
{
	int num_cycles = 0;

	cout << "Welcome to Fluid Sim 9000" << endl;
	cout << "Enter input file location: ";
	cin >> input_path;
	cout << "\nEnter output file location: ";
	cin >> output_path;
	cout << "\nEnter length of run (seconds): ";
	cin >> num_cycles;
	cout << endl;

	return num_cycles;
}

/********************************************************************
*	Purpose: Outputs to a csv file, formats header of csv
*
*	Entry: Populated list of entities
*
*	Exit: csv file of data is updated with names of entities
*
********************************************************************/
void PrintToFile(string output_file, int num_cycles, vector<Entity> entities)
{
	// create output stream 
	ofstream ofs;

	// open the specified csv file to output to in append mode 
	ofs.open(output_file, ofstream::out);

	// check if file was opening correctly 
	if (!ofs.is_open())
	{
		cout << "Error opening output file!" << endl;
	}
	else
	{
		// TO DO: Break output "," and "\n" into own functions
		// start off header with names of entities
		ofs << "Entity type/Index, ";
		int size = entities.size();
		for (int i = 0; i < size; i++)
		{
			ofs << entities[i].name;

			// if not last element, add ,
			if (i < (size - 1))
				ofs << ",";
			// if last element, add a newline
			else
				ofs << "\n";
		}

		// print entity index
		ofs << "Numeric Index, ";
		for (int i = 0; i < size; i++)
		{
			ofs << entities[i].entity_index;

			// if not last element, add ,
			if (i < (size - 1))
				ofs << ",";
			// if last element, add a newline
			else
				ofs << "\n";
		}

		// print entity capacity
		ofs << "Max capacity, ";
		for (int i = 0; i < size; i++)
		{
			ofs << entities[i].max_cap;

			// if not last element, add ,
			if (i < (size - 1))
				ofs << ",";
			// if last element, add a newline
			else
				ofs << "\n";
		}

		// min pressure
		ofs << "Min Pressure, ";
		for (int i = 0; i < size; i++)
		{
			ofs << entities[i].min_press;

			// if not last element, add ,
			if (i < (size - 1))
				ofs << ",";
			// if last element, add a newline
			else
				ofs << "\n";
		}

		// min/max pressure
		ofs << "Max Pressure, ";
		for (int i = 0; i < size; i++)
		{
			ofs << entities[i].max_press;

			// if not last element, add ,
			if (i < (size - 1))
				ofs << ",";
			// if last element, add a newline
			else
				ofs << "\n";
		}
	}

	// finished with file, close.
	ofs.close();
}

/********************************************************************
*	Purpose: Outputs to a csv file, adds data to csv
*
*	Entry: Calculated fluid level per entity per cycle
*
*	Exit: csv file of data is appended to with incoming fluid levels
*
********************************************************************/
void PrintToFile(string output_file, vector<Entity> entities, int cycle)
{
	// create output stream 
	ofstream ofs;

	// get size of entities so size function isn't being called repeatedly
	int size = entities.size();

	// open the specified csv file to output to in append mode 
	ofs.open(output_file, ofstream::out | ofstream::app);

	// check if file was opening correctly 
	if (!ofs.is_open())
	{
		cout << "Error opening output file!" << endl;
	}
	else
	{
		ofs.precision(1);
		ofs << fixed;

		// print which cycle this is for
		ofs << cycle << ",";

		// for each entity in entitites, print out it's fluid level
		for (int i = 0; i < size; i++)
		{
			// prints the fluid level for the entity at the end of this cycle
			ofs << entities[i].fluid_level;

			// if not last element, add ,
			if (i < (size - 1))
				ofs << ",";
			// if last element, add a newline
			else
				ofs << "\n";
		}
	}


	// finished with file, close.
	ofs.close();
}