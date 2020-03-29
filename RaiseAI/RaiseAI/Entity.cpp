//
//  Entity.cpp
//  LunarLander
//
//  Created by Angie Beck on 3/13/20.
//  Copyright © 2020 sabrena beck. All rights reserved.
//

#include "Entity.hpp"
#include "vector"


Entity::Entity()
{
    position = glm::vec3(0);
    speed = 0;
    position = glm::vec3(0);
    movement = glm::vec3(0);
    acceleration = glm::vec3(0);
    velocity = glm::vec3(0);
    speed = 0;
    modelMatrix = glm::mat4(1.0f);
    modelMatrix = glm::mat4(1.0f);
}

bool Entity::CheckCollision(Entity *other, Entity *success, Entity *failure){
    
    
    if (isActive == false || other->isActive == false) return false;
    
    float xdist = fabs(position.x - other->position.x) - ((width + other->width)/2.0f);
    float ydist = fabs(position.y - other->position.y) -((height + other->height)/2.0f);
    
    if (xdist < 0 && ydist < 0){
        return true;
    }
    
    return false;
    
}
void Entity::CheckCollisionsY(Entity* objects, int objectCount, Entity *success, Entity *failure, Entity *l1, Entity *l2, Entity *l3){
    for (int i =0; i < objectCount; i++){
        
        Entity* object = &objects[i];
        
        if (CheckCollision(object, success, failure)){
            float xdist = fabs(position.x - object->position.x) - ((width + object->width)/2.0f);
            float ydist = (position.y - object->position.y);
            if (entityType==PLAYER && object->entityType== ENEMY && xdist < -0.17){
                if(object->entityType == ENEMY && object->aiType== PATROL && !failure->isActive){
                    l1->isActive = false;
                    object->isActive = false;
                    l2->isActive = true;
                    l2->enemy->isActive = true;
                }
                if (object->entityType == ENEMY && object->aiType == SHOOTER && ydist>0){

                    object->isActive = false;
                    l3->isActive = false;
                    success->isActive = true;
                }
                if (object->entityType == ENEMY && object->aiType == JUMPER && ydist>0){
                    
                    object->isActive = false;
                    l2->isActive = false;
                    l3->isActive = true;
                    l3->enemy->isActive = true;
                }
                if (object->entityType == ENEMY && object->aiType == JUMPER && ydist<0){
                    
                    failure->isActive = true;
                    l2->isActive = false;
                    object->aiState = STOP;
                    object->velocity.y = 0;
                }
                if (object->entityType == ENEMY && object->aiType == JUMPER && ydist<0){

                    failure->isActive = true;
                    l3->isActive = false;
                    object->aiState = STOP;
                    object->velocity.y = 0;
                }
            }
            if(object->entityType == TILE){
                float ydist = fabs(position.y - object->position.y);
                float penetrationY = fabs(ydist - (height / 2.0f) - (object->height / 2.0f) );
                if (velocity.y > 0) {
                    position.y -= penetrationY;
                    velocity.x =0;
                    velocity.y =0;
                    
                    
                } else if (velocity.y < 0) {
                    position.y += penetrationY;
                    velocity.x =0;
                    velocity.y =0;
                    
                }
            }
            
            
        }
    }
}

void Entity::CheckCollisionsX(Entity* objects, int objectCount, Entity *success, Entity *failure, Entity *l1, Entity *l2, Entity *l3){
    
    for (int i =0; i < objectCount; i++){
        Entity* object = &objects[i];
        
        if (CheckCollision(object, success, failure) && entityType == PLAYER){
            if(success->isActive){
                return;
            }
            if(object->entityType == ENEMY && !success->isActive){
                failure->isActive = true;
                l1->isActive = false;
                l2->isActive = false;
                object->aiState = STOP;
                
            }
            if(object->entityType == BACTERIA && !success->isActive){
                failure->isActive = true;
                l3->isActive = false;
                object->aiState = STOP;
                
            }
            if(object->entityType == ENEMY && object->aiType==SHOOTER && !success->isActive){
                failure->isActive = true;
                l3->isActive = false;
                object->aiState = STOP;
                
            }
            
            
            float xdist = fabs(position.x - object->position.x );
            float penetrationX = fabs(xdist - (width / 2.0f) - (object->width / 2.0f) );
            if (velocity.x > 0) {
                position.x -= penetrationX;
                velocity.x =0;
                velocity.y =0;
            } else if (velocity.x < 0) {
                position.x += penetrationX;
                velocity.x = 0;
                velocity.y =0;
            }
            
            
        }
    }
}

