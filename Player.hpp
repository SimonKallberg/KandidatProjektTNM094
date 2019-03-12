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
    
    void setPosition(int x, int y){
        theta += y; //Placeholders for changing position
        phi += x;
        //direction = tan(y/x);
    }
    
    float getTheta(){
        return theta;
    }
    
    float getPhi(){
        return phi;
    }
};


