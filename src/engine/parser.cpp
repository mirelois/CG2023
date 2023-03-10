#include "parser.h"

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
        char *path = node_models->first_attribute()->value();
        //ifstream file("../generator/" + path); // como ir buscar o model num path different?
        char buffer[128];
        #ifdef _WIN32
            strcpy(buffer, "../generator/");
        #else
            strcpy(buffer, "../../models/");
        #endif
        strcat(buffer, path);
        //FILE *file = fopen(buffer,"r");
        //printf("%s %d\n", buffer, file);
        //printf("%s\n", buffer);
        fstream filestream;
        filestream.open(buffer, ios::in|ios::binary);
        int n;
        //filestream.seekg(0);
        filestream.read((char*)&n,sizeof(int));
        //filestream >> n;
        tuple<float,float,float>* tuples = new tuple<float,float,float>[n];
        filestream.read((char*)tuples, sizeof(tuple<float,float,float>) * n);
        filestream.close();
        /*
        if(file){
            unsigned int x;
            fread(&x, sizeof(unsigned int), 1, file);
            tuples = new tuple<float,float,float>[x];
            fread(&tuples, sizeof(tuple<float,float,float>), x, file);
            printf("%d\n", get<0>(tuples[0]));
            for (int i = 0; i<x; i++) {
                tuple<float,float,float> tuple = tuples[i];
                printf("Debug %d, %d\n", i, x);
                printf("%d: %f %f %f\n", i, get<0>(tuple), get<1>(tuple), get<2>(tuple));
            }
            //copy(istream_iterator<tuple<float,float,float>>(file), istream_iterator<tuple<float,float,float>>(),
            //back_inserter(tuples));
        } // Se não entrar no if, então não conseguiu abrir o ficheiro
        */
        /*model->figure = (tuple<float,float,float>*) malloc(sizeof(tuples->size()));
        for(int i=0; i<tuples->size(); i++){
            model->figure[i] = tuples[i];
        }
        group->models.push_back(model);*/
        
        model->figure = tuples;
        model->size = n;
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