void Entity::DrawText(ShaderProgram *program, GLuint fontTextureID, std::string text, float size, float spacing, glm::vec3 position){
    
    if(isActive == false) return;
    
    float width = 1.0f / 16.0f;
    float height = 1.0f / 16.0f;
    
    
    
    std::vector<float> vertices;
    std::vector<float> texCoords;
    
    for(int i = 0; i< text.size(); i++ ){
        int index = (int)text[i];
        float offset = (size + spacing) * i;
        float u = (float)(index % 16) / 16.0f;
        float v = (float)(index / 16) / 16.0f;
        
        vertices.insert(vertices.end(), {
            offset + (-0.5f * size), 0.5f * size,
            offset + (-0.5f * size), -0.5f * size,
            offset + (0.5f * size), 0.5f * size,
            offset + (0.5f * size), -0.5f * size,
            offset + (0.5f * size), 0.5f * size,
            offset + (-0.5f * size), -0.5f * size,
        });
        
        texCoords.insert(texCoords.end(), {
            u,v,
            u,v + height,
            u + width, v,
            u + width, v + height,
            u + width, v,
            u, v + height,
        });
        
    }
    
    glm::mat4 modelMatrix = glm::mat4(1.0f);
    modelMatrix = glm::translate(modelMatrix, position);
    program->SetModelMatrix(modelMatrix);
    
    glUseProgram(program->programID);
    
    glVertexAttribPointer(program->positionAttribute, 2, GL_FLOAT, false, 0, vertices.data());
    glEnableVertexAttribArray(program->positionAttribute);
    
    glVertexAttribPointer(program->texCoordAttribute, 2, GL_FLOAT, false, 0, texCoords.data());
    glEnableVertexAttribArray(program->texCoordAttribute);
    
    glBindTexture(GL_TEXTURE_2D, fontTextureID);
    glDrawArrays(GL_TRIANGLES, 0, (int)(text.size()*6));
    
    glDisableVertexAttribArray(program->positionAttribute);
    glDisableVertexAttribArray(program->texCoordAttribute);
}

void Entity::AIPatrol(Entity* player){
    switch (aiState) {
        case WALKING:
            if (player->position.x < position.x){
                movement = glm::vec3(-1,0,0);
            }
            else{
                movement = glm::vec3(1,0,0);
            }
            
            break;
        case STOP:
            movement = glm::vec3(0,0,0);
            break;
        case IDLE:
            if(glm::distance(position, player->position)<7.5f){
                aiState = WALKING;
            }
            break;
        case ATTACKING:
            break;
        case JUMP:
            break;
    }
    
}

void Entity::AIJumper(Entity* player, float ticks){
    switch (aiState) {
        case WALKING:
            if (player->position.x < position.x){
                movement = glm::vec3(-1,0,0);
                if (int(ticks)%4 == 0){
                    aiState = JUMP;
                }
            }
            else{
                movement = glm::vec3(1,0,0);
                if (int(ticks)%4 == 0){
                    
                    aiState = JUMP;
                }
            }
            break;
        case STOP:
            movement = glm::vec3(0,0,0);
            break;
        case IDLE:
            if(glm::distance(position, player->position)<7.5f){
                aiState = WALKING;
            }
            break;
        case JUMP:
            
            velocity.y += jumpPower;
            //movement = glm::vec3(-1,0,0);
            aiState = WALKING;
            break;
        case ATTACKING:
            break;
        
        
    }
    
}

