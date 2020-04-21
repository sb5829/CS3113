//
//  Level1.cpp
//  Platforms
//
//  Created by Angie Beck on 4/20/20.
//  Copyright © 2020 sabrena beck. All rights reserved.
//

#include "Level1.hpp"
#include "Util.hpp"
#include <string>
#define ENEMY_COUNT 1
#define LEVEL1_WIDTH 18
#define LEVEL1_HEIGHT 8
GLuint fontTextureID1;

unsigned int level1_data[] =
{
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 4, 4, 0, 0, 0, 0, 0, 0,
    4, 4, 4, 4, 0, 0, 4, 4, 4, 9, 9, 9, 4, 4, 4, 4, 4, 4,
    9, 9, 9, 9, 0, 0, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9
};
void Level1::Initialize() {
    GLuint mapTextureID = Util::LoadTexture("tilesheet.png");
    state.map = new Map(LEVEL1_WIDTH, LEVEL1_HEIGHT, level1_data, mapTextureID, 1.0f, 5, 2);
//    // Move over all of the player and enemy code from initialization.
//
    state.nextScene = -1;
    state.success = new Entity();
    state.success->entityType = SUCCESS;
    state.success->isActive = false;
    
    state.failure = new Entity();
    state.failure->entityType = FAILURE;
    state.failure->isActive = false;
    
    // Initialize Player
    

   // state.success->Update(0,state.enemies, NULL, 0);
    // Initialize Enemy
    state.enemies = new Entity[ENEMY_COUNT];
    GLuint enemyTextureID = Util::LoadTexture("coronavirus.png");

    state.enemies[0].textureID = enemyTextureID;
    state.enemies[0].position = glm::vec3(10.0f, -2.0f, 0);
    state.enemies[0].speed = 0.7f;
    state.enemies[0].entityType = ENEMY;
    state.enemies[0].acceleration = glm::vec3(0, -1.5f, 0);
    state.enemies[0].aiType = PATROL;
    state.enemies[0].aiState = IDLE;
    state.enemies[0].width = 0.80f;
    state.enemies[0].height = 1.0f;

    fontTextureID1 = Util::LoadTexture("font.png");
    
}


void Level1::Update(float deltaTime) {
    state.player->Update(deltaTime, state.enemies, state.player, ENEMY_COUNT);
    state.player->Update_platforms(deltaTime, state.map);
    for (int i=0; i < ENEMY_COUNT; i++){
        state.enemies[i].Update_platforms(deltaTime, state.map);
        state.enemies[i].Update(deltaTime, NULL, state.player, NULL);
        
    }
    if (state.player->position.x >=17){
        state.nextScene = 2;
    }
    
}

void Level1::Render(ShaderProgram *program) {
    
    if (state.player->lives>0){
        if (state.player && state.player->position.x> 10){
            Util::DrawText(program, fontTextureID1, "Lives: "+ std::to_string(state.player->lives), 0.7f, -0.45f, glm::vec3(10 + 5.0f, -0.5f, 0));
            
        }
        else{
            Util::DrawText(program, fontTextureID1, "Lives: "+ std::to_string(state.player->lives), 0.7f, -0.45f, glm::vec3(state.player->position.x + 5.0f, -0.5f, 0));
        }
    }
    
    
    state.player->Render(program);
    state.map->Render(program);
    for(int i = 0; i < ENEMY_COUNT; i++){
        state.enemies[i].Render(program);
    }
    
    //state.success->Update(0, state.enemies, NULL, 0, NULL, NULL);
    
}
