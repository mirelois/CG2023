 // Breaking POO like a real dude

class Camera{
public:
    float position[3];
    float lookAt[3];
    float up[3];
    float projection[3];
};

class Window{
public:
    float width;
    float height;
};

class Models{
public:
    float** models; 
};

class Group{
public:
    Models models;
    // Futuramente Group group;
};