void Entity::AIShooter(Entity* player, float ticks){
    switch (aiState) {
        case WALKING:
            if (player->position.x < position.x){
                movement = glm::vec3(-1,0,0);
                if (int(ticks)%4 == 0){
                    aiState = JUMP;
                }
            }
            else{
                movement = glm::vec3(1,0,0);
                if (int(ticks)%4 == 0){
                    
                    aiState = JUMP;
                }
            }
            break;
        case STOP:
            movement = glm::vec3(0,0,0);
            break;
        case IDLE:
            if(glm::distance(position, player->position)<7.5f){
                aiState = ATTACKING;
            }
            break;
        case JUMP:
            
            velocity.y += jumpPower;
            //movement = glm::vec3(-1,0,0);
            aiState = ATTACKING;
            break;
        case ATTACKING:
            int i =0;
            bacteria[i].isActive = true;
            bacteria[i].movement = glm::vec3(-1,0,0);
            if (player->position.x < position.x){
                movement = glm::vec3(-1,0,0);
                if (int(ticks)%3 == 0){
                    i++;
                    bacteria[i].position = glm::vec3(position.x,position.y,0);
                    bacteria[i].isActive = true;
                    bacteria[i].movement = glm::vec3(-1,0,0);
                }
                if (int(ticks)%4 == 0){


                    aiState = JUMP;
                }
            }
            else{
                movement = glm::vec3(1,0,0);
                if (int(ticks)%3 == 0){
                    i++;
                    bacteria[i].position = glm::vec3(position.x,position.y,0);
                    bacteria[i].isActive = true;
                    bacteria[i].movement = glm::vec3(1,0,0);
                }
                if (int(ticks)%4 == 0){
                    
                    aiState = JUMP;
                }
            }
            break;
            break;
            
            
    }
    
}

void Entity::AI(Entity* player, float ticks){
    switch(aiType){
        case PATROL:
            AIPatrol(player);
            break;
        case JUMPER:
            AIJumper(player, ticks);
            break;
        case SHOOTER:
            AIShooter(player, ticks);
            break;
    }
}
void Entity::Update(float deltaTime, Entity *platforms, Entity *player, int platformCount, Entity *success, Entity *failure, Entity *l1, Entity *l2, Entity *l3, float ticks){
    
    if(isActive == false) return;
    
    if (entityType == ENEMY){
        AI(player, ticks);
    }
    
    if(jump){
        jump = false;
        velocity.y += jumpPower;
    }
    
    
    velocity.x = movement.x * speed;
    
    velocity += acceleration * deltaTime;
    
   
    
    position.y += velocity.y * deltaTime;
    CheckCollisionsY(platforms, platformCount, success, failure, l1, l2, l3);
    
    position.x += velocity.x * deltaTime;
    CheckCollisionsX(platforms, platformCount, success, failure, l1, l2,l3);
    
    
    
    modelMatrix = glm::mat4(1.0f);
    modelMatrix = glm::translate(modelMatrix, position);
}

void Entity::Render(ShaderProgram *program) {
    
    if(isActive == false) return;
    
    program->SetModelMatrix(modelMatrix);
    
    if (entityType== BACTERIA){
        float vertices[]  = { -0.2, -0.2, 0.2, -0.2, 0.2, 0.2, -0.2, -0.2, 0.2, 0.2, -0.2, 0.2 };
        float texCoords[] = { 0.0, 1.0, 1.0, 1.0, 1.0, 0.0, 0.0, 1.0, 1.0, 0.0, 0.0, 0.0 };
        
        glBindTexture(GL_TEXTURE_2D, textureID);
        
        glVertexAttribPointer(program->positionAttribute, 2, GL_FLOAT, false, 0, vertices);
        glEnableVertexAttribArray(program->positionAttribute);
        
        glVertexAttribPointer(program->texCoordAttribute, 2, GL_FLOAT, false, 0, texCoords);
        glEnableVertexAttribArray(program->texCoordAttribute);
        
        glDrawArrays(GL_TRIANGLES, 0, 6);
        
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        
        glDisableVertexAttribArray(program->positionAttribute);
        glDisableVertexAttribArray(program->texCoordAttribute);
    }
    else {
        float vertices[]  = { -0.5, -0.5, 0.5, -0.5, 0.5, 0.5, -0.5, -0.5, 0.5, 0.5, -0.5, 0.5 };
        float texCoords[] = { 0.0, 1.0, 1.0, 1.0, 1.0, 0.0, 0.0, 1.0, 1.0, 0.0, 0.0, 0.0 };
        
        glBindTexture(GL_TEXTURE_2D, textureID);
        
        glVertexAttribPointer(program->positionAttribute, 2, GL_FLOAT, false, 0, vertices);
        glEnableVertexAttribArray(program->positionAttribute);
        
        glVertexAttribPointer(program->texCoordAttribute, 2, GL_FLOAT, false, 0, texCoords);
        glEnableVertexAttribArray(program->texCoordAttribute);
        
        glDrawArrays(GL_TRIANGLES, 0, 6);
        
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        
        glDisableVertexAttribArray(program->positionAttribute);
        glDisableVertexAttribArray(program->texCoordAttribute);
        
    }
    
    
    
}
