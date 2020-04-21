//
//  Level3.cpp
//  Platforms
//
//  Created by Angie Beck on 4/21/20.
//  Copyright © 2020 sabrena beck. All rights reserved.
//

#include "Level3.hpp"
#include "Util.hpp"

#define ENEMY_COUNT 1
#define LEVEL3_WIDTH 18
#define LEVEL3_HEIGHT 8
GLuint fontTextureID3;
unsigned int level3_data[] =
{
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 4, 4, 0, 0, 0, 4, 4, 4, 4, 4, 4, 4,
    4, 4, 4, 4, 4, 0, 0, 0, 0, 4, 4, 9, 9, 9, 9, 9, 9, 9,
    9, 9, 9, 9, 9, 0, 0, 0, 0, 9, 9, 9, 9, 9, 9, 9, 9, 9
};
void Level3::Initialize() {
    GLuint mapTextureID = Util::LoadTexture("tilesheet.png");
    state.map = new Map(LEVEL3_WIDTH, LEVEL3_HEIGHT, level3_data, mapTextureID, 1.0f, 5, 2);
    //    // Move over all of the player and enemy code from initialization.
    //
    state.nextScene = -1;
    
    state.success = new Entity();
    state.success->entityType = SUCCESS;
    state.success->isActive = false;
    
    state.failure = new Entity();
    state.failure->entityType = FAILURE;
    state.failure->isActive = false;
    
    
    //
    //    GLuint mapTextureID = Util::LoadTexture("tilesheet.png");
    //    state.map = new Map(LEVEL2_WIDTH, LEVEL2_HEIGHT, level2_data, mapTextureID, 2.0f, 5, 2);
    //    state.platforms = new Entity[PLATFORM_COUNT];
    //
    //    GLuint platformTextureID = LoadTexture("tile.png");
    //
    //    for (int i=0; i < PLATFORM_COUNT; i++){
    //        state.platforms[i].textureID = platformTextureID;
    //        state.platforms[i].entityType = TILE;
    //        state.platforms[i].position = glm::vec3(-5 + i, -3.25f, 0);
    //    }
    //
    //    for (int i=0; i < PLATFORM_COUNT; i++){
    //        state.platforms[i].Update(0, NULL, NULL, 0, NULL, NULL);
    //    }
    
    //    state.success->Update(0,state.enemies, NULL, 0, NULL, NULL);
    //state.success->Update(0,state.enemies, NULL);
    // Initialize Enemy
    state.enemies = new Entity[ENEMY_COUNT];
    GLuint enemyTextureID = Util::LoadTexture("coronavirus.png");
    
    state.enemies[0].textureID = enemyTextureID;
    state.enemies[0].position = glm::vec3(13.0f, -3.0f, 0);
    state.enemies[0].speed = 0.5f;
    state.enemies[0].entityType = ENEMY;
    state.enemies[0].aiType = SHOOTER;
    state.enemies[0].jumpPower = 0.2f;
    state.enemies[0].aiState = IDLE;
    state.enemies[0].width = 0.90f;
    state.enemies[0].height = 0.85f;
    state.enemies[0].acceleration = glm::vec3(0, -3.0f, 0);
    state.bacteria = new Entity[20];
    for(int i=0; i<20; i++){
        state.bacteria[i].textureID = Util::LoadTexture("bacteria.png");
        state.bacteria[i].position = glm::vec3(state.enemies[0].position.x, state.enemies[0].position.y,0);
        state.bacteria[i].entityType = BACTERIA;
        state.bacteria[i].speed = 3.0f;
        state.bacteria[i].isActive = false;
        state.bacteria[i].height=0.15;
        state.bacteria[i].width=0.15;
    }
    state.enemies[0].bacteria = state.bacteria;
    
    state.player->position = glm::vec3(3.0f, -4.0f, 0);
    
    fontTextureID3 = Util::LoadTexture("font.png");
}


void Level3::Update(float deltaTime) {
    state.player->Update(deltaTime, state.enemies, state.player, ENEMY_COUNT, lives);
    state.player->Update(deltaTime, state.bacteria, state.player, ENEMY_COUNT, lives);
    state.player->Update_platforms(deltaTime, state.map);
    for (int i=0; i < ENEMY_COUNT; i++){
        state.enemies[i].Update_platforms(deltaTime, state.map);
        state.enemies[i].Update(deltaTime, NULL, state.player, NULL);
    }
    for (int i=0; i<20; i++){
        
        state.bacteria[i].Update(deltaTime, state.player, state.player, NULL);
    }
}

void Level3::Render(ShaderProgram *program) {
    if(state.player->lives>0){
        if (state.player && state.player->position.x> 10){
            Util::DrawText(program, fontTextureID3, "Lives: "+ std::to_string(state.player->lives), 0.7f, -0.45f, glm::vec3(10 + 5.0f, -0.5f, 0));
            
        }
        else{
            Util::DrawText(program, fontTextureID3, "Lives: "+ std::to_string(state.player->lives), 0.7f, -0.45f, glm::vec3(state.player->position.x + 5.0f, -0.5f, 0));
        }
        if (state.player->position.x>=14){
            state.player->success->isActive=true;
            state.player->movement = glm::vec3(0);
            
            state.player->velocity.x=0;
            state.player->speed=0;
        
            state.player->speed=0;
        }
        
    }
    for(int i = 0; i<20; i++){
        state.bacteria[i].Render(program);
    }
    state.player->Render(program);
    state.map->Render(program);
    for(int i = 0; i < ENEMY_COUNT; i++){
        state.enemies[i].Render(program);
    }
    
    //state.success->Update(0, state.enemies, NULL);
    
}
