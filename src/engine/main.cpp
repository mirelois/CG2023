
#include "parser.cpp"
#include "engine.cpp"

int main(int argc, char  *argv[]){

    Window* window = new Window;
    Camera* camera = new Camera;
    Group* group = new Group;
    
    char file[] = "phase1.xml";
    parser(file, window, );
    run(window, camera, group, argc, argv);
    delete(window);
    delete(camera);
    delete(group);
    return 0;
}