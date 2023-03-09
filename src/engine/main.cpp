#include "main.h"

int main(int argc, char  *argv[]){

    Window* window = new Window;
    Camera* camera = new Camera;
    Group* group = new Group;
    
    char file[] = "phase1.xml";
    parser(file, window, camera, group);
    run(window, camera, group, argc, argv);
    delete(window);
    delete(camera);
    delete(group);
    return 0;
}