//
//  map.cpp
//  Platform
//
//  Created by Angie Beck on 4/20/20.
//  Copyright © 2020 sabrena beck. All rights reserved.
//

#include <stdio.h>
#include "map.h"
#define GL_SILENCE_DEPRECATION
#ifdef _WINDOWS
#include <GL/glew.h>
#endif

#define GL_GLTEXT_PROTYOTYPES 1
#include <vector>
#include <math.h>
#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>
#include "glm/mat4x4.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "ShaderProgram.h"

Map::Map(int width, int height, unsigned int *levelData, GLuint textureID, float tile_size, int tile_count_x, int tile_count_y)
{
    this->width = width;
    this->height = height;
    this->levelData = levelData;
    this->textureID = textureID;
    this->tile_size = tile_size;
    this->tile_count_x = tile_count_x;
    this->tile_count_y = tile_count_y;
    
    this->Build();
}

void Map::Build()
{
    for(int y = 0; y <this->height; y++){
        for(int x = 0; x < this->width; x++){
            int tile = levelData[y*width +x];
            if (tile == 0) continue;
            
            float u = (float)(tile%tile_count_x)/ (float)tile_count_x;
            float v = (float)(tile / tile_count_x)/ (float)tile_count_y;
            
            float tileWidth = 1.0f/(float)tile_count_x;
            float tileHeight = 1.0f/(float)tile_count_y;
            
            float xoffset = (tile_size/2);
            float yoffset = (tile_size/2);
            
            vertices.insert(vertices.end(),{
                xoffset + (tile_size * x), yoffset + -tile_size * y,
                xoffset + (tile_size * x), yoffset + (-tile_size * y) - tile_size,
                xoffset + (tile_size * x) + tile_size, yoffset + (-tile_size * y) - tile_size,
                
                xoffset + (tile_size * x), yoffset + -tile_size * y,
                xoffset + (tile_size * x) + tile_size, yoffset + (-tile_size * y) - tile_size,
                xoffset + (tile_size * x) + tile_size, yoffset + -tile_size * y
            });
            textCoords.insert(textCoords.end(), {
                u, v,
                u, v+(tileHeight),
                u+tileWidth, v+(tileHeight),
                
                u, v,
                u+tileWidth, v+(tileHeight),
                u+tileWidth, v
            });
        }
    }
    left_bound = 0 - (tile_size / 2);
    right_bound = (tile_size * width) - (tile_size / 2);
    
    top_bound = 0 + (tile_size / 2);
    bottom_bound = -(tile_size * height) + (tile_size / 2);
}

void Map::Render(ShaderProgram *program)
{
    glm::mat4 modelMatrix = glm::mat4(1.0f);
    program->SetModelMatrix(modelMatrix);
    
    glUseProgram(program->programID);
    
    glVertexAttribPointer(program->positionAttribute, 2, GL_FLOAT, false, 0, vertices.data());
    glEnableVertexAttribArray(program->positionAttribute);
    
    glVertexAttribPointer(program->textCoordAttribute, 2, GL_FLOAT, false, 0, texCoords.data());
    glEnableVertexAttribArray(program->texCoordAttribute);
    
    glBindTexture(GL_TEXTURE_2D, textureID);
    glDrawArrays(GL_TRIANGLES, 0, (int)vertices.size() / 2);
    
    glDisableVertexAttribArray(program->positionAttribute);
    glDisableVertexAttribArray(program->texCoordAttribute);
}

