#include "MapCreator.h"

#include <fstream>
using std::ifstream;
using std::ofstream;

#include <sstream>
using std::stringstream;


#include <algorithm>
using std::transform;

#include <iostream>
using std::cout;
using std::endl;

void Reader(string input_file, vector<Entity> & entities, unordered_map<int, int[4]> & connections)
{
	int max_index = 0;

	vector< vector<string> > map;

	string line = "";

	ifstream ifs;
	ifs.open(input_file, ifstream::in);
	
	// If file was not open successfully, exit try again
	if (!ifs.is_open())
	{
		cout << "File did not open sucessfully, please try again." << endl;
		return;
	}
	else
	{

		// Populate the map, find the largest index used.
		while (getline(ifs, line))
		{
			vector<string> temp_strvec;

			// temp vars for tokenizing the current line in the file
			string token;

			stringstream ss(line);

			//tokenize the string based on the delimiter ','
			while (getline(ss, token, ','))
			{

				temp_strvec.push_back(token);

				// ignore if comment, won't contain an index
				if ((token.find("#") == string::npos) || (token != ""))
				{
					// create temp entity 
					Entity temp_ent = Entity();

					// find if data has an index, if so remove the index
					size_t found = token.find_first_of("0123456789");

					// if a numeric value is found, compare to largest saved index
					if (found != string::npos)
					{
						string temp_str = token.substr(0, found);
						int temp_index = atoi(token.substr(found).c_str());

						// Add those entities that have indices
						temp_ent.entity_index = temp_index;

						transform(temp_str.begin(), temp_str.end(), temp_str.begin(), ::toupper);

						if (!InitEntities(temp_ent, temp_str) == 1)
						{
							cout << "Error with entity type, please check your file." << endl;
						}

						/*******************************************************************
						*	Need to find out how non-default values will be in file
						*******************************************************************/

						// store if larger. Will give starting place for un-indexed entities
						if (temp_index > max_index)
							max_index = temp_index;

						// add data to vectors
						// add new entity to the vector
						entities.push_back(Entity(temp_ent.entity_index,
							temp_ent.name,
							temp_ent.max_press,
							temp_ent.min_press,
							temp_ent.fluid_level,
							temp_ent.max_cap,
							temp_ent.prod_rate,
							temp_ent.current_press));
					}
				}
			}
			map.push_back(temp_strvec);
		}

		// iterate through the map, create indices for un-indexed entities.
		// Create entity objects and entity connections
		for (int i = 0; i < map.size(); i++)
		{
			for (int j = 0; j < map[i].size(); j++)
			{
				// store the string value of the entity
				string element = map[i][j];

				// Skip if comment
				if (element.find("#") == string::npos)
				{
					// create temp entity 
					Entity temp_ent = Entity();

					size_t found = element.find_first_of("0123456789");

					// Add those entities that had no default index
					if (found == string::npos)
					{
						// create new index for the entity
						temp_ent.entity_index = max_index++;

						transform(element.begin(), element.end(), element.begin(), ::toupper);

						// populate with the default value
						if (!InitEntities(temp_ent, element) == 1)
						{
							cout << "Error with entity type, please check your file." << endl;
						}

						// add data to vectors
						// add new entity to the vector
						entities.push_back(Entity(temp_ent.entity_index,
							temp_ent.name,
							temp_ent.max_press,
							temp_ent.min_press,
							temp_ent.fluid_level,
							temp_ent.max_cap,
							temp_ent.prod_rate,
							temp_ent.current_press));

					}

					// TODO: create the connections

				}
			}
		}
	}
}