#include "./Player.hpp"
#include "./Weapon.hpp"
#include "./Projectile.hpp"
#include "./Scene.hpp"
#include <stdlib.h>
#include <stdio.h>
#include <cstdlib>
#include <ctime>
#include <vector>

#ifndef DOMEGAME
#define DOMEGAME

class DomeGame{
public:
    // Constructor
    DomeGame(sgct::Engine * gEngine);
    
    void init();
    void render() const;
    void addPlayer(std::string &name, std::string weaponType, glm::quat pos = glm::quat());
	void renderPlayer(Player *p) const;
	void renderWeapon(Player *p) const;
	void update(float dt);
    
    size_t textureHandle;
    Scene * myScene;
	
	sgct::SharedVector<Player> added_players;
	sgct::SharedVector<Projectile> added_projectiles;
	sgct::SharedVector<int> removed_projectiles;

	// needs to be Player* for the weapons to find their owner Player
    std::vector<Player*> players; 
    std::vector<Projectile> projectiles;

	glm::mat4 MVP;
    
private:
	const float DOME_RADIUS = 5.4f; // where domedrawables are rendered

	const static int N_LIGHTS = 8; // needs to be consistent with playershader(f and v)
	struct PlayerShader{
		GLint MVP_loc = -1;
		GLint model_loc = -1;
		GLint d_tex_loc = -1;
		GLint b_tex_loc = -1;
		GLint light_pos_loc[N_LIGHTS];
		GLint light_color_loc[N_LIGHTS];

	} playershader;

	struct ProjectileShader {
		GLint MVP_loc = -1;
		GLint model_loc = -1;
		GLint d_tex_loc = -1;
		GLint color = -1;

	} projectileshader;

};

#endif // DOMEGAME
