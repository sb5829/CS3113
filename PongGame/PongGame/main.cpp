#define GL_SILENCE_DEPRECATION

#ifdef _WINDOWS
#include <GL/glew.h>
#endif

#define GL_GLEXT_PROTOTYPES 1
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <SDL.h>
#include <SDL_opengl.h>
#include "glm/mat4x4.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "ShaderProgram.h"

SDL_Window* displayWindow;
bool gameIsRunning = true;

ShaderProgram program;
glm::mat4 viewMatrix, modelMatrix, projectionMatrix;

glm::mat4 viewMatrix2, modelMatrix2, projectionMatrix2;

glm::mat4 viewMatrix3, modelMatrix3, projectionMatrix3;
//start at 0, 0, 0
//paddle 1
glm::vec3 player_position = glm::vec3(-6.5,0,0);

glm::vec3 player_position2 = glm::vec3(6.5,0,0);

glm::vec3 ball_position = glm::vec3(0,0,0);

//paddle 2
glm::vec3 player_movement = glm::vec3(0,0,0);

glm::vec3 player_movement2 = glm::vec3(0,0,0);

glm::vec3 ball_movement = glm::vec3(0,0,0);
//ball






float player_speed = 1.0f;

bool ball_activate = false;

GLuint pictureTextureID;
GLuint pictureTextureID2;
GLuint pictureTextureID3;

GLuint LoadTexture(const char* filePath){
    int w, h, n;
    unsigned char* image = stbi_load(filePath, &w, &h, &n, STBI_rgb_alpha);
    
    if (image == NULL){
        std::cout << "Unable to load image. Please check path of the file \n";
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

SDL_Joystick *playerOneController;
void Initialize() {
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_JOYSTICK);
    displayWindow = SDL_CreateWindow("Project 2!", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480, SDL_WINDOW_OPENGL);
    SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
    SDL_GL_MakeCurrent(displayWindow, context);
    
    //open 1st controller
    playerOneController = SDL_JoystickOpen(0);
    
#ifdef _WINDOWS
    glewInit();
#endif
    
    glViewport(0, 0, 640, 480);
    
    program.Load("shaders/vertex_textured.glsl", "shaders/fragment_textured.glsl");
    
    pictureTextureID = LoadTexture("pongpaddle.png");
    
    pictureTextureID2 = LoadTexture("pongpaddle.png");
    
    pictureTextureID3 = LoadTexture("ball.png");
    
    
    
    viewMatrix = glm::mat4(2.0f);
    modelMatrix = glm::mat4(1.0f);
    projectionMatrix = glm::ortho(-5.0f, 5.0f, -4.75f, 4.75f, -3.0f, 3.0f);
    
    viewMatrix2 = glm::mat4(4.0f);
    modelMatrix2 = glm::mat4(4.0f);
    projectionMatrix2 = glm::ortho(-6.0f, 6.0f, -4.75f, 4.75f, -2.0f, 2.0f);
    
    viewMatrix3 = glm::mat4(4.0f);
    modelMatrix3 = glm::mat4(4.0f);
    projectionMatrix3 = glm::ortho(-6.0f, 6.0f, -4.75f, 4.75f, -2.0f, 2.0f);
    
    program.SetProjectionMatrix(projectionMatrix);
    program.SetViewMatrix(viewMatrix);
    
    program.SetProjectionMatrix(projectionMatrix2);
    program.SetViewMatrix(viewMatrix2);
    
    program.SetProjectionMatrix(projectionMatrix3);
    program.SetViewMatrix(viewMatrix3);
    //program.SetColor(1.0f, 0.0f, 0.0f, 1.0f);
    
    
    glUseProgram(program.programID);
    
    
    glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
    
    
}
float lastTicks = 0.0f;

void ProcessInput() {
    
    //mouse coordinates are not in world coordinates
    //window is 640 pixels x 480 pixels
    //-5.0 to positive 1.0f in our world
    //convert mouse x,y to world unit
    
    //unit_x = ((x/width) * ortho_width) - (ortho_width/2.0)
    int x, y;
    SDL_GetMouseState(&x, &y);
    
    SDL_Event event;
    //if they're not moving don't go anywhere
    player_movement = glm::vec3(0,0,0);
    player_movement2 = glm::vec3(0,0,0);
    ball_movement = glm::vec3(0,0,0);
    //SDL_GetKeyboardState: give all of the keys states, 1 means is pressed'
    //SDL_Scancode gives you index to the array
    const Uint8 *keys = SDL_GetKeyboardState(NULL);
    if (keys[SDL_SCANCODE_UP]){
        //move up
        player_movement.y = 3.0f;
    }
    else if (keys[SDL_SCANCODE_DOWN]){
        //move down
        player_movement.y = -3.0f;
    }
    else if(keys[SDL_SCANCODE_W]){
        player_movement2.y = 3.0f;
    }
    else if(keys[SDL_SCANCODE_S]){
        player_movement2.y = -3.0f;
    }
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_QUIT:
            case SDL_WINDOWEVENT_CLOSE:
                gameIsRunning = false;
                break;
            //when a key is pressed, to jump, not for held
            //sdl_keyup when a key is released
            //https://wiki.libsdl.org/SDL_Scancode
            case SDL_KEYDOWN:
                switch (event.key.keysym.sym) {
                    //move to the right
                    case SDLK_SPACE:
                        ball_activate = true;
                        break;
                    
                }
                break;
        }
    }
}

