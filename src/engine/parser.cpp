#include "parser.h"

#define MAX_CATMULL_POINTS 10000

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

    if((temp = camera_node->first_node("settings"))){
        xml_attribute<> *attr;
        if((attr = temp->first_attribute("delta")))
            camera->camera_move_delta = atof(attr->value());
    }

}

void parse_lights(xml_node<> *lights_node, vector<Light*>* lights){
    xml_node<> *temp;
    xml_attribute<> *attr;
    GLuint numbers[] = {GL_LIGHT0, GL_LIGHT1, GL_LIGHT2, GL_LIGHT3, GL_LIGHT4, GL_LIGHT5, GL_LIGHT6, GL_LIGHT7};
    int number = 0;
    for(temp = lights_node->first_node("light"); number < 8 && temp; temp = temp->next_sibling("light"), number++){
        if((attr = temp->first_attribute("type")) && !strcmp(attr->value(), "point")){
            Point* point = new Point();
            if((attr = temp->first_attribute("posx")))
                point->point[0] = atof(attr->value());
        
            if((attr = temp->first_attribute("posy")))
                point->point[1] = atof(attr->value());
        
            if((attr = temp->first_attribute("posz")))
                point->point[2] = atof(attr->value());

            point->point[3] = 1.0;

            point->number = numbers[number];
            lights->push_back(point);
        } else if((attr = temp->first_attribute("type")) && !strcmp(attr->value(), "directional")){
            Directional* directional = new Directional();
            if((attr = temp->first_attribute("dirx")))
                directional->point[0] = atof(attr->value());
        
            if((attr = temp->first_attribute("diry")))
                directional->point[1] = atof(attr->value());
        
            if((attr = temp->first_attribute("dirz")))
                directional->point[2] = atof(attr->value());

            directional->point[3] = 0.0;

            printf("%f %f %f\n", directional->point[0], directional->point[1], directional->point[2]);

            directional->number = numbers[number];
            lights->push_back(directional);
        } else if((attr = temp->first_attribute("type")) && !strcmp(attr->value(), "spotlight")){
            Spotlight* spotlight = new Spotlight();
            if((attr = temp->first_attribute("posx")))
                spotlight->point[0] = atof(attr->value());
        
            if((attr = temp->first_attribute("posy")))
                spotlight->point[1] = atof(attr->value());
        
            if((attr = temp->first_attribute("posz")))
                spotlight->point[2] = atof(attr->value());

            spotlight->point[3] = 1.0;
        
            if((attr = temp->first_attribute("dirx")))
                spotlight->dir[0] = atof(attr->value());
        
            if((attr = temp->first_attribute("diry")))
                spotlight->dir[1] = atof(attr->value());
        
            if((attr = temp->first_attribute("dirz")))
                spotlight->dir[2] = atof(attr->value());

            if((attr = temp->first_attribute("cutoff")))
                spotlight->cutoff = atof(attr->value());
            spotlight->number = numbers[number];
            lights->push_back(spotlight);
        }
    }
}

