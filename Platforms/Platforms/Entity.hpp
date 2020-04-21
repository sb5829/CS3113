//
//  Entity.hpp
//  Platforms
//
//  Created by Angie Beck on 4/20/20.
//  Copyright © 2020 sabrena beck. All rights reserved.
//

#define GL_SILENCE_DEPRECATION

#ifdef _WINDOWS
#include <GL/glew.h>
#endif

#define GL_GLEXT_PROTOTYPES 1
#include <SDL.h>
#include <SDL_opengl.h>
#include "glm/mat4x4.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "ShaderProgram.h"
#include "Map.hpp"


extern int lives;


enum EntityType {PLAYER, ENEMY, TILE, SUCCESS, FAILURE, LEVEL1, LEVEL2, LEVEL3, BACTERIA};

enum AIType { PATROL, JUMPER, SHOOTER};

enum AIState {STOP, IDLE, WALKING, ATTACKING, JUMP};

class Entity {
public:
    
    EntityType entityType;
    AIType aiType;
    AIState aiState;
    
    Entity *enemy;
    Entity *bacteria;
    Entity *failure;
    Entity *success;
    bool isActive = true;
    
    glm::vec3 position;
    glm::vec3 movement;
    glm::vec3 acceleration;
    glm::vec3 velocity;
    float speed;
    
    bool jump = false;
    float jumpPower = 0;
    
    float width = 1;
    float height = 1;
    
    GLuint textureID;
    
    glm::mat4 modelMatrix;
    
    int *animRight = NULL;
    int *animLeft = NULL;
    int *animUp = NULL;
    int *animDown = NULL;
    
    int *animIndices = NULL;
    int animFrames = 0;
    int animIndex = 0;
    float animTime = 0;
    int animCols = 0;
    int animRows = 0;
    int lives = 3;
    
    Entity();
    
    void AI(Entity* player, float ticks);
    void AIPatrol(Entity* player);
    void AIJumper(Entity* player, float ticks);
    void AIShooter(Entity* player, float ticks);
    void Jump();
    void Stop();
    
    
    bool CheckCollision(Entity *other);
    
    void Update(float deltaTime, Entity *platforms, Entity *player, int platformCount,float ticks=NULL );
    void Update_platforms(float deltaTime, Map *platforms);
    void Render(ShaderProgram *program);
    void DrawSpriteFromTextureAtlas(ShaderProgram *program, GLuint textureID, int index);
    void CheckCollisionsY(Entity* objects, int objectCount);
    void CheckCollisionsX(Entity* objects, int objectCount);
    
    void CheckCollisionsX(Map *map);
    void CheckCollisionsY(Map *map);
    
    void DrawText(ShaderProgram *program, GLuint fontTextureID, std::string text, float size, float spacing, glm::vec3 position);
};



