//
//  main.cpp
//  Platform
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

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "Entity.hpp"

#define PLATFORM_COUNT 11
#define ENEMY_COUNT 3

struct GameState {
    Entity *player;
    Entity *platforms;
    Entity *failure;
    Entity *success;
    Entity *enemies;
    Entity *shot;
    Entity *level1;
    Entity *level2;
    Entity *level3;
    Entity *bacteria;
    
};

GameState state;

SDL_Window* displayWindow;
bool gameIsRunning = true;

ShaderProgram program;
glm::mat4 viewMatrix, modelMatrix, projectionMatrix;

GLuint LoadTexture(const char* filePath) {
    int w, h, n;
    unsigned char* image = stbi_load(filePath, &w, &h, &n, STBI_rgb_alpha);
    
    if (image == NULL) {
        std::cout << "Unable to load image. Make sure the path is correct\n";
        assert(false);
    }
    
    GLuint textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    
    stbi_image_free(image);
    return textureID;
}


void Initialize() {
    SDL_Init(SDL_INIT_VIDEO);
    displayWindow = SDL_CreateWindow("Project 4!", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480, SDL_WINDOW_OPENGL);
    SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
    SDL_GL_MakeCurrent(displayWindow, context);
    
#ifdef _WINDOWS
    glewInit();
#endif
    
    glViewport(0, 0, 640, 480);
    
    program.Load("shaders/vertex_textured.glsl", "shaders/fragment_textured.glsl");
    
    viewMatrix = glm::mat4(1.0f);
    modelMatrix = glm::mat4(1.0f);
    projectionMatrix = glm::ortho(-5.0f, 5.0f, -3.75f, 3.75f, -1.0f, 1.0f);
    
    program.SetProjectionMatrix(projectionMatrix);
    program.SetViewMatrix(viewMatrix);
    
    glUseProgram(program.programID);
    
    glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
    glEnable(GL_BLEND);
    
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    
    //Initialize success font
    state.success = new Entity();
    state.success->entityType = SUCCESS;
    state.success->isActive = false;
    
    //Initialize level1
    state.level1 = new Entity();
    state.level1->entityType = LEVEL1;
    state.level1->isActive = true;
    
    //Initialize level2
    state.level2 = new Entity();
    state.level2->entityType = LEVEL2;
    state.level2->isActive = false;
    
    //Initialize level3
    state.level3 = new Entity();
    state.level3->entityType = LEVEL3;
    state.level3->isActive = false;
    
    //Initialize failure font
    state.failure = new Entity();
    state.failure->entityType = FAILURE;
    state.failure->isActive = false;
    
    // Initialize Game Objects
    
    // Initialize Player
    state.player = new Entity();
    state.player->position = glm::vec3(0);
    state.player->movement = glm::vec3(0);
    state.player->speed = 2.0f;
    state.player->position = glm::vec3(-4.0f, -1.0f, 0);
    state.player->acceleration = glm::vec3(0, -1.0f, 0);
    state.player->textureID = LoadTexture("soap.png");
    state.player->width = 1.0;
    state.player->height = 0.5f;
    state.player->entityType = PLAYER;
    state.player->jumpPower = 1.7f;
    
    state.platforms = new Entity[PLATFORM_COUNT];
    
    GLuint platformTextureID = LoadTexture("tile.png");
    
    for (int i=0; i < PLATFORM_COUNT; i++){
        state.platforms[i].textureID = platformTextureID;
        state.platforms[i].entityType = TILE;
        state.platforms[i].position = glm::vec3(-5 + i, -3.25f, 0);
    }
    
    for (int i=0; i < PLATFORM_COUNT; i++){
        state.platforms[i].Update(0, NULL, NULL, 0, NULL, NULL);
    }
    
    state.success->Update(0,NULL, NULL, 0, NULL, NULL);
    
    // Initialize Enemy
    state.enemies = new Entity[ENEMY_COUNT];
    GLuint enemyTextureID = LoadTexture("coronavirus.png");
    
    state.enemies[0].textureID = enemyTextureID;
    state.enemies[0].position = glm::vec3(4, -2.25f, 0);
    state.enemies[0].speed = 1.0f;
    state.enemies[0].entityType = ENEMY;
    state.enemies[0].aiType = PATROL;
    state.enemies[0].aiState = IDLE;
    state.enemies[0].width = 0.80f;
    state.enemies[0].height = 1.0f;
    
    //enemy2
    
    state.enemies[1].textureID = enemyTextureID;
    state.enemies[1].position = glm::vec3(4, -2.25f, 0);
    state.enemies[1].isActive = false;
    state.enemies[1].speed = 1.0f;
    state.enemies[1].entityType = ENEMY;
    state.enemies[1].aiType = JUMPER;
    state.enemies[1].jumpPower = 0.2f;
    state.enemies[1].aiState = IDLE;
    state.enemies[1].width = 0.90f;
    state.enemies[1].height = 0.85f;
    state.enemies[1].acceleration = glm::vec3(0, -3.0f, 0);
    
    //enemy3
    state.level2->enemy= &state.enemies[1];
    
    state.enemies[2].textureID = enemyTextureID;
    state.enemies[2].position = glm::vec3(4, -2.25f, 0);
    state.enemies[2].isActive = false;
    state.enemies[2].speed = 1.0f;
    state.enemies[2].entityType = ENEMY;
    state.enemies[2].aiType = SHOOTER;
    state.enemies[2].jumpPower = 0.2f;
    state.enemies[2].aiState = IDLE;
    state.enemies[2].width = 0.90f;
    state.enemies[2].height = 0.85f;
    state.enemies[2].acceleration = glm::vec3(0, -3.0f, 0);
    state.level3->enemy= &state.enemies[2];
    
    
    state.bacteria = new Entity[20];
    for(int i=0; i<20; i++){
        state.bacteria[i].textureID = LoadTexture("bacteria.png");
        state.bacteria[i].position = glm::vec3(state.enemies[2].position.x, state.enemies[2].position.y,0);
        state.bacteria[i].entityType = BACTERIA;
        state.bacteria[i].speed = 3.0f;
        state.bacteria[i].isActive= false;
        state.bacteria[i].height=0.15;
        state.bacteria[i].width=0.15;
    }
    state.enemies[2].bacteria = state.bacteria;
    
    
}

