

class Player {

public:
    
    //Spawns new players at origin
    float theta = -3.0f;
    float phi = 0.0f;
    float score = 0.0f;
    float direction = 0.0f; //angle unit cirle
    
    Player(){
    }
    
    Player(int in_theta, int in_phi, int in_dir)
    : theta(in_theta), phi(in_phi), direction(in_dir)
    {}
    
    ~Player(){
    }
    
    void setPosition(float x, float y){
        theta += y; //Placeholders for changing position
        phi += x;
        direction += x+y;
    }
    
    float getTheta(){
        return theta;
    }
    
    float getPhi(){
        return phi;
    }
    
    void render(){
        
        //Converting spherical coordinates to cartesian coordinates
        float x = RADIUS*sin(theta)*cos(phi);
        float y = RADIUS*sin(theta)*sin(phi);
        float z = RADIUS*cos(theta);
        
        //render a single triangle
        glBegin(GL_TRIANGLES);
        glColor3f(1.0f, 0.0f, 0.0f); //Red
        glVertex3f(x-0.5f, y-0.5f, z+0.0f);
        
        glColor3f(0.0f, 1.0f, 0.0f); //Green
        glVertex3f(x+0.0f, y+0.5f, z+1.0f);
        
        glColor3f(0.0f, 0.0f, 1.0f); //Blue
        glVertex3f(x+0.5f, y-0.5f, z+0.0f);
        glEnd();
    }
    
    
};


