//
//  Scene.hpp
//  Platforms
//
//  Created by Angie Beck on 4/20/20.
//  Copyright © 2020 sabrena beck. All rights reserved.
//

#pragma once
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
#include "Util.h"
#include "Entity.hpp"
#include "Map.hpp"

struct GameState {
    Entity *player;
    Entity *failure;
    Entity *success;
    Entity *enemies;
    Entity *shot;
    Entity *level1;
    Entity *level2;
    Entity *level3;
    Entity *bacteria;
    int nextScene;
    Map *map;

};
class Scene {
public:
    
    GameState state;
    
    int livess=3;
    virtual void Initialize() = 0;
    virtual void Update(float deltaTime) = 0;
    virtual void Render(ShaderProgram *program) = 0;
};
