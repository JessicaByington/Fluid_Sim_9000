#pragma once
#ifndef MAP_CREATOR_H
#define MAP_CREATOR_H

#include "Entity.h"

#include <string>
using std::string;

#include <vector>
using std::vector;

#include <unordered_map>
using std::unordered_map;

void extern Reader(string input_file, vector<Entity> & entities, unordered_map<int, int[4]> & connections);

#endif