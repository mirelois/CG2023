#include <iostream>
#include <fstream>
#include <vector>
#include <string.h>
#include <iterator>
#include "rapidxml.hpp"
#include "classes.cpp"

using namespace std;
using namespace rapidxml;

void parse_window(xml_node<> *window_node, Window* window){
    xml_attribute<> *temp;
    
    if((temp = window_node->first_attribute("width")))
        window->width = atof(temp->value());

    if((temp = window_node->first_attribute("height")))
        window->height = atof(temp->value());
}

void parse_camera(xml_node<> *camera_node, Camera* camera){
    xml_node<> *temp;

    if((temp = camera_node->first_node("position"))){
        xml_attribute<> *attr;
        if((attr = temp->first_attribute("x")))
            camera->position[0] = atof(attr->value());
        
        if((attr = temp->first_attribute("y")))
            camera->position[1] = atof(attr->value());
        
        if((attr = temp->first_attribute("z")))
            camera->position[2] = atof(attr->value());
    }

    if((temp = camera_node->first_node("lookAt"))){
        xml_attribute<> *attr;
        if((attr = temp->first_attribute("x")))
            camera->lookAt[0] = atof(attr->value());
        
        if((attr = temp->first_attribute("y")))
            camera->lookAt[1] = atof(attr->value());
        
        if((attr = temp->first_attribute("z")))
            camera->lookAt[2] = atof(attr->value());
    }

    if((temp = camera_node->first_node("up"))){
        xml_attribute<> *attr;
        if((attr = temp->first_attribute("x")))
            camera->up[0] = atof(attr->value());
        
        if((attr = temp->first_attribute("y")))
            camera->up[1] = atof(attr->value());
        
        if((attr = temp->first_attribute("z")))
            camera->up[2] = atof(attr->value());
    }

    if((temp = camera_node->first_node("projection"))){
        xml_attribute<> *attr;
        if((attr = temp->first_attribute("fov")))
            camera->projection[0] = atof(attr->value());
        
        if((attr = temp->first_attribute("near")))
            camera->projection[1] = atof(attr->value());
        
        if((attr = temp->first_attribute("far")))
            camera->projection[2] = atof(attr->value());
    }

}

// Esta função será muito provavelmente recursiva nos próximos guiões
void parse_group(xml_node<> *group_node, Group* group){
    for(xml_node<> *node_models = group_node->first_node("models")->first_node();node_models; node_models = node_models->next_sibling()){
        Model* model = new Model;
        string path = node_models->first_attribute()->value();
        ifstream file("generator/" + path); // como ir buscar o model num path different?
        vector<tuple<float,float,float>> tuples;
        if(file){
            copy(istream_iterator<tuple<float,float,float>>(file), istream_iterator<tuple<float,float,float>>(),
            back_inserter(tuples));
        } // Se não entrar no if, então não conseguiu abrir o ficheiro
        
        model->figure = (tuple<float,float,float>*) malloc(sizeof(tuples.size()));
        for(int i=0; i<tuples.size(); i++){
            model->figure[i] = tuples[i];
        }
        group->models.push_back(model);
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
    
    xml_node<> *temp;

    // Janela
    if((temp = root_node->first_node("window")))
        parse_window(temp, window);
    
    // Câmara
    if((temp = root_node->first_node("camera")))
        parse_camera(temp, camera);

    // Grupo
    if((temp = root_node->first_node("group")))
        parse_group(temp, group);
}