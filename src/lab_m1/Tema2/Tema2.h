#pragma once

#include "components/simple_scene.h"
#include "lab_m1/Tema2/tcamera.h"


namespace m1
{
    class Tema2 : public gfxc::SimpleScene
    {
    public:
        Tema2();
        ~Tema2();

        void Init() override;

    private:
        void FrameStart() override;
        void Update(float deltaTimeSeconds) override;
        void FrameEnd() override;
        void RenderSimpleMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix, const glm::vec3& color = glm::vec3(1));

        void OnInputUpdate(float deltaTime, int mods) override;
        void OnKeyPress(int key, int mods) override;
        void OnKeyRelease(int key, int mods) override;
        void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
        void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
        void OnWindowResize(int width, int height) override;

    protected:
        float left, right, bottom, top, zNear, zFar;
        float fov;
        bool projectionMode;
        float playerX, playerY, playerZ;
        float EnemyX, EnemyY, EnemyZ;

        bool enemyAnim;
        int enemyStage, mode;
        float playerAngle;

        int mazeMatrix[10][10];

        float lifeBar, timeBar;
        bool gameOver;
        int playerPosX, mazeY, playerPosY;
        int moveFeet;
        float feetAngle;
        int steps;

        float projectileAngle;
        float fireRate, projectileSpeed;
        glm::vec3 projectilePosition, projectileMove;
        bool disappearProjectile;

        float enemyDyingTime;

        implemented::Cam* camera;
        glm::mat4 projectionMatrix;
        bool firstPersCam;
    };
}   // namespace m1
