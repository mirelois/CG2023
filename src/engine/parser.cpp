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

void parse_group_models(xml_node<> *node_Models, Group* group, vector<float> *points, vector<unsigned int>* indices, unordered_map<string, Model*> *model_map, unsigned int *index){
    string model_name;
    for(xml_node<> *node_models = node_Models->first_node();node_models; node_models = node_models->next_sibling()){
        // Criar fstream e abrir
        model_name = node_models->first_attribute()->value();
        if (model_map->find(model_name) == model_map->end()) {
            fstream filestream;
            filestream.open(node_models->first_attribute()->value(), ios::in | ios::binary);
            assert( (filestream.rdstate() & std::ifstream::failbit ) == 0 );
                // Ler inteiro para o n
            unsigned int n;
            filestream.read((char*)&n, sizeof(unsigned int));

            // Ler array de tuplos
            //tuple<float,float,float>* tuples = new tuple<float,float,float>[n];
            unsigned int before = points->size();
            points->resize(before + n);
            //filestream.read((char*)tuples, sizeof(float) * n);
            filestream.read((char*)(points->data() + before), sizeof(float) * n);
            unsigned int n_indices;
            filestream.read((char*)&n_indices, sizeof(unsigned int));
            before = indices->size();
            indices->resize(before + n_indices);
            filestream.read((char*)(indices->data() + before), sizeof(unsigned int) * n_indices);
            // fechar o ficheiro
            filestream.close();
            // Criar o model, guardar os tuplos e o inteiro no model, guardar o model no group
            Model* model = new Model;
            //model->figure = tuples;
            model->size = n_indices;
            model->index = *index;
            *index += n / 3;
            //lista dos modelos
            model_map->insert(make_pair(model_name, model));
            group->models.push_back(model);
        }
        else {
            group->models.push_back(model_map->at(node_models->first_attribute()->value()));
        }
        
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

void parse_group_transform(xml_node<> *node_transform, Group* group, vector<float> *points, vector<unsigned int> *indices, unsigned int *index){
    short max = 100;
    for(xml_node<> *node_temp = node_transform->first_node(); node_temp; node_temp = node_temp->next_sibling()){
        
        if(!strcmp(node_temp->name(), "translate")){

            xml_attribute<>* attr;
            if ((attr = node_temp->first_attribute("time"))) {
                float time = atof(attr->value());
                Translate_Catmull* translation;
                if ((attr = node_temp->first_attribute("align")) && !strcmp(attr->value(), "True")) {
                    translation = new Translate_Catmull_Align();
                }
                else {
                    translation = new Translate_Catmull();
                }
                
                translation->setTime(time);
                parse_translate_points(translation, node_temp);
                group->transformations.push_back(translation);

                Model* catmull = new Model();
                catmull->index = *index;
                catmull->size = 3*max;
                *index += catmull->size;
                group->models.push_back(catmull);

                if ((attr = node_temp->first_attribute("draw")) && !strcmp(attr->value(), "True")) {
                    float p[3], d[3];
                    // draw curve using line segments with GL_LINE_LOOP
                    translation->getGlobalCatmullRomPoint(0, p, d);
                    points->push_back(p[0]);
                    points->push_back(p[1]);
                    points->push_back(p[2]);
                    indices->push_back(0);
                    for (float t = 1; t < max; t += 1) {
                        translation->getGlobalCatmullRomPoint(t/max, p, d);
                        points->push_back(p[0]);
                        points->push_back(p[1]);
                        points->push_back(p[2]);
                        indices->push_back(t);
                        indices->push_back(t);
                        indices->push_back(t);
                    }
                    indices->push_back(0);
                    indices->push_back(0);
                }
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
            else if ((attr = node_temp->first_attribute("angle"))) {
                rotation = new Rotate_Alpha(atof(attr->value()));
            }
            else {
                rotation = new Rotate_Alpha(0.0f);
            }
        
            if((attr = node_temp->first_attribute("x")))
                rotation->setArgOne(atof(attr->value()));
            else
                rotation->setArgOne(0.0f);
        
            if((attr = node_temp->first_attribute("y")))
                rotation->setArgTwo(atof(attr->value()));
            else
                rotation->setArgTwo(0.0f);
        
            if((attr = node_temp->first_attribute("z")))
                rotation->setArgThree(atof(attr->value()));
            else
                rotation->setArgThree(0.0f);

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


void parse_group(xml_node<> *group_node, Group* group, vector<float>* points, vector<unsigned int>* indices,
                unordered_map<string, Model*> *model_map, unsigned int *index){
    xml_node<>* temp;
    // Transformações
    if((temp = group_node->first_node("transform")))
        parse_group_transform(temp, group, points, indices, index);

    // Modelos 
    if((temp = group_node->first_node("models")))
        parse_group_models(temp, group, points, indices, model_map, index);
    
    // Grupos
    for(temp = group_node->first_node("group"); temp; temp = temp->next_sibling("group")){
        Group *groupChild = new Group;
        group->subGroups.push_back(groupChild);
        parse_group(temp, groupChild, points, indices, model_map, index);
    }
}

void parser(char* fileName, Window* window, Camera* camera, Group* group, vector<float>* points, vector<unsigned int>* indices)
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
    unsigned int index = 0;
    unordered_map<string, Model*> model_map = {};
    if((temp = root_node->first_node("group")))
        parse_group(temp, group, points, indices, &model_map, &index);

    //percorrer o mapa e pôr nos points os pontos ao mesmo tempo que se põe o índice (ou fazer logo que se coloca)
}
