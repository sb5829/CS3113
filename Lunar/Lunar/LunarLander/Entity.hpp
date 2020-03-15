//
//  Entity.hpp
//  LunarLander
//
//  Created by Angie Beck on 3/13/20.
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


enum EntityType {PLAYER, GOAL, TILE, SUCCESS, FAILURE};

class Entity {
public:
    
    EntityType entityType;
    bool isActive = true;
    //inside Update : exit right away
    //checkcollision: always false either object is false
    //if (isActive == false || other.isActive == false) return false;
    //render: no render
    
    glm::vec3 position;
    glm::vec3 movement;
    glm::vec3 acceleration;
    glm::vec3 velocity;
    
    int width = 1;
    int height = 1;
    
    
    float speed;
    
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
    
    Entity();
    
    bool CheckCollision(Entity *other, Entity *success, Entity *failure);
    
    void Update(float deltaTime, Entity *platforms, int platformCount, Entity *success, Entity *failure);
    void Render(ShaderProgram *program);
    void DrawSpriteFromTextureAtlas(ShaderProgram *program, GLuint textureID, int index);
    void CheckCollisionsY(Entity* objects, int objectCount, Entity *success, Entity *failure);
    void CheckCollisionsX(Entity* objects, int objectCount, Entity *success, Entity *failure);

    void DrawText(ShaderProgram *program, GLuint fontTextureID, std::string text, float size, float spacing, glm::vec3 position);
    };



