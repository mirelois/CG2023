#include "parser.h"

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

void parse_group_models(xml_node<> *node_Models, Group* group, vector<float>* points){
    for(xml_node<> *node_models = node_Models->first_node();node_models; node_models = node_models->next_sibling()){
        // Criar fstream e abrir
        fstream filestream;
        filestream.open(node_models->first_attribute()->value(), ios::in|ios::binary);
        // Ler inteiro para o n
        int n;
        filestream.read((char*)&n,sizeof(int));

        // Ler array de tuplos
        //tuple<float,float,float>* tuples = new tuple<float,float,float>[n];
        int before = points->size();
        points->resize(before + n);
        filestream.read((char*)(points->data() + before), sizeof(float) * n);
        // fechar o ficheiro
        filestream.close();

        // Criar o model, guardar os tuplos e o inteiro no model, guardar o model no group
        Model* model = new Model;
        //model->figure = tuples;
        model->size = n/3;
        group->models.push_back(model);
    }
}

void parse_translate_points(Translate_Catmull* translation, xml_node<>* node) {
    xml_attribute<>* attr;
    float p[3];
    for (node = node->first_node(); node; node = node->next_sibling()) {
        if ((attr = node->first_attribute("x"))) {
            p[0] = atof(attr->value());
        }
        else {
            p[0] = 0.0f;
        }

        if ((attr = node->first_attribute("y"))) {
            p[1] = atof(attr->value());
        }
        else {
            p[1] = 0.0f;
        }

        if ((attr = node->first_attribute("z"))) {
            p[2] = atof(attr->value());
        }
        else {
            p[2] = 0.0f;
        }
        translation->addPoint(p);
    }
}

void parse_group_transform(xml_node<> *node_transform, Group* group){
    
    for(xml_node<> *node_temp = node_transform->first_node(); node_temp; node_temp = node_temp->next_sibling()){
        
        if(!strcmp(node_temp->name(), "translate")){

            xml_attribute<>* attr;
            if ((attr = node_temp->first_attribute("time"))) {
                Translate_Catmull* translation = new Translate_Catmull();
                translation->setTime(atof(attr->value()));
                if ((attr = node_temp->first_attribute("align"))) {
                    translation->setAlign(!strcmp("true", attr->value()));
                }
                else translation->setAlign(false);

                parse_translate_points(translation, node_temp);

                group->transformations.push_back(translation);
            }
            else {
                Translate* translation = new Translate();

                if ((attr = node_temp->first_attribute("x")))
                    translation->setArgOne(atof(attr->value()));
                else
                    translation->setArgOne(0.0f);

                if ((attr = node_temp->first_attribute("y")))
                    translation->setArgTwo(atof(attr->value()));
                else
                    translation->setArgTwo(0.0f);

                if ((attr = node_temp->first_attribute("z")))
                    translation->setArgThree(atof(attr->value()));
                else
                    translation->setArgThree(0.0f);

                group->transformations.push_back(translation);
            }
        } else if(!strcmp(node_temp->name(), "rotate")){

            Rotate* rotation;
            xml_attribute<>* attr;

            if ((attr = node_temp->first_attribute("time"))) {
                rotation = new Rotate_Time(atof(attr->value()));
            }
            else {
                rotation = new Rotate();
                if ((attr = node_temp->first_attribute("angle"))) {
                    rotation->setArgOne(atof(attr->value()));
                }
                else {
                    rotation->setArgOne(0.0f);
                }
            }
        
            if((attr = node_temp->first_attribute("x")))
                rotation->setArgTwo(atof(attr->value()));
            else
                rotation->setArgTwo(0.0f);
        
            if((attr = node_temp->first_attribute("y")))
                rotation->setArgThree(atof(attr->value()));
            else
                rotation->setArgThree(0.0f);
        
            if((attr = node_temp->first_attribute("z")))
                rotation->setArgFour(atof(attr->value()));
            else
                rotation->setArgFour(0.0f);

            group->transformations.push_back(rotation);
        } else if(!strcmp(node_temp->name(), "scale")){
            Scale* scale = new Scale();

            xml_attribute<> *attr;
            if((attr = node_temp->first_attribute("x")))
                scale->setArgOne(atof(attr->value()));
            else
                scale->setArgOne(0.0f);
        
            if((attr = node_temp->first_attribute("y")))
                scale->setArgTwo(atof(attr->value()));
            else
                scale->setArgTwo(0.0f);
        
            if((attr = node_temp->first_attribute("z")))
                scale->setArgThree(atof(attr->value()));
            else
                scale->setArgThree(0.0f);

            group->transformations.push_back(scale);
        }
    }
}


void parse_group(xml_node<> *group_node, Group* group, vector<float>* points){
    xml_node<>* temp;
    
    // Transformações
    if((temp = group_node->first_node("transform")))
        parse_group_transform(temp, group);

    // Modelos 
    if((temp = group_node->first_node("models")))
        parse_group_models(temp, group, points);
    
    // Grupos
    for(temp = group_node->first_node("group"); temp; temp = temp->next_sibling("group")){
        Group *groupChild = new Group;
        group->subGroups.push_back(groupChild);
        parse_group(temp, groupChild, points);
    }
}

void parser(char* fileName, Window* window, Camera* camera, Group* group, vector<float>* points)
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
        parse_group(temp, group, points);
}
