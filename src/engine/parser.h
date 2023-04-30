#include <iostream>
#include <fstream>
#include <string.h>
#include <iterator>
#include <unordered_map>
#include "rapidxml.hpp"
#include "classes.h"

using namespace std;
using namespace rapidxml;

void parse_group(xml_node<>* group_node, Group* group, vector<float>* points,
    unordered_map<string, Model*>* model_map, int* index);