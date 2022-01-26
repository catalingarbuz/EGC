#pragma once
#include "components/simple_scene.h"

namespace m1
{
    class Enemy 
    {
    public:
        Enemy();
        Enemy(float posX, float posY, float enemyAngle, float speed, float sizeX, float sizeY);
        ~Enemy();

        void moveEnemy(float playerX, float playerY, float deltaTime);
        // void Init() override;
        
        /* void FrameStart() override;
         void Update(float deltaTimeSeconds) override;
         void FrameEnd() override;

         void OnInputUpdate(float deltaTime, int mods) override;
         void OnKeyPress(int key, int mods) override;
         void OnKeyRelease(int key, int mods) override;
         void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
         void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
         void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
         void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
         void OnWindowResize(int width, int height) override;*/

        //void drawMap(glm::mat3 ModelMatrix);
    public:
        float posX, posY, enemyAngle;
        float sizeX, sizeY;
        float speed;
        bool dead;

    };
}   // namespace m1
#pragma once
