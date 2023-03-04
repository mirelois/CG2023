#include "parser.cpp"

int main(int argc, char  *argv[]){

    Window window;
    Camera camera;
    Group group;

    parser("phase1.xml", &window, &camera, &group);
    
    return 0;
}