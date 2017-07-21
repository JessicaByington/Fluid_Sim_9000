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

#include <string>
using std::string;

//Main loop of program, loops through vectors and updates data in them every 60hz
void Update(vector<Entity> & entites, vector< tuple<int, int, float> > & entity_pair_data);

//Gets data to create entities, and populates vectors to store the data
void Populate(vector<Entity> & entites, vector< tuple<int, int, float> > & entity_pair_data);

//Calcuates the current water pressure in an entity
void CalculatePressure(vector<Entity> entites, int index);

//Calculates water level changes between entities
void CalculateFlow(vector<Entity> entites, int index_a, int index_b, float prev_flow);

//TEMP DATA FOR TESTING
const string FILE_PATH = "C:\\Users\\Jessica\\Documents\\csv_test1.csv";

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
	/*******************************************************************
	*
	*	NOTE: Look up stringstream, this way can split on more that one
	*		delimiter. 
	*
	*******************************************************************/
	//Used to indicated if the file opened successfully
	//if false, user will be prompted again and will attempt to open again
	//if true, data will be taken out of file and populate the vectors
	bool success_flag = true;

	//temp storage of word inbetween ',' delimiter being read from file
	string word = "";

	//Temp struct to store entity data coming out of the file
	Entity temp_ent = Entity();
	
	//***Temp variables as place holders for when data is read in****/
	int temp_entity_index;
	int temp_adjacent_index;
	//default to 0 unless otherwise specified in the file
	float prev_flow = 0;

	//Loop until the file is sucessfully opened
	do
	{
		//reset flag for new run
		success_flag = true;

		//create file stream object
		ifstream ifs;

		//open file for reading
		ifs.open(FILE_PATH, ifstream::in);

		//check to see 
		if (!ifs.is_open())
		{
			cout << "Failed to open file, please try again" << endl;
			success_flag = false;
		}

		/* the loop uses delimiters to break up data and store in temp_ent,
		 * this is where entites and entity_pair_data is populated,
		 * loop until end of file (no more data to be read), 
		 * this will read in the first line of the file.
		 */
		while (getline(ifs, word, ','))
		{
			//read in the data for the current line
			temp_ent.name = word;

			//word is read in as a string, then it coverted into a int. 
			//atoi requires a cstring
			getline(ifs, word, ',');
			temp_ent.max_press = atoi(word.c_str());
			
			getline(ifs, word, ',');
			temp_ent.min_press = atoi(word.c_str());

			getline(ifs, word, ',');
			temp_ent.fluid_level = atof(word.c_str());

			getline(ifs, word, ',');
			temp_ent.max_cap = atoi(word.c_str());

			getline(ifs, word, ',');
			temp_ent.consump_rate = atof(word.c_str());

			getline(ifs, word, ',');
			temp_ent.prod_rate = atof(word.c_str());

			getline(ifs, word, ',');
			temp_ent.current_press = atof(word.c_str());

			getline(ifs, word, ',');
			temp_entity_index = atoi(word.c_str());

			getline(ifs, word, ',');
			temp_adjacent_index = atoi(word.c_str());

			//add data to vectors
			//Add new entity to the vector
			entites.push_back(Entity(temp_ent.name,
									 temp_ent.max_press,
									 temp_ent.min_press,
									 temp_ent.fluid_level,
									 temp_ent.max_cap,
									 temp_ent.consump_rate,
									 temp_ent.prod_rate,
									 temp_ent.current_press));

			//Temp tuple to store index data coming out the file 
			auto temp_index = make_tuple(temp_entity_index, temp_adjacent_index, prev_flow);

			//Add new temp_index data to the vector
			entity_pair_data.push_back(temp_index);
		}

	} while (!success_flag);

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