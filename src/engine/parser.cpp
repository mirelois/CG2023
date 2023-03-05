#include <iostream>
#include <fstream>
#include <vector>
#include <string.h>
#include "rapidxml.hpp"
#include "classes.cpp"

using namespace std;
using namespace rapidxml;

void parse_window(xml_node<> *window_node, Window* window){
    xml_attribute<> *first = window_node->first_attribute();
    window->width = atof(first->value());
    window->height = atof(first->next_attribute()->value());
}

void parse_camera(xml_node<> *camera_node, Camera* camera){
    int i = 0;
    for(xml_node<> *node = camera_node->first_node(); node; node = node->next_sibling()){
        for(xml_attribute<> *attr = node->first_attribute(); attr; attr = attr->next_attribute()){
            camera->options[i++] = atof(attr->value());
        }
    }

    if(i==7){ // Default Options
        camera->options[7] = 0;
        camera->options[8] = 1;
        camera->options[9] = 0;
        camera->options[10] = 60;
        camera->options[11] = 1;
        camera->options[12] = 1000;
    }
}

// Esta função será muito provavelmente recursiva nos próximos guiões
void parse_group(xml_node<> *group_node, Group* group){
    for(xml_node<> *node_models = group_node->first_node()->first_node();node_models; node_models = node_models->next_sibling()){
        //group->models.push_back();
    }
}

void parser(char* fileName, Window* window, Camera* camera, Group* group)
{  
    
    xml_document<> doc;
    xml_node<> * root_node = NULL;

    // Leitura do Ficheiro
    ifstream theFile (fileName);
    vector<char> buffer((istreambuf_iterator<char>(theFile)), istreambuf_iterator<char>());
    buffer.push_back('\0');
   
    // Parsing
    doc.parse<0>(&buffer[0]);
   
    // Encontrar o nodo raiz aka world
    root_node = doc.first_node("world");
   
    // Janela
    xml_node<> *window_node = root_node->first_node();
    parse_window(window_node, window);

    // Camara
    xml_node<> *camera_node = window_node->next_sibling();
    parse_camera(camera_node, camera);

    // Luzes: No futuro
    // 

    // Grupo
    xml_node<> *group_node = camera_node->next_sibling();
    parse_group(group_node, group);
    
}