void ProcessInput() {
    
    state.player->movement = glm::vec3(0);
    
    
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_QUIT:
            case SDL_WINDOWEVENT_CLOSE:
                gameIsRunning = false;
                break;
                
            case SDL_KEYDOWN:
                switch (event.key.keysym.sym) {
                    case SDLK_LEFT:
                        // Move the player left
                        break;
                        
                    case SDLK_RIGHT:
                        // Move the player right
                        break;
                        
                    case SDLK_SPACE:
                        state.player->jump = true;
                        break;
                }
                break; // SDL_KEYDOWN
        }
    }
    
    const Uint8 *keys = SDL_GetKeyboardState(NULL);
    
    if (keys[SDL_SCANCODE_LEFT]) {
        state.player->movement.x = -1.0f;
        state.player->animIndices = state.player->animLeft;
    }
    else if (keys[SDL_SCANCODE_RIGHT]) {
        state.player->movement.x = 1.0f;
        state.player->animIndices = state.player->animRight;
    }
    
    
    if (glm::length(state.player->movement) > 1.0f) {
        state.player->movement = glm::normalize(state.player->movement);
    }
    
}

#define FIXED_TIMESTEP 0.0166666f

float lastTicks = 0;
float accumulator = 0.0f;
void Update() {
    
    float ticks = (float)SDL_GetTicks() / 1000.0f;
    
    
    float deltaTime = ticks - lastTicks;
    lastTicks = ticks;
    deltaTime += accumulator;
    if (deltaTime < FIXED_TIMESTEP) {
        accumulator = deltaTime;
        return; }
    while (deltaTime >= FIXED_TIMESTEP) {
        // Update. Notice it's FIXED_TIMESTEP. Not deltaTime
        state.player->Update(FIXED_TIMESTEP, state.enemies, state.player, ENEMY_COUNT, state.success, state.failure, state.level1, state.level2, state.level3);
        state.player->Update(FIXED_TIMESTEP, state.platforms, state.player, PLATFORM_COUNT, state.success, state.failure, state.level1, state.level2, state.level3);
        state.player->Update(FIXED_TIMESTEP, state.bacteria, state.player, 20, state.success, state.failure, state.level1, state.level2, state.level3);
        for (int i=0; i < ENEMY_COUNT; i++){
            state.enemies[i].Update(FIXED_TIMESTEP, state.platforms, state.player, PLATFORM_COUNT, NULL, NULL, NULL,NULL, NULL, ticks);
        }
        for (int i=0; i<20; i++){
            state.bacteria[i].Update(FIXED_TIMESTEP, state.platforms, state.player, PLATFORM_COUNT, NULL, NULL, NULL,NULL, NULL, ticks);
        }
        
        
        deltaTime -= FIXED_TIMESTEP;
    }
    
    accumulator = deltaTime;
    
}


