//
//  main.cpp
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
#include "vector"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "Entity.hpp"

#define PLATFORM_COUNT 33
struct GameState {
    Entity *player;
    Entity *platforms;
    Entity *success;
    Entity *failure;
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
    displayWindow = SDL_CreateWindow("Textured!", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480, SDL_WINDOW_OPENGL);
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
    
    //Initialize failure font
    state.failure = new Entity();
    state.failure->entityType = FAILURE;
    state.failure->isActive = false;
    
    
    // Initialize Game Objects
    
    // Initialize Player
    state.player = new Entity();
    state.player->position = glm::vec3(0, 4, 0);
    state.player->movement = glm::vec3(0);
    state.player->acceleration = glm::vec3(0, -0.1f, 0);
    state.player->speed = 1.5f;
    state.player->textureID = LoadTexture("playerShip3_blue.png");
    
    state.player -> height = 1.0f;
    state.player -> width = 1.0f;
    
    state.platforms = new Entity[PLATFORM_COUNT];
    //tiles for the floor
    GLuint platformTextureID = LoadTexture("platform.png");
    
    state.platforms[0].textureID = platformTextureID;
    state.platforms[0].position = glm::vec3(-1, -3.25f, 0);
    state.platforms[0].entityType = TILE;

    state.platforms[1].textureID = platformTextureID;
    state.platforms[1].position = glm::vec3(0, -3.25f, 0);
    state.platforms[1].entityType = TILE;
    
    state.platforms[2].textureID = platformTextureID;
    state.platforms[2].position = glm::vec3(1, -3.25f, 0);
    state.platforms[2].entityType = TILE;
    
    state.platforms[3].textureID = platformTextureID;
    state.platforms[3].position = glm::vec3(-2, -3.25f, 0);
    state.platforms[3].entityType = TILE;
    
    state.platforms[4].textureID = platformTextureID;
    state.platforms[4].position = glm::vec3(-3, -3.25f, 0);
    state.platforms[4].entityType = TILE;
    
    state.platforms[5].textureID = platformTextureID;
    state.platforms[5].position = glm::vec3(-4, -3.25f, 0);
    state.platforms[5].entityType = TILE;
    
    state.platforms[6].textureID = platformTextureID;
    state.platforms[6].position = glm::vec3(-5, -3.25f, 0);
    state.platforms[6].entityType = TILE;
    
    state.platforms[13].textureID = platformTextureID;
    state.platforms[13].position = glm::vec3(5, -3.25f, 0);
    state.platforms[13].entityType = TILE;
    
    //platform for the goal
    GLuint goalTextureID = LoadTexture("goal.png");
    state.platforms[7].textureID = goalTextureID;
    state.platforms[7].position = glm::vec3(0.55, -2.25f, 0);
    state.platforms[7].height = 0.5f;
    state.platforms[7].entityType = GOAL;
    state.platforms[9].textureID = platformTextureID;
    state.platforms[9].position = glm::vec3(2, -3.25f, 0);
    state.platforms[9].entityType = TILE;
    
    
    state.platforms[8].textureID = goalTextureID;
    state.platforms[8].position = glm::vec3(1.55, -2.25f, 0);
    state.platforms[8].height = 0.5f;
    state.platforms[8].entityType = GOAL;

    state.platforms[10].textureID = platformTextureID;
    state.platforms[10].position = glm::vec3(3, -3.25f, 0);
    state.platforms[10].entityType = TILE;
    
    state.platforms[11].textureID = goalTextureID;
    state.platforms[11].position = glm::vec3(2.55, -2.25f, 0);
    state.platforms[11].height = 0.5f;
    state.platforms[11].entityType = GOAL;

    state.platforms[12].textureID = platformTextureID;
    state.platforms[12].position = glm::vec3(4, -3.25f, 0);
    state.platforms[12].entityType = TILE;
    
    //platforms for the wall
    state.platforms[14].textureID = platformTextureID;
    state.platforms[14].position = glm::vec3(-4.5, -2.25, 0);
    state.platforms[14].entityType = TILE;
    
    state.platforms[15].textureID = platformTextureID;
    state.platforms[15].position = glm::vec3(-4.5, -1.25, 0);
    state.platforms[15].entityType = TILE;
    
