#include <string>
using std::string;

#include <fstream>
using std::ifstream;
using std::ofstream;

#include <vector>
using std::vector;

#include <sstream>
using std::stringstream;

#include <unordered_map>
using std::unordered_map;

#include "Entity.h"

void Reader(string input_file)
{
	int row = 0;

	vector< vector<string> > map;
	unordered_map<int, int[4]> connections;
	vector<Entity> entities;

	string line = "";

	ifstream ifs;
	ifs.open(input_file, ifstream::in);
	
	while (getline(ifs, line))
	{
		// temp vars for tokenizing the current line in the file
		string token;

		stringstream ss(line);

		//tokenize the string based on the delimiter ','
		while (getline(ss, token, ','))
		{
			string tempstr = "";
			int tempindex = 0;

			map[row].push_back(token);

			// check to see if cell contains a comment or data
			if (token.find("#") == string::npos)
			{
				// find if data has an index, if so remove the index
				size_t found = token.find_first_of("0123456789");

				if (found != string::npos)
				{
					tempstr = token.substr(0, found);
					tempindex = atoi(token.substr(found + 1).c_str());
				}
			}
		}
		row++;
	}
}