void parse_group_models(xml_node<> *node_Models, Group* group, vector<float> *points, vector<float>* normals, vector<float>* texCoords,
    vector<unsigned int>* indices, unordered_map<string, Model*> *model_map){
    string model_name;
    for(xml_node<> *node_models = node_Models->first_node();node_models; node_models = node_models->next_sibling()){
        // Criar fstream e abrir
        model_name = node_models->first_attribute()->value();
        Model* model = new Model(GL_TRIANGLES);
        if (model_map->find(model_name) == model_map->end()) {
            fstream filestream;
            filestream.open(node_models->first_attribute()->value(), ios::in | ios::binary);
            assert( (filestream.rdstate() & std::ifstream::failbit ) == 0 );
                // Ler inteiro para o n
            unsigned int n;
            filestream.read((char*)&n, sizeof(unsigned int));

            // Ler array de tuplos
            unsigned int before = points->size();
            points->resize(before + n);
            normals->resize(before + n);
            texCoords->resize(2*before/3 + n);
            filestream.read((char*)(points->data() + before), sizeof(float) * n);
            filestream.read((char*)(normals->data() + before), sizeof(float) * n);
            filestream.read((char*)(texCoords->data() + 2*before/3), sizeof(float) * n);

            unsigned int n_indices;
            filestream.read((char*)&n_indices, sizeof(unsigned int));
            printf("%d\n", n_indices);
            unsigned int* indices_buf = (unsigned int*)malloc(sizeof(unsigned int) * n_indices);
            filestream.read((char*)(indices_buf), sizeof(unsigned int) * n_indices);
            

            // Guardar os tuplos e o inteiro no model, guardar o model no group
            model->size = n_indices;
            model->index = indices->size();

            for (unsigned int i = 0; i < n_indices;i++) {
                indices->push_back(indices_buf[i] + before/3);
            }

            free(indices_buf);
            // fechar o ficheiro
            filestream.close();
            
            //lista dos modelos
            model_map->insert(make_pair(model_name, model));
            group->models.push_back(model);
        }
        else {
            group->models.push_back(model_map->at(node_models->first_attribute()->value()));
        }

        xml_node<> *temp;
        if((temp = node_models->first_node("texture"))){
            unsigned int t,tw,th;
	        unsigned char *texData;

	        ilInit();
	        ilEnable(IL_ORIGIN_SET);
	        ilOriginFunc(IL_ORIGIN_LOWER_LEFT);
	        ilGenImages(1,&t);
	        ilBindImage(t);
	        ilLoadImage((ILstring)temp->first_attribute("file")->value());
	        tw = ilGetInteger(IL_IMAGE_WIDTH);
	        th = ilGetInteger(IL_IMAGE_HEIGHT);
	        ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE);
	        texData = ilGetData();

	        glGenTextures(1,&model->texID);
	
	        glBindTexture(GL_TEXTURE_2D,model->texID);
	        glTexParameteri(GL_TEXTURE_2D,	GL_TEXTURE_WRAP_S,		GL_REPEAT);
	        glTexParameteri(GL_TEXTURE_2D,	GL_TEXTURE_WRAP_T,		GL_REPEAT);

	        glTexParameteri(GL_TEXTURE_2D,	GL_TEXTURE_MAG_FILTER,   	GL_LINEAR);
	        glTexParameteri(GL_TEXTURE_2D,	GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		
	        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, tw, th, 0, GL_RGBA, GL_UNSIGNED_BYTE, texData);
	        glGenerateMipmap(GL_TEXTURE_2D);

	        glBindTexture(GL_TEXTURE_2D, 0);
        }

        if((temp = node_models->first_node("color"))){
            xml_node<> *tempColor;
            xml_attribute<> *tempAttr;
            if((tempColor = temp->first_node("diffuse"))){
                if((tempAttr = tempColor->first_attribute("R"))){
                    model->diffuse[0] = atof(tempAttr->value())/255;
                }

                if((tempAttr = tempColor->first_attribute("G"))){
                    model->diffuse[1] = atof(tempAttr->value())/255;
                }

                if((tempAttr = tempColor->first_attribute("B"))){
                    model->diffuse[2] = atof(tempAttr->value())/255;
                }
            }

            if((tempColor = temp->first_node("ambient"))){
                if((tempAttr = tempColor->first_attribute("R"))){
                    model->ambient[0] = atof(tempAttr->value())/255;
                }

                if((tempAttr = tempColor->first_attribute("G"))){
                    model->ambient[1] = atof(tempAttr->value())/255;
                }

                if((tempAttr = tempColor->first_attribute("B"))){
                    model->ambient[2] = atof(tempAttr->value())/255;
                }
            }

            if((tempColor = temp->first_node("specular"))){
                if((tempAttr = tempColor->first_attribute("R"))){
                    model->specular[0] = atof(tempAttr->value())/255;
                }

                if((tempAttr = tempColor->first_attribute("G"))){
                    model->specular[1] = atof(tempAttr->value())/255;
                } 

                if((tempAttr = tempColor->first_attribute("B"))){
                    model->specular[2] = atof(tempAttr->value())/255;
                }
            }

            if((tempColor = temp->first_node("emissive"))){
                if((tempAttr = tempColor->first_attribute("R"))){
                    model->emissive[0] = atof(tempAttr->value())/255;
                }

                if((tempAttr = tempColor->first_attribute("G"))){
                    model->emissive[1] = atof(tempAttr->value())/255;
                }

                if((tempAttr = tempColor->first_attribute("B"))){
                    model->emissive[2] = atof(tempAttr->value())/255;
                }
            }

            if((tempColor = temp->first_node("shininess"))){
                if((tempAttr = tempColor->first_attribute("value"))){
                    model->shininess = atof(tempAttr->value());
                }
            }
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

void parse_group_transform(xml_node<> *node_transform, Group* group, Group* parent, vector<float> *points, vector<unsigned int> *indices){
    
    for(xml_node<> *node_temp = node_transform->first_node(); node_temp; node_temp = node_temp->next_sibling()){
        
        if(!strcmp(node_temp->name(), "translate")){

            xml_attribute<>* attr;
            if ((attr = node_temp->first_attribute("time"))) {
                float time = atof(attr->value());
                Translate_Catmull* translation;
                if ((attr = node_temp->first_attribute("align")) && !strcmp(attr->value(), "true")) {
                    translation = new Translate_Catmull_Align();
                }
                else {
                    translation = new Translate_Catmull();
                }
                
                translation->setTime(time);
                parse_translate_points(translation, node_temp);
                group->transformations.push_back(translation);

                if ((attr = node_temp->first_attribute("draw")) && !strcmp(attr->value(), "true")) {
                    unsigned int max = MAX_CATMULL_POINTS;
                    if ((attr = node_temp->first_attribute("n_points"))) {
                        max = atoi(attr->value());
                    }
                    float p[3], d[3];
                    unsigned int before = points->size();
                    // draw curve using line segments with GL_LINE_LOOP
                    Model* catmull = new Model(GL_LINE_LOOP);
                    catmull->index = indices->size();
                    catmull->size = max;

                    for (unsigned int t = 0; t < max; t += 1) {
                        translation->getGlobalCatmullRomPoint(float(t)/max, p, d);
                        points->push_back(p[0]);
                        points->push_back(p[1]);
                        points->push_back(p[2]);
                        indices->push_back(t+before/3);
                    }
                    parent->models.push_back(catmull);
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


void parse_group(xml_node<> *group_node, Group* group, Group* parent, 
                vector<float>* points, vector<float>* normals, vector<float>* texCoords, vector<unsigned int>* indices,
                unordered_map<string, Model*> *model_map){
    xml_node<>* temp;
    // Transformações
    if((temp = group_node->first_node("transform")))
        parse_group_transform(temp, group, parent, points, indices);

    // Modelos 
    if((temp = group_node->first_node("models")))
        parse_group_models(temp, group, points, normals, texCoords, indices, model_map);
    
    // Grupos
    for(temp = group_node->first_node("group"); temp; temp = temp->next_sibling("group")){
        Group *groupChild = new Group;
        group->subGroups.push_back(groupChild);
        parse_group(temp, groupChild, group, points, normals, texCoords, indices, model_map);
    }
}

void parser(char* fileName, Window* window, Camera* camera, vector<Light*>* lights, Group* group, 
    vector<float>* points, vector<float>* normals, vector<float>* texCoords, vector<unsigned int>* indices)
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

    // Lights
    if((temp = root_node->first_node("lights")))
        parse_lights(temp, lights);

    // Grupo
    unordered_map<string, Model*> model_map = {};
    Group* decoy = new Group();
    group->subGroups.push_back(decoy);
    if((temp = root_node->first_node("group")))
        parse_group(temp, decoy, group, points, normals, texCoords, indices, &model_map);
    //percorrer o mapa e pôr nos points os pontos ao mesmo tempo que se põe o índice (ou fazer logo que se coloca)
}