void Render() {
    glClear(GL_COLOR_BUFFER_BIT);
    
    for(int i = 0; i < PLATFORM_COUNT; i++){
        state.platforms[i].Render(&program);
    }
    for(int i = 0; i < ENEMY_COUNT; i++){
        state.enemies[i].Render(&program);
        
        
    }
    for(int i = 0; i<20; i++){
        state.bacteria[i].Render(&program);
    }
    
    state.player->Render(&program);
    
    
    
    GLuint fontTextureID = LoadTexture("font.png");
    
    
    state.success->DrawText(&program, fontTextureID, "You survived the coronavirus!", 0.50f, -0.2f, glm::vec3(-4.05, 0.0f, 0));
    state.success->Update(0,NULL, NULL, 0, NULL, NULL);
    
    state.level1->DrawText(&program, fontTextureID, "Level 1: ", 0.50f, -0.28f, glm::vec3(-0.8, 3.0f, 0));
    state.level1->DrawText(&program, fontTextureID, "Jump on top of the virus ", 0.50f, -0.25f, glm::vec3(-2.85, 2.0f, 0));
    state.level1->DrawText(&program, fontTextureID, "before it touches you!", 0.50f, -0.28f, glm::vec3(-2.25, 1.0f, 0));
    
    
    state.level2->DrawText(&program, fontTextureID, "Level 2: ", 0.50f, -0.28f, glm::vec3(-0.8, 3.0f, 0));
    state.level2->DrawText(&program, fontTextureID, "Jump on top of the virus ", 0.50f, -0.25f, glm::vec3(-2.85, 2.0f, 0));
    state.level2->DrawText(&program, fontTextureID, "but be careful, this one jumps too!", 0.50f, -0.28f, glm::vec3(-3.75, 1.0f, 0));
    
    
    state.level3->DrawText(&program, fontTextureID, "Level 3: ", 0.50f, -0.28f, glm::vec3(-0.8, 3.0f, 0));
    state.level3->DrawText(&program, fontTextureID, "Jump on top of the virus ", 0.50f, -0.25f, glm::vec3(-2.85, 2.0f, 0));
    state.level3->DrawText(&program, fontTextureID, "but be careful, this one shoots too!", 0.50f, -0.28f, glm::vec3(-3.75, 1.0f, 0));
    
    
    state.failure->DrawText(&program, fontTextureID, "Oh no! The coronavirus got you too :(", 0.50f, -0.28f, glm::vec3(-4.05, 0.0f, 0));
    state.success->Update(0, NULL, NULL, 0, NULL, NULL);
    
    
    SDL_GL_SwapWindow(displayWindow);
}


void Shutdown() {
    SDL_Quit();
}

int main(int argc, char* argv[]) {
    Initialize();
    
    while (gameIsRunning) {
        ProcessInput();
        Update();
        Render();
    }
    
    Shutdown();
    return 0;
}

