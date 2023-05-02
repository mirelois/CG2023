#include <iostream>
#include <fstream>
#include <string.h>
#include <iterator>
#include <unordered_map>
#include "rapidxml.hpp"
#include "classes.h"

using namespace std;
using namespace rapidxml;


void parser(char* fileName, Window* window, Camera* camera, Group* group, vector<float>* points, vector<unsigned int>* indices);