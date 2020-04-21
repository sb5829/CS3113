//
//  Menu.cpp
//  Platforms
//
//  Created by Angie Beck on 4/21/20.
//  Copyright © 2020 sabrena beck. All rights reserved.
//

#include "Menu.hpp"
#include "Util.hpp"

GLuint fontTextureID4;
#define ENEMY_COUNT 1
#define LEVEL3_WIDTH 14
#define LEVEL3_HEIGHT 8


void Menu::Initialize() {
    state.nextScene = -1;
    
    
    state.success = new Entity();
    state.success->entityType = SUCCESS;
    state.success->isActive = false;
    
    state.failure = new Entity();
    state.failure->entityType = FAILURE;
    state.failure->isActive = false;
    fontTextureID4 = Util::LoadTexture("font.png");
    state.player = new Entity();
    state.player->position = glm::vec3(0);
    state.player->movement = glm::vec3(0);
    state.player->speed = 2.0f;
    state.player->position = glm::vec3(3.0f, 1.0f, 0);
    state.player->acceleration = glm::vec3(0, -1.0f, 0);
    state.player->textureID = Util::LoadTexture("soap.png");
    state.player->width = 1.0;
    state.player->height = 0.5f;
    state.player->entityType = PLAYER;
    state.player->jumpPower = 1.7f;
    //state.success->Update(0,state.enemies, NULL, 0, NULL, NULL);
    // Initialize Enemy
    state.enemies = new Entity[ENEMY_COUNT];
    GLuint enemyTextureID = Util::LoadTexture("coronavirus.png");
    
    state.enemies[0].textureID = enemyTextureID;
    state.enemies[0].position = glm::vec3(8.0f, -2.0f, 0);
    state.enemies[0].speed = 1.0f;
    state.enemies[0].entityType = ENEMY;
    state.enemies[0].acceleration = glm::vec3(0, -1.0f, 0);
    state.enemies[0].aiType = PATROL;
    state.enemies[0].aiState = IDLE;
    state.enemies[0].width = 0.80f;
    state.enemies[0].height = 1.0f;
    
}


void Menu::Update(float deltaTime) {
//    state.player->Update(deltaTime, state.enemies, state.player, ENEMY_COUNT, state.success, state.failure, state.level1, state.level2, state.level3);
//    state.player->Update_platforms(deltaTime, state.map);
//    for (int i=0; i < ENEMY_COUNT; i++){
//        state.enemies[i].Update_platforms(deltaTime, state.map);
//    }
//    state.success->Update(0, state.enemies, NULL, 0, NULL, NULL);
//    if (state.player->position.x >=12){
//        state.nextScene = 1;
//    }
    
    
    const Uint8 *keys = SDL_GetKeyboardState(NULL);
    
    if (keys[SDL_SCANCODE_RETURN]) {
        state.nextScene=1;
        
    }
    
    
    
   
    
    
}
void Menu::Render(ShaderProgram *program) {
    Util::DrawText(program, fontTextureID4, "Help us kill the Coronavirus!", 0.7f, -0.45f, glm::vec3(2, -3, 0));
    Util::DrawText(program, fontTextureID4, "Press Enter to start", 0.7f, -0.45f, glm::vec3(3, -5, 0));
//    state.player->Render(program);
//    state.map->Render(program);
//    for(int i = 0; i < ENEMY_COUNT; i++){
//        state.enemies[i].Render(program);
//    }
    //state.success->Update(0, state.enemies, NULL, 0, NULL, NULL);
    
}