    state.platforms[16].textureID = platformTextureID;
    state.platforms[16].position = glm::vec3(-4.5, -0.25, 0);
    state.platforms[16].entityType = TILE;
    
    state.platforms[17].textureID = platformTextureID;
    state.platforms[17].position = glm::vec3(-4.5, 0.75, 0);
    state.platforms[17].entityType = TILE;
    
    state.platforms[18].textureID = platformTextureID;
    state.platforms[18].position = glm::vec3(-4.5, 1.75, 0);
    state.platforms[18].entityType = TILE;
    
    state.platforms[19].textureID = platformTextureID;
    state.platforms[19].position = glm::vec3(-4.5, 2.75, 0);
    state.platforms[19].entityType = TILE;
    
    state.platforms[20].textureID = platformTextureID;
    state.platforms[20].position = glm::vec3(4.5, 3.75, 0);
    state.platforms[20].entityType = TILE;
    
    state.platforms[21].textureID = platformTextureID;
    state.platforms[21].position = glm::vec3(4.5, -2.25, 0);
    state.platforms[21].entityType = TILE;
    
    state.platforms[22].textureID = platformTextureID;
    state.platforms[22].position = glm::vec3(4.5, -1.25, 0);
    state.platforms[22].entityType = TILE;
    
    state.platforms[23].textureID = platformTextureID;
    state.platforms[23].position = glm::vec3(4.5, -0.25, 0);
    state.platforms[23].entityType = TILE;
    
    state.platforms[24].textureID = platformTextureID;
    state.platforms[24].position = glm::vec3(4.5, 0.75, 0);
    state.platforms[24].entityType = TILE;
    
    state.platforms[25].textureID = platformTextureID;
    state.platforms[25].position = glm::vec3(4.5, 1.75, 0);
    state.platforms[25].entityType = TILE;
    
    state.platforms[26].textureID = platformTextureID;
    state.platforms[26].position = glm::vec3(4.5, 2.75, 0);
    state.platforms[26].entityType = TILE;
    
    state.platforms[27].textureID = platformTextureID;
    state.platforms[27].position = glm::vec3(-4.5, 3.75, 0);
    state.platforms[27].entityType = TILE;
    
    state.platforms[28].textureID = platformTextureID;
    state.platforms[28].position = glm::vec3(-3.5, 1.75, 0);
    state.platforms[28].entityType = TILE;
    
    state.platforms[29].textureID = platformTextureID;
    state.platforms[29].position = glm::vec3(-2.5, 1.75, 0);
    state.platforms[29].entityType = TILE;

    state.platforms[30].textureID = platformTextureID;
    state.platforms[30].position = glm::vec3(0.5, 0.5, 0);
    state.platforms[30].entityType = TILE;
    
    state.platforms[31].textureID = platformTextureID;
    state.platforms[31].position = glm::vec3(1.5, 0.5, 0);
    state.platforms[31].entityType = TILE;
    
    state.platforms[32].textureID = platformTextureID;
    state.platforms[32].position = glm::vec3(2.5, 0.5, 0);
    state.platforms[32].entityType = TILE;
    

    for (int i = 0; i < PLATFORM_COUNT; i++){
        state.platforms[i].Update(0, NULL, 0, NULL, NULL);
    }
    
    state.success->Update(0, NULL, 0, NULL, NULL);
    
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
                        // Some sort of action
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
        state.player->Update(FIXED_TIMESTEP, state.platforms, PLATFORM_COUNT, state.success, state.failure);
        
        
        deltaTime -= FIXED_TIMESTEP;
    }
    accumulator = deltaTime;
}


void Render() {
    glClear(GL_COLOR_BUFFER_BIT);
    
    for(int i = 0; i < PLATFORM_COUNT; i++){
        state.platforms[i].Render(&program);
    }
    
    state.player->Render(&program);
    GLuint fontTextureID = LoadTexture("font.png");
    
    
    state.success->DrawText(&program, fontTextureID, "Mission Successful!", 0.70f, -0.25f, glm::vec3(-4.05, 0.0f, 0));
    state.success->Update(0, NULL, 0, NULL, NULL);
    
    state.failure->DrawText(&program, fontTextureID, "Mission Failed!", 0.70f, -0.25f, glm::vec3(-3.05, 0.0f, 0));
    state.failure->Update(0, NULL, 0, NULL, NULL);
    
    
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
