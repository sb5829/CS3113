//
//  Util.hpp
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
#include <vector>
#include <SDL.h>
#include <SDL_opengl.h>
#include "glm/mat4x4.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "ShaderProgram.h"
class Util {
public:
    static GLuint LoadTexture(const char* filePath);
    static void DrawText(ShaderProgram *program, GLuint fontTextureID, std::string
                         text, float size, float spacing, glm::vec3 position);
};
