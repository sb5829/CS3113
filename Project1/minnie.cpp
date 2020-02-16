//
//  minnie.cpp
//  SDLSimple
//
//  Created by Angie Beck on 2/16/20.
//  Copyright © 2020 Angie Beck. All rights reserved.
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

class Minnie {
public:
    GLuint textureID;
    
    glm::mat4 viewMatrix, modelMatrix, projectionMatrix;
    GLuint pictureTextureID;
    
    float minnie_rotate =0;
    
    float lastTicks = 0.0f;
    void Update() {
        float ticks = (float)SDL_GetTicks() / 1000.0f;
        float deltaTime = ticks - lastTicks;
        lastTicks = ticks;
        
        minnie_rotate += -90.0f *deltaTime;

        modelMatrix= glm::mat4(1.0f);
        modelMatrix = glm::rotate(modelMatrix,glm::radians(minnie_rotate), glm::vec3(0.0f, 0.0f, 1.0f));
        
    }
    void Render(ShaderProgram *program) {
        glClear(GL_COLOR_BUFFER_BIT);
        
        program->SetModelMatrix(modelMatrix);
        
        float vertices[] = { -0.5, -0.5, 0.5, -0.5, 0.5, 0.5, -0.5, -0.5, 0.5, 0.5, -0.5, 0.5 };
        float textCoords[] = { 0.0, 1.0, 1.0, 1.0, 1.0, 0.0, 0.0, 1.0, 1.0, 0.0, 0.0, 0.0 };
        
        
        
        glVertexAttribPointer(program->positionAttribute, 2, GL_FLOAT, false, 0, vertices);
        glEnableVertexAttribArray(program->positionAttribute);
        glVertexAttribPointer(program->texCoordAttribute, 2, GL_FLOAT, false, 0, textCoords);
        glEnableVertexAttribArray(program->texCoordAttribute);
        
        glBindTexture(GL_TEXTURE_2D, pictureTextureID);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        
        
        
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        
        
        glDisableVertexAttribArray(program->positionAttribute);
        glDisableVertexAttribArray(program->texCoordAttribute);
        
        //SDL_GL_SwapWindow(displayWindow);
    }

 
};