bool collide_left = false;
bool collide_right = false;
bool bounce_lower = false;
bool bounce_upper = false;
int positive_x = 1;
int positive_y = 1;
void Update() {
    
    float ticks = (float)SDL_GetTicks() / 1000.0f;
    float deltaTime = ticks - lastTicks;
    lastTicks = ticks;

    
    //how much they wanna move * how fast they can move * how much time has gone by
    player_position2 += player_movement2 * player_speed * deltaTime;
    player_position += player_movement * player_speed * deltaTime;
    
    if(glm::length(player_movement2) > 1.0f){
        player_movement2 = glm::normalize(player_movement2);
    }
    if(glm::length(player_movement) > 1.0f){
        player_movement = glm::normalize(player_movement);
    }
    if(glm::length(ball_movement) > 1.0f){
        ball_movement = glm::normalize(ball_movement);
    }
    
    if (ball_activate && !collide_left &&!collide_right && !bounce_lower && !bounce_upper){
        ball_position.x += 4.5f * deltaTime;
        ball_position.y += 3.0f * deltaTime;
        
        modelMatrix3 = glm::mat4(1.0f);
        modelMatrix3 = glm::translate(modelMatrix3, ball_position);
    }
    if (ball_activate && collide_left &&!collide_right && !bounce_lower && !bounce_upper){
        
        ball_position.x += 4.5f * deltaTime;
        if (positive_y == 1){
            ball_position.y += 3.0f * deltaTime;
        }
        else {
            ball_position.y -= 3.0f * deltaTime;
        }
        
        modelMatrix3 = glm::mat4(1.0f);
        modelMatrix3 = glm::translate(modelMatrix3, ball_position);
    }
    if(ball_activate && !collide_left && collide_right && !bounce_lower && !bounce_upper){
        ball_position.x -= 4.5f * deltaTime;
        if (positive_y == -1){
            ball_position.y -= 3.0f * deltaTime;
        }
        else{
            ball_position.y += 3.0f * deltaTime;
        }
        
        
        modelMatrix3 = glm::mat4(1.0f);
        modelMatrix3 = glm::translate(modelMatrix3, ball_position);
    }
 
    if (bounce_lower){
        
        
        if (positive_y==-1 and positive_x==-1){
            ball_position.y += 3.0f * deltaTime;
            ball_position.x += 4.0f * deltaTime;

        }
        else{
       
            ball_position.y += 3.0f * deltaTime;
            ball_position.x += 4.0f * deltaTime;
        }
        
    }
    if (bounce_upper){
        if (positive_y==1 and positive_x==1){
            ball_position.y -= 3.0f * deltaTime;
            ball_position.x += 4.0f * deltaTime;
        }
        else{
            ball_position.y -= 3.0f * deltaTime;
            ball_position.x -= 4.0f * deltaTime;
        }
        
        
        modelMatrix3 = glm::mat4(1.0f);
        modelMatrix3 = glm::translate(modelMatrix3, ball_position);
        
    }

    //check for collison
    float w1 = 2.90;
    float w2 = 0.05;
    float xdist1 = fabs(ball_position.x - player_position2.x) - ((w1 + w2) / 2.0f);
    float ydist1 = fabs(ball_position.y - player_position2.y) - ((w1 + w2) / 2.0f);
    float xdist2 = fabs(ball_position.x - player_position.x) - ((w1 + w2) / 2.0f);
    float ydist2 = fabs(ball_position.y - player_position.y) - ((w1 + w2) / 2.0f);
    
    //++
    //+-
    //--
    
    if (xdist1 < 0 && ydist1 < 0 ){
        positive_x *= -1;
        collide_right = true;
        collide_left = false;
        bounce_lower = false;
        bounce_upper = false;
    }
    if (xdist2 < 0 && ydist2 < 0){
        positive_x *= -1;
        collide_right = false;
        collide_left = true;
        bounce_upper= false;
        bounce_lower =false;
        
    }
    
    if (ball_position.x> 5.75 or ball_position.x < -5.75){
        bounce_lower = false;
        bounce_upper = false;
        ball_activate = false;
    }
    if (ball_position.y< -4.5){
        positive_y *= -1;
        bounce_lower = true;
    }
    if (ball_position.y > 4.5){
        positive_y *= -1;
        bounce_upper = true;
    }
    
    modelMatrix3 = glm::mat4(1.0f);
    modelMatrix3 = glm::translate(modelMatrix3, ball_position);
    
    modelMatrix2 = glm::mat4(1.0f);
    modelMatrix2 = glm::translate(modelMatrix2, player_position2);
    modelMatrix = glm::mat4(1.0f);
    modelMatrix = glm::translate(modelMatrix, player_position);
    
}
void Render() {
    glClear(GL_COLOR_BUFFER_BIT);
    
    program.SetModelMatrix(modelMatrix);
    
    
    float vertices[] = { -1.5, -1.5, 1.5, -1.5, 1.5, 1.5, -1.5, -1.5, 1.5, 1.5, -1.5, 1.5 };
    float textCoords[] = { 0.0, 1.0, 1.0, 1.0, 1.0, 0.0, 0.0, 1.0, 1.0, 0.0, 0.0, 0.0 };
    
    float ball_vertices[] = { -0.25, -0.25, 0.25, -0.25, 0.25, 0.25, -0.25, -0.25, 0.25, 0.25, -0.25, 0.25 };
    
    glBindTexture(GL_TEXTURE_2D, pictureTextureID);
    glVertexAttribPointer(program.positionAttribute, 2, GL_FLOAT, false, 0, vertices);
    glEnableVertexAttribArray(program.positionAttribute);
    glVertexAttribPointer(program.texCoordAttribute, 2, GL_FLOAT, false, 0, textCoords);
    glEnableVertexAttribArray(program.texCoordAttribute);
    glActiveTexture(pictureTextureID);
    
    glDrawArrays(GL_TRIANGLES, 0, 6);
    
    glDisableVertexAttribArray(program.positionAttribute);
    glDisableVertexAttribArray(program.texCoordAttribute);
    
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    //second texture
    program.SetModelMatrix(modelMatrix2);
    glActiveTexture(pictureTextureID2);
    glBindTexture(GL_TEXTURE_2D, pictureTextureID2);
    
    
    glVertexAttribPointer(program.positionAttribute, 2, GL_FLOAT, false, 0, vertices);
    glEnableVertexAttribArray(program.positionAttribute);
    glVertexAttribPointer(program.texCoordAttribute, 2, GL_FLOAT, false, 0, textCoords);
    glEnableVertexAttribArray(program.texCoordAttribute);
    
    
    glDrawArrays(GL_TRIANGLES, 0, 6);
    
    
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    glDisableVertexAttribArray(program.positionAttribute);
    glDisableVertexAttribArray(program.texCoordAttribute);
    
    //third texture
    program.SetModelMatrix(modelMatrix3);
    glActiveTexture(pictureTextureID3);
    glBindTexture(GL_TEXTURE_2D, pictureTextureID3);
    
    
    glVertexAttribPointer(program.positionAttribute, 2, GL_FLOAT, false, 0, ball_vertices);
    glEnableVertexAttribArray(program.positionAttribute);
    glVertexAttribPointer(program.texCoordAttribute, 2, GL_FLOAT, false, 0, textCoords);
    glEnableVertexAttribArray(program.texCoordAttribute);
    
    
    glDrawArrays(GL_TRIANGLES, 0, 6);
    
    
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    glDisableVertexAttribArray(program.positionAttribute);
    glDisableVertexAttribArray(program.texCoordAttribute);
    SDL_GL_SwapWindow(displayWindow);
}

void Shutdown() {
    SDL_JoystickClose(playerOneController);
    SDL_Quit();
}

int main(int argc, char* argv[]) {
    Initialize();
    
    while (gameIsRunning) {
        ProcessInput(); //stores players intent to move/jump/etc
        Update();//test/aply movement
        //player, enemies, moving platforms
        Render();//draw the current state of the game
    }
    
    Shutdown();
    return 0;
}
