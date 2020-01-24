#include "./Player.hpp"
#include "./Weapon.hpp"
#include "./Projectile.hpp"
#include "./Scene.hpp"
#include <stdlib.h>
#include <stdio.h>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <algorithm> 

#ifndef DOMEGAME
#define DOMEGAME

class DomeGame{
public:
    
    struct playerScore {
        std::string playerName = "";
        int score = 0;
        bool operator>(const DomeGame::playerScore &b) const
        {
            return score > b.score;
        }
    };
    
    // Constructor
    DomeGame(sgct::Engine * gEngine, std::string in_rootDir);
	~DomeGame();
    
    void init();
    void render() const;
	void printScoreboard() const;
    void addPlayer(std::string &name, std::string weaponType, glm::quat pos = glm::quat());
	void renderPlayer(Player *p) const;
	void renderWeapon(Player *p) const;
	void renderPlayerName(Player *p) const;
	void update(float dt);
    
    std::string getScoreboard();
    
    size_t textureHandle;
    Scene * myScene;
	
	sgct::SharedVector<Player> added_players;
	sgct::SharedVector<Projectile> added_projectiles;
	sgct::SharedVector<int> removed_projectiles;

	struct weaponInfo {
		int id = -1;
		std::string weaponType = "";
	};
	sgct::SharedVector<weaponInfo> changed_weapons;

	// needs to be Player* for the weapons to find their owner Player
    std::vector<Player*> players; 
    std::vector<Projectile> projectiles;

	glm::mat4 MVP;
    
    std::string scoreboard = "";
    
private:
	const float DOME_DRAWABLES_RADIUS = 5.4f; // where domedrawables are rendered
    std::string rootDir = "";

	const static int N_LIGHTS = 8; // needs to be consistent with playershader(f and v)
	struct PlayerShader{
		GLint MVP_loc = -1;
		GLint model_loc = -1;
		GLint d_tex_loc = -1;
		GLint b_tex_loc = -1;
		GLint light_pos_loc[N_LIGHTS];
		GLint light_color_loc[N_LIGHTS];
		GLint ambient = -1;

	} playershader;

	struct ProjectileShader {
		GLint MVP_loc = -1;
		GLint model_loc = -1;
		GLint d_tex_loc = -1;
		GLint color = -1;

	} projectileshader;

};

#endif // DOMEGAME
