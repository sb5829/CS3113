//
//  Level1.hpp
//  Platforms
//
//  Created by Angie Beck on 4/20/20.
//  Copyright © 2020 sabrena beck. All rights reserved.
//

#include "Scene.hpp"
extern int lives;
class Level1 : public Scene {
    
public:
    
    void Initialize() override;
    void Update(float deltaTime) override;
    void Render(ShaderProgram *program) override;
};

