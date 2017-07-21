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

//Main loop of program, loops through vectors and updates data in them every 60hz
void Update(vector<Entity> & entites, vector< tuple<int, int, float> > & entity_pair_data);

//Gets data to create entities, and populates vectors to store the data
void Populate(vector<Entity> & entites, vector< tuple<int, int, float> > & entity_pair_data);

//Calcuates the current water pressure in an entity
void CalculatePressure(vector<Entity> entites, int index);

//Calculates water level changes between entities
void CalculateFlow(vector<Entity> entites, int index_a, int index_b, float prev_flow);

int main()
{
	//Init main structs
	vector<Entity> entites;
	vector< tuple<int, int, float> > entity_pair_data;
	
	//Get data
	Populate(entites, entity_pair_data);
	
	//Start main loop of program
	Update(entites, entity_pair_data);

	return 0;
}

/********************************************************************
*	Purpose:
*	Main loop of program, loops through vectors and updates data in 
*	them every 60hz
*
*	Entry:
*
*	Exit:
*
********************************************************************/
void Update(vector<Entity> & entites, vector<tuple<int, int, float>> & entity_pair_data)
{
}

/********************************************************************
*	Purpose:
*	Gets data to create entities, and populates vectors to store the data
*
*	Entry:
*
*	Exit:
*
********************************************************************/
void Populate(vector<Entity> & entites, vector< tuple<int, int, float> > & entity_pair_data)
{
	//Temp struct to store entity data coming out of the file
	Entity temp_ent = Entity();
	
	//***Temp variables as place holders for when data is read in****/
	int temp_entity_index;
	int temp_adjacent_index;
	//default to 0 unless otherwise specified in the file
	float prev_flow = 0;

	//open file
	//loop while not at end of file
	//use delimiters to break up data and store in temp_ent
	
	//Temp tuple to store index data coming out the file 
	auto temp_index = make_tuple(temp_entity_index, temp_adjacent_index, prev_flow);

	//Add new entity to the vector
	entites.push_back(Entity ( temp_ent.max_press, 
							   temp_ent.min_press, 
							   temp_ent.fluid_level,
							   temp_ent.max_cap,
							   temp_ent.consump_rate,
							   temp_ent.prod_rate,
							   temp_ent.current_press ));

	//Add new temp_index data to the vector
	entity_pair_data.push_back(temp_index);
	//end loop
}

/********************************************************************
*	Purpose:
*	Calcuates the current water pressure in an entity
*
*	Entry:
*
*	Exit:
*
********************************************************************/
void CalculatePressure(vector<Entity> entites, int index)
{

}

/********************************************************************
*	Purpose:
*	Calculates water level changes between entities
*
*	Entry:
*
*	Exit:
*
********************************************************************/
void CalculateFlow(vector<Entity> entites, int index_a, int index_b, float prev_flow)
{

}