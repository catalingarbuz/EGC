#pragma once

#include "components/simple_scene.h"

namespace m1
{
    class Projectile
    {
    public:
        Projectile();
        Projectile(float posX, float posY, float projectileAngle, float speed, float maxDistance);
        ~Projectile();

        void moveProjectile(float deltaTime);
 
    public:
        float posX, posY, projectileAngle;
        float speed;
        float maxDistance;
        int setAngle;
        int distance;
        bool destroyed;

    };
}   // namespace m1
#pragma once
