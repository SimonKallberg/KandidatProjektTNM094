#include "Scene.hpp"

// Constructor
Scene::Scene(){
    background = new Quad("mmdg", 4.0f, 3.0f);
}


// Draws the scene in dome or normal desktop mode
void Scene::draw() {
        background->draw(0.0f, 0.0f, -0.01f);
}
