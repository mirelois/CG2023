#include <iostream>
#include <fstream>
#include <vector>
#include <string.h>
#include "rapidxml.hpp"
#include "classes.cpp"

using namespace std;
using namespace rapidxml;

void parse_window(xml_node<> *window_node, Window* window){
    for (xml_attribute<> *window_attr = window_node->first_attribute();
     window_attr; window_attr = window_attr->next_attribute())
    {
        if(strcmp("width", window_attr->name())){
            window->width = atof(window_attr->value());
        }else{
            window->height = atof(window_attr->value());
        }
    }
}

void parse_camera(xml_node<> *camera_node, Camera* camera){
    for(xml_node<> *camera_option_node = camera_node->first_node(); camera_option_node; camera_option_node = camera_option_node->next_sibling()){
        //cout << camera_option_node->name() << ":\n";
        for(xml_attribute<> *camera_option_attr = camera_option_node->first_attribute(); camera_option_attr; camera_option_attr = camera_option_attr->next_attribute()){
            //cout << camera_option_attr->name() << " " << camera_option_attr->value() << "\n";
        }
    }
}

// Esta função será muito provavelmente recursiva nos próximos guiões
void parse_group(xml_node<> *group_node, Group* group){

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
   
    // Ciclo?? No futuro??
    //for(xml_node<> *next = root_node->first_node(); next; next = next->next_sibling()){}
    xml_node<> *window_node = root_node->first_node();
    parse_window(window_node, window);

    xml_node<> *camera_node = window_node->next_sibling();
    parse_camera(camera_node, camera);

    xml_node<> *group_node = camera_node->next_sibling();
    parse_group(group_node, group);
    
}