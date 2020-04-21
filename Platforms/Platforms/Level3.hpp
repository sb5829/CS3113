//
//  Level3.hpp
//  Platforms
//
//  Created by Angie Beck on 4/21/20.
//  Copyright © 2020 sabrena beck. All rights reserved.
//

#include "Scene.hpp"
extern int lives;
class Level3 : public Scene {
    
public:
    
    void Initialize() override;
    void Update(float deltaTime) override;
    void Render(ShaderProgram *program) override;
    
};

