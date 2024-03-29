#include <iostream>
#include <fstream>
#include <string.h>
#include <iterator>
#include <unordered_map>
#include "rapidxml.hpp"
#include "classes.h"

using namespace std;
using namespace rapidxml;


void parser(char* fileName, Window* window, Camera* camera, vector<Light*>* lights, float* amb, int* amb_active, Group* group, 
    vector<float>* points, vector<float>* normals, vector<float>* texCoords, vector<unsigned int>* indices);