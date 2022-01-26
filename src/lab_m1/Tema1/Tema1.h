#pragma once

#include "components/simple_scene.h"
#include "Enemy.h"
#include <lab_m1/Tema1/Projectile.h>
#include <lab_m1/Tema1/Obstacle.h>
namespace m1
{
    class Tema1 : public gfxc::SimpleScene
    {
    public:
        struct ViewportSpace
        {
            ViewportSpace() : x(0), y(0), width(1), height(1) {}
            ViewportSpace(int x, int y, int width, int height)
                : x(x), y(y), width(width), height(height) {}
            int x;
            int y;
            int width;
            int height;
        };

        struct LogicSpace
        {
            LogicSpace() : x(0), y(0), width(1), height(1) {}
            LogicSpace(float x, float y, float width, float height)
                : x(x), y(y), width(width), height(height) {}
            float x;
            float y;
            float width;
            float height;
        };

     public:
        Tema1();
        ~Tema1();

        void Init() override;

     private:
        void FrameStart() override;
        void Update(float deltaTimeSeconds) override;
        void FrameEnd() override;

        void OnInputUpdate(float deltaTime, int mods) override;
        void OnKeyPress(int key, int mods) override;
        void OnKeyRelease(int key, int mods) override;
        void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
        void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
        void OnWindowResize(int width, int height) override;

        bool CheckCollision(Projectile p, float posX, float posY, float width, float height);
        bool CheckCollision(float objX, float objY, float objW, float objH);
        bool checkPlayerMapCollisions();
        void checkEnemyPlayerCollisions();
        bool checkProjectilMapCollisions(Projectile p);


        glm::mat3 VisualizationTransf2D(const LogicSpace& logicSpace, const ViewportSpace& viewSpace);
        glm::mat3 VisualizationTransf2DUnif(const LogicSpace& logicSpace, const ViewportSpace& viewSpace);

        void SetViewportArea(const ViewportSpace& viewSpace, glm::vec3 colorColor = glm::vec3(0), bool clear = true);
        void initObstacles();
        void DrawScene(glm::mat3 visMatrix, float deltaTime);
        void DrawMiniMapScene(glm::mat3 visMatrix);
        void DrawMap(glm::mat3 ModelMatrix, glm::mat3 visMatrix);
        void DrawEnemy(Enemy en, glm::mat3 modelMatrix);
        void DrawProjectile(glm::mat3 visMatrix, Projectile p);

     protected:
        ViewportSpace viewSpace;
        LogicSpace logicSpace;
        float cx, cy;
        glm::mat3 modelMatrix, visMatrix, visMatrixMiniMap;
        float angularStep;

        float globTime;

        float playerRadius;
        float  playerSpeed;
        float resX, resY;

        int projectileState;
        float projectileSpeed, projectileMaxDistance;
        Enemy en1;
        int nrOfenemies, maxNrOfEnemies;
        Enemy enemies[50];
        Projectile projVec[5];
        int projNr, projIdx;
        Projectile projectile;
        float lastShootms, fireRate;
        float lastEnemySpawn;
        float spawnTime;
        // COLISIONS
        Obstacle obstacles[200];
        int nrOfObstacles;
        bool playerBoxColision;
        // TODO(student): If you need any other class variables, define them here.

        //INTERFACE
        float health_level;
        long int score;
        float score_level;
        bool gameOver;
        bool printNewGameMessage;
    };
}   // namespace m1
