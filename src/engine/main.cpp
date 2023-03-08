#include "parser.cpp"
#include "engine.cpp"

int main(int argc, char  *argv[]){

    Window* window = new Window;
    Camera* camera = new Camera;
    Group* group = new Group;
    
    char file[] = "phase1.xml";
    parser(file, window, camera, group);
    run(window, camera, group);
    delete(window);
    delete(camera);
    delete(group);
    return 